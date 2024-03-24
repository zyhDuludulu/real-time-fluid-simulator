#include "ParticleSystem.h"
#include "Container.h"
#include <cstdint>
#include "Debug.h"


ParticleSystem::ParticleSystem(int numParticles, float delta_time)
	: m_particle_num(numParticles), m_delta_time(delta_time) {
	m_neighbors.resize(m_particle_num);
}

void 
ParticleSystem::addParticle() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < m_particle_num / 10; j++) {
			Particle p;
			p.position = glm::vec2(0.1f + i * 0.05f, 0.1f + j * 0.05f);
			ASSERT((p.position.x > 0 && p.position.x < 1.0 && p.position.y > 0 && p.position.y < 1), "Out of container");
			p.velocity = glm::vec2(0.0f);
			p.acceleration = glm::vec2(0.0f);
			p.density = 0.0f;
			p.pressure = 0.0f;
			p.press_div_dens2 = 0.0f;
			m_particles.push_back(p);
		}
	}
}

// May be could be written in cuda
void
ParticleSystem::update() {
	for (int i = 0; i < m_particle_num; i++) {
		uint32_t block_id = m_container.getBlockId(m_particles[i].position.x, m_particles[i].position.y);
		findNeighbors(i, block_id);
	}
}

// Can cut off some blocks for optimization
void 
ParticleSystem::findNeighbors(uint32_t particle_id, uint32_t block_id) {
	m_neighbors[particle_id].clear();
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (block_id + i + j * m_container.num_blocks_x < 0 || block_id + i + j * m_container.num_blocks_x >= m_container.num_blocks_x * m_container.num_blocks_y) {
				continue;
			}
			std::vector<uint32_t> &block = m_container.blocks[block_id + i + j * m_container.num_blocks_x];

			for (uint32_t k : block) {
				if (k == particle_id) {
					continue;
				}
				struct Neighbor n;
				n.id = k;
				n.radius = m_particles[k].position - m_particles[particle_id].position;
				n.distance2 = glm::dot(n.radius, n.radius);
				n.distance = sqrt(n.distance2);
				if (n.distance < m_support_radius) {
					m_neighbors[particle_id].push_back(n);
				}
			}
		}
	}
}

void ParticleSystem::findAllNeighbors() {
	for (int i = 0; i < m_particle_num; i++) {
		auto& particle = m_particles[i];
		uint32_t block_id = m_container.getBlockId(particle.position);
		findNeighbors(i, block_id);
	}
}

void 
ParticleSystem::init() {
	addParticle();
	m_container = Container(1.0f, 0.0f, 0.0f, 1.0f, 10, 10);
	m_container.initContainer();
	fillContainer();
}


void
ParticleSystem::fillContainer() {
	for (int i = 0; i < m_particle_num; i++) {
		int b_id = m_container.getBlockId(m_particles[i].position);
		m_container.blocks[b_id].push_back(i);
	}
}