
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_device_base.h"

#include <memory>

namespace gui
{
	struct RENDERING_CONTEXT_VULKAN;
	enum RHI_Queue_Type;

	class RENDERING_DEVICE_VULKAN : public RENDERING_DEVICE_BASE
	{
	public:
		RENDERING_DEVICE_VULKAN();
		~RENDERING_DEVICE_VULKAN();

		RENDERING_CONTEXT_VULKAN* get_context() const { return m_context.get(); }

		uint32_t Queue_Index( RHI_Queue_Type type ) const;

		void* Queue_Get( RHI_Queue_Type type ) const;

		bool Queue_Submit( const RHI_Queue_Type type, const uint32_t wait_flags, void* cmd_buffer, void* wait_semaphore /*= nullptr*/, void* signal_semaphore /*= nullptr*/, void* signal_fence /*= nullptr*/ ) const;

		bool Queue_Wait( RHI_Queue_Type type ) const;

		bool Queue_WaitAll() const;

	private:
		void set_api_version();

		bool create_instance();

		bool create_device();

		std::unique_ptr<RENDERING_CONTEXT_VULKAN> m_context;
	};
}
