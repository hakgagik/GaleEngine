#pragma once
#include <string>
#include <map>
#include "../lib/json.hpp"

namespace Rendering {
	namespace Materials {
		struct IMaterial;
	}
}

namespace Managers {
	class Material_Manager {
	public:
		Material_Manager();
		~Material_Manager();

		void DeleteMaterial(const std::string& materialName);
		void LoadFromJSON(nlohmann::json &j);
		void WriteToJSON(nlohmann::json &j);
		const Rendering::Materials::IMaterial* GetMaterial(const std::string &materialName) const;
		const void AddMaterial();

	private:
		std::map<std::string, Rendering::Materials::IMaterial*> materialList; // I'm a materialList! I'm a materialLIst!
	};
}