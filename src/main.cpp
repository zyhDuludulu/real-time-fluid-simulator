#include <chrono>
#include <iomanip>
#include <ostream>
#include "ParticleSystem.h"
#include "Parameters.h"
#include "Debug.h"
#include "Renderer.h"
#include "Solver.h"

int main() {
	// init particle system
    ParticleSystem ps(600);
	ps.init();
	DEBUG("Init ParticleSystem");

	// init solver
	Solver sv = Solver(&ps);
	DEBUG("Init Solver");

	// init renderer
	Renderer rd = Renderer(&ps);
	DEBUG("Init Renderer");


	int frame = 0;
	// press 'q' to exit
	while (!rd.shouldClose()) {
		frame++;
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < Para::step; i++) {
			// DEBUG("FIND NEIGHBORS");
			ps.findAllNeighbors();
			// DEBUG("SOLVE");
			sv.solve();
			// DEBUG("SOLVE WAN LE");
		}
		// DEBUG("frame" << frame++);
		rd.render();
		auto end = std::chrono::high_resolution_clock::now();
		std::cerr << "\rfps: " << static_cast<float>(1.0 / std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count()) << std::setprecision(4);
	}
}
