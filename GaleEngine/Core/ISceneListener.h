// Modified from http://in2gpu.com/opengl-3/
#pragma once

namespace Core {
	class ISceneListener {
	public:
		//drawing functions
		virtual void NotifySceneInit() = 0;
		virtual void NotifyBeginFrame() = 0;
		virtual void NotifyDisplayFrame() = 0;
		virtual void NotifyEndFrame() = 0;
		virtual void NotifyReshape(int width, int height, int preview_width, int previous_height) = 0;
		virtual void NotifySceneClose() = 0;
	};
}