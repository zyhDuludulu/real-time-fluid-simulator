#ifndef RENDERER_H
#define RENDERER_H

#include "ParticleSystem.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

class Renderer {
public:
	Renderer() = delete;
	explicit Renderer(ParticleSystem* ps);
	~Renderer();

	bool shouldClose() const;

	bool loadShader(const char* vertexPath, const char* fragmentPath);

	void bindVAO() const;


	unsigned int fbo;
	unsigned int vao;
	unsigned int vbo;

	unsigned int m_texure_sdf;
	GLFWwindow* window;

private:
	ParticleSystem* ps;
	
};

#endif // RENDERER_H