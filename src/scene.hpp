#pragma once 

#include "raylib.h"
#include "sphere_primitive.hpp"
#include "options.hpp"

#include <vector>

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
		//std::shared_ptr<MMesh> mesh;
};
};
