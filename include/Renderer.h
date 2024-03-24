#ifndef RENDERER_H
#define RENDERER_H

#include "ParticleSystem.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Renderer {
public:
	Renderer() = delete;
	explicit Renderer(ParticleSystem* ps);
	~Renderer();

	void render();

	bool shouldClose();

private:
	GLFWwindow* window;
	ParticleSystem* ps;
};

#endif // RENDERER_H