#pragma once
#include "GL/glew.h"
#include "../lib/json.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <unordered_map>

namespace Rendering {
	class Texture;
	class GlyphTexture;
}

namespace Managers {
	class Texture_Manager {
	public:
		static Texture_Manager& Get();

		~Texture_Manager();
		
		void Init();
		void DeleteTexture(const std::string& textureName);
		const Rendering::Texture* GetTexture(const std::string& textureName) const;
		const Rendering::GlyphTexture* GetCharTexture(char c) const;
		Rendering::Texture* LoadandAddTexture(const std::string& filename);
		void LoadFromJSON(nlohmann::json &j);
		void WriteToJSON(nlohmann::json &j);

	private:
		static Texture_Manager instance;

		FT_Library ft;
		FT_Face face;
		FT_GlyphSlot g;

		Texture_Manager();
		//Texture_Manager(const Texture_Manager &);
		void buildFontTextures();
		std::unordered_map<std::string, Rendering::Texture*> textureList;
		std::unordered_map<char, Rendering::GlyphTexture*> charTexList;
	};
}