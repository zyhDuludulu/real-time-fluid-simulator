#ifndef SOLVER_H
#define SOLVER_H

#include "ParticleSystem.h"
#include "KernelFunc.h"


class Solver {
public:
	Solver() = delete;
	~Solver() = default;
	explicit Solver(ParticleSystem *ps) : m_ps(ps), m_kernel(m_ps->m_support_radius) {}

	void solve();

private:
	void updateDensityAndPressure();
	void updateGravity();
	void updateViscosity();
	void updatePressure();
	void updatePosAndVelocity();
	void updateBoundary();

	ParticleSystem* m_ps;
	KernelFunc m_kernel;

};

#endif // SOLVER_H