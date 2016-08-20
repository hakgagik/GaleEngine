#pragma once
#include <vector>
#include "../../lib/json.hpp"

namespace Rendering {
	namespace GameObjects {
		namespace Models {
			class Model;
		}
	}
}

namespace Physics {
	namespace Particles {
		class Particle;
	}
	namespace Transmuters {
		class Transmuter {
		public:
			virtual ~Transmuter() = 0;
			virtual void Transmute() = 0;
			virtual nlohmann::json GetJSON() = 0;
			virtual void BuildFromJSON(nlohmann::json j) = 0;
		protected:
			std::vector<Particles::Particle*> particleList;
			Rendering::GameObjects::Models::Model* model;
			std::string fragName;
		};
		inline Transmuter::~Transmuter(){ }
	}
}