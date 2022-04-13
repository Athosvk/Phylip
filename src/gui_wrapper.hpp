#pragma once
#include <raylib.h>

namespace phyl{
	class Scene;

	class GuiWrapper{
		public:
			GuiWrapper(Scene& clothSimulator);

			void draw();
		private:
			void drawSettings(Rectangle offsetView);

			Scene& scene;
			Vector2 panelScroll = Vector2 { 0, 0 };
			Rectangle panelRectangle;
			Rectangle contentRectangle;
			bool showContentArea = false;
	};
};
