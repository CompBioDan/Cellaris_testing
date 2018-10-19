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

#ifndef CELLARIS_H_
#define CELLARIS_H_

#include <vector>
#include <random>
#include <climits>
#include <iostream>

#include "utilities/timestepper.h"
#include "utilities/datastore.h"
#include "utilities/scenetime.h"
//#include "utilities/scenes.h"
#include "cell/cell.h"
//#include "utilities/math/vec3.h"
//#include "utilities/dataexporter.h"


/**
*
* Main cellaris simulation class.
* Set up the simulation: bounds, timestep information, exporters etc.
*
**/

//namespace Cellaris {

class cellaris
{

protected: 

	// Simulation parameters
	double dt = 0.1; // simulation timestep (default 0.1)
	double scene_start_time = 0.0; // simulation start time (default 0.0)
	double scene_end_time; // simulation end time
	double simulation_time; // length of simulation
	myVec3d scene_bounds = myVec3d(1.0f, 1.0f, 1.0f); // scene bounds (default 1,1,1)
	std::string scene_output_directory; // output directory
	double cell_cycle_length = 10.0;


private:

	unsigned number_births = 0; // number of cell divisions occured during sim
	unsigned simulated_cell_count = 0;
	int number_active_particles; // number of active particles in sim
	unsigned output_sampling_timestep_multiple = 1; // number of timesteps between outputs
		 
	//static cellaris* instance;  // pointer to the singleton instance of this class 

public:

	/*static cellaris* Instance();*/
	//cellaris * scene = cellaris::Instance(); // instance of 'scene'
	SceneTime* scene_time = SceneTime::instance(); // instance of 'scene time' which handles timestepping

	void add_cell(Cell* cell);

	void set_dt(double timestep);
	void set_simulation_time(double sim_time);
	void set_start_and_end_times(double start_time, double end_time);
	void destroy_scene_time();
	//void set_end_time_and_timesteps(double end_time, unsigned int number_timesteps);
	void set_scene_bounds(float x, float y, float z);
	void set_num_births(int births);
	void set_num_simulated_cells(int cell_count);
	void set_output_directory(std::string output_dir);
	void set_sampling_timestep(unsigned sampling_timestep);
	void set_number_active_particles(int num_active_particles);
	void set_cell_positions(int number_cells, int position_flag = 0);
	void set_cell_cycle_length(double cc_length);
	void set_times_and_steps(double start_time, double end_time, double dt);
		

	double get_dt();
	double get_simulation_time();
	double get_start_time();
	myVec3d get_scene_bounds();
	Cell* get_cell(unsigned int cell_id);
	int get_number_cells();
	int get_number_active_particles();
	unsigned get_number_births();

	void evolve();

	CellarisVector<Cell*> cell_population;

	/*static void destroy();*/
};

//}

#endif /* CELLARIS_H_ */