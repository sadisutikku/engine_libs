
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_types.h"

#include <string>

namespace gui
{
	struct DESCRIPTOR
	{
		DESCRIPTOR( const std::string& name, const RHI_Descriptor_Type type, const uint32_t slot, const uint32_t stage, const bool is_storage, const bool is_dynamic_constant_buffer )
		{
			this->type = type;
			this->slot = slot;
			this->stage = stage;
			this->is_storage = is_storage;
			this->is_dynamic_constant_buffer = is_dynamic_constant_buffer;
			this->name = name;
		}

		bool is_storage{};
		bool is_dynamic_constant_buffer{};
		RHI_Descriptor_Type type = RHI_Descriptor_Type::Undefined;
		RHI_Image_Layout layout = RHI_Image_Layout::Undefined;
		uint32_t slot{};
		uint32_t stage{};
		void* resource{};
		uint64_t offset{};
		uint64_t range{};
		std::string name = "";
	};
}
