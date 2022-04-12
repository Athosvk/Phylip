#pragma once 

#include "raylib.h"
#include "cloth_mesh.hpp"
#include "cloth_simulator.hpp"
#include "sphere_primitive.hpp"
#include "options.hpp"

#include <vector>
#include <memory>

namespace phyl{
class Scene {
	public: 
		Scene(Options* options);
		virtual void update(const float dt);
		virtual void draw() ;
		const Camera& getCamera() const;

	private:
		Camera camera;
		std::vector<SpherePrimitive> primitives;
		std::shared_ptr<ClothMesh> cloth;
		std::unique_ptr<ClothSimulator> simulator;
};
};
