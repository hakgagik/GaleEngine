// Modified from http://in2gpu.com/opengl-3/
#pragma once
#include "Shader_Manager.h"
#include "Model_Manager.h"
#include "../Core/IListener.h"

namespace Managers {
	class Scene_Manager : public Core::IListener {
	public:
		Scene_Manager();
		~Scene_Manager();

		virtual void notifyBeginFrame();
		virtual void notifyDisplayFrame();
		virtual void notifyEndFrame();
		virtual void notifyReshape(int width, int height, int previous_width, int previous_height);

	private:
		Managers::Shader_Manager* shader_manager;
		Managers::Model_Manager* model_manager;
	};
}