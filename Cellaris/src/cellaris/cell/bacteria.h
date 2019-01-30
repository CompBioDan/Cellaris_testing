/**
*
* Cellaris is an open-source, multi-scale agent-based modelling framework to allow for the particle-constraint based modelling of
* complex cellular population dynamics using GPU optimised physics solving (via the FleX engine)
*
* Cellaris is developed as part of the Biocompute lab, University of Bristol, in collaboration with BrisSynBio
* see: https://biocomputelab.github.io/index.html
*
* Cellaris is developed using C++ in Microsoft Visual Studio, Windows 10
*
*
* @author Daniel Ward (daniel.ward@bristol.ac.uk)
* @version 1.0
*
**/

#ifndef BACTERIA_H_
#define BACTERIA_H_

#include "cell.h"

class Bacteria: public Cell {

public:

	//virtual void update();

	virtual Cell* divide();

	virtual bool ready_to_grow();

	virtual bool ready_to_divide();

	virtual void grow_cell();

	//void set_particle_offset(int particleoffset);

	//void set_number_particles(int numparticles);

	//void set_spring_offset(int springoffset);

	//void set_number_springs(int numbersprings);

	//int get_number_particles();

	//int get_particle_offset();

	//int get_number_springs();

	//int get_spring_offset();

	int number_growth_particles = 1;

	int number_growth_springs = 2;

	int max_particles = 6;

	int max_springs = 9;

	double growth_prob = 0.01;

private:

	//void grow();

	//int m_particle_offset; // integer position of first cell particle within the FleX buffers

	//int m_number_particles; // number of particles that form the cell

	//int m_spring_offset; // integer position of first cell spring within the FleX buffers

	//int m_number_springs; // number of springs forming the cell


};

#endif /* BACTERIA_H_ */