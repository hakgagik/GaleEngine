#pragma once
#include "../IGameObject.h"
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
				std::string source;
				
				Model(std::string name);
				Model(const Model* other, std::string name);				
				Model(std::string name, std::vector<VertexFormat> &verts, std::vector<unsigned int> &indices, glm::vec4 color = glm::vec4(1.0f));
				virtual ~Model();

				virtual void Destroy() override;

				GLuint GetVao() const;
				void GenerateVAO();
				const std::vector<GLuint>& GetVbos() const;
				void InvalidateVBO();

				void Update();
				void UpdateVBO(bool force);
				void UpdateIBO(bool force);
				void UpdatePosition(int ind, glm::vec4 &pos);
				void UpdatePosition(int ind, glm::vec3 &pos);
				void UpdateNormal(int ind, glm::vec3 &norm);
				void UpdateUV(int ind, glm::vec2 &uv);
				void UpdateTangent(int ind, glm::vec4 &tan);
				void UpdateTangent(int ind, glm::vec3 &tan);

				void RecalculateNormals();
				void RecalculateTangents();

				const Fragment* GetFragment(std::string fragName) const;
				std::vector<std::string> GetFragNames() const;
				void DeleteFragment(std::string fragName);
				Materials::IMaterial* GetFragmentMat(std::string fragName);
				void SetFragmentMat(std::string fragName, Materials::IMaterial* mat);

				virtual nlohmann::json GetSourceJSON() const override;
				void PrintVBO() const;
			protected:
				std::unordered_map<std::string, Fragment*> fragments;
				GLuint vao;
				std::vector<GLuint> vbos;
				std::vector<VertexFormat> verts;
				std::vector<unsigned int> indices;
				bool vboInvalid = false;
				bool iboInvalid = false;
				bool calculatedNormals = false;
				bool calculatedTangents = false;
			};
		}
	}
}