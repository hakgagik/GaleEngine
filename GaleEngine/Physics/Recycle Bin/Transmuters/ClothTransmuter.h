#pragma once
#include "Transmuter.h"

namespace Physics {
	namespace Transmuters {
		class ClothTransmuter : public Transmuter {
		public:
			virtual void Transmute() override;
			virtual nlohmann::json GetJSON() override;
		private:
			void transmuteToBasicMaterial();
		};
	}
}