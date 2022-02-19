
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/vulkan/shader_vulkan.h"

#include "core/shared/debugging/asserts.h"
#include "core/shared/filing/file_system.h"
#include "core/shared/vertex.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_context_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_device_vulkan.h"
#include "gui/shared/graphics/RHI/descriptor.h"
#include "gui/shared/graphics/RHI/input_layout.h"

#include <atlbase.h>
#include <gui/thirdparty/spirv_cross/spirv_hlsl.hpp>
#include <gui/thirdparty/DirectXShaderCompiler/dxcapi.h>
#include <sstream>
#include <string>

namespace
{
	constexpr uint32_t rhi_shader_shift_storage_texture = 000;
	constexpr uint32_t rhi_shader_shift_buffer = 100;
	constexpr uint32_t rhi_shader_shift_texture = 200;
	constexpr uint32_t rhi_shader_shift_sampler = 300;
}

namespace
{
	inline bool error_check( IDxcResult* dxc_result )
	{
		// Get error buffer
		IDxcBlobEncoding* error_buffer = nullptr;
		HRESULT result = dxc_result->GetErrorBuffer( &error_buffer );
		if ( SUCCEEDED( result ) )
		{
			// Log info, warnings and errors
			std::stringstream ss( std::string( static_cast<char*>(error_buffer->GetBufferPointer()), error_buffer->GetBufferSize() ) );
			std::string line;
			while ( getline( ss, line, '\n' ) )
			{
				if ( line.find( "error" ) != std::string::npos )
				{
					ASSERT_FAILED( line );
				}
				else if ( line.find( "warning" ) != std::string::npos )
				{
					//LOG_WARNING( line );
				}
				//else if ( !FileSystem::IsEmptyOrWhitespace( line ) )
				//{
				//	LOG_INFO( line );
				//}
			}
		}
		else
		{
			ASSERT_FAILED( "Failed to get error buffer" );
		}

		// Release error buffer
		if ( error_buffer )
		{
			error_buffer->Release();
			error_buffer = nullptr;
		}

		// Return status
		dxc_result->GetStatus( &result );
		return result == S_OK;
	}

	class Compiler
	{
	public:
		Compiler()
		{
			DxcCreateInstance( CLSID_DxcUtils, IID_PPV_ARGS( &m_utils ) );
			DxcCreateInstance( CLSID_DxcCompiler, IID_PPV_ARGS( &m_compiler ) );
		}

		CComPtr<IDxcBlob> Compile( const std::string& shader, std::vector<std::string>& arguments )
		{
			// Get shader source
			DxcBuffer dxc_buffer;
			CComPtr<IDxcBlobEncoding> blob_encoding = nullptr;
			{
				HRESULT result;
				if ( true/*FileSystem::IsSupportedShaderFile( shader )*/ )
				{
					result = m_utils->LoadFile( std::wstring( shader.begin(), shader.end() ).c_str(), nullptr, &blob_encoding );
				}
				else // Source
				{
					result = m_utils->CreateBlobFromPinned( shader.c_str(), static_cast<uint32_t>(shader.size()), CP_UTF8, &blob_encoding );
				}

				if ( FAILED( result ) )
				{
					ASSERT_FAILED( "Failed to load shader source." );
					return nullptr;
				}

				dxc_buffer.Ptr = blob_encoding->GetBufferPointer();
				dxc_buffer.Size = blob_encoding->GetBufferSize();
				dxc_buffer.Encoding = DXC_CP_ACP; // Assume BOM says UTF8 or UTF16 or this is ANSI text.
			}

			// Convert arguments to wstring
			std::vector<std::wstring> arguments_wstring;
			arguments_wstring.reserve( arguments.size() );
			for ( const std::string& str : arguments )
			{
				arguments_wstring.emplace_back(  std::wstring( str.begin(), str.end() ) );
			}

			// Convert arguments to LPCWSTR
			std::vector<LPCWSTR> arguments_lpcwstr;
			arguments_lpcwstr.reserve( arguments.size() );
			for ( const std::wstring& wstr : arguments_wstring )
			{
				arguments_lpcwstr.emplace_back( wstr.c_str() );
			}

			// Create and include handler
			CComPtr<IDxcIncludeHandler> include_handler;
			m_utils->CreateDefaultIncludeHandler( &include_handler );

			// Compile
			CComPtr<IDxcResult> dxc_result = nullptr;
			[[maybe_unused]] auto result = m_compiler->Compile
			(
				&dxc_buffer,                                        // Source text to compile
				arguments_lpcwstr.data(),                           // Array of pointers to arguments
				static_cast<uint32_t>(arguments_lpcwstr.size()),    // Number of arguments
				include_handler,                                    // user-provided interface to handle #include directives (optional)
				IID_PPV_ARGS( &dxc_result )                           // IDxcResult: status, buffer, and errors
			);

			if ( !error_check( dxc_result ) )
			{
				ASSERT_FAILED( "Failed to compile" );
				return nullptr;
			}

			// Get compiled shader buffer
			CComPtr<IDxcBlob> blob_compiled = nullptr;
			if ( FAILED( dxc_result->GetResult( &blob_compiled ) ) )
			{
				ASSERT_FAILED( "Failed to get compiled shader buffer" );
				return nullptr;
			}

			return blob_compiled;
		}

		CComPtr<IDxcUtils> m_utils;
		CComPtr<IDxcCompiler3> m_compiler;
	};

	static Compiler& get_compiler()
	{
		static Compiler instance;
		return instance;
	}
}

namespace gui
{
	SHADER_VULKAN::SHADER_VULKAN() = default;

	SHADER_VULKAN::~SHADER_VULKAN()
	{
		//const auto rhi_context = m_rhi_device->GetContextRhi();

		//if ( HasResource() )
		//{
		//	// Wait in case it's still in use by the GPU
		//	m_rhi_device->Queue_WaitAll();

		//	vkDestroyShaderModule( rhi_context->device, static_cast<VkShaderModule>(m_resource), nullptr );
		//	m_resource = nullptr;
		//}
	}

	void* SHADER_VULKAN::api_compile( RENDERING_DEVICE_BASE& device, const std::string& shader )
	{
		RENDERING_DEVICE_VULKAN& device_vulkan = static_cast<RENDERING_DEVICE_VULKAN&>(device);

		std::vector<std::string> arguments;

		// Arguments
		{
			arguments.emplace_back( "-E" ); arguments.emplace_back( GetEntryPoint() );
			arguments.emplace_back( "-T" ); arguments.emplace_back( GetTargetProfile() );
			arguments.emplace_back( "-spirv" ); // Generate SPIR-V code
			//arguments.emplace_back( "-DENABLE_SPIRV_CODEGEN=ON" );
			arguments.emplace_back( "-fspv-reflect" ); // Emit additional SPIR-V instructions to aid reflection
			arguments.emplace_back( "-fspv-target-env=vulkan1.1" );                                                                                           // Specify the target environment: vulkan1.0 (default) or vulkan1.1
			arguments.emplace_back( "-fvk-b-shift" ); arguments.emplace_back( std::to_string( rhi_shader_shift_buffer ) );             arguments.emplace_back( "all" );  // Specify Vulkan binding number shift for b-type (buffer) register
			arguments.emplace_back( "-fvk-t-shift" ); arguments.emplace_back( std::to_string( rhi_shader_shift_texture ) );            arguments.emplace_back( "all" );  // Specify Vulkan binding number shift for t-type (texture) register
			arguments.emplace_back( "-fvk-s-shift" ); arguments.emplace_back( std::to_string( rhi_shader_shift_sampler ) );            arguments.emplace_back( "all" );  // Specify Vulkan binding number shift for s-type (sampler) register
			arguments.emplace_back( "-fvk-u-shift" ); arguments.emplace_back( std::to_string( rhi_shader_shift_storage_texture ) );    arguments.emplace_back( "all" );  // Specify Vulkan binding number shift for u-type (read/write buffer) register
			arguments.emplace_back( "-fvk-use-dx-position-w" );                                                                                               // Reciprocate SV_Position.w after reading from stage input in PS to accommodate the difference between Vulkan and DirectX
			arguments.emplace_back( "-fvk-use-dx-layout" );                                                                                                   // Use DirectX memory layout for Vulkan resources
			arguments.emplace_back( "-flegacy-macro-expansion" ); // Expand the operands before performing token-pasting operation (fxc behavior)
			//arguments.emplace_back( "-D" ); arguments.emplace_back( "ENABLE_SPIRV_CODEGEN=ON" );

#ifdef DEBUG
			arguments.emplace_back( "-Od" ); // Disable optimizations
			arguments.emplace_back( "-Zi" ); // Enable debug information
#endif

			// Negate SV_Position.y before writing to stage output in VS/DS/GS to accommodate Vulkan's coordinate system
			if ( m_shader_type == RHI_Shader_Type::RHI_Shader_Vertex )
			{
				arguments.emplace_back( "-fvk-invert-y" );
			}

			// Add include directory
			if ( core::FILE_SYSTEM::is_file( shader ) )
			{
				arguments.emplace_back( "-I" ); arguments.emplace_back( core::FILE_SYSTEM::get_directory_from_filepath( shader ) );
			}
		}

		// Defines
		{
			// Add standard defines
			arguments.emplace_back( "-D" ); arguments.emplace_back( "VS=" + std::to_string( static_cast<uint8_t>(m_shader_type == RHI_Shader_Type::RHI_Shader_Vertex) ) );
			arguments.emplace_back( "-D" ); arguments.emplace_back( "PS=" + std::to_string( static_cast<uint8_t>(m_shader_type == RHI_Shader_Type::RHI_Shader_Pixel) ) );
			arguments.emplace_back( "-D" ); arguments.emplace_back( "CS=" + std::to_string( static_cast<uint8_t>(m_shader_type == RHI_Shader_Type::RHI_Shader_Compute) ) );

			// Add the rest of the defines
			for ( const auto& define : m_defines )
			{
				arguments.emplace_back( "-D" ); arguments.emplace_back( define.first + "=" + define.second );
			}
		}

		if ( CComPtr<IDxcBlob> shader_buffer = get_compiler().Compile( shader, arguments ) )
		{
			// Create shader module
			VkShaderModule shader_module = nullptr;
			VkShaderModuleCreateInfo create_info = {};
			create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			create_info.codeSize = static_cast<size_t>(shader_buffer->GetBufferSize());
			create_info.pCode = reinterpret_cast<const uint32_t*>(shader_buffer->GetBufferPointer());

			if ( vkCreateShaderModule( device_vulkan.get_context()->device, &create_info, nullptr, &shader_module ) != VK_SUCCESS )
			{
				ASSERT_FAILED( "Failed to create shader module." );
				return nullptr;
			}

			// Reflect shader resources (so that descriptor sets can be created later)
			_Reflect
			(
				m_shader_type,
				reinterpret_cast<uint32_t*>(shader_buffer->GetBufferPointer()),
				static_cast<uint32_t>(shader_buffer->GetBufferSize() / 4)
			);

			// Create input layout
			//if ( m_vertex_type != RHI_Vertex_Type::RHI_Vertex_Type_Unknown )
			{
				if ( !m_input_layout->create( device_vulkan, m_vertex_type, nullptr ) )
				{
					ASSERT_FAILED( "Failed to create input layout" );
					return nullptr;
				}
			}

			return static_cast<void*>(shader_module);
		}

		ASSERT_SHOULDNT_BE_HERE();
		return nullptr;
	}

	void SHADER_VULKAN::_Reflect( const RHI_Shader_Type shader_type, const uint32_t* ptr, const uint32_t size )
	{
		// Initialize compiler with SPIR-V data
		const auto compiler = spirv_cross::CompilerHLSL( ptr, size );

		// The SPIR-V is now parsed, and we can perform reflection on it
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		// Get storage images
		for ( const auto& resource : resources.storage_images )
		{
			m_descriptors.emplace_back(
				resource.name,                                                  // name
				RHI_Descriptor_Type::Texture,                                   // type
				compiler.get_decoration( resource.id, spv::DecorationBinding ),   // slot
				(uint32_t)shader_type,                                                    // stage
				true,                                                           // is_storage
				false                                                           // is_dynamic_constant_buffer
			);
		}

		// Get constant buffers
		for ( const auto& resource : resources.uniform_buffers )
		{
			m_descriptors.emplace_back
			(
				resource.name,                                                  // name
				RHI_Descriptor_Type::ConstantBuffer,                            // type
				compiler.get_decoration( resource.id, spv::DecorationBinding ),   // slot
				(uint32_t)shader_type,                                                    // stage
				false,                                                          // is_storage
				false                                                           // is_dynamic_constant_buffer
			);
		}

		// Get textures
		for ( const auto& resource : resources.separate_images )
		{
			m_descriptors.emplace_back
			(
				resource.name,                                                  // name
				RHI_Descriptor_Type::Texture,                                   // type
				compiler.get_decoration( resource.id, spv::DecorationBinding ),   // slot
				(uint32_t)shader_type,                                                    // stage
				false,                                                          // is_storage
				false                                                           // is_dynamic_constant_buffer
			);
		}

		// Get samplers
		for ( const auto& resource : resources.separate_samplers )
		{
			m_descriptors.emplace_back
			(
				resource.name,                                                  // name
				RHI_Descriptor_Type::Sampler,                                   // type
				compiler.get_decoration( resource.id, spv::DecorationBinding ),   // slot
				(uint32_t)shader_type,                                                    // stage
				false,                                                          // is_storage
				false                                                           // is_dynamic_constant_buffer
			);
		}
	}
}
