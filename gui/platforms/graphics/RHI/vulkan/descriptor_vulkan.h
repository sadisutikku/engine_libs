
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_types.h"

#include <gui/thirdparty/vulkan/vulkan.h>
#include <string>

namespace gui
{
	struct DESCRIPTOR_VULKAN
	{
		~DESCRIPTOR_VULKAN() = default;

		DESCRIPTOR_VULKAN( const std::string& name, const RHI_Descriptor_Type type, const uint32_t slot, const uint32_t stage, const bool is_storage, const bool is_dynamic_constant_buffer )
		{
			this->type = type;
			this->slot = slot;
			this->stage = stage;
			this->is_storage = is_storage;
			this->is_dynamic_constant_buffer = is_dynamic_constant_buffer;
			this->name = name;
		}

		uint32_t slot = 0;
		uint32_t stage = 0;
		uint64_t offset = 0;
		uint64_t range = 0;
		RHI_Descriptor_Type type = RHI_Descriptor_Type::Undefined;
		RHI_Image_Layout layout = RHI_Image_Layout::Undefined;
		bool is_storage = false;
		bool is_dynamic_constant_buffer = false;
		std::string name = "";
		void* resource = nullptr;
	};
}
