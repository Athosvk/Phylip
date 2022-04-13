#include "gui_wrapper.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <string>
#include "scene.hpp"

namespace phyl{
	GuiWrapper::GuiWrapper(Scene& scene) :
		scene(scene) {
		GuiLoadStyle("../external/raygui/styles/dark/dark.rgs");

		panelRectangle = { 20, 10, 200, 350 };
		contentRectangle = { 0, 0, 180, 750 };
	}
	
	void GuiWrapper::draw(){
		Rectangle view = GuiScrollPanel(panelRectangle, "Settings", contentRectangle, &panelScroll);

		BeginScissorMode(view.x, view.y, view.width, view.height);
			drawSettings(Rectangle { view.x + panelScroll.x, view.y + panelScroll.y, view.width, view.height});
		EndScissorMode();
	}

	void GuiWrapper::drawSettings(Rectangle offsetView){
		int style = GuiGetStyle(SCROLLBAR, BORDER_WIDTH);
		const int Spacing = 40;
		std::string simulationState = scene.isSimulationPaused() ? "Play" : "Pause";
		if (GuiButton(Rectangle{ offsetView.x + 5, offsetView.y + 15, 60, 30 }, simulationState.c_str()))
		{
			if (scene.isSimulationPaused())
			{
				scene.runSimulation();
			}
			else
			{
				scene.pauseSimulation();
			}
		}
		GuiSetStyle(SCROLLBAR, BORDER_WIDTH, style);
	}
}
