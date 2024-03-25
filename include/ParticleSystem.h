#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <cstdint>
#include <vector>
#include "glm/glm.hpp"
#include "Container.h"
#include "Debug.h"
#include "Parameters.h"


struct Neighbor {
	uint32_t id;
	float distance;
	float distance2;
	glm::vec2 radius;
};

class ParticleSystem {
public:
	ParticleSystem() = delete;
	~ParticleSystem() = default;
	explicit ParticleSystem(float delta_time = 0.01f);

	void update();

	void findAllNeighbors();

	void init();

	void solve();

private:
	void addParticle();
	void findNeighbors(uint32_t particle_id, uint32_t block_id);
	void fillContainer();

public:
	std::vector<std::vector<Neighbor>> m_neighbors; /* neighbors for each particles */
	Container m_container;							/* container */
	float 		m_density 		[PARTICLES_NUM];
	float 		m_pressure		[PARTICLES_NUM];
	float 		m_pres_per_dens2[PARTICLES_NUM];
	glm::vec2 	m_acceleration	[PARTICLES_NUM];
	glm::vec2 	m_velocity		[PARTICLES_NUM];
	glm::vec2 	m_pos			[PARTICLES_NUM];

private:
	float m_delta_time;								/* time step */
};

#endif // PARTICLESYSTEM_H