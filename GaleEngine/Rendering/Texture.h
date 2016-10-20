#pragma once
#include <string>
#include "GL/glew.h"

namespace Rendering {
	class Texture {
	public:
		Texture();
		Texture(const std::string &filename);
		~Texture();

		void Destroy();

		unsigned char* data;
		unsigned int width;
		unsigned int height;
		GLuint textureLocation;
		std::string name;
	private:
		void LoadFromBMPFile(const std::string &filename);
	};

	struct BMP_Header
	{
		unsigned char type[2];
		int f_lenght;
		short rezerved1;
		short rezerved2;
		int offBits;
	};

	struct BMP_Header_Info
	{
		int size;
		int width;
		int height;
		short planes;
		short bitCount;
		int compresion;
		int sizeImage;
		int xPelsPerMeter;
		int yPelsPerMeter;
		int clrUsed;
		int clrImportant;
	};
}