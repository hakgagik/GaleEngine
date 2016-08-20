// Modified from http://in2gpu.com/opengl-3/
#pragma once

namespace Core {
	class ISceneListener {
	public:
		bool sceneInitialized;

		virtual ~ISceneListener() = 0;

		//drawing functions
		virtual void notifyBeginFrame() = 0;
		virtual void notifyDisplayFrame() = 0;
		virtual void notifyEndFrame() = 0;
		virtual void notifyReshape(int width, int height, int preview_width, int previous_height) = 0;
	};

	inline ISceneListener::~ISceneListener() {
		// implementation of pure virtual destructor
	}
}