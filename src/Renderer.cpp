#include "Renderer.h"


Renderer::Renderer(ParticleSystem* ps) : ps(ps) {
	glfwInit();
	window = glfwCreateWindow(800, 600, "2D Fluid Simulation", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewInit();

	glViewport(0, 0, 800, 600);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0f);
}

Renderer::~Renderer() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	for (const auto& p : ps->m_particles) {
		glVertex2f(p.position.x, p.position.y);
	}
	glEnd();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Renderer::shouldClose() {
	return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
}