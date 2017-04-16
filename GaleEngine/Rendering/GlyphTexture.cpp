#include "GlyphTexture.h"

using namespace Rendering;

GlyphTexture::GlyphTexture(FT_Face face, char c) {

	glGenTextures(1, &textureLocation);
	glBindTexture(GL_TEXTURE_2D, textureLocation);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FT_Load_Char(face, c, FT_LOAD_RENDER);
	FT_GlyphSlot g = face->glyph;
	FT_Bitmap b = g->bitmap;
	width = b.width;
	height = b.rows;
	bitmap_left = g->bitmap_left;
	bitmap_top = g->bitmap_top;
	advance_x = (float)g->advance.x;
	advance_y = (float)g->advance.y;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, b.width, b.rows, 0, GL_RED, GL_UNSIGNED_BYTE, b.buffer);
}