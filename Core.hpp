#ifndef _SHADE_ENGINE_CORE_
#define _SHADE_ENGINE_CORE_
#include <iostream>
#include <regex>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace Shade {
	std::function<void()> Init;
	std::function<void(float)> Update;
	std::function<void()> Render;

	GLenum err;
	auto CheckForErrors() -> void;

	class Window {
		private:
			GLFWwindow *window;
			int width, height;
			float FPS = 60.0;
			int gl_version_major, gl_version_minor;

		public:
			Window(int width, int height, char* title);
			~Window();
			auto Width() -> int;
			auto Height() -> int;
			auto GameLoopPre4_3() -> void;
			auto GameLoopPost4_3() -> void;
			auto IsKeyDown(int key) -> bool;
			auto ShouldClose() -> bool;
			auto PollEvents() -> void;
	};
}

auto Shade::CheckForErrors() -> void {
	while ((err = glGetError()) != GL_NO_ERROR){
		if (err == GL_INVALID_ENUM){
			fprintf( stderr, "GL CALLBACK: ** GL ERROR ** message = GL_INVALID_ENUM\n");
		} else if (err == GL_INVALID_VALUE){
			fprintf( stderr, "GL CALLBACK: ** GL ERROR ** message = GL_INVALID_VALUE\n");
		} else if (err == GL_INVALID_OPERATION){
			fprintf( stderr, "GL CALLBACK: ** GL ERROR ** message = GL_INVALID_OPERATION\n");
		} else if (err == GL_STACK_OVERFLOW){
			fprintf( stderr, "GL CALLBACK: ** GL ERROR ** message = GL_STACK_OVERFLOW\n");
		} else if (err == GL_STACK_UNDERFLOW){
			fprintf( stderr, "GL CALLBACK: ** GL ERROR ** message = GL_STACK_UNDERFLOW\n");
		} else if (err == GL_OUT_OF_MEMORY){
			fprintf( stderr, "GL CALLBACK: ** GL ERROR ** message = GL_OUT_OF_MEMORY\n");
		} else if (err == GL_INVALID_FRAMEBUFFER_OPERATION){
			fprintf( stderr, "GL CALLBACK: ** GL ERROR ** message = GL_INVALID_FRAMEBUFFER_OPERATION\n");
		} else if (err == GL_CONTEXT_LOST){
			fprintf( stderr, "GL CALLBACK: ** GL ERROR ** message = GL_CONTEXT_LOST\n");
		}
		exit(1);
	}
}

Shade::Window::Window(int width, int height, char* title){
	this->width = width;
	this->height = height;

	glewExperimental = true;
	if (!glfwInit()){
		std::cout << "Failed to initialize GLFW" << std::endl;
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (this->window == NULL){
		std::cout << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(this->window);

	glfwSetKeyCallback(this->window, [](auto *window, int key, int scancode, int action, int mods){
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	});

	if (glewInit() != GLEW_OK){
		std::cout << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
		exit(1);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shade::Init();

	glGetIntegerv(GL_MAJOR_VERSION, &this->gl_version_major);
	glGetIntegerv(GL_MINOR_VERSION, &this->gl_version_minor);
	std::cout << "OpenGL Version: " << this->gl_version_major << "." << this->gl_version_minor << std::endl;
	if ((this->gl_version_major == 4 && this->gl_version_minor >= 3) || (this->gl_version_major > 4)){
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](GLenum source, GLenum type, GLenum id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam){
			fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
			exit(1);
		}, 0);
		this->GameLoopPost4_3();
	} else {
		this->GameLoopPre4_3();
	}
}

Shade::Window::~Window(){
	glfwTerminate();
}

auto Shade::Window::Width() -> int {
	return this->width;
}

auto Shade::Window::Height() -> int {
	return this->height;
}

auto Shade::Window::GameLoopPre4_3() -> void {
	double frameTime = 1.0 / this->FPS;
	double lastTime, currentTime, dt, nextFrame = 0;
	lastTime = glfwGetTime();
	double temp = 0.0;
	do {
		currentTime = glfwGetTime();	
		dt = currentTime - lastTime;
		nextFrame += dt;
		temp += dt;
		lastTime = currentTime;

		if (nextFrame >= frameTime) {
			Shade::Update(nextFrame);
			Shade::Render();
			nextFrame = 0.0;
			this->PollEvents();
			Shade::CheckForErrors();
		}
	} while (!this->ShouldClose());
}

auto Shade::Window::GameLoopPost4_3() -> void {
	double frameTime = 1.0 / this->FPS;
	double lastTime, currentTime, dt, nextFrame = 0;
	lastTime = glfwGetTime();
	double temp = 0.0;
	do {
		currentTime = glfwGetTime();	
		dt = currentTime - lastTime;
		nextFrame += dt;
		temp += dt;
		lastTime = currentTime;

		if (nextFrame >= frameTime) {
			Shade::Update(nextFrame);
			Shade::Render();
			nextFrame = 0.0;
			this->PollEvents();
		}
	} while (!this->ShouldClose());
}

auto Shade::Window::IsKeyDown(int key) -> bool {
	return glfwGetKey(this->window, key);
}

auto Shade::Window::ShouldClose() -> bool {
	return glfwWindowShouldClose(this->window) == GLFW_TRUE;
}

auto Shade::Window::PollEvents() -> void {
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}
#endif
