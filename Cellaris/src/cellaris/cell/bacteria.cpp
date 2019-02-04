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

#include "bacteria.h"


//void Bacteria::set_number_particles(int numparticles)
//{
//	m_number_particles = numparticles;
//}
//
//int Bacteria::get_number_particles()
//{
//	return m_number_particles;
//}
//void Bacteria::set_particle_offset(int particleoffset)
//{
//	m_particle_offset = particleoffset;
//}
//
//int Bacteria::get_particle_offset()
//{
//	return m_particle_offset;
//}
//
//
//void Bacteria::set_number_springs(int numbersprings)
//{
//	m_number_springs = numbersprings;
//}
//
//void Bacteria::set_spring_offset(int springoffset)
//{
//	m_spring_offset = springoffset;
//}
//
//int Bacteria::get_spring_offset()
//{
//	return m_spring_offset;
//}
//
//int Bacteria::get_number_springs()
//{
//	return m_number_springs;
//}

Cell* Bacteria::divide()
{
	/** Find number of births, used to allocate new ID for daughter cell */
	//unsigned numberBirths = Scene::Instance()->getNumBirths();

	/** Reset the birthtime of the mother cell */
	reset_cell();

	/** Create a new daughter cell */
	//Cell* child(new Cell());
	Cell* daughter = new Bacteria();

	daughter->set_birth_time(SceneTime::instance()->get_time()); /** Allocate the birth time of new cell to current simulation time*/
	//daughter->set_cell_id(numberBirths + 1); /** New daughter cell ID is set as the number of births */

	daughter->set_flex_particle_count(get_flex_particle_count()); /** TO CHANGE: number of particles will vary, depending on cell-type */
	//daughter->set_particle_offset(Scene::Instance()->getNumberActiveParticles()); /** Particle offset for flex buffers need seperate methods*/
	daughter->set_flex_spring_buffer_offset(get_flex_spring_count());
	daughter->set_flex_spring_count(get_flex_spring_count());

	return daughter;
}

bool Bacteria::ready_to_grow()
{
	double growth_prob = 0.01;

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> prob(0,1);

	if (prob(gen) <= growth_prob)
	{
		return true;
	} else {
		return false;
	}

}

bool Bacteria::ready_to_divide()
{

	int max_size = 6;

	if (get_flex_particle_count() == max_size) {
		return true;
	}
	else {
		return false;
	}

}

void Bacteria::grow_cell()
{
	/* For a bacterium the growth involves adding a single particle */
	set_flex_particle_count(get_flex_particle_count() + 1);

	/* We add an additional two springs for each growth step */
	set_flex_spring_count(get_flex_spring_count() + 2);
}

