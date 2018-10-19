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


void Bacteria::set_number_particles(int numparticles)
{
	m_number_particles = numparticles;
}

int Bacteria::get_number_particles()
{
	return m_number_particles;
}
void Bacteria::set_particle_offset(int particleoffset)
{
	m_particle_offset = particleoffset;
}

int Bacteria::get_particle_offset()
{
	return m_particle_offset;
}


void Bacteria::set_number_springs(int numbersprings)
{
	m_number_springs = numbersprings;
}

void Bacteria::set_spring_offset(int springoffset)
{
	m_spring_offset = springoffset;
}

int Bacteria::get_spring_offset()
{
	return m_spring_offset;
}

int Bacteria::get_number_springs()
{
	return m_number_springs;
}

void Bacteria::divide()
{
	/** Find number of births, used to allocate new ID for daughter cell */
	unsigned numberBirths = Scene::Instance()->getNumBirths();

	/** Reset the birthtime of the mother cell */
	reset_cell();

	/** Create a new daughter cell */
	//Cell* child(new Cell());
	Bacteria* daughter = new Bacteria();

	daughter->set_birth_time(SceneTime::Instance()->GetTime()); /** Allocate the birth time of new cell to current simulation time*/
	daughter->set_cell_id(numberBirths + 1); /** New daughter cell ID is set as the number of births */

	daughter->set_number_particles(get_number_particles()); /** TO CHANGE: number of particles will vary, depending on cell-type */
	daughter->set_particle_offset(Scene::Instance()->getNumberActiveParticles()); /** Particle offset for flex buffers need seperate methods*/
	daughter->set_spring_offset(get_number_springs());
	daughter->set_number_springs(get_number_springs());

}