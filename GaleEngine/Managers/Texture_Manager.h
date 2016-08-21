#pragma once
#include "GL/glew.h"
#include "../lib/json.hpp"
#include <string>
#include <map>

namespace Rendering {
	class Texture;
}

namespace Managers {
	class Texture_Manager {
	public:
		Texture_Manager();
		~Texture_Manager();
		
		void DeleteTexture(const std::string& textureName);
		const Rendering::Texture* GetTexture(const std::string& textureName) const;
		Rendering::Texture* LoadandAddTexture(const std::string& filename);
		void LoadFromJSON(nlohmann::json &j);
		void WriteToJSON(nlohmann::json &j);

	private:
		std::map<std::string, Rendering::Texture*> textureList;
	};
}