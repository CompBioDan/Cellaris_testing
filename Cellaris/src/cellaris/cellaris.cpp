#include "cellaris.h"

//namespace Cellaris {


	void cellaris::set_dt(double timestep)
	{
		dt = timestep;
	}

	void cellaris::set_simulation_time(double sim_time)
	{
		simulation_time = sim_time;
	}

	void cellaris::set_start_time(double start_time)
	{
		scene_start_time = start_time;
		scene_time->SetStartTime(scene_start_time);

	}

	void cellaris::destroy_scene_time()
	{
		scene_time->Destroy();
	}

	void cellaris::set_end_time_and_timesteps(double end_time, unsigned int num_timesteps)
	{
		scene_time->SetEndTimeAndNumberOfTimeSteps(end_time, num_timesteps);
	}

	void cellaris::set_scene_bounds(float x, float y, float z)
	{
		scene_bounds = myVec3d(x, y, z);
	}

	double cellaris::get_dt()
	{
		return dt;
	}

	double cellaris::get_start_time()
	{
		double start_time = scene_time->GetStartTime();
		return start_time;
	}

	myVec3d cellaris::get_scene_bounds()
	{
		return scene_bounds;
	}

	double cellaris::get_simulation_time()
	{
		return simulation_time;
	}

	// pointer to singleton instance
	cellaris* cellaris::instance = nullptr;

	cellaris* cellaris::Instance()
	{
		if (instance == nullptr)
		{
			instance = new cellaris;
			std::atexit(destroy);
		}
		return instance;
	}
	

	void cellaris::set_num_births(int births)
	{
		number_births = births;
	}

	unsigned cellaris::get_number_births()
	{
		return number_births;
	}

	void cellaris::set_output_directory(std::string output_dir)
	{
		scene_output_directory = output_dir;
	}

	void cellaris::set_sampling_timestep(unsigned sampling_timestep_multiple)
	{
		output_sampling_timestep_multiple = sampling_timestep_multiple;
	}

	void cellaris::destroy()
	{
		if (instance)
		{
			delete instance;
			instance = nullptr;
		}
	}

	int cellaris::get_number_cells()
	{
		//return cell_population.getCount();
	}

	int cellaris::get_number_active_particles()
	{
		return number_active_particles;
	}

	void cellaris::set_number_active_particles(int active_particles)
	{
		number_active_particles = active_particles;
	}

	void cellaris::add_cell(Cell* cell)
	{
		//cell_population.add(cell);
	}

	Cell* cellaris::get_cell(unsigned int cell_id)
	{
		/*if (cell_id < cell_population.getSize())
			return NULL;
		else
			return cell_population[cell_id];*/
	}

	void cellaris::evolve()
	{
		//// set up simulation time
		SceneTime* p_scene_time = SceneTime::Instance();
		double current_time = p_scene_time->GetTime();

		// set the end time and number of timesteps
		unsigned num_time_steps = (unsigned)((scene_end_time - current_time) / dt + 0.5);
		std::cout << "tsteps " << dt << '\n';
		if (current_time > 0)
		{
			p_scene_time->ResetEndTimeAndNumberOfTimeSteps(scene_end_time, num_time_steps);
		}
		else
		{
			p_scene_time->SetEndTimeAndNumberOfTimeSteps(scene_end_time, num_time_steps);
		}

		// set the current time
		double time_now = p_scene_time->GetTime();

		// correctly age all cells before main solve loop
		for (int i = 0; i < get_number_cells(); ++i)
		{
			//cell_population[i]->age();
		}
		std::cout << "end time: " << p_scene_time->GetEndTime() << '\n';
		std::cout << "current time: " << p_scene_time->GetTime() << '\n';
		std::cout << "at end? " << p_scene_time->HasFinished() << '\n';

		// Main solve loop
		while (!(p_scene_time->HasFinished()))
		{

			//* Output simulation population data with set frequency*/
			if ((p_scene_time->GetTimeStepsElapsed() % output_sampling_timestep_multiple) == 0) {
				for (int i = 0; i < get_number_cells(); ++i)
				{
					//outputcellsdata(i,getCell(i));

					//allCellDataWriter(get_cell(i), "C:/Users/BCL/Documents/Cellaris/CellarisFlex/celldata.txt");
				}

			}

			//UpdateCellPopulation();
			for (int i = 0; i < get_number_cells(); ++i)
			{
				/*if (cell_population[i]->readyToDivide())
				{
					add_cell(cell_population[i]->divide());
				}*/
			}

			p_scene_time->IncrementTimeOneStep();
		}
	}

	void cellaris::set_cell_positions(int num_cells, int position_flag)
	{
		if (position_flag == 0)
		{
			std::uniform_real_distribution<double> rand_x(0.0, scene_bounds.pos.x), rand_y(0.0, scene_bounds.pos.y), rand_z(0.0, scene_bounds.pos.z);
			std::default_random_engine re;

			for (int i = 0; i < num_cells; i++)
			{
				double pos_x = rand_x(re), pos_y = rand_y(re), pos_z = rand_z(re);
				//cell_population[i]->cell_position = myVec3d(pos_x,pos_y,pos_z);
			}
		}

	}

//}