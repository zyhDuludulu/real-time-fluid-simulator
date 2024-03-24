#include <chrono>
#include "ParticleSystem.h"
#include "Parameters.h"
#include "Debug.h"
#include "Solver.h"

int main() {
    ParticleSystem ps(100);
	DEBUG("Init ParticleSystem");
	ps.init();
	Solver sv = Solver(&ps);
	DEBUG("Init Solver");
	int frame = 0;
	while (true) {
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < Para::step; i++) {
			ps.findAllNeighbors();
			sv.solve();
		}
		// DEBUG("frame" << frame++);
		auto end = std::chrono::high_resolution_clock::now();
		DEBUG("fps: " << 1.0 / std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count() << "fps");
	}
}
