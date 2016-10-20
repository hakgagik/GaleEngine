// Modified from http://in2gpu.com/opengl-3/
#pragma once
#include "../Rendering/VertexFormat.h"
#include "../lib/json.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <unordered_map>

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
		static Model_Manager& Get();

		Rendering::GameObjects::Models::ModelClone* getSphereCopy(std::string name);
		Rendering::GameObjects::Models::ModelClone* getCubeCopy(std::string name);
		Rendering::GameObjects::Models::ModelClone* getRectCopy(std::string name);

		~Model_Manager();
		
		void Init();
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
		void WriteModelsToSourceJSON();
		std::vector<std::string> GetModelNames();
		std::unordered_map<std::string, Rendering::GameObjects::Models::Model*> GetModelList();
		std::unordered_map<std::string, Rendering::GameObjects::Models::ModelClone*> GetCloneList();

		std::string ReadFile(const std::string &filename);

	private:
		static Model_Manager instance;

		Model_Manager();
		Model_Manager(const Model_Manager &other);
		void createSphereTemplate(unsigned int thetaDiv = 16, unsigned int phiDiv = 32);
		void createCubeTemplate();
		void createRectTemplate(unsigned int xDiv = 10, unsigned int yDiv = 10);
		Rendering::GameObjects::Models::Model* sphereTemplate;
		Rendering::GameObjects::Models::Model* cubeTemplate;
		Rendering::GameObjects::Models::Model* rectTemplate;
		std::unordered_map<std::string, Rendering::GameObjects::Models::Model*> modelList;
		std::unordered_map<std::string, Rendering::GameObjects::Models::ModelClone*> cloneList;
	};
}