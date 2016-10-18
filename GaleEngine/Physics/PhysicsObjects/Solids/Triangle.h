#pragma once

namespace Physics {
	namespace Particles {
		class Particle;
	}
	namespace PhysicsObjects {
		namespace Solids {

			class Triangle {
			public:
				Particles::Particle* p1;
				Particles::Particle* p2;
				Particles::Particle* p3;

				Triangle(Particles::Particle* p1, Particles::Particle* p2, Particles::Particle* p3);
				
				Particles::Particle* GetOther(Particles::Particle* pa, Particles::Particle* pb);
			};
		}
	}
}