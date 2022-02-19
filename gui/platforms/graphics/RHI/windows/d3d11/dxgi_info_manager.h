

#pragma once

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include <dxgidebug.h>
#include <string>
#include <vector>
#include <wrl/client.h>

namespace gui
{
	class DXGI_INFO_MANAGER
	{
	public:
		DXGI_INFO_MANAGER();
		~DXGI_INFO_MANAGER() = default;
		DXGI_INFO_MANAGER( const DXGI_INFO_MANAGER& ) = delete;
		DXGI_INFO_MANAGER& operator=( const DXGI_INFO_MANAGER& ) = delete;

		void Set() noexcept;

		std::vector<std::string> GetMessages() const;

	private:
		uint64_t next{ 0u };
		Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
	};
}
