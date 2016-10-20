#pragma once
#include "GL/glew.h"
#include "../lib/json.hpp"
#include <string>
#include <unordered_map>

namespace Rendering {
	class Texture;
}

namespace Managers {
	class Texture_Manager {
	public:
		static Texture_Manager& Get();

		~Texture_Manager();
		
		void Init();
		void DeleteTexture(const std::string& textureName);
		const Rendering::Texture* GetTexture(const std::string& textureName) const;
		Rendering::Texture* LoadandAddTexture(const std::string& filename);
		void LoadFromJSON(nlohmann::json &j);
		void WriteToJSON(nlohmann::json &j);

	private:
		static Texture_Manager instance;

		Texture_Manager();
		//Texture_Manager(const Texture_Manager &);
		std::unordered_map<std::string, Rendering::Texture*> textureList;
	};
}