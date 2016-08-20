// Modified from http://in2gpu.com/opengl-3/
#pragma once
#include "../Rendering/VertexFormat.h"
#include "../lib/json.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <map>

namespace Rendering {
	class IRenderer;
	namespace GameObjects {
		namespace Models {
			class Model;
		}
	}
}

namespace Managers {
	//TODO: Make this a gameobject manager?
	class Model_Manager {
	public:
		Rendering::GameObjects::Models::Model* getSphereCopy(std::string name);
		Rendering::GameObjects::Models::Model* getCubeCopy(std::string name);
		//Model* getRectCopy(string name);

		Model_Manager();
		~Model_Manager();
		
		void Draw(Rendering::IRenderer* renderer);
		void Update();
		void DeleteModel(const std::string& gameModelName);
		void AddModel(Rendering::GameObjects::Models::Model* model);
		Rendering::GameObjects::Models::Model* CreateAndAdd(std::string name, std::vector<Rendering::VertexFormat> &verts, std::vector<unsigned int> &indices, glm::vec4 color = glm::vec4(1, 1, 1, 1));
		void BuildAndAddFromOBJ(const std::string& filename);
		const Rendering::GameObjects::Models::Model* GetModel(const std::string& gameModelName) const;

		void LoadFromJSON(nlohmann::json &j);
		void WriteToJSON(nlohmann::json &j);

	private:
		static Rendering::GameObjects::Models::Model* createSphere(int thetaDiv = 16, int phiDiv = 32);
		static Rendering::GameObjects::Models::Model* createCube();
		static Rendering::GameObjects::Models::Model* createRect();
		Rendering::GameObjects::Models::Model* sphereTemplate;
		Rendering::GameObjects::Models::Model* cubeTemplate;
		Rendering::GameObjects::Models::Model* rectTemplate;
		std::map<std::string, Rendering::GameObjects::Models::Model*> modelList;
	};
}