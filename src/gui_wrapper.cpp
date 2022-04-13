#include "gui_wrapper.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <string>
#include "scene.hpp"

namespace phyl{
	GuiWrapper::GuiWrapper(Scene& scene) :
		scene(scene) {
		GuiLoadStyle("../external/raygui/styles/dark/dark.rgs");

		panelRectangle = { 20, 10, 280, 350 };
		contentRectangle = { 0, 0, 260, 400 };
	}
	
	void GuiWrapper::draw(){
		Rectangle view = GuiScrollPanel(panelRectangle, "Settings", contentRectangle, &panelScroll);

		BeginScissorMode(view.x, view.y, view.width, view.height);
			drawSettings(Rectangle { view.x + panelScroll.x, view.y + panelScroll.y, view.width, view.height});
		EndScissorMode();
	}

	void GuiWrapper::drawSettings(Rectangle offsetView){
		const int Spacing = 40;
		std::string simulationState = scene.isSimulationPaused() ? "Play" : "Pause";
		Rectangle lastElement = Rectangle{ offsetView.x + 5, offsetView.y + 15, 60, 30 };
		if (GuiButton(lastElement, simulationState.c_str()))
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

		const int LabelSpacing = 165;
		ClothSimulator& clothSimulator = scene.getClothSimulator();

		lastElement.y += Spacing;
		GuiLabel(lastElement, "Elasticity Stiffness");
		lastElement.x += LabelSpacing;
		clothSimulator.setElasticStiffness(GuiSlider(lastElement, "0.0 ", 
			" 160", clothSimulator.getElasticStiffness(), 0.0, 160.f));
		lastElement.y += 20;
		GuiLabel(lastElement, TextFormat("%.2f", clothSimulator.getElasticStiffness()));

		lastElement.y += Spacing;
		lastElement.x -= LabelSpacing;
		GuiLabel(lastElement, "Bending Stiffness");
		lastElement.x += LabelSpacing;
		clothSimulator.setBendingStiffness(GuiSlider(lastElement, "0.0 ", 
			" 120", clothSimulator.getBendingStiffness(), 0.0, 120.f));
		lastElement.y += 20;
		GuiLabel(lastElement, TextFormat("%.2f", clothSimulator.getBendingStiffness()));

		lastElement.y += Spacing;
		lastElement.x -= LabelSpacing;
		GuiLabel(lastElement, "Attachment Stiffness");
		lastElement.x += LabelSpacing;
		clothSimulator.setAttachmentStiffness(GuiSlider(lastElement, "0.0 ", 
			" 120", clothSimulator.getAttachmentStiffness(), 0.0, 120.f));
		lastElement.y += 20;
		GuiLabel(lastElement, TextFormat("%.2f", clothSimulator.getAttachmentStiffness()));

		lastElement.y += Spacing;
		lastElement.x -= LabelSpacing;
		GuiLabel(lastElement, "Gravity");
		lastElement.x += LabelSpacing;
		clothSimulator.setGravity(GuiSlider(lastElement, "-100 ", 
			" 100", clothSimulator.getGravity(), -100., 100.f));
		lastElement.y += 20;
		GuiLabel(lastElement, TextFormat("%.2f", clothSimulator.getGravity()));

		lastElement.y += Spacing;
		lastElement.x -= LabelSpacing;
		GuiLabel(lastElement, "Dampening");
		lastElement.x += LabelSpacing;
		clothSimulator.setDampening(GuiSlider(lastElement, "0 ",
			" 0.1", clothSimulator.getDampening(), 0., 0.1));
		lastElement.y += 20;
		GuiLabel(lastElement, TextFormat("%.2f", clothSimulator.getDampening()));
	}
}
