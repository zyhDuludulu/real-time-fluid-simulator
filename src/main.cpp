#include <chrono>
#include <ostream>
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
	expand_shader.loadVGFShader("../shader/milk.vs", "../shader/milk.gs", "../shader/milk.fs");

	// press 'q' to exit
	while (!rd.shouldClose()) {
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < Para::step; i++) {
			ps.findAllNeighbors();
			sv.solve();
		}
		// rd.update();
		expand_shader.use();
		rd.bindVAO();
		rd.render();
		auto end = std::chrono::high_resolution_clock::now();
		std::cout << "fps: " << 1e6 / std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;
	}
}
