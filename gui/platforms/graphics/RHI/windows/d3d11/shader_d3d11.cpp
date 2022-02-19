
//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/windows/d3d11/shader_d3d11.h"

#include "core/shared/debugging/asserts.h"
#include "core/shared/filing/file_system.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_device_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/input_layout_d3d11.h"


#include <d3dcompiler.h>
#include <sstream>
#include <string>

namespace wrl = Microsoft::WRL;

namespace gui
{
	SHADER_D3D11::SHADER_D3D11()
	{
		m_input_layout = std::make_unique<INPUT_LAYOUT_D3D11>();
	}

	SHADER_D3D11::~SHADER_D3D11() = default;

	void* SHADER_D3D11::api_compile( RENDERING_DEVICE_BASE& device, const std::string& shader )
	{
		RENDERING_DEVICE_D3D11& device_d3d11 = static_cast<RENDERING_DEVICE_D3D11&>(device);

		uint32_t compile_flags = 0;
#ifdef DEBUG
		compile_flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_PREFER_FLOW_CONTROL;
#else
		compile_flags |= D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

		std::vector<D3D_SHADER_MACRO> defines =
		{
			D3D_SHADER_MACRO{ "VS", m_shader_type == RHI_Shader_Type::RHI_Shader_Vertex ? "1" : "0" },
			D3D_SHADER_MACRO{ "PS", m_shader_type == RHI_Shader_Type::RHI_Shader_Pixel ? "1" : "0" },
			D3D_SHADER_MACRO{ "CS", m_shader_type == RHI_Shader_Type::RHI_Shader_Compute ? "1" : "0" }
		};
		for ( const auto& define : m_defines )
		{
			defines.emplace_back( D3D_SHADER_MACRO{ define.first.c_str(), define.second.c_str() } );
		}
		defines.emplace_back( D3D_SHADER_MACRO{ nullptr, nullptr } );

		// Compile
		wrl::ComPtr<ID3DBlob> blob_error;
		wrl::ComPtr<ID3DBlob> shader_blob;
		HRESULT result;
		if ( core::FILE_SYSTEM::is_file( shader ) )
		{
			const std::wstring file_path( shader.begin(), shader.end() );
			result = D3DCompileFromFile
			(
				file_path.c_str(),
				defines.data(),
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				GetEntryPoint().data(),
				GetTargetProfile().data(),
				compile_flags,
				0,
				shader_blob.GetAddressOf(),
				blob_error.GetAddressOf()
			);
		}
		else if ( shader.find( "return" ) != std::string::npos ) // From source ?
		{
			result = D3DCompile
			(
				shader.c_str(),
				static_cast<SIZE_T>(shader.size()),
				nullptr,
				defines.data(),
				nullptr,
				GetEntryPoint().data(),
				GetTargetProfile().data(),
				compile_flags,
				0,
				shader_blob.GetAddressOf(),
				blob_error.GetAddressOf()
			);
		}
		else
		{
			ASSERT_SHOULDNT_BE_HERE();
			return nullptr;
		}

		// Log any compilation possible warnings and/or errors
		if ( blob_error )
		{
			std::stringstream ss( static_cast<char*>(blob_error->GetBufferPointer()) );
			std::string line;
			while ( std::getline( ss, line, '\n' ) )
			{
				const auto is_error = line.find( "error" ) != std::string::npos;
				if ( is_error )
				{
					ASSERT_FAILED( line );
				}
				else
				{
					//LOG_WARNING( line );
				}
			}
		}

		// Log compilation failure
		if ( FAILED( result ) || !shader_blob )
		{
			const auto shader_name = core::FILE_SYSTEM::get_filename_from_filepath( shader );
			if ( result == HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) )
			{
				ASSERT_FAILED( "Failed to find shader" );
			}
			else
			{
				ASSERT_FAILED( "An error occurred when trying to load and compile" );
			}
		}

		// Create shader
		void* shader_view = nullptr;
		if ( shader_blob )
		{
			if ( m_shader_type == RHI_Shader_Type::RHI_Shader_Vertex )
			{
				if ( FAILED( device_d3d11.get_device()->CreateVertexShader( shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, reinterpret_cast<ID3D11VertexShader**>(&shader_view) ) ) )
				{
					ASSERT_FAILED( "Failed to create vertex shader" );
				}

				// Create input layout
				if ( !m_input_layout->create( device, m_vertex_type, shader_blob.Get() ) )
				{
					ASSERT_FAILED( "Failed to create input layout for" );
				}
			}
			else if ( m_shader_type == RHI_Shader_Type::RHI_Shader_Pixel )
			{
				if ( FAILED( device_d3d11.get_device()->CreatePixelShader( shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, reinterpret_cast<ID3D11PixelShader**>(&shader_view) ) ) )
				{
					ASSERT_FAILED( "Failed to create pixel shader" );
				}
			}
			else if ( m_shader_type == RHI_Shader_Type::RHI_Shader_Compute )
			{
				if ( FAILED( device_d3d11.get_device()->CreateComputeShader( shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), nullptr, reinterpret_cast<ID3D11ComputeShader**>(&shader_view) ) ) )
				{
					ASSERT_FAILED( "Failed to create compute shader" );
				}
			}
		}

		return shader_view;
	}
}
