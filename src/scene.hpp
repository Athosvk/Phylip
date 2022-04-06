#pragma once 

#include "camera.hpp"
#include "sphere_primitive.hpp"
#include "options.hpp"

#include <vector>
#include <memory>

namespace phyl{
class Scene {
	public: 
		Scene(Options *options);
		virtual void update(const float dt);
		virtual void draw() ;

	private:
		std::vector<SpherePrimitive> primitives;
		glm::mat4 proj;
		std::unique_ptr<Camera> camera;
		//std::shared_ptr<MMesh> mesh;
};
};
