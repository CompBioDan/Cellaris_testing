/**
* 
* Main cellaris simulation class.
* Set up the simulation: bounds, timestep information, exporters etc.
*
**/

#ifndef CELLARIS_H_
#define CELLARIS_H_

#include <vector>
#include <random>
#include <climits>
#include <iostream>

#include "utilities/datastore.h"
#include "utilities/timestepper.h"
#include "utilities/scenetime.h"
#include "utilities/scenes.h"
#include "cell/cellpopulation.h"
#include "cell/cell.h"
//#include "utilities/math/vec3.h"
//#include "utilities/dataexporter.h"




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


	private:

		unsigned number_births = 0; // number of cell divisions occured during sim
		int number_active_particles; // number of active particles in sim
		unsigned output_sampling_timestep_multiple; // number of timesteps between outputs
		 
		static cellaris* instance;  // pointer to the singleton instance of this class 
		//CellarisVector<Cell*> cell_population;


	public:

		static cellaris* Instance();
		cellaris * scene = cellaris::Instance(); // instance of 'scene'
		SceneTime * scene_time = SceneTime::Instance(); // instance of 'scene time' which handles timestepping

		void add_cell(Cell* cell);

		void set_dt(double timestep);
		void set_simulation_time(double sim_time);
		void set_start_time(double start_time);
		void destroy_scene_time();
		void set_end_time_and_timesteps(double end_time, unsigned int number_timesteps);
		void set_scene_bounds(float x, float y, float z);
		void set_num_births(int births);
		void set_output_directory(std::string output_dir);
		void set_sampling_timestep(unsigned sampling_timestep);
		void set_number_active_particles(int num_active_particles);
		void set_cell_positions(int number_cells, int position_flag = 0);
		

		double get_dt();
		double get_simulation_time();
		double get_start_time();
		myVec3d get_scene_bounds();
		Cell* get_cell(unsigned int cell_id);
		int get_number_cells();
		int get_number_active_particles();
		unsigned get_number_births();

		void evolve();

		static void destroy();


	};

//}


#endif /* CELLARIS_H_ */