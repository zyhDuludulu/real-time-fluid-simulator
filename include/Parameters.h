#ifndef PARAMETERS_H
#define PARAMETERS_H

#define PARTICLES_NUM 600
#define PARTICLES_NUM_X 30

namespace Para {
	const float support_radius 		= 0.025f;
	const float support_radius2 	= support_radius * support_radius;
	const float particle_radius	 	= 0.005f;
	const float particle_diameter 	= particle_radius * 2.0f;
	const float volume 				= 0.8f * particle_radius * particle_radius;
	const float particle_mass 		= 0.02f;
	const float viscosity 			= 0.01f;
	const float stiffness 			= 50.0f;

	const float gravity 			= 9.8f;
	const float density0 			= 1000.0f;
	const float dt 					= 2e-4;
	const float m_exponent			= 7.0f;
	const int   step   				= 10;
}

#endif