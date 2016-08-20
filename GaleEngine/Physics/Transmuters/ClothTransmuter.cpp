#include "ClothTransmuter.h"
#include "../../Rendering/Materials/MaterialsHeader.h"
#include "../../Rendering/GameObjects/Models/Model.h"
#include "../../Rendering/GameObjects/Models/Fragment.h"
#include "../Particles/Particle.h"

using namespace Physics;
using namespace Particles;
using namespace Transmuters;
using namespace Rendering;
using namespace Materials;
using namespace GameObjects::Models;
using json = nlohmann::json;

void ClothTransmuter::Transmute() {
	Fragment* fragment = model->GetFragment(fragName);
	if (dynamic_cast<SingleColorMaterial*>(fragment->material) || dynamic_cast<LambertianMaterial*>(fragment->material) || dynamic_cast<BlinnPhongMaterial*>(fragment->material)) {
		transmuteToBasicMaterial();
	}
}

json ClothTransmuter::GetJSON() {
	json j;
	j["Type"] = "Cloth";
	j["Model"] = model->name;
	j["Fragment"] = fragName;
	return j;
}

void ClothTransmuter::transmuteToBasicMaterial() {
	Fragment* fragment = model->GetFragment(fragName);
	for (int i = 0; i < particleList.size(); i++) {
		model->UpdatePosition(i + fragment->startingVertex, particleList[i]->x);
	}
	model->RecalculateNormals();
	model->InvalidateVBO();
}