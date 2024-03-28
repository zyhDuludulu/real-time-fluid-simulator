#include "Renderer.h"
#include "GLFW/glfw3.h"


Renderer::Renderer(ParticleSystem* ps) : ps(ps) {
	glfwInit();
	window = glfwCreateWindow(800, 600, "2D Fluid Simulation", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		throw std::runtime_error("Failed to initialize GLAD");
	}

	glViewport(0, 0, 800, 600);
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glDisable(GL_DEPTH_TEST);
	
}

Renderer::~Renderer() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.3f, 0.3f, 0.5f, 1.0f);

	// glUseProgram(shader_program);
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, PARTICLES_NUM);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Renderer::shouldClose() {
	return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
}

void Renderer::bindVAO() {
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * PARTICLES_NUM, ps->m_pos, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}