#include <chrono>
#include "ParticleSystem.h"
#include "Parameters.h"
#include "Debug.h"
#include "Renderer.h"
#include "Solver.h"

int main() {
	// init particle system
    ParticleSystem ps(100);
	ps.init();
	DEBUG("Init ParticleSystem");

	// init solver
	Solver sv = Solver(&ps);
	DEBUG("Init Solver");

	// init renderer
	Renderer rd = Renderer(&ps);


	int frame = 0;
	// press 'q' to exit
	while (!rd.shouldClose()) {
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < Para::step; i++) {
			ps.findAllNeighbors();
			sv.solve();
		}
		// DEBUG("frame" << frame++);
		rd.render();
		auto end = std::chrono::high_resolution_clock::now();
		DEBUG("fps: " << 1.0 / std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count() << "fps");
	}
}
