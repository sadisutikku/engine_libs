

//////////////////////////////////////////////////////////////////////
//! Includes
//////////////////////////////////////////////////////////////////////

#include "gui/shared/graphics/RHI/texture2d_base.h"

#include "gui/shared/importers/image_importer.h"

namespace gui
{
	TEXTURE2D_BASE* TEXTURE2D_BASE::load_from_file( const std::string& filepath )
	{
		TEXTURE2D_BASE* texture = IMAGE_IMPORTER::get_singleton().load_from_file( filepath );

		return texture;
	}
}
