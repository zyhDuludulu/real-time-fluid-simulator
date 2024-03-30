#include <chrono>
#include "ParticleSystem.h"
#include "Parameters.h"
#include "Debug.h"
#include "Renderer.h"
#include "Shader.h"
#include "Solver.h"


int main() {
	// init particle system
    ParticleSystem ps(0.01f);
	ps.init();
	DEBUG("Init ParticleSystem");

	// init solver
	Solver sv = Solver(&ps);
	DEBUG("Init Solver");

	// init renderer
	Renderer rd = Renderer(&ps);
	DEBUG("Init Renderer");

	// init shader
	// expand shader expands the particles to squares
	Shader expand_shader = Shader();
	expand_shader.loadVGFShader("../shader/expand.vs", "../shader/expand.gs", "../shader/expand.fs");
	// Save the result to a texture, then process the texture
	Shader milk_shader = Shader();
	milk_shader.loadVFShader("../shader/milk.vs", "../shader/milk.fs");
	milk_shader.setInt("textureSdf", 0);

	// press 'q' to exit
	while (!rd.shouldClose()) {

		//////////////   Simulation   //////////////
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < Para::step; i++) {
			ps.findAllNeighbors();
			sv.solve();
		}
		
		//////////////     Render     //////////////

		// Step 1: expand the particles to squares and draw the result to a texture
		glBindFramebuffer(GL_FRAMEBUFFER, rd.fbo);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		rd.bindVAO();

		expand_shader.use();
		glEnable(GL_PROGRAM_POINT_SIZE);
		glBindVertexArray(rd.vao);
		glDrawArrays(GL_POINTS, 0, PARTICLES_NUM);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(0);

		// Step 2: blur the texture and draw the milk to the screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		milk_shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rd.m_texure_sdf);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		//////////////      Draw      //////////////
		auto end = std::chrono::high_resolution_clock::now();
		float fps = 1e6 / std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		char title[100];
		sprintf(title, "2D Fluid Simulation - %.2f FPS", fps);
		glfwSetWindowTitle(rd.window, title);
		glfwSwapBuffers(rd.window);
		glfwPollEvents();
	}
}
