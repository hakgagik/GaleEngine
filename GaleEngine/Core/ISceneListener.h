// Modified from http://in2gpu.com/opengl-3/
#pragma once

namespace Core {
	class ISceneListener {
	public:
		bool sceneInitialized;

		virtual ~ISceneListener() = 0;

		//drawing functions
		virtual void NotifyBeginFrame() = 0;
		virtual void NotifyDisplayFrame() = 0;
		virtual void NotifyEndFrame() = 0;
		virtual void NotifyReshape(int width, int height, int preview_width, int previous_height) = 0;
	};

	inline ISceneListener::~ISceneListener() {
		// implementation of pure virtual destructor
	}
}