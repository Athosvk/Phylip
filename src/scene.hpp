#pragma once 

#include "primitive.hpp"

#include <vector>

namespace phyl{
class Scene {
	public: 
		Scene();
		virtual void update(const float dt);
		virtual void draw() ;
		void addPrimitive(const std::string fp);

	private:
		std::vector<Primitive> primitives;
		//std::shared_ptr<MMesh> mesh;
};
};
