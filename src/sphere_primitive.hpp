#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "transform.hpp"

#include <string>
#include <vector>

namespace phyl {
	class SpherePrimitive{
		public:
			SpherePrimitive(int radius = 1);

			void draw(const glm::mat4& projView, const glm::vec3 &cameraPos);
			void update(const float dt);
			void unload();

		private:
			Transform transform;
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> normals;
			std::vector<unsigned int> indices;
			glm::vec3 materialDiffuse;
			//std::vector<glm::vec2> texCoords;
			
			int radius;
	};
};
