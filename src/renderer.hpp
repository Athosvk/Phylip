#pragma once

#include "glad/glad.h"
#include "options.hpp"
#include "scene.hpp"

#include "GLFW/glfw3.h"

#include <vector>
#include <filesystem>

namespace phyl{
	class Renderer{
		public:
			Renderer(Options *option);
			~Renderer();

			void run();
			void addMesh(const std::filesystem::path &fp);
			void setScene(std::shared_ptr<Scene> scene);

		private:
			Options *options;
			std::shared_ptr<Scene> currScene;
			GLFWwindow *window;

			void draw();
			void update(const float dt);
	};
}
