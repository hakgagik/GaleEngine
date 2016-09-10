#pragma once
#include <string>
#include <unordered_map>
#include "../lib/json.hpp"

namespace Rendering {
	namespace Materials {
		struct IMaterial;
	}
}

namespace Managers {
	class Texture_Manager;
	class Material_Manager {
	public:
		Material_Manager();
		~Material_Manager();

		void DeleteMaterial(const std::string& materialName);
		void LoadFromJSON(nlohmann::json &j, Texture_Manager*);
		void WriteToJSON(nlohmann::json &j);
		const Rendering::Materials::IMaterial* GetMaterial(const std::string &materialName) const;
		const void AddMaterial();

	private:
		std::unordered_map<std::string, Rendering::Materials::IMaterial*> materialList; // I'm a materialList! I'm a materialLIst!
	};
}