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
			class ModelClone;
		}
	}
}

namespace Managers {
	class Model_Manager {
	public:
		Rendering::GameObjects::Models::ModelClone* getSphereCopy(std::string name);
		Rendering::GameObjects::Models::ModelClone* getCubeCopy(std::string name);
		//Model* getRectCopy(string name);

		Model_Manager();
		~Model_Manager();
		
		void Draw(Rendering::IRenderer* renderer);
		void Update();
		void DeleteModel(const std::string& gameModelName);
		void DeleteClone(const std::string& cloneNme);
		Rendering::GameObjects::Models::Model* CreateAndAdd(std::string name, std::vector<Rendering::VertexFormat> &verts, std::vector<unsigned int> &indices, glm::vec4 color = glm::vec4(1, 1, 1, 1));
		Rendering::GameObjects::Models::ModelClone* CloneAndAddd(std::string name, Rendering::GameObjects::Models::Model* source);
		void BuildAndAddFromOBJ(const std::string& filename);
		Rendering::GameObjects::Models::Model* PromoteToModel(Rendering::GameObjects::Models::ModelClone* clone);
		const Rendering::GameObjects::Models::Model* GetModel(const std::string& gameModelName) const;
		const Rendering::GameObjects::Models::ModelClone* GetClone(const std::string& cloneName) const;

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
		std::map<std::string, Rendering::GameObjects::Models::ModelClone*> cloneList;
	};
}