#pragma once 

#include "raylib.h"
#include "cloth_mesh.hpp"
#include "cloth_simulator.hpp"
#include "sphere_primitive.hpp"
#include "options.hpp"
#include "gui_wrapper.hpp"

#include <vector>
#include <memory>

namespace phyl{
class Scene {
	public: 
		Scene(Options* options);
		~Scene();
		virtual void update(const float dt);
		virtual void draw() ;
		void drawUI();
		const Camera& getCamera() const;

		void pauseSimulation();
		void runSimulation();
		bool isSimulationPaused() const;
		ClothSimulator& getClothSimulator() const;
	private:
		Camera camera;
		std::vector<SpherePrimitive> primitives;
		std::shared_ptr<ClothMesh> cloth;
		std::unique_ptr<ClothSimulator> simulator;
		double m_fixedDt;
		Shader m_shader;
		GuiWrapper m_guiWrapper;
		bool m_paused = true;
		bool m_movingSpheres = true;
};
};
