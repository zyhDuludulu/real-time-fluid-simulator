#ifndef RENDERER_H
#define RENDERER_H

#include "ParticleSystem.h"
#include "Shader.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

class Renderer {
public:
	Renderer() = delete;
	explicit Renderer(ParticleSystem* ps);
	~Renderer();

	void render();

	bool shouldClose();

	bool loadShader(const char* vertexPath, const char* fragmentPath);

	void bindVAO();

private:
	GLFWwindow* window;
	ParticleSystem* ps;
	unsigned int VAO;
	unsigned int VBO;
};

#endif // RENDERER_H