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
		static Material_Manager* Get();

		~Material_Manager();

		void DeleteMaterial(const std::string& materialName);
		void LoadFromJSON(nlohmann::json &j);
		void WriteToJSON(nlohmann::json &j);
		const Rendering::Materials::IMaterial* GetMaterial(const std::string &materialName) const;
		const void AddMaterial();

	private:
		static Material_Manager* instance;
		Material_Manager();
		std::unordered_map<std::string, Rendering::Materials::IMaterial*> materialList; // I'm a materialList! I'm a materialLIst!
	};
}