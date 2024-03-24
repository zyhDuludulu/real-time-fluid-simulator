#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <cstdint>
#include <vector>
#include "glm/glm.hpp"
#include "Container.h"
#include "Debug.h"


struct Particle {
	glm::vec2 	position;
	glm::vec2 	velocity;
	glm::vec2 	acceleration;
	float 		density;
	float 		pressure;
	float 		press_div_dens2;
};

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
	explicit ParticleSystem(int numParticles, float delta_time = 0.01f);

	void update();

	void findAllNeighbors();

	void init();

	void solve();

private:
	void addParticle();
	void findNeighbors(uint32_t particle_id, uint32_t block_id);
	void fillContainer();

public:
	float m_support_radius 		= 0.025f;
	float m_support_radius2 	= m_support_radius * m_support_radius;
	float m_particle_radius	 	= 0.005f;
	float m_particle_diameter 	= m_particle_radius * 2.0f;
	float m_volume 				= 0.8f * m_particle_radius * m_particle_radius;
	float m_particle_mass 		= 0.02f;
	float m_viscosity 			= 0.01f;
	float m_presure_exponent 	= 7.0f;
	float m_stiffness 			= 50.0f;
	uint32_t m_particle_num 	= 1000;
	std::vector<Particle> m_particles;				/* all particles will be saved here*/
	std::vector<std::vector<Neighbor>> m_neighbors; /* neighbors for each particles */
	Container m_container;							/* container */

private:
	float m_delta_time;								/* time step */
};

#endif // PARTICLESYSTEM_H