
#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/RHI_types.h"

#include <string>

namespace gui
{
	class PYHSICAL_DEVICE
	{
	public:
		PYHSICAL_DEVICE( const uint32_t api_version, const uint32_t driver_version, const uint32_t vendor_id, const RHI_PhysicalDevice_Type type, const char* name, const uint32_t memory, void* data )
			: m_vendor_id{ vendor_id }
			, m_type{ type }
			, m_name{ name }
			, m_memory{ memory / 1024u / 1024u } // mb
			, m_data{ data }
		{
			//m_api_version = decode_driver_version( api_version );
			//m_driver_version = decode_driver_version( driver_version );
		}

		/*
			0x10DE - Nvidia
			0x8086 - Intel
			0x1002 - Amd
			0x13B5 - ARM
			0x5143 - Qualcomm
			0x1010 - ImgTec

		*/
		bool IsNvidia()     const { return m_vendor_id == 0x10DE || m_name.find( "Nvidia" ) != std::string::npos; }
		bool IsAmd()        const { return m_vendor_id == 0x1002 || m_vendor_id == 0x1022 || m_name.find( "Amd" ) != std::string::npos; }
		bool IsIntel()      const { return m_vendor_id == 0x8086 || m_vendor_id == 0x163C || m_vendor_id == 0x8087 || m_name.find( "Intel" ) != std::string::npos; }
		bool IsArm()        const { return m_vendor_id == 0x13B5 || m_name.find( "Arm," ) != std::string::npos; }
		bool IsQualcomm()   const { return m_vendor_id == 0x5143 || m_name.find( "Qualcomm" ) != std::string::npos; }

		void* GetData() const { return m_data; }

	private:
		std::string m_api_version; // version of api supported by the device
		std::string m_driver_version; // vendor-specified version of the driver.
		uint32_t m_vendor_id{}; // unique identifier of the vendor
		RHI_PhysicalDevice_Type m_type{ RHI_PhysicalDevice_Unknown };
		std::string m_name;
		uint32_t m_memory{};
		void* m_data{};
	};
}
