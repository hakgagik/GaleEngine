// Modified from http://in2gpu.com/2015/07/22/texture-mapping/
#include "Texture.h"
#include <iostream>
#include <fstream>

using namespace Rendering;
using namespace std;

Texture::Texture() {}

Texture::~Texture() {
	Destroy();
}

void Texture::Destroy() {
	delete data;
	glDeleteTextures(1, &textureLocation);
}

Texture::Texture(const string &filename) {
	name = filename;
	unsigned int dotPos = filename.find('.');
	string extension = filename.substr(dotPos);

	if (extension == ".bmp" || extension == ".BMP") {
		LoadFromBMPFile(filename);
	}
	else {
		cout << "Texture Loader: Unknown file type" << endl;
		return;
	}

	glGenTextures(1, &textureLocation); //ask gl for a texture object
	glBindTexture(GL_TEXTURE_2D, textureLocation); // bind it as the current one being edited.

	// Using mipmap settings for now, until I figure out how to actually set this stuff up.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set maximum anisotropy to the maximum value allowed by our GPU/OpenGL implementation
	float maxAniso;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

	// Because we were forced to reformat the BMP (since RGB is in reverse order), we threw out the padding
	// Now, we have to make sure GL knows that the data's rows are not padded (by default, it assumes they are)
	// glPixelStorei(GL_UNPACK_ALIGNMENT, pvalue) tells GL how to read data
	// with pvalue telling it what number row sizes are guaranteed to be divisible by (aka their alignment).
	// Acceptable values are 1, 2, 4, and 8
	// Here, rows are unpaded, so no alignmnet is guaranteed
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Tell GL to read the data
	// format is glTexImage2D(target, level, internalformat, width, height, border, format, type, data)
	// target: the texture target this can be GL_TEXTURE_2D or any one of the six cubemap textures
	// level: specifies the mipmap level to load to (0 being unscaled image)
	// internalformat: how to store pixel data. Can be one of: GL_ALPHA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA
	// width/heigh: self-explanatory
	// boder: width of the border (?), but this value must be 0
	// format: specifies how the data is stored in the buffer. must match internalFormat
	// data: actual source of the data in an array (pointer to the first element)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	// Tell gl to automatically generate a mipmap for us for the image between the minimum and maximum mipmap level of the target image.
	// This automatically populates everything between those two levels.
	glGenerateMipmap(GL_TEXTURE_2D);
}


void Texture::LoadFromBMPFile(const string &filename) {
	ifstream file(filename.c_str(), ios::in | ios::binary);
	if (!file.good()) {
		cout << "Texture Loader: Cannot open texture file " << filename << endl;
		width = 0;
		height = 0;
		return;
	}
	// Read a bunch of header information from the BMP file
	BMP_Header h; BMP_Header_Info h_info;
	file.read((char*)&(h.type[0]), sizeof(char));
	file.read((char*)&(h.type[1]), sizeof(char));
	file.read((char*)&(h.f_lenght), sizeof(int));
	file.read((char*)&(h.rezerved1), sizeof(short));
	file.read((char*)&(h.rezerved2), sizeof(short));
	file.read((char*)&(h.offBits), sizeof(int));
	file.read((char*)&(h_info), sizeof(BMP_Header_Info));

	// Allocate the array in which we store data
	data = new unsigned char[h_info.width * h_info.height * 3];
	// Bitmaps require each row to start with a memory address that is a multiple of 4.
	// If row size % 4 != 0, rows are padded with the remainder
	long padd = 0;
	if ((h_info.width) % 4 != 0) padd = 4 - (h_info.width * 3 % 4);
	width = h_info.width;
	height = h_info.height;

	// pointer to keep track of where we are in the file stream, since we have to deal with padding
	long pointer;
	unsigned char r, g, b;

	for (unsigned int i = 0; i < height; i++) {
		for (unsigned int j = 0; j < width; j++) {

			//bitmaps store colors in reverse order
			file.read((char*)&b, 1);
			file.read((char*)&g, 1);
			file.read((char*)&r, 1);

			pointer = (i * width + j) * 3;
			data[pointer] = r;
			data[pointer + 1] = g;
			data[pointer + 2] = b;
		}

		// seek forward by the padding to make sure we skip the padded bits
		// (that were added to make this row length a multpile of 4)
		// FYI rows are called "Pixel Arrays"
		file.seekg(padd, ios_base::cur); // format for seekg is: seekg(amount to seek by, position to seek from). ios_base::cur is the current position
	}
	// always remember to close the file
	file.close();
}