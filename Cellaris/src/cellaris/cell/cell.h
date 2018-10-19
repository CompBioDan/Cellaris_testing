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

#ifndef CELL_H_
#define CELL_H_

#include <random>
#include <vector>
#include <cmath>

#include "../utilities/datastore.h"
#include "../utilities/scenetime.h"
//#include "../cellaris.h"
//#include "../utilities/scenes.h"

/*
* Basic cell class which represents most abstract 'cell' to model.
*
* Default single flex particle (spherical cell)
*
* Key cell data: m_cell_birth_time, m_cell_radius, m_cell_id, m_cell_cycle_length, m_cell_position
* 
*/

//namespace cell_space {

class Cell {

public:

	const double pi = 3.14159265358979323846;

	//Cell(const myVec3d& p_position); // cell constructor

	Cell* divide(); // method for dividing the cell creating new daughter cell and allocating data

	//virtual void update(); // update method for updating cell data following physics solving (flex)

	void set_birth_time(double birthTime); // set the cells birthtime

	double get_birth_time() const; // return cell birth time

	double get_cell_age() const; // return cell age

	void set_cell_position(myVec3d position); // set cell's position

	myVec3d get_cell_position() const; // return cell position

	void set_cell_id(unsigned cellId); // allocate cell ID

	unsigned get_cell_id() const; // return cell ID value

	void reset_cell(); // reset cell: currently used to reset the age of the mother cell following division

	bool ready_to_divide(); // flag for division

	void set_cell_radius(double p_radius);

	double get_cell_radius();

	void set_cell_cycle_length(double cc_length);

	double get_cell_cycle_length();

protected:

	bool m_cellcycle_end; // has the cell reached the end of its cell-cycle -> check to pass to initiate cell division

	myVec3d m_cell_position; // position of first particle in cell (arbitrary)

	double m_cell_birth_time; // cell's birth time

	unsigned m_cell_id; // identifier id for the cell

	int m_grow; /** flag specifying whether a cell should grow this scene timestep */
	
	double m_cell_cycle_length; // TO CHANGE :- implement specific cell-cycle models
	
	double m_cell_radius; // default single 'particle' cell radius corresponds to flex particle radius
};
//}

#endif /* CELL_H_ */