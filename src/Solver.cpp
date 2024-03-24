# include "Solver.h"
#include "ParticleSystem.h"
#include "Parameters.h"
#include <vector>


void
Solver::solve() {
	updateDensityAndPressure();
	updateGravity();
	updateViscosity();
	updatePressure();
	updatePosAndVelocity();
}

void
Solver::updateDensityAndPressure() {
	for (int i = 0; i < m_ps->m_particle_num; i++) {
		if (!m_ps->m_neighbors.empty()) {
			float density = 0.0f;
			for (const auto & j : m_ps->m_neighbors[i]) {
				density += m_kernel->value(j.distance);
			}
			density *= m_ps->m_volume * Para::density0;
			// no expantion
			m_ps->m_particles[i].density = std::max(density, Para::density0);
		}
		// update pressure
		m_ps->m_particles[i].pressure = m_ps->m_stiffness * (powf(m_ps->m_particles[i].density / Para::density0, Para::m_exponent) - 1.f);
	}	
}

void
Solver::updateGravity() {
	for (auto& p : m_ps->m_particles) {
		p.acceleration = glm::vec2(0.0f, -Para::gravity);
	}
}

void
Solver::updateViscosity() {
	float dim = 2.0;
	float const_factor = 2.0f * (dim + 2.0f) * m_ps->m_viscosity;
	for (int i = 0; i < m_ps->m_particle_num; i++) {
		if (m_ps->m_neighbors.empty()) { break; }
		std::vector<Neighbor> neighbors = m_ps->m_neighbors[i];
		glm::vec2 viscosity_force(0.f, 0.f);
		for (auto& n_info : neighbors) {
			int j = n_info.id;
			float dot_dv_to_rad = glm::dot(m_ps->m_particles[i].velocity - m_ps->m_particles[j].velocity, n_info.radius);
			float denom = n_info.distance2 + 0.01f * m_ps->m_support_radius2;
			viscosity_force += (m_ps->m_particle_mass / m_ps->m_particles[j].density)
							* dot_dv_to_rad * m_kernel->gradient(n_info.radius) / denom;
		}
		viscosity_force *= const_factor;
		m_ps->m_particles[i].acceleration += viscosity_force;
	}
}

void
Solver::updatePressure() {
	std::vector<float> press_div_dens2(m_ps->m_particle_num, 0.0f);
	for (int i = 0; i < m_ps->m_particle_num; i++) {
		press_div_dens2[i] = m_ps->m_particles[i].pressure 
						/ (m_ps->m_particles[i].density * m_ps->m_particles[i].density);
	}

	for (int i = 0; i < m_ps->m_particle_num; i++) {
		if (m_ps->m_neighbors.empty()) { continue; }
		std::vector<Neighbor>& neighbors = m_ps->m_neighbors[i];
		glm::vec2 pressure_force(0.f, 0.f);
		for (auto& n_info : neighbors) {
			int j = n_info.id;
			pressure_force += m_ps->m_particles[j].density 
						* (press_div_dens2[i] + press_div_dens2[j]) 
						* m_kernel->gradient(n_info.radius);
		}
		m_ps->m_particles[i].acceleration -= pressure_force * m_ps->m_volume;
	}
}

void Solver::updatePosAndVelocity() {
	for (int i = 0; i < m_ps->m_particle_num; i++) {
		m_ps->m_particles[i].velocity += Para::dt * m_ps->m_particles[i].acceleration;
		m_ps->m_particles[i].velocity = glm::clamp(m_ps->m_particles[i].velocity, 
								glm::vec2(-100.f), 
								glm::vec2(100.f));
		m_ps->m_particles[i].position += Para::dt * m_ps->m_particles[i].velocity;
	}
}