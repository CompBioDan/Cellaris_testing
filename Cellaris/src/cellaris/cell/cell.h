// Generic cell class to contain the representation of 'cells' constructed from particles and particle-particle constraints
//
// Base cell class contains information about the cells particle construction which is passed to FleX to solve physical interactions
//
// 
//

#ifndef CELL_H_
#define CELL_H_

//#include "../stdafx.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <random>
#include "../utilities/datastore.h"
#include "../utilities/scenetime.h"
#include "../utilities/scenes.h"


//namespace cell_space {

	class Cell {

	private:

		bool mEndofCycle; // has the cell reached the end of its cell-cycle -> check to pass to initiate cell division

	public:

		//Cell(); // cell constructor

		Cell* divide(); // method for dividing the cell creating new daughter cell and allocating data

		//virtual void update();

		// Cell methods
		void setBirthTime(double birthTime); // set the cells birthtime

		double getBirthTime() const; // return cell birth time

		double getAge() const; // return cell age

		void age();

		void setCellPos(myVec3d position); // set cell's position

		myVec3d getCellPos() const; // return cell position

		bool readyToDivide(); // flag for division

		void setCellId(unsigned cellId); // allocate cell ID

		unsigned getCellId() const; // return cell ID value

		void resetCell(); // reset cell: currently used to reset the age of the mother cell following division

		void setParticleOffset(int particleoffset);

		void setNumberParticles(int numparticles);

		void setSpringOffset(int springoffset);

		void setNumberSprings(int numbersprings);

		int getNumberParticles();

		int getParticleOffset();

		int getNumberSprings();

		int getSpringOffset();

		// Cell data
		int particle_offset; // integer position of first cell particle within the FleX buffers

		int number_particles; // number of particles that form the cell

		int spring_offset; // integer position of first cell spring within the FleX buffers

		int number_springs; // number of springs forming the cell

		double cell_birth_time; // cell's birth time

		double cell_age; // cell age

		unsigned cell_id; // identifier id for the cell

		myVec3d cell_position; // position of first particle in cell (arbitrary)

		int cell_type; // CHANGE: create a seperate 'attribute' class to encompass cell differentiators

		int grow; /** flag specifying whether a cell should grow this scene timestep */

	};
//}

#endif /* CELL_H_ */