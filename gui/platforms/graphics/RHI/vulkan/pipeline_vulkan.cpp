


//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/platforms/graphics/RHI/vulkan/pipeline_vulkan.h"

#include "core/shared/debugging/asserts.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_context_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/descriptor_set_layout_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/RHI_device_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/pipeline_state_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/shader_vulkan.h"
#include "gui/platforms/graphics/RHI/vulkan/vulkan_types.h"
#include "gui/shared/graphics/RHI/input_layout.h"

#include <array>

namespace gui
{
	PIPELINE_VULKAN::PIPELINE_VULKAN( RENDERING_DEVICE_VULKAN& device_vulkan, PIPELINE_STATE_VULKAN& pipeline_state, DESCRIPTOR_SET_LAYOUT_VULKAN* descriptor_set_layout )
		: m_device{ device_vulkan }
		, m_pipeline_state{ pipeline_state }
	{
		// Pipeline layout
		{
			std::array<VkDescriptorSetLayout, 1> layouts = { descriptor_set_layout->get_resource() };

			// Validate descriptor set layouts
			for ( void* layout : layouts )
			{
				//SP_ASSERT( layout != nullptr );
			}

			// Pipeline layout
			VkPipelineLayoutCreateInfo pipeline_layout_info = {};
			pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipeline_layout_info.pushConstantRangeCount = 0;
			pipeline_layout_info.setLayoutCount = static_cast<uint32_t>(layouts.size());
			pipeline_layout_info.pSetLayouts = reinterpret_cast<VkDescriptorSetLayout*>(layouts.data());

			// Create
			if ( vkCreatePipelineLayout( m_device.get_context()->device, &pipeline_layout_info, nullptr, &m_pipeline_layout ) != VK_SUCCESS )
				return;

			// Name
			//vulkan_utility::debug::set_name( static_cast<VkPipelineLayout>(m_pipeline_layout), m_pipeline_state.pass_name );
		}

		if ( pipeline_state.IsCompute() )
		{
			// Shader
			VkPipelineShaderStageCreateInfo shader_stage_info = {};
			{
				shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shader_stage_info.stage = VK_SHADER_STAGE_COMPUTE_BIT;
				shader_stage_info.module = static_cast<VkShaderModule>(m_pipeline_state.m_compute_shader->GetResource());
				shader_stage_info.pName = m_pipeline_state.m_compute_shader->GetEntryPoint().data();

				// Validate shader stage
				ASSERT_TRUE( shader_stage_info.module );
				ASSERT_TRUE( shader_stage_info.pName );
			}

			// Pipeline
			{
				VkComputePipelineCreateInfo pipeline_info = {};
				pipeline_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
				pipeline_info.layout = static_cast<VkPipelineLayout>(m_pipeline_layout);
				pipeline_info.stage = shader_stage_info;

				// Pipeline creation
				VkPipeline* pipeline = reinterpret_cast<VkPipeline*>(&m_pipeline);
				if ( vkCreateComputePipelines( m_device.get_context()->device, nullptr, 1, &pipeline_info, nullptr, pipeline ) != VK_SUCCESS )
					return;

				// Name
				//vulkan_utility::debug::set_name( *pipeline, m_pipeline_state.pass_name );
			}
		}
		else if ( pipeline_state.IsGraphics() || pipeline_state.IsDummy() )
		{
			if ( pipeline_state.IsGraphics() )
			{
				m_pipeline_state.create_framebuffer();
			}

			// Viewport & Scissor
			std::vector<VkDynamicState> dynamic_states;
			VkPipelineDynamicStateCreateInfo dynamic_state = {};
			VkViewport vkViewport = {};
			VkRect2D scissor = {};
			VkPipelineViewportStateCreateInfo viewport_state = {};
			{
				// If no viewport has been provided, assume dynamic
				if ( !m_pipeline_state.m_viewport.is_valid() )
				{
					dynamic_states.emplace_back( VK_DYNAMIC_STATE_VIEWPORT );
				}

				if ( m_pipeline_state.dynamic_scissor )
				{
					dynamic_states.emplace_back( VK_DYNAMIC_STATE_SCISSOR );
				}

				// Dynamic states
				dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
				dynamic_state.pNext = nullptr;
				dynamic_state.flags = 0;
				dynamic_state.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
				dynamic_state.pDynamicStates = dynamic_states.data();

				// Viewport
				vkViewport.x = m_pipeline_state.m_viewport.m_area.get_left();
				vkViewport.y = m_pipeline_state.m_viewport.m_area.get_top();
				vkViewport.width = m_pipeline_state.m_viewport.m_area.get_width();
				vkViewport.height = m_pipeline_state.m_viewport.m_area.get_height();
				vkViewport.minDepth = m_pipeline_state.m_viewport.m_min_depth;
				vkViewport.maxDepth = m_pipeline_state.m_viewport.m_max_depth;

				// Scissor
				if ( !m_pipeline_state.m_scissor.is_valid() )
				{
					scissor.offset = { 0, 0 };
					scissor.extent.width = static_cast<uint32_t>(vkViewport.width);
					scissor.extent.height = static_cast<uint32_t>(vkViewport.height);
				}
				else
				{
					scissor.offset = { static_cast<int32_t>(m_pipeline_state.m_scissor.get_left()), static_cast<int32_t>(m_pipeline_state.m_scissor.get_top()) };
					scissor.extent.width = static_cast<uint32_t>(m_pipeline_state.m_scissor.get_width());
					scissor.extent.height = static_cast<uint32_t>(m_pipeline_state.m_scissor.get_height());
				}

				// Viewport state
				viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
				viewport_state.viewportCount = 1;
				viewport_state.pViewports = &vkViewport;
				viewport_state.scissorCount = 1;
				viewport_state.pScissors = &scissor;
			}

			// Shader stages
			std::vector<VkPipelineShaderStageCreateInfo> shader_stages;

			// Vertex shader
			if ( m_pipeline_state.m_vertex_shader )
			{
				VkPipelineShaderStageCreateInfo shader_stage_info_vertex = {};
				shader_stage_info_vertex.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shader_stage_info_vertex.stage = VK_SHADER_STAGE_VERTEX_BIT;
				shader_stage_info_vertex.module = static_cast<VkShaderModule>(m_pipeline_state.m_vertex_shader->GetResource());
				shader_stage_info_vertex.pName = m_pipeline_state.m_vertex_shader->GetEntryPoint().data();

				// Validate shader stage
				ASSERT_TRUE( shader_stage_info_vertex.module );
				ASSERT_TRUE( shader_stage_info_vertex.pName );

				shader_stages.push_back( shader_stage_info_vertex );
			}
			else
			{
				ASSERT_FAILED( "Vertex shader is invalid" );
				return;
			}

			// Pixel shader
			if ( m_pipeline_state.m_pixel_shader )
			{
				VkPipelineShaderStageCreateInfo shader_stage_info_pixel = {};
				shader_stage_info_pixel.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				shader_stage_info_pixel.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				shader_stage_info_pixel.module = static_cast<VkShaderModule>(m_pipeline_state.m_pixel_shader->GetResource());
				shader_stage_info_pixel.pName = m_pipeline_state.m_pixel_shader->GetEntryPoint().data();

				// Validate shader stage
				ASSERT_TRUE( shader_stage_info_pixel.module );
				ASSERT_TRUE( shader_stage_info_pixel.pName );

				shader_stages.push_back( shader_stage_info_pixel );
			}

			// Binding description
			VkVertexInputBindingDescription binding_description = {};
			binding_description.binding = 0;
			binding_description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			binding_description.stride = m_pipeline_state.vertex_buffer_stride;

			// Vertex attributes description
			std::vector<VkVertexInputAttributeDescription> vertex_attribute_descs;
			if ( m_pipeline_state.m_vertex_shader )
			{
				if ( auto input_layout = m_pipeline_state.m_vertex_shader->get_input_layout() )
				{
					vertex_attribute_descs.reserve( input_layout->GetAttributeDescriptions().size() );
					for ( const auto& desc : input_layout->GetAttributeDescriptions() )
					{
						vertex_attribute_descs.push_back
						( {
							desc.location,              // location
							desc.binding,               // binding
							vulkan_format[desc.format], // format
							desc.offset                 // offset
						  } );
					}
				}
			}

			// Vertex input state
			VkPipelineVertexInputStateCreateInfo vertex_input_state = {};
			{
				vertex_input_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
				vertex_input_state.vertexBindingDescriptionCount = 1;
				vertex_input_state.pVertexBindingDescriptions = &binding_description;
				vertex_input_state.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertex_attribute_descs.size());
				vertex_input_state.pVertexAttributeDescriptions = vertex_attribute_descs.data();
			}

			// Input assembly
			VkPipelineInputAssemblyStateCreateInfo input_assembly_state = {};
			{
				input_assembly_state.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
				input_assembly_state.topology = vulkan_primitive_topology[m_pipeline_state.primitive_topology];
				input_assembly_state.primitiveRestartEnable = VK_FALSE;
			}

			// Rasterizer state
			VkPipelineRasterizationStateCreateInfo rasterizer_state = {};
			VkPipelineRasterizationDepthClipStateCreateInfoEXT rasterizer_state_depth_clip = {};
			{
				rasterizer_state_depth_clip.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_DEPTH_CLIP_STATE_CREATE_INFO_EXT;
				rasterizer_state_depth_clip.pNext = nullptr;
				rasterizer_state_depth_clip.flags = 0;
				rasterizer_state_depth_clip.depthClipEnable = true;// m_pipeline_state.rasterizer_state->GetDepthClipEnabled();

				rasterizer_state.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
				rasterizer_state.pNext = &rasterizer_state_depth_clip;
				rasterizer_state.depthClampEnable = VK_FALSE;
				rasterizer_state.rasterizerDiscardEnable = VK_FALSE;
				/*rasterizer_state.polygonMode = vulkan_polygon_mode[m_pipeline_state.rasterizer_state->GetFillMode()];
				rasterizer_state.lineWidth = m_device.get_context()->device_features.features.wideLines ? m_pipeline_state.rasterizer_state->GetLineWidth() : 1.0f;
				rasterizer_state.cullMode = vulkan_cull_mode[m_pipeline_state.rasterizer_state->GetCullMode()];*/
				rasterizer_state.frontFace = VK_FRONT_FACE_CLOCKWISE;
				rasterizer_state.depthBiasEnable = VK_TRUE;// m_pipeline_state.rasterizer_state->GetDepthBias() != 0.0f ? VK_TRUE : VK_FALSE;
				/*rasterizer_state.depthBiasConstantFactor = Math::Helper::Floor( m_pipeline_state.rasterizer_state->GetDepthBias() * (float)(1 << 24) );
				rasterizer_state.depthBiasClamp = m_pipeline_state.rasterizer_state->GetDepthBiasClamp();
				rasterizer_state.depthBiasSlopeFactor = m_pipeline_state.rasterizer_state->GetDepthBiasSlopeScaled();*/
			}

			// Mutlisampling
			VkPipelineMultisampleStateCreateInfo multisampling_state = {};
			{
				multisampling_state.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
				multisampling_state.sampleShadingEnable = VK_FALSE;
				multisampling_state.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			}

			VkPipelineColorBlendStateCreateInfo color_blend_state = {};
			std::vector<VkPipelineColorBlendAttachmentState> blend_state_attachments;
			{
				// Blend state attachments
				{
					// Same blend state for all
					VkPipelineColorBlendAttachmentState blend_state_attachment = {};
					blend_state_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
					blend_state_attachment.blendEnable = VK_TRUE;// m_pipeline_state.blend_state->GetBlendEnabled() ? VK_TRUE : VK_FALSE;
					/*blend_state_attachment.srcColorBlendFactor = vulkan_blend_factor[m_pipeline_state.blend_state->GetSourceBlend()];
					blend_state_attachment.dstColorBlendFactor = vulkan_blend_factor[m_pipeline_state.blend_state->GetDestBlend()];
					blend_state_attachment.colorBlendOp = vulkan_blend_operation[m_pipeline_state.blend_state->GetBlendOp()];
					blend_state_attachment.srcAlphaBlendFactor = vulkan_blend_factor[m_pipeline_state.blend_state->GetSourceBlendAlpha()];
					blend_state_attachment.dstAlphaBlendFactor = vulkan_blend_factor[m_pipeline_state.blend_state->GetDestBlendAlpha()];
					blend_state_attachment.alphaBlendOp = vulkan_blend_operation[m_pipeline_state.blend_state->GetBlendOpAlpha()];*/

					// Swapchain
					if ( m_pipeline_state.m_render_target_swapchain )
					{
						blend_state_attachments.push_back( blend_state_attachment );
					}

					// Render target(s)
					for ( uint8_t i = 0; i < rhi_max_render_target_count; i++ )
					{
						if ( m_pipeline_state.render_target_color_textures[i] != nullptr )
						{
							blend_state_attachments.push_back( blend_state_attachment );
						}
					}
				}

				color_blend_state.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
				color_blend_state.logicOpEnable = VK_FALSE;
				color_blend_state.logicOp = VK_LOGIC_OP_COPY;
				color_blend_state.attachmentCount = static_cast<uint32_t>(blend_state_attachments.size());
				color_blend_state.pAttachments = blend_state_attachments.data();
				/*color_blend_state.blendConstants[0] = m_pipeline_state.blend_state->GetBlendFactor();
				color_blend_state.blendConstants[1] = m_pipeline_state.blend_state->GetBlendFactor();
				color_blend_state.blendConstants[2] = m_pipeline_state.blend_state->GetBlendFactor();
				color_blend_state.blendConstants[3] = m_pipeline_state.blend_state->GetBlendFactor();*/
			}

			// Depth-stencil state
			VkPipelineDepthStencilStateCreateInfo depth_stencil_state = {};
			{
				depth_stencil_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
				/*depth_stencil_state.depthTestEnable = m_pipeline_state.depth_stencil_state->GetDepthTestEnabled();
				depth_stencil_state.depthWriteEnable = m_pipeline_state.depth_stencil_state->GetDepthWriteEnabled();
				depth_stencil_state.depthCompareOp = vulkan_compare_operator[m_pipeline_state.depth_stencil_state->GetDepthComparisonFunction()];
				depth_stencil_state.stencilTestEnable = m_pipeline_state.depth_stencil_state->GetStencilTestEnabled();
				depth_stencil_state.front.compareOp = vulkan_compare_operator[m_pipeline_state.depth_stencil_state->GetStencilComparisonFunction()];
				depth_stencil_state.front.failOp = vulkan_stencil_operation[m_pipeline_state.depth_stencil_state->GetStencilFailOperation()];
				depth_stencil_state.front.depthFailOp = vulkan_stencil_operation[m_pipeline_state.depth_stencil_state->GetStencilDepthFailOperation()];
				depth_stencil_state.front.passOp = vulkan_stencil_operation[m_pipeline_state.depth_stencil_state->GetStencilPassOperation()];
				depth_stencil_state.front.compareMask = m_pipeline_state.depth_stencil_state->GetStencilReadMask();
				depth_stencil_state.front.writeMask = m_pipeline_state.depth_stencil_state->GetStencilWriteMask();*/
				depth_stencil_state.front.reference = 1;
				depth_stencil_state.back = depth_stencil_state.front;
			}

			// Pipeline
			VkGraphicsPipelineCreateInfo pipeline_info = {};
			{
				// Describe
				pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
				pipeline_info.stageCount = static_cast<uint32_t>(shader_stages.size());
				pipeline_info.pStages = shader_stages.data();
				pipeline_info.pVertexInputState = &vertex_input_state;
				pipeline_info.pInputAssemblyState = &input_assembly_state;
				pipeline_info.pDynamicState = dynamic_states.empty() ? nullptr : &dynamic_state;
				pipeline_info.pViewportState = &viewport_state;
				pipeline_info.pRasterizationState = &rasterizer_state;
				pipeline_info.pMultisampleState = &multisampling_state;
				pipeline_info.pColorBlendState = &color_blend_state;
				pipeline_info.pDepthStencilState = &depth_stencil_state;
				pipeline_info.layout = static_cast<VkPipelineLayout>(m_pipeline_layout);
				pipeline_info.renderPass = static_cast<VkRenderPass>(m_pipeline_state.GetRenderPass());

				// Create
				auto pipeline = reinterpret_cast<VkPipeline*>(&m_pipeline);
				if ( vkCreateGraphicsPipelines( m_device.get_context()->device, nullptr, 1, &pipeline_info, nullptr, pipeline ) != VK_SUCCESS )
					return;

				// Name
				//vulkan_utility::debug::set_name( *pipeline, m_pipeline_state.pass_name );
			}
		}
	}

	PIPELINE_VULKAN::~PIPELINE_VULKAN()
	{
		// Wait in case it's still in use by the GPU
		//m_device.Queue_WaitAll();

		vkDestroyPipeline( m_device.get_context()->device, m_pipeline, nullptr );
		m_pipeline = nullptr;

		vkDestroyPipelineLayout( m_device.get_context()->device, m_pipeline_layout, nullptr );
		m_pipeline_layout = nullptr;
	}
}
