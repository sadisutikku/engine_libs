


//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/vulkan/descriptor_set_layout_cache_vulkan.h"

#include "gui/platforms/graphics/RHI/vulkan/RHI_context_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_device_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/pipeline_state_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/shader_vulkan.h"
#include "gui/shared/graphics/RHI/descriptor.h"

#include <array>

static const uint8_t rhi_descriptor_max_storage_textures = 10;
static const uint8_t rhi_descriptor_max_constant_buffers = 10;
static const uint8_t rhi_descriptor_max_constant_buffers_dynamic = 10;
static const uint8_t rhi_descriptor_max_samplers = 10;
static const uint8_t rhi_descriptor_max_textures = 10;

namespace gui
{
	DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN::DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN( RENDERING_DEVICE_VULKAN& device )
		: m_device{ device }
	{
		// Set the descriptor set capacity to an initial value
		SetDescriptorSetCapacity( m_descriptor_set_capacity );
	}

	DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN::~DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN()
	{
		if ( m_descriptor_pool )
		{
			// Wait in case it's still in use by the GPU
			//m_device.Queue_WaitAll();

			vkDestroyDescriptorPool( m_device.get_context()->device, m_descriptor_pool, nullptr );
		}
	}

	void DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN::Reset( uint32_t descriptor_set_capacity )
	{
		// If the requested capacity is zero, then only recreate the descriptor pool
		if ( descriptor_set_capacity == 0 )
		{
			descriptor_set_capacity = m_descriptor_set_capacity;
		}

		// Destroy layouts (and descriptor sets)
		m_descriptor_set_layouts_being_cleared = true;
		m_descriptor_set_layouts.clear();
		m_descriptor_set_layouts_being_cleared = false;
		m_descriptor_layout_current = nullptr;

		// Destroy pool
		if ( m_descriptor_pool )
		{
			// Wait in case it's still in use by the GPU
			//m_device.Queue_WaitAll();

			vkDestroyDescriptorPool( m_device.get_context()->device, static_cast<VkDescriptorPool>(m_descriptor_pool), nullptr );
			m_descriptor_pool = nullptr;
		}

		// Create pool
		CreateDescriptorPool( descriptor_set_capacity );

		// Log
		if ( descriptor_set_capacity > m_descriptor_set_capacity )
		{
			//LOG_INFO( "Capacity has been increased to %d elements", descriptor_set_capacity );
		}
		else if ( descriptor_set_capacity < m_descriptor_set_capacity )
		{
			//LOG_INFO( "Capacity has been decreased to %d elements", descriptor_set_capacity );
		}
		else
		{
			//LOG_INFO( "Descriptor pool has been reset" );
		}
	}

	void DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN::SetPipelineState( PIPELINE_STATE_VULKAN& pipeline_state )
	{
		// Get pipeline descriptors
		GetDescriptors( pipeline_state, m_descriptors );

		//// Compute a hash for the descriptors
		//uint32_t hash = 0;
		//for ( const RHI_Descriptor& descriptor : m_descriptors )
		//{
		//	Utility::Hash::hash_combine( hash, descriptor.ComputeHash( false ) );
		//}

		//// If there is no descriptor set layout for this particular hash, create one
		//auto it = m_descriptor_set_layouts.find( hash );
		//if ( it == m_descriptor_set_layouts.end() )
		//{
		//	// Create a name for the descriptor set layout, very useful for Vulkan debugging
		//	std::string name = "CS:" + (pipeline_state.shader_compute ? pipeline_state.shader_compute->GetName() : "null");
		//	name += "-VS:" + (pipeline_state.shader_vertex ? pipeline_state.shader_vertex->GetName() : "null");
		//	name += "-PS:" + (pipeline_state.shader_pixel ? pipeline_state.shader_pixel->GetName() : "null");

		//	// Emplace a new descriptor set layout
		//	it = m_descriptor_set_layouts.emplace( make_pair( hash, make_shared<RHI_DescriptorSetLayout>( m_rhi_device, m_descriptors, name.c_str() ) ) ).first;
		//}

		//// Get the descriptor set layout we will be using
		//m_descriptor_layout_current = it->second.get();
		//m_descriptor_layout_current->NeedsToBind();
	}

	void DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN::GetDescriptors( PIPELINE_STATE_VULKAN& pipeline_state, std::vector<DESCRIPTOR>& descriptors )
	{
		/*if ( !pipeline_state.IsValid() )
		{
			ASSERT_FAILED( "Invalid pipeline state" );
			descriptors.clear();
			return;
		}*/

		bool descriptors_acquired = false;

		if ( pipeline_state.IsCompute() )
		{
			// Wait for compilation
			//pipeline_state.m_compute_shader->WaitForCompilation();

			// Get compute shader descriptors
			descriptors = pipeline_state.m_compute_shader->get_descriptors();
			descriptors_acquired = true;
		}
		else if ( pipeline_state.IsGraphics() )
		{
			// Wait for compilation
			//pipeline_state.m_vertex_shader->WaitForCompilation();

			// Get vertex shader descriptors
			descriptors = pipeline_state.m_vertex_shader->get_descriptors();
			descriptors_acquired = true;

			// If there is a pixel shader, merge it's resources into our map as well
			if ( pipeline_state.m_pixel_shader )
			{
				// Wait for compilation
				//pipeline_state.m_pixel_shader->WaitForCompilation();

				for ( const DESCRIPTOR& descriptor_reflected : pipeline_state.m_pixel_shader->get_descriptors() )
				{
					// Assume that the descriptor has been created in the vertex shader and only try to update it's shader stage
					bool updated_existing = false;
					for ( DESCRIPTOR& descriptor : descriptors )
					{
						bool is_same_resource =
							(descriptor.type == descriptor_reflected.type) &&
							(descriptor.slot == descriptor_reflected.slot);

						if ( (descriptor.type == descriptor_reflected.type) && (descriptor.slot == descriptor_reflected.slot) )
						{
							descriptor.stage |= descriptor_reflected.stage;
							updated_existing = true;
							break;
						}
					}

					// If no updating took place, this descriptor is new, so add it
					if ( !updated_existing )
					{
						descriptors.emplace_back( descriptor_reflected );
					}
				}
			}
		}

		// Change constant buffers to dynamic (if requested)
		if ( descriptors_acquired )
		{
			for ( uint32_t i = 0; i < rhi_max_constant_buffer_count; i++ )
			{
				for ( DESCRIPTOR& descriptor : descriptors )
				{
					if ( descriptor.type == RHI_Descriptor_Type::ConstantBuffer )
					{
						if ( descriptor.slot == pipeline_state.dynamic_constant_buffer_slots[i] + rhi_shader_shift_buffer )
						{
							descriptor.is_dynamic_constant_buffer = true;
						}
					}
				}
			}
		}
	}

	void DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN::SetDescriptorSetCapacity( const uint32_t descriptor_set_capacity )
	{
		if ( m_descriptor_set_capacity == descriptor_set_capacity )
		{
			//LOG_INFO( "Capacity is already %d elements", m_descriptor_set_capacity );
			return;
		}

		// Re-create descriptor pool
		Reset( descriptor_set_capacity );

		// Update capacity
		m_descriptor_set_capacity = descriptor_set_capacity;
	}

	bool DESCRIPTOR_SET_LAYOUT_CACHE_VULKAN::CreateDescriptorPool( const uint32_t descriptor_set_capacity )
	{
		// Pool sizes
		std::array<VkDescriptorPoolSize, 5> pool_sizes =
		{
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLER,                   rhi_descriptor_max_samplers },
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,             rhi_descriptor_max_textures },
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,             rhi_descriptor_max_storage_textures },
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,            rhi_descriptor_max_constant_buffers },
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,    rhi_descriptor_max_constant_buffers_dynamic }
		};

		// Create info
		VkDescriptorPoolCreateInfo pool_create_info = {};
		pool_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_create_info.flags = 0;
		pool_create_info.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
		pool_create_info.pPoolSizes = pool_sizes.data();
		pool_create_info.maxSets = descriptor_set_capacity;

		// Pool
		return vkCreateDescriptorPool( m_device.get_context()->device, &pool_create_info, nullptr, &m_descriptor_pool ) == VK_SUCCESS;
	}
}
