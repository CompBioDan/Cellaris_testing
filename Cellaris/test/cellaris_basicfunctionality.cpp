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

#include "../lib/stdafx.h"
#include "../src/cellaris/cellaris.h"
#include "../src/cellaris/cell/cell.h"


#include <iostream>

//using namespace Cellaris;

int main()
{
	// Set up cellaris simulation (bounds, times etc)
	cellaris sim;

	double simulation_end_time = 9.0, simulation_start_time = 0.0;
	int number_time_steps = 9, num_cells = 2;
	double dt = (simulation_end_time - simulation_start_time) / number_time_steps;
	myVec3d sim_bounds = myVec3d(2.0f, 2.0f, 2.0f);
	double cell_radius = 0.1;

	// TESTING THE SETUP
	sim.set_start_and_end_times(simulation_start_time,simulation_end_time);
	//sim.set_end_time_and_timesteps(simulation_end_time, time_steps);
	sim.set_times_and_steps(simulation_start_time, simulation_end_time, dt);
	sim.set_num_simulated_cells(num_cells);

	// Test 'pre-set' simulation settings (default dt, bounds)
	std::cout << "Preset dt = " << sim.get_dt() << '\n';
	std::cout << "Preset bounds = " << sim.get_scene_bounds().pos.x << " " << sim.get_scene_bounds().pos.y << " " << sim.get_scene_bounds().pos.z << '\n';

	// Test setting the timestep and bounds
	sim.set_dt(dt);
	sim.set_scene_bounds(sim_bounds.pos.x,sim_bounds.pos.y,sim_bounds.pos.z);
	std::cout << "New dt = " << sim.get_dt() << '\n';
	std::cout << "New bounds = " << sim.get_scene_bounds().pos.x << " " << sim.get_scene_bounds().pos.y << " " << sim.get_scene_bounds().pos.z << '\n';

	// Set scene start time
	//sim.set_start_and_end_times(10.0,10.0);

	// Check start time set correctly
	std::cout << "Start time = " << sim.get_start_time() << '\n';

	//// CHANGE: need some way of customising the particular cell i.e each 'Cell' has a set number of particles/constraints
	//int particle_offset = 0, spring_offset = 0, number_cell_particles = 3, number_cell_springs = 3;

	// Random basic cell position
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> rand_x(0.0 + cell_radius, sim_bounds.pos.x - cell_radius);
	std::uniform_real_distribution<> rand_y(0.0 + cell_radius, sim_bounds.pos.y - cell_radius);
	std::uniform_real_distribution<> rand_z(0.0 + cell_radius, sim_bounds.pos.z - cell_radius);
	std::uniform_real_distribution<> cc_length(10, 12);
	std::uniform_real_distribution<> random_birth(-8, -4);

	for (int i = 0; i < num_cells; i++)
	{
		Cell* cell = new Cell;

		myVec3d position; 
		position.pos.x = rand_x(gen); position.pos.y = rand_y(gen); position.pos.z = rand_z(gen);

		cell->set_cell_position(position);
		//cell->set_birth_time(random_birth(gen));
		cell->set_birth_time(0.0);
		cell->set_cell_id(i);
		cell->set_cell_radius(cell_radius);
		//cell->set_cell_cycle_length(cc_length(gen));
		cell->set_cell_cycle_length(10.0);

		sim.add_cell(cell);

	}


	//Test cell population saving
	Cell* fetched_cell = sim.get_cell(0);

	std::cout << "Cell cycle length: " << fetched_cell->get_cell_cycle_length() << '\n';
	/*std::cout << "Fetched cell data 1: " << '\n';
	std::cout << "x pos: " << fetched_cell->get_cell_position().pos.x << '\n';
	std::cout << "y pos: " << fetched_cell->get_cell_position().pos.y << '\n';
	std::cout << "z pos: " << fetched_cell->get_cell_position().pos.z << '\n';
	
	Cell* fetched_cell_2 = sim.get_cell(1);

	std::cout << "Fetched cell data 2: " << '\n';
	std::cout << "x pos: " << fetched_cell_2->get_cell_position().pos.x << '\n';
	std::cout << "y pos: " << fetched_cell_2->get_cell_position().pos.y << '\n';
	std::cout << "z pos: " << fetched_cell_2->get_cell_position().pos.z << '\n';*/

	sim.evolve();

	sim.destroy_scene_time();


	std::cin.ignore();
	return 0;
}

