#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(int numParticles, float delta_time)
	: m_particle_num(numParticles), m_delta_time(delta_time) {
	m_neighbors.resize(m_particle_num);
}

void 
ParticleSystem::addParticle() {
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < m_particle_num / 40; j++) {
			Particle p;
			p.position = glm::vec2(0.03f + i * 0.005f, 0.05f + j * 0.005f);
			ASSERT((p.position.x > m_container.left && p.position.x < m_container.right && p.position.y > m_container.lower && p.position.y < m_container.upper), "Out of container" << p.position.x << ' ' << p.position.y);
			p.velocity = glm::vec2(2.f, -2.f);
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
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (block_id + i + j * m_container.num_blocks_x < 0 || block_id + i + j * m_container.num_blocks_x >= m_container.num_blocks_x * m_container.num_blocks_y) {
				continue;
			}
			std::vector<uint32_t> &block = m_container.blocks[block_id + i + j * m_container.num_blocks_x];

			for (uint32_t k : block) {
				if (k == particle_id) { continue; }
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
	m_container.blocks = std::vector<std::vector<uint32_t>>(m_container.num_block, std::vector<uint32_t>(0));
	for (int i = 0; i < m_particle_num; i++) {
		auto& particle = m_particles[i];
		uint32_t block_id = m_container.getBlockId(particle.position);
		// DEBUG("shini fault lema? O_o?");
		m_container.blocks[block_id].push_back(i);
	}
	// DEBUG("Finsh adding blocks");
	m_neighbors = std::vector<std::vector<Neighbor>>(m_particle_num, std::vector<Neighbor>(0));
	for (int i = 0; i < m_particle_num; i++) {
		auto& particle = m_particles[i];
		uint32_t block_id = m_container.getBlockId(particle.position);
		findNeighbors(i, block_id);
	}
	// DEBUG("Find neighbors");
}

void 
ParticleSystem::init() {
	m_container = Container(0.5f, 0.f, 0.f, 0.5f, 10, 10);
	m_container.initContainer();
	addParticle();
	fillContainer();
}


void
ParticleSystem::fillContainer() {
	for (int i = 0; i < m_particle_num; i++) {
		int b_id = m_container.getBlockId(m_particles[i].position);
		m_container.blocks[b_id].push_back(i);
	}
}