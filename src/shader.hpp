#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <string>
#include <vector>
namespace phyl{
class Shader {
	public:
		Shader();
		~Shader();

		void use();
		void compileShaders();
		int loadShader(const char* filePath, int type);
		int checkCompileErrors(unsigned int shader, std::string type);

		void setUniform(const std::string &name, const float v) const;
		void setUniform(const std::string &name, const int v) const;
		void setUniform(const std::string &name, const glm::mat4 &v) const;
		void setUniform(const std::string &name, const glm::vec3 &v) const;
		void setUniform(const std::string &name, const float a, const float b, const float c) const;

	private:
		unsigned int programID;
		std::vector<int> shadersID;
		bool loaded;
};
}
