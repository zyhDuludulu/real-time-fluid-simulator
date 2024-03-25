#include "Solver.h"
#include "ParticleSystem.h"
#include "Parameters.h"
#include "Debug.h"
#include <vector>
#include <cmath>
#include <cstring>


void
Solver::solve() {
	updateDensityAndPressure();
	updateGravity();
	updateViscosity();
	updatePressure();
	updatePosAndVelocity();
	updateBoundary();
}

void
Solver::updateDensityAndPressure() {
	for (int i = 0; i < PARTICLES_NUM; i++) {
		// DEBUG("BG " << i);
		float density = 0.0f;
		if (!m_ps->m_neighbors[i].empty()) {
			// DEBUG("in");
			for (const auto & j : m_ps->m_neighbors[i]) {
				density += m_kernel.value(j.distance);
			}
			// DEBUG("out");
			density *= Para::volume * Para::density0;
		}
		// no expantion
		m_ps->m_density[i] = std::max(density, Para::density0);
		// update pressure
		m_ps->m_pressure[i] = Para::stiffness * (powf(m_ps->m_density[i] / Para::density0, Para::m_exponent) - 1.f);
	}	
}

void
Solver::updateGravity() {
	// set gravity to (0, -gravity)
	for (auto & i : m_ps->m_acceleration)
		i = glm::vec2(0.f, -Para::gravity);
}

void
Solver::updateViscosity() {
	float dim = 2.0;
	float const_factor = (dim + 2.0f) * Para::viscosity;
	for (int i = 0; i < PARTICLES_NUM; i++) {
		std::vector<Neighbor>& neighbors = m_ps->m_neighbors[i];
		if (neighbors.empty()) { continue; }
		glm::vec2 viscosity_force(0.f, 0.f);
		for (auto& n_info : neighbors) {
			int j = n_info.id;
			float dot_dv_to_rad = glm::dot(m_ps->m_velocity[i] - m_ps->m_velocity[j], n_info.radius);
			float denom = n_info.distance2 + 0.01f * Para::support_radius2;
			viscosity_force += (Para::particle_mass / m_ps->m_density[j])
							* dot_dv_to_rad * m_kernel.gradient(n_info.radius) / denom;
		}
		viscosity_force *= const_factor;
		m_ps->m_acceleration[i] += viscosity_force;
	}
}

void
Solver::updatePressure() {
	memset(m_ps->m_pres_per_dens2, 0, sizeof(float) * PARTICLES_NUM);
	for (int i = 0; i < PARTICLES_NUM; i++) {
		m_ps->m_pres_per_dens2[i] = m_ps->m_pressure[i]
						/ powf(m_ps->m_density[i], 2);
	}

	for (int i = 0; i < PARTICLES_NUM; i++) {
		if (m_ps->m_neighbors.empty()) { continue; }
		std::vector<Neighbor>& neighbors = m_ps->m_neighbors[i];
		glm::vec2 pressure_force( 0.f);
		for (auto& n_info : neighbors) {
			int j = n_info.id;
			pressure_force += m_ps->m_density[j]
						* (m_ps->m_pres_per_dens2[i] + m_ps->m_pres_per_dens2[j]) 
						* m_kernel.gradient(n_info.radius);
		}
		m_ps->m_acceleration[i] += pressure_force * Para::volume;
	}
}

void Solver::updatePosAndVelocity() {
	for (int i = 0; i < PARTICLES_NUM; i++) {
		m_ps->m_velocity[i] += Para::dt * m_ps->m_acceleration[i];
		m_ps->m_velocity[i] = glm::clamp(m_ps->m_velocity[i], 
								glm::vec2(-100.f), 
								glm::vec2(100.f));
		m_ps->m_pos[i] += Para::dt * m_ps->m_velocity[i];
	}
}

void Solver::updateBoundary() {
	for (int i = 0; i < PARTICLES_NUM; i++) {
		glm::vec2& velocity = m_ps->m_velocity[i];
		glm::vec2& position = m_ps->m_pos[i];
		bool crush = false;
		if (position.y < m_ps->m_container.lower + Para::particle_radius) {
			velocity.y = std::abs(velocity.y);
			crush = true;
		}
		if (position.y > m_ps->m_container.upper - Para::particle_radius) {
			velocity.y = -std::abs(velocity.y);
			crush = true;
		}
		if (position.x < m_ps->m_container.left + Para::particle_radius) {
			velocity.x = std::abs(velocity.x);
			crush = true;
		}
		if (position.x > m_ps->m_container.right - Para::particle_radius) {
			velocity.x = -std::abs(velocity.x);
			crush = true;
		}

		if (crush) {
			position += Para::dt * velocity;
			velocity = glm::clamp(velocity, glm::vec2(-100.f), glm::vec2(100.f));	
		}
	}
}