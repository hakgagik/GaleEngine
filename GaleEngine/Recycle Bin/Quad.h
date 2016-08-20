#pragma once
#include "Model.h"

namespace Rendering {
	namespace Models {
		class Quad : public Model {
		public:
			Quad();
			~Quad();

			void Create();
			virtual void Draw() override final;
			virtual void Update() override final;
		};
	}
}