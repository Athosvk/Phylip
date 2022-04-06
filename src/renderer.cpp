#include "renderer.hpp"
#include "shader_map.hpp"

#include <iostream>

namespace phyl{
	Renderer::Renderer(Options *_options)
	: options{_options} {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		window = glfwCreateWindow(options->getInt("win_width", 800), options->getInt("win_height", 400), "Phylip", NULL, NULL);

		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, this);
		//glfwSetKeyCallback(m_window, keyboardCallback);
		//glfwSetMouseButtonCallback(m_window, mouseCallback);
		//glfwSetMouseButtonCallback(m_window, mouseCallback);
		//glfwSetWindowSizeCallback(m_window, windowSizeCallback);
		//glfwSetScrollCallback(m_window, scrollCallback);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		glEnable(GL_DEPTH_TEST);
		auto s = ShaderMap::Instance()->getShader(SHADER_BASE);
		s->loadShader("shaders/vert.glsl", GL_VERTEX_SHADER);
		s->loadShader("shaders/frag.glsl", GL_FRAGMENT_SHADER);
		s->compileShaders();
	}

	Renderer::~Renderer(){ 
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void Renderer::run(){
		int w = options->getInt("win_width", 800);
		int h = options->getInt("win_height", 400);
		glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
		while(!glfwWindowShouldClose(window)){
			glfwPollEvents();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, w, h);
			
			// TODO: Fixed timestep updates
			const float dt = 0;
			std::cout << "Current dt: "<< dt << "\n";
			update(dt);

			draw();

			glfwSwapBuffers(window);
		}
	}

	void Renderer::draw() {
		if(currScene){
			currScene->draw();
		}
	}

	void Renderer::setScene(std::shared_ptr<Scene> scene){
		currScene = scene;
	}

	void Renderer::update(const float dt) {
		if(currScene){
			currScene->update(dt);
		}
	}
}

