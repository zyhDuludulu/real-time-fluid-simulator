#include "ParticleSystem.h"
#include "Parameters.h"
#include <cstring>


ParticleSystem::ParticleSystem(float delta_time)
	: m_delta_time(delta_time) {
	m_neighbors.resize(PARTICLES_NUM);
}

void 
ParticleSystem::init() {
	// init particles data
	memset(m_density, 0.f, sizeof(m_density));
	memset(m_pressure, 0.f, sizeof(m_pressure));

	// Sequence is important below
	m_container = Container(0.5f, 0.f, 0.f, 0.5f, 20, 20);
	m_container.initContainer();
	addParticle();
	fillContainer();
}

void 
ParticleSystem::addParticle() {
	for (int i = 0; i < PARTICLES_NUM_X; i++) {
		for (int j = 0; j < PARTICLES_NUM / PARTICLES_NUM_X; j++) {
			int index = i * PARTICLES_NUM / PARTICLES_NUM_X + j;
			m_pos[index] = glm::vec2(0.03f + i * 0.005f, 0.05f + j * 0.005f);
			ASSERT((m_pos[index].x > m_container.left && m_pos[index].x < m_container.right && m_pos[index].y > m_container.lower && m_pos[index].y < m_container.upper), "Out of container" << m_pos[index].x << ' ' << m_pos[index].y);
			m_velocity[index] = glm::vec2(2.f, -2.f);
		}
	}
}

// May be could be written in cuda
void
ParticleSystem::update() {
	for (int i = 0; i < PARTICLES_NUM; i++) {
		uint32_t block_id = m_container.getBlockId(m_pos[i]);
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
				n.radius = m_pos[k] - m_pos[particle_id];
				n.distance2 = glm::dot(n.radius, n.radius);
				n.distance = sqrt(n.distance2);
				if (n.distance < Para::support_radius) {
					m_neighbors[particle_id].push_back(n);
				}
			}
		}
	}
}

void ParticleSystem::findAllNeighbors() {
	m_container.blocks = std::vector<std::vector<uint32_t>>(m_container.num_block, std::vector<uint32_t>(0));
	for (int i = 0; i < PARTICLES_NUM; i++) {
		uint32_t block_id = m_container.getBlockId(m_pos[i]);
		// DEBUG("shini fault lema? O_o?");
		m_container.blocks[block_id].push_back(i);
	}
	// DEBUG("Finsh adding blocks");
	m_neighbors = std::vector<std::vector<Neighbor>>(PARTICLES_NUM, std::vector<Neighbor>(0));
	for (int i = 0; i < PARTICLES_NUM; i++) {
		uint32_t block_id = m_container.getBlockId(m_pos[i]);
		findNeighbors(i, block_id);
	}
	// DEBUG("Find neighbors");
}

void
ParticleSystem::fillContainer() {
	for (int i = 0; i < PARTICLES_NUM; i++) {
		int b_id = m_container.getBlockId(m_pos[i]);
		m_container.blocks[b_id].push_back(i);
	}
}