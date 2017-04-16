#include "Texture_Manager.h"
#include "../Rendering/Texture.h"
#include "../Rendering/GlyphTexture.h"

using namespace Rendering;
using namespace Managers;
using namespace std;
using json = nlohmann::json;

Texture_Manager Texture_Manager::instance;

const string allChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789[]{}?-_=+!.:;, ";

void Texture_Manager::Init() {
	buildFontTextures();
}

Texture_Manager& Texture_Manager::Get() {
	return instance;
}

Texture_Manager::Texture_Manager() {
	string font("C:\\Windows\\Fonts\\consola.ttf");
	if (FT_Init_FreeType(&ft)) {
		cout << "Forward Renderer: Could not init freetype library." << endl;
		return;
	}

	if (FT_New_Face(ft, font.c_str(), 0, &face)) {
		cout << "Forward Renderer: Could not open font """ << font << """." << endl;
		return;
	}
	FT_Set_Pixel_Sizes(face, 0, 48);
	g = face->glyph;
}

Texture_Manager::~Texture_Manager()
{
	for (auto texture : textureList) {
		delete texture.second;
	}
	for (auto texture : charTexList) {
		delete texture.second;
	}
}

void Texture_Manager::DeleteTexture(const string & textureName)
{
	Texture* texture = textureList[textureName];
	texture->Destroy();
	textureList.erase(textureName);
}

const Texture * Texture_Manager::GetTexture(const string & textureName) const
{
	return textureList.at(textureName);
}

const GlyphTexture * Texture_Manager::GetCharTexture(char c) const {
	return charTexList.at(c);
}

Texture* Texture_Manager::LoadandAddTexture(const string & filename)
{
	Texture* texture = new Texture(filename);
	textureList[texture->name] = texture;
	return texture;
}

void Texture_Manager::LoadFromJSON(json &j)
{
	for (string tex : j["Textures"]) {
		LoadandAddTexture(tex);
	}
}

void Texture_Manager::WriteToJSON(json &j)
{
	for (auto kv : textureList) {
		j["Textures"].push_back(kv.first);
	}
}

void Texture_Manager::buildFontTextures() {
	for (char c : allChars) {
		charTexList[c] = new GlyphTexture(face, c);
	}
}