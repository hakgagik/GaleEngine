#pragma once
#include "../IGameObject.h"
#include "../../IRenderable.h"
#include "../../VertexFormat.h"
#include "../../../lib/json.hpp"
#include "GL/glew.h"
#include <vector>
#include <unordered_map>
#include <iostream>
#include <glm/glm.hpp>

namespace Rendering {
	struct VertexFormat;
	namespace Materials {
		struct IMaterial;
	}
	namespace GameObjects {
		namespace Models {
			class Fragment;

			class Model : public IGameObject {
			public:
				Model();
				Model(const Model* other, std::string name);				
				Model(std::vector<VertexFormat> &verts, std::vector<unsigned int> &indices, glm::vec4 color = glm::vec4(1.0f));
				virtual ~Model();

				virtual void Update();
				virtual void Destroy() override;
				virtual GLuint GetVao() const;
				virtual const std::vector<GLuint>& GetVbos() const;
				virtual void InvalidateVBO();

				virtual void UpdatePosition(int ind, glm::vec4 &pos);
				virtual void UpdatePosition(int ind, glm::vec3 &pos);
				virtual void UpdateNormal(int ind, glm::vec3 &norm);
				virtual void UpdateUV(int ind, glm::vec2 &uv);
				virtual void UpdateTangent(int ind, glm::vec4 &tan);
				virtual void UpdateTangent(int ind, glm::vec3 &tan);

				virtual void RecalculateNormals();
				virtual void RecalculateTangents();
				virtual Fragment* GetFragment(std::string fragName);
				std::vector<std::string> GetFragNames() const;
				void DeleteFragment(std::string fragName);
				Materials::IMaterial* GetFragmentMat(std::string fragName);
				void SetFragmentMat(std::string fragName, Materials::IMaterial* mat);
				//void AddFragment(Fragment* frag);
				nlohmann::json GetJSON();
				void PrintVBO();
			protected:
				std::unordered_map<std::string, Fragment*> fragments;
				GLuint vao;
				std::vector<GLuint> vbos;
				std::vector<VertexFormat> verts;
				std::vector<unsigned int> indices;
				bool updateVBO = false;
				bool calculatedNormals = false;
				bool calculatedTangents = false;
			};
		}
	}
}