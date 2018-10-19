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

#include "cell.h"


//virtual void update(){};


/* Method for setting the cell's birth time. Birth time can remain constant, with age calculated using simulation time */
void Cell::set_birth_time(double birthTime)
{
	m_cell_birth_time = birthTime;
}

/* Method for retrieving cell's birth time */
double Cell::get_birth_time() const
{
	return m_cell_birth_time;
}

/* Method for retrieving a cell age, calculated as the difference between current simulation time and the cell birth time */
double Cell::get_cell_age() const
{
	return SceneTime::instance()->get_time() - m_cell_birth_time;
}

/* Method for setting cell position. Called on cell birth and in the update() method following flex call */
void Cell::set_cell_position(myVec3d position)
{
	m_cell_position = position;
}

/* Method for retrieving cell position. Called to find cell position to input into flex physics solving */
myVec3d Cell::get_cell_position() const
{
	return m_cell_position;
}

/* Simple method fro allocating an id for each cell */
void Cell::set_cell_id(unsigned cellId)
{
	m_cell_id = cellId;
}

/* Method for retrieving cell id */
unsigned Cell::get_cell_id() const
{
	return m_cell_id;
}

/* Method for flagging cells which have reached the end of their cell cycle */
bool Cell::ready_to_divide()
{
	/** If cell has reached the end of its cell cycle then return true */
	if (Cell::get_cell_age() >= m_cell_cycle_length)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

/* 
Method for basic cell division.

Default division method acts on single particle 'cell', creating a new particle in a random position around the mother cell.
*/
Cell* Cell::divide()
{
	/** Find number of births, used to allocate new ID for daughter cell */
	//unsigned numberBirths = Scene::Instance()->getNumBirths();

	/** Reset the birthtime of the mother cell */
	reset_cell();

	/** Create a new daughter cell */
	//Cell* child(new Cell());
	Cell* daughter = new Cell();

	daughter->set_birth_time(SceneTime::instance()->get_time()); /** Allocate the birth time of new cell to current simulation time*/
	//daughter->set_cell_id(number_births + 1); /** New daughter cell ID is set as the number of births */

	// Place daughter cell close to mother cell
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> azim(-pi, pi);
	std::uniform_real_distribution<> zen(0.0, pi);
	std::uniform_real_distribution<> cc(-1.5, 1.5);


	myVec3d daughter_position = get_cell_position();
	daughter_position.pos.x = daughter_position.pos.x + (m_cell_radius*cos(azim(gen)*sin(zen(gen))));
	daughter_position.pos.y = daughter_position.pos.y + (m_cell_radius*sin(azim(gen)*sin(zen(gen))));
	daughter_position.pos.z = daughter_position.pos.z + (m_cell_radius*cos(zen(gen)));

	daughter->set_cell_position(daughter_position);
	daughter->set_cell_radius(m_cell_radius);
	//daughter->set_cell_cycle_length(m_cell_cycle_length + cc(gen));
	daughter->set_cell_cycle_length(m_cell_cycle_length);

	//Scene::Instance()->setNumberActiveParticles(Scene::Instance()->getNumberActiveParticles() + 1); /** TO CHANGE: Number of active particles probably returned from Flex instance*/

	///** Increment the number of births during the scene */
	//Scene::Instance()->setBirths(numberBirths + 1);

	/** Return the new cell */
	return daughter;
}

void Cell::reset_cell()
{
	/** reset the mother cell age following cell division */
	set_birth_time(SceneTime::instance()->get_time());
}

void Cell::set_cell_radius(double p_radius)
{
	m_cell_radius = p_radius;
}

double Cell::get_cell_radius()
{
	return m_cell_radius;
}

void Cell::set_cell_cycle_length(double cc_length)
{
	m_cell_cycle_length = cc_length;
}

double Cell::get_cell_cycle_length()
{
	return m_cell_cycle_length;
}

