


//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/windows/d3d11/input_layout_d3d11.h"

#include "gui/platforms/graphics/RHI/windows/d3d11/RHI_device_d3d11.h"
#include "gui/platforms/graphics/RHI/windows/d3d11/d3d11_types.h"

namespace gui
{
	bool INPUT_LAYOUT_D3D11::_CreateResource( RENDERING_DEVICE_BASE& device, void* vertex_shader_blob )
	{
		RENDERING_DEVICE_D3D11& device_d3d11 = static_cast<RENDERING_DEVICE_D3D11&>(device);

		if ( !vertex_shader_blob )
		{
			ASSERT_SHOULDNT_BE_HERE();
			return false;
		}

		if ( m_vertex_attributes.empty() )
		{
			ASSERT_SHOULDNT_BE_HERE();
			return false;
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> vertex_attributes;
		for ( const auto& vertex_attribute : m_vertex_attributes )
		{
			vertex_attributes.emplace_back( D3D11_INPUT_ELEMENT_DESC
											{
												vertex_attribute.name.c_str(),            // SemanticName
												0,                                        // SemanticIndex
												d3d11_format[vertex_attribute.format],    // Format
												0,                                        // InputSlot
												vertex_attribute.offset,                // AlignedByteOffset
												D3D11_INPUT_PER_VERTEX_DATA,            // InputSlotClass
												0                                        // InstanceDataStepRate
											} );
		}

		// Create input layout
		auto d3d_blob = static_cast<ID3D10Blob*>(vertex_shader_blob);
		const auto result = device_d3d11.get_device()->CreateInputLayout
		(
			vertex_attributes.data(),
			static_cast<UINT>(vertex_attributes.size()),
			d3d_blob->GetBufferPointer(),
			d3d_blob->GetBufferSize(),
			reinterpret_cast<ID3D11InputLayout**>(&m_resource)
		);

		if ( FAILED( result ) )
		{
			ASSERT_FAILED( "Failed to create input layout" );
			return false;
		}

		return true;
	}
}
