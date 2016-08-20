#pragma once
#include <iostream>

namespace Rendering {
	namespace Materials {
		struct IMaterial {
		public:
			std::string name;

			virtual ~IMaterial();
		};

		inline IMaterial::~IMaterial() {}
	}
}