// Cellaris.cpp : Defines the entry point for the console application.
//

#include "../lib/stdafx.h"
#include "../src/cellaris/cellaris.h"
#include "../src/cellaris/cell/cell.h"


#include <iostream>

//using namespace Cellaris;

int main()
{
	// Set up cellaris simulation (bounds, times etc)
	//cellaris sim;
	cellaris* sim = cellaris::Instance();
	
	////cellaris* sim = new cellaris;

	//// Test 'pre-set' simulation settings (default dt, bounds)
	//std::cout << "Preset dt = " << sim->get_dt() << '\n';
	//std::cout << "Preset bounds = " << sim->get_scene_bounds().pos.x << " " << sim->get_scene_bounds().pos.y << " " << sim->get_scene_bounds().pos.z << '\n';

	//// Test setting the timestep and bounds
	//sim->set_dt(0.5);
	//sim->set_scene_bounds(2.0f,2.0f,2.0f);
	//std::cout << "New dt = " << sim->get_dt() << '\n';
	//std::cout << "New bounds = " << sim->get_scene_bounds().pos.x << " " << sim->get_scene_bounds().pos.y << " " << sim->get_scene_bounds().pos.z << '\n';

	//// Set scene start time
	//sim->set_start_time(1.0);

	//// Check start time set correctly
	//std::cout << "Start time = " << sim->get_start_time() << '\n';


	//// TESTING THE SETUP
	//double simulation_end_time = 25.0;
	//int time_steps = 2500;
	//int num_cells = 1;

	//sim->set_start_time(0.0);
	//sim->set_end_time_and_timesteps(simulation_end_time, time_steps);

	////// Test 'pre-set' simulation settings (default dt, bounds)
	////std::cout << "Preset dt = " << sim.get_dt() << '\n';
	////std::cout << "Preset bounds = " << sim.get_scene_bounds().pos.x << " " << sim.get_scene_bounds().pos.y << " " << sim.get_scene_bounds().pos.z << '\n';

	////// Test setting the timestep and bounds
	////sim.set_dt(0.5);
	////sim.set_scene_bounds(2.0f, 2.0f, 2.0f);
	////std::cout << "New dt = " << sim.get_dt() << '\n';
	////std::cout << "New bounds = " << sim.get_scene_bounds().pos.x << " " << sim.get_scene_bounds().pos.y << " " << sim.get_scene_bounds().pos.z << '\n';

	////// Set scene start time
	////sim.set_start_time(1.0);

	////// Check start time set correctly
	////std::cout << "Start time = " << sim.get_start_time() << '\n';


	////// TESTING THE SETUP
	////double simulation_end_time = 25.0;
	////int time_steps = 2500;
	////int num_cells = 1;

	////sim.set_start_time(0.0);
	////sim.set_end_time_and_timesteps(simulation_end_time, time_steps);

	//std::vector<Cell*> cell_pop;
	//cell_pop.reserve(num_cells);

	//// CHANGE: need some way of customising the particular cell i.e each 'Cell' has a set number of particles/constraints
	//int particle_offset = 0, spring_offset = 0, number_cell_particles = 3, number_cell_springs = 3;

	//for (int i = 0; i < num_cells; i++)
	//{
	//	//Cell* cell(new Cell());


	//}
	//



	//sim->destroy_scene_time();



	std::cin.ignore();
	return 0;
}

