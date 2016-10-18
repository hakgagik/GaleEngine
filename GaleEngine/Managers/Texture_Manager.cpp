#include "Texture_Manager.h"
#include "../Rendering/Texture.h"

using namespace Rendering;
using namespace Managers;
using namespace std;
using json = nlohmann::json;

Texture_Manager* Texture_Manager::instance = nullptr;

Texture_Manager* Texture_Manager::Get() {
	if (instance == nullptr) {
		instance = new Texture_Manager();
	}
	return instance;
}

Texture_Manager::Texture_Manager()
{
}

Texture_Manager::~Texture_Manager()
{
	for (auto texture : textureList) {
		delete texture.second;
	}
}

void Texture_Manager::DeleteTexture(const string & textureName)
{
	Texture* texture = textureList[textureName];
	texture->Destroy();
	textureList.erase(textureName);
}

const Texture * Texture_Manager::GetTexture(const string & textureName) const
{
	return textureList.at(textureName);
}

Texture* Texture_Manager::LoadandAddTexture(const string & filename)
{
	Texture* texture = new Texture(filename);
	textureList[texture->name] = texture;
	return texture;
}

void Texture_Manager::LoadFromJSON(json &j)
{
	for (string tex : j["Textures"]) {
		LoadandAddTexture(tex);
	}
}

void Texture_Manager::WriteToJSON(json &j)
{
	for (auto kv : textureList) {
		j["Textures"].push_back(kv.first);
	}
}