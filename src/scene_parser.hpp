#pragma once

#include "cloth_mesh.hpp"
#include "yaml-cpp/yaml.h"
#include <string>
#include <vector>

namespace phyl{
	class SceneParser {
		public:
			SceneParser(const std::string &fp);
			void GetCloth(std::shared_ptr<ClothMesh> &cloth);
			void GetPrimitives(std::vector<SpherePrimitive> &primitives);

		private:
			YAML::Node m_sceneNode;
	};
}
