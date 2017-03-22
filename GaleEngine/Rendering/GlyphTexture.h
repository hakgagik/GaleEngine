#pragma once
#include "Texture.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Rendering {
	class GlyphTexture : public Texture {
	public:
		int bitmap_left;
		int bitmap_top;
		float advance_x;
		float advance_y;

		GlyphTexture(FT_Face face, char c);
	};
}