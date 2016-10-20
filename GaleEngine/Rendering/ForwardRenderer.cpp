#include "ForwardRenderer.h"
#include "GameObjects/IGameObject.h"
#include "GameObjects/Models/Model.h"
#include "GameObjects/Models/ModelClone.h"
#include "GameObjects/Models/fragment.h"
#include "Materials/IMaterial.h"
#include "Materials/MaterialsHeader.h"
#include "../Managers/Shader_Manager.h"
#include "GameObjects/Cameras/Camera.h"
#include "GameObjects/Lights/Light.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

using namespace Rendering;
using namespace GameObjects;
using namespace Cameras;
using namespace Lights;
using namespace Models;
using namespace Materials;
using namespace Managers;
using namespace std;
using namespace glm;

const GLenum ForwardRenderer::textureIndexMap[] = { GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8 };
const int ForwardRenderer::MAX_LIGHTS = 40;

ForwardRenderer* ForwardRenderer::instance = nullptr;

ForwardRenderer* ForwardRenderer::Get() {
	if (instance == nullptr) {
		instance = new ForwardRenderer();
	}
	return instance;
}

ForwardRenderer::ForwardRenderer() {}

ForwardRenderer::~ForwardRenderer() {}

void ForwardRenderer::Render(Model* model) {
	calculateOrientationMatrices(model);
	renderFromModel(model);
}

void ForwardRenderer::Render(ModelClone* model) {
	calculateOrientationMatrices(model);
	renderFromModel(model->source);
}

void ForwardRenderer::calculateOrientationMatrices(IGameObject* gameObject) {
	modelViewMatrix = viewMatrix * gameObject->toWorldMatrix;
	normalMatrix = transpose(inverse(mat3(modelViewMatrix)));
	MVPMatrix = projectionMatrix*modelViewMatrix;
}

void ForwardRenderer::renderFromModel(const Model* model) {
	vector<string> fragNames = model->GetFragNames();
	for (string f : fragNames) {
		const Fragment* fragment = model->GetFragment(f);
		if (SingleColorMaterial* singleColorMat = dynamic_cast<SingleColorMaterial*>(fragment->material)) {
			renderFragment(model, fragment, singleColorMat);
		}
		else if (LambertianMaterial* lambertianMat = dynamic_cast<LambertianMaterial*>(fragment->material)) {
			renderFragment(model, fragment, lambertianMat);
		}
		else if (BlinnPhongMaterial* blinnPhongMat = dynamic_cast<BlinnPhongMaterial*>(fragment->material)) {
			renderFragment(model, fragment, blinnPhongMat);
		}
		else {
			cout << "Forward Renderer: Can't find material for model  " << model->name << ". Rendering as white SingleColor." << endl;
			renderFragment(model, fragment, new SingleColorMaterial());
		}
	}
}

void ForwardRenderer::renderFragment(const Model* model, const Fragment* fragment, SingleColorMaterial* mat) {
	GLint loc;

	GLuint program = Shader_Manager::Get()->GetShader("single_color");
	glUseProgram(program);

	setMatrixUniforms(program);
	glBindVertexArray(model->GetVao());

	loc = glGetUniformLocation(program, "color");
	glUniform4fv(loc, 1, value_ptr(mat->diffuseColor));
	glDrawElements(fragment->primitiveType, fragment->indexCount, GL_UNSIGNED_INT, fragment->indexStartPointer);
}

void ForwardRenderer::renderFragment(const Model* model, const Fragment* fragment, LambertianMaterial* mat) {
	GLint loc;

	GLuint program = Shader_Manager::Get()->GetShader("lambertian");
	glUseProgram(program);
	glBindVertexArray(model->GetVao());

	setMatrixUniforms(program);
	setLightUniforms(program);

	loc = glGetUniformLocation(program, "diffuseColor");
	
	glUniform4fv(loc, 1, value_ptr(mat->diffuseColor));

	useTexture(program, "diffuseTexture", "hasDiffuseTexture", mat->diffuseTexture, 0);

	glDrawElements(fragment->primitiveType, fragment->indexCount, GL_UNSIGNED_INT, fragment->indexStartPointer);
}

void ForwardRenderer::renderFragment(const Model* model, const Fragment* fragment, BlinnPhongMaterial* mat) {
	GLint loc;

	GLuint program = Shader_Manager::Get()->GetShader("blinn_phong");
	glUseProgram(program);
	glBindVertexArray(model->GetVao());

	setMatrixUniforms(program);
	setLightUniforms(program);

	// TODO: test if setting uniforms is necessary every draw loop, or if I don't need to unless I update them
	loc = glGetUniformLocation(program, "diffuseColor");
	glUniform4fv(loc, 1, value_ptr(mat->diffuseColor));

	loc = glGetUniformLocation(program, "specularColor");
	glUniform4fv(loc, 1, value_ptr(mat->specularColor));

	loc = glGetUniformLocation(program, "exponent");
	glUniform1f(loc, mat->exponent);

	useTexture(program, "diffuseTexture", "hasDiffuseTexture", mat->diffuseTexture, 0);
	useTexture(program, "specularTexture", "hasSpecularTexture", mat->specularTexture, 1);
	useTexture(program, "exponentTexture", "hasExponentTexture", mat->exponentTexture, 2);

	glDrawElements(fragment->primitiveType, fragment->indexCount, GL_UNSIGNED_INT, fragment->indexStartPointer);
}

void ForwardRenderer::setCamera(Camera* camera) {
	this->camera = camera;
	projectionMatrix = camera->GetProjMatrix();
	viewMatrix = camera->GetViewMatrix();
	inverseViewMatrix = inverse(viewMatrix);
}

void ForwardRenderer::setLights(vector<Light*> lights) {
	mat4 view = camera->GetViewMatrix();
	this->lights.clear();
	this->lightPositions.clear();
	this->lightAttenuations.clear();
	this->lightColors.clear();
	this->lightCutoffs.clear();
	for (auto light : lights) {
		if (light->enabled) {
			this->lights.push_back(light);
			lightPositions.push_back(vec3(view * vec4(light->getPosition(), 1)));
			lightAttenuations.push_back(light->attenuation);
			lightColors.push_back(light->color);
			lightCutoffs.push_back(light->cutoff);
		}
	}
}

void ForwardRenderer::setMatrixUniforms(GLuint program) {
	GLint loc;
	loc = glGetUniformLocation(program, "MVPMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(MVPMatrix));
	loc = glGetUniformLocation(program, "modelViewMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(modelViewMatrix));
	loc = glGetUniformLocation(program, "projectionMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(projectionMatrix));
	loc = glGetUniformLocation(program, "normalMatrix");
	glUniformMatrix3fv(loc, 1, GL_FALSE, value_ptr(normalMatrix));
	loc = glGetUniformLocation(program, "viewMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(viewMatrix));
	loc = glGetUniformLocation(program, "inverseViewMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, value_ptr(inverseViewMatrix));
}

void ForwardRenderer::setLightUniforms(GLuint program) {
	//TODO figure out if we can use uniform buffers for this
	//TODO support more than point lights
	int loc = -1;
	int lightCount = glm::min(MAX_LIGHTS, (int)lights.size());

	loc = glGetUniformLocation(program, "lightCount");
	glUniform1i(loc, lightCount);

	// This SCREAMS uniform buffer
	loc = glGetUniformLocation(program, "lightPosition");
	glUniform3fv(loc, lightCount, &lightPositions[0].x);
	loc = glGetUniformLocation(program, "lightAttenuation");
	glUniform3fv(loc, lightCount, &lightAttenuations[0].x);
	loc = glGetUniformLocation(program, "lightColor");
	glUniform3fv(loc, lightCount, &lightColors[0].x);
	loc = glGetUniformLocation(program, "lightCutoff");
	glUniform1fv(loc, lightCount, &lightCutoffs[0]);
}

void ForwardRenderer::useTexture(GLuint program, const GLchar* uniformName, const GLchar* hasUniformName, const Texture* texture, int textureUnit) {
	GLint hasTexUniLoc = glGetUniformLocation(program, hasUniformName);
	GLint texUniLoc = glGetUniformLocation(program, uniformName);
	if (texture != nullptr && texUniLoc != -1 && hasTexUniLoc != -1) {
		glActiveTexture(textureIndexMap[textureUnit]);
		glBindTexture(GL_TEXTURE_2D, texture->textureLocation);
		glUniform1i(texUniLoc, textureUnit);
		glUniform1i(hasTexUniLoc, GL_TRUE);
	}
	else {
		glUniform1i(hasTexUniLoc, GL_FALSE);
	}
}

void ForwardRenderer::unuseTexture(GLuint program, Texture* texture) {
	//TODO ForwardRenderer::unuseTexture
}
//void ForwardRenderer::bindPosNorTexTanAttributes(GLuint program, Model* model, Fragment* fragment) {}