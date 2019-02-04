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
#include <ctime>
#include <iostream>

//#include "../lib/flex/core/NvFlex.h"
//#include "../lib/SDL2-2.0.4/include/SDL.h"
#include "../src/cellaris/cellaris.h"

//#include "../src/cellaris/cell/cell.h"
//#include "../src/flex/buffers.h"

//#include "../src/flex/buffers.h"
//#include "../lib/flex/core/NvFlexExt.h"
//#include "../lib/flex/maths/maths.h"

//NvFlexSolver* g_flex; // Instance of the NvFlexSolver
//NvFlexLibrary* g_flexLib; // Instance of flex library
//NvFlexParams g_params;
//NvFlexTimers g_timers;

////using namespace Cellaris;
//#if defined(_WIN32) && !defined(WIN32)
//#define WIN32
//#endif

#include "../lib/SDL2-2.0.4/include/begin_code.h"

#ifdef __cplusplus
  extern "C" 
#endif


int main(int argc, char *argv[])
{

	int start_s = clock();

	// Set up cellaris simulation (bounds, times etc)
	cellaris sim;

	double simulation_end_time = 52.0, simulation_start_time = 0.0;
	int number_time_steps = 520, num_cells = 1, sampling_count = 1000;
	double dt = (simulation_end_time - simulation_start_time) / number_time_steps;
	myVec3f sim_bounds = myVec3f(2.0f, 2.0f, 2.0f);
	double cell_radius = 0.1;

	// TESTING THE SETUP
	//sim.set_start_and_end_times(simulation_start_time,simulation_end_time);
	//sim.set_end_time_and_timesteps(simulation_end_time, time_steps);
	sim.set_times_and_steps(simulation_start_time, simulation_end_time, dt);
	//sim.set_num_simulated_cells(num_cells);

	sim.set_output_directory("C:/Users/BCL/Documents/GitHub/Cellaris_testing/Cellaris/out/celldata.txt");
	sim.set_sampling_timestep(sampling_count);
	std::cout << "Output directory: " << sim.get_output_directory() << '\n' << '\n';

	// Test 'pre-set' simulation settings (default dt, bounds)
	std::cout << "Preset dt = " << sim.get_dt() << '\n';
	std::cout << "Preset bounds = " << sim.get_scene_upper_bounds().x << " " << sim.get_scene_upper_bounds().y << " " << sim.get_scene_upper_bounds().z << '\n';

	// Test setting the timestep and bounds
	//sim.set_dt(dt);
	sim.set_scene_upper_bounds(sim_bounds.pos.x,sim_bounds.pos.y,sim_bounds.pos.z);
	std::cout << "New dt = " << sim.get_dt() << '\n';
	std::cout << "New bounds = " << sim.get_scene_upper_bounds().x << " " << sim.get_scene_upper_bounds().y << " " << sim.get_scene_upper_bounds().z << '\n';

	// Set scene start time
	//sim.set_start_and_end_times(10.0,10.0);

	// Check start time set correctly
	std::cout << "Start time = " << sim.get_start_time() << '\n';

	//// CHANGE: need some way of customising the particular cell i.e each 'Cell' has a set number of particles/constraints
	//int particle_offset = 0, spring_offset = 0, number_cell_particles = 3, number_cell_springs = 3;

	// Random basic cell position
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> rand_x(0.0 + cell_radius * 3, sim_bounds.pos.x - cell_radius * 3);
	std::uniform_real_distribution<> rand_y(0.0 + cell_radius * 3, sim_bounds.pos.y - cell_radius * 3);
	std::uniform_real_distribution<> rand_z(0.0 + cell_radius * 3, sim_bounds.pos.z - cell_radius * 3);
	std::uniform_real_distribution<> cc_length(10, 12);
	std::uniform_real_distribution<> random_birth(-8, -4);

	

	//for (int i = 0; i < num_cells; i++)
	//{
	//	Cell* cell = new Cell;

	//	myVec3d position; 
	//	position.pos.x = rand_x(gen); position.pos.y = rand_y(gen); position.pos.z = rand_z(gen);

	//	//cell->set_cell_position(position);
	//	cell->set_birth_time(random_birth(gen));
	//	//cell->set_birth_time(0.0);
	//	cell->set_cell_id(i);
	//	cell->set_flex_particle_buffer_offset(i);
	//	//cell->set_cell_radius(cell_radius);
	//	cell->set_cell_cycle_length(cc_length(gen));
	//	//cell->set_cell_cycle_length(10.0);

	//	sim.add_cell(cell);
	//}

	for (int i = 0; i < num_cells; i++)
	{
		Cell *cell = new Bacteria;

		//std::uniform_int_distribution<> particle_count(cell->max_particles/2, cell->max_particles);

		myVec3d position;
		position.pos.x = rand_x(gen); position.pos.y = rand_y(gen); position.pos.z = rand_z(gen);

		cell->set_birth_time(random_birth(gen));
		cell->set_cell_id(i);
		cell->set_cell_cycle_length(cc_length(gen));
		cell->set_flex_particle_buffer_offset(0);
		cell->set_flex_particle_count(3);
		cell->set_flex_spring_buffer_offset(0);
		cell->set_flex_spring_count(3);

		/*if (i = 0) {
			cell->set_particle_offset(0);
		}*/
		//else if (i > 0) {

		//cell->set_particle_offset(sim.get_cell(i-1)->get_particle_offset()+particle_count(gen));

		//}

		sim.add_cell(cell);

		std::cout << "Number cells = " << i << '\n';
	}

	std::cout << "Number cells in sim = " << sim.get_number_cells() << '\n';

	//Test cell population saving
	/*Cell* fetched_cell = sim.get_cell(0);

	std::cout << "Particle count: " << fetched_cell->get_flex_particle_count() << '\n';*/
	/*std::cout << "Fetched cell data 1: " << '\n';
	std::cout << "x pos: " << fetched_cell->get_cell_position().pos.x << '\n';
	std::cout << "y pos: " << fetched_cell->get_cell_position().pos.y << '\n';
	std::cout << "z pos: " << fetched_cell->get_cell_position().pos.z << '\n';
	
	Cell* fetched_cell_2 = sim.get_cell(1);

	std::cout << "Fetched cell data 2: " << '\n';
	std::cout << "x pos: " << fetched_cell_2->get_cell_position().pos.x << '\n';
	std::cout << "y pos: " << fetched_cell_2->get_cell_position().pos.y << '\n';
	std::cout << "z pos: " << fetched_cell_2->get_cell_position().pos.z << '\n';*/


	// cellaris::flex_scene_initialize() <- this is where we need to initialise the particles and constraints from the individual cells
	sim.intialise_flex_context();

	////for (int y = 0; y < sim.get_number_cells(); y++)
	////{
	////	std::cout << "Cell position: x= " << sim.cell_population[y]->get_cell_position().pos.x << " y= " << sim.cell_population[y]->get_cell_position().pos.y << " z= " << sim.cell_population[y]->get_cell_position().pos.z << '\n';
	////}
	////

	sim.evolve();

	sim.map_buffers(sim.flex_buffers);

	Cell* fetched_cell = sim.get_cell(0);
	for (int y = 0; y < fetched_cell->get_flex_particle_count(); y++)
	{
		std::cout << "x: " << sim.flex_buffers->positions[y].x << " y: " << sim.flex_buffers->positions[y].y << " z: " << sim.flex_buffers->positions[y].z << '\n'; 
	}
	sim.unmap_buffers(sim.flex_buffers);
	//
	sim.flex_shutdown();

	std::cout << "Cells in simulation at end: " << sim.get_number_cells() << '\n';

	sim.destroy_scene_time();

	// the code you wish to time goes here
	int stop_s = clock();

	std::cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 <<'\n';


	std::cin.ignore();


	return 0;
}

#include "../lib/SDL2-2.0.4/include/close_code.h"


