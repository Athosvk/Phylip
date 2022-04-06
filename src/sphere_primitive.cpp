#include "sphere_primitive.hpp"

#include "defs.hpp"
#include "shader_map.hpp"

#include <cassert>
#include <cmath>

namespace phyl {
	SpherePrimitive::SpherePrimitive(int radius) : radius(radius) {
		materialDiffuse = glm::vec3(1.0, 0.4, 0.0);
		const int sectorCount = 32;
		const int stackCount = 32;
		std::vector<glm::vec3>().swap(vertices);
		std::vector<glm::vec3>().swap(normals);
		//std::vector<glm::vec2>().swap(texCoords);

		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord

		float sectorStep = 2 * M_PI / sectorCount;
		float stackStep = PI / stackCount;
		float sectorAngle, stackAngle;

		for(int i = 0; i <= stackCount; ++i)
		{
			stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			xy = radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for(int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				vertices.push_back(glm::vec3(x,y,z));

				// normalized vertex normal (nx, ny, nz)
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				normals.push_back(glm::vec3(nx,ny,nz));

				// vertex tex coord (s, t) range between [0, 1]
				//s = (float)j / sectorCount;
				//t = (float)i / stackCount;
				//texCoords.push_back(s);
				//texCoords.push_back(t);
			}
		}

		indices.clear();
		int k1, k2;
		for(int i = 0; i < stackCount; ++i) {
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for(int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if(i != 0) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if(i != (stackCount-1)) {
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}
	}

	void SpherePrimitive::unload() {
	}

	void SpherePrimitive::draw(const glm::mat4& projView, const glm::vec3 &cameraPos) {
		ShaderMap::Instance()->getShader(SHADER_BASE)->use();
		ShaderMap::Instance()->getShader(SHADER_BASE)->setUniform("projView", projView);
		ShaderMap::Instance()->getShader(SHADER_BASE)->setUniform("model", transform.getTransformationMatrix());
		ShaderMap::Instance()->getShader(SHADER_BASE)->setUniform("viewPos", cameraPos); 
		ShaderMap::Instance()->getShader(SHADER_BASE)->setUniform("material.diffuse", materialDiffuse);
		ShaderMap::Instance()->getShader(SHADER_BASE)->setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		ShaderMap::Instance()->getShader(SHADER_BASE)->setUniform("material.shininess", 32.0f);

		glDrawElements(GL_TRIANGLES, 3*indices.size(), GL_UNSIGNED_INT, 0);
	}

	void SpherePrimitive::update(const float dt) {
	}
}
