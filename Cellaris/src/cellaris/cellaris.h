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

#include <map>
#include <vector>
#include <random>
#include <climits>
#include <iostream>

#include "cell/cell.h"
//#include "../flex/buffers.h" // buffers now set here, could attempt to shift it back in future
#include "utilities/datastore.h"
#include "utilities/scenetime.h"
#include "utilities/dataexporter.h"
#include "../../lib/flex/core/NvFlex.h"
#include "../../lib/flex/core/NvFlexExt.h"
#include "../../lib/flex/core/NvFlexDevice.h"
#include "../../lib/flex/maths/maths.h"
//#include "../../lib/flex/core/helpers.h"
#include "../../lib/flex/core/platform.h"

/**
*
* Main cellaris simulation class.
* Set up the simulation: bounds, timestep information, exporters etc.
*
* Also contains the FleX physics solver setup and maintains the connections between FleX solving and Cellaris simulation methods
*
**/

//namespace Cellaris {

class cellaris
{

protected: 

	/* Main Cellaris Simulation Parameters */
	double dt = 0.1; // simulation timestep (default 0.1)
	double scene_start_time = 0.0; // simulation start time (default 0.0)
	double scene_end_time; // simulation end time
	double simulation_time; // length of simulation
	Vec3 scene_upper_bounds = Vec3(1.0f, 1.0f, 1.0f); // scene bounds (default 1,1,1)
	Vec3 scene_lower_bounds = Vec3(0.0f, 0.0f, 0.0f); // scene bounds (default 0,0,0)
	std::string scene_output_directory; // output directory
	double cell_cycle_length = 10.0; // simple cell cycle length


private:

	unsigned number_births = 0; // number of cell divisions occured during sim
	unsigned simulated_cell_count = 0; // number of cells in the simulation
	int number_active_particles; // number of active particles in simulation (FleX)
	unsigned output_sampling_timestep_multiple = 1; // number of timesteps between outputs in exporter class
		 
	//static cellaris* instance;  // pointer to the singleton instance of this class 

public:

	const double pi = 3.14159265358979323846; 
	/*static cellaris* Instance();*/
	//cellaris * scene = cellaris::Instance(); // instance of 'scene'
	SceneTime* scene_time = SceneTime::instance(); // instance of 'scene time' which handles timestepping

	void add_cell(Cell* cell); // method for adding a 'cell' to the cell population

	void set_dt(double timestep); // method to set the timestep 
	void set_simulation_time(double sim_time); // method for setting simulation length
	void set_start_and_end_times(double start_time, double end_time); // method for setting start and end times of simulation (and extended version is used in set_times_and_steps())
	//void set_end_time_and_timesteps(double end_time, unsigned int number_timesteps);
	void set_scene_upper_bounds(float x, float y, float z); // method for setting x, y, z dimensions for simulation
	void set_num_births(int births); // method for setting number of births in the simulation
	void set_num_simulated_cells(int cell_count); // method for setting the number of cells in simulation
	void set_output_directory(std::string output_dir); // method for setting output directory (used by the exporter class)
	void set_sampling_timestep(unsigned sampling_timestep); // method for setting number of timesteps between data exporting
	void set_number_active_particles(int num_active_particles); // method for setting number of active particles in simulation (used by FleX)
	void set_cell_positions(int number_cells, int position_flag = 0); // method for allocating random locations to cell population (alternative is to allocate in population creation in different simulations i.e in test main)
	void set_cell_cycle_length(double cc_length); // simple method for setting a 'cell-cycle' length for all cells in simulation
	void set_times_and_steps(double start_time, double end_time, double dt); // method for setting the scene_time start and end times of simulation, and setting the timestep
	void destroy_scene_time(); // method for destroying the scene_time instance	

	double get_dt(); // method for retrieving the timestep
	double get_simulation_time(); // method for retrieving simulation length 
	double get_start_time(); // method for retrieving simulation start time (from scene_time) useful for cases where start_time != 0
	Vec3 get_scene_upper_bounds(); // method for retrieving simulation scene bounds
	Cell* get_cell(unsigned int cell_id); // method for retrieving 'cell' data
	int get_number_cells(); // method for retrieving the number of cells in simulation
	int get_number_active_particles(); // method for retrieving number of active particles in scene (used for FleX)
	unsigned get_number_births(); // method for retrieving number of births in the simulation
	const std::string get_output_directory(); // method for retrieving the output directory set for the exporter method

	void evolve(); // main simulation 'solve' method (evolves the scene each timestep)
	void cell_division(int cell_id); 

	CellarisVector<Cell*> cell_population; // vector containing the population of cells
	/*static void destroy();*/

	/* FleX */
	NvFlexSolver* flex_solver; // instance of the NvFlexSolver
	NvFlexLibrary* flex_library; // instance of flex library
	NvFlexParams flex_params; // flex parameters
	NvFlexTimers flex_timers; // flex timers
	int num_extra_particles = 10000; // maximum number of particles in flex solver (sets size of buffers)
	unsigned char max_neighbours_per_particle = 96;
	int max_diffuse_particles = 0; // FOR FLUIDS
	int graphics_device = -1; // flag for specifying which graphics card to use in flex solver
	char device_name[256]; // name of graphics card set to be used with flex solver
	int number_substeps = 2; // number of substeps in flex solver
	float real_dt; // the real world time delta between updates
	int flex_frame = 0;
	float particle_radius = 0.1f;

	float wave_floor_tilt = 0.0f;
	float wave_plane;

	std::vector<int> g_meshSkinIndices; // FOR RENDERING
	std::vector<float> g_meshSkinWeights; // FOR RENDERING
	

	/* Initialise flex scene */
	void intialise_flex_context(); // method for initialising flex context (CUDA context, flex library, flex descriptor)
	void initialise_flex_scene(); // method for scene specific set up (allocating buffers, parameters etc)
	//void initialise_flex_cells();
	void flex_shutdown(); // method for shutting down flex library, buffers and solver following simulation

	/* Flex buffers */
	struct SimBuffers // Struct containing all Flex simulation data buffers
	{
		NvFlexVector<Vec4> positions;
		NvFlexVector<Vec4> restPositions;
		NvFlexVector<Vec3> velocities;
		NvFlexVector<int> phases;
		NvFlexVector<float> densities;
		NvFlexVector<Vec4> anisotropy1;
		NvFlexVector<Vec4> anisotropy2;
		NvFlexVector<Vec4> anisotropy3;
		NvFlexVector<Vec4> normals;
		NvFlexVector<Vec4> smoothPositions;
		NvFlexVector<Vec4> diffusePositions;
		NvFlexVector<Vec4> diffuseVelocities;
		NvFlexVector<int> diffuseIndices;
		NvFlexVector<int> activeIndices;

		// convexes
		NvFlexVector<NvFlexCollisionGeometry> shapeGeometry;
		NvFlexVector<Vec4> shapePositions;
		NvFlexVector<Quat> shapeRotations;
		NvFlexVector<Vec4> shapePrevPositions;
		NvFlexVector<Quat> shapePrevRotations;
		NvFlexVector<int> shapeFlags;

		// rigids
		NvFlexVector<int> rigidOffsets;
		NvFlexVector<int> rigidIndices;
		NvFlexVector<int> rigidMeshSize;
		NvFlexVector<float> rigidCoefficients;
		NvFlexVector<Quat> rigidRotations;
		NvFlexVector<Vec3> rigidTranslations;
		NvFlexVector<Vec3> rigidLocalPositions;
		NvFlexVector<Vec4> rigidLocalNormals;

		// inflatables
		NvFlexVector<int> inflatableTriOffsets;
		NvFlexVector<int> inflatableTriCounts;
		NvFlexVector<float> inflatableVolumes;
		NvFlexVector<float> inflatableCoefficients;
		NvFlexVector<float> inflatablePressures;

		// springs
		NvFlexVector<int> springIndices;
		NvFlexVector<float> springLengths;
		NvFlexVector<float> springStiffness;

		NvFlexVector<int> triangles;
		NvFlexVector<Vec3> triangleNormals;
		NvFlexVector<Vec3> uvs;

		SimBuffers(NvFlexLibrary* l) :
			positions(l), restPositions(l), velocities(l), phases(l), densities(l),
			anisotropy1(l), anisotropy2(l), anisotropy3(l), normals(l), smoothPositions(l),
			diffusePositions(l), diffuseVelocities(l), diffuseIndices(l), activeIndices(l),
			shapeGeometry(l), shapePositions(l), shapeRotations(l), shapePrevPositions(l),
			shapePrevRotations(l), shapeFlags(l), rigidOffsets(l), rigidIndices(l), rigidMeshSize(l),
			rigidCoefficients(l), rigidRotations(l), rigidTranslations(l),
			rigidLocalPositions(l), rigidLocalNormals(l), inflatableTriOffsets(l),
			inflatableTriCounts(l), inflatableVolumes(l), inflatableCoefficients(l),
			inflatablePressures(l), springIndices(l), springLengths(l),
			springStiffness(l), triangles(l), triangleNormals(l), uvs(l)
		{}
	};
	SimBuffers* flex_buffers;

	SimBuffers* alloc_buffers(NvFlexLibrary* lib); // Allocate the simulation buffers
	void map_buffers(SimBuffers* buffers); // Maps the sim data buffers from the CPU for GPU usage
	void unmap_buffers(SimBuffers* buffers); // Following CPU mapping/writting to the buffer it then unmaps the buffer to allow asynchronous access to GPU
	void destroy_buffers(SimBuffers* buffers); // Destroy the buffers following completion of the simulation

	/* Flex emitter */
	struct Emitter
	{
		Emitter() : mSpeed(0.0f), mEnabled(false), mLeftOver(0.0f), mWidth(8) {}

		Vec3 mPos;
		Vec3 mDir;
		Vec3 mRight;
		float mSpeed;
		bool mEnabled;
		float mLeftOver;
		int mWidth;
	};

	//void error_callback(NvFlexErrorSeverity, const char* msg, const char* file, int line);
	/* ------------------------------------------------- */
	// Helper functions (as an initial check, need to be moved to helpers.h)
	void get_particle_bounds(Vec3& lower, Vec3& upper);

	/* Flex 'scene' setup */
	void flex_scene_initialize();
	//void flex_scene_post_initialize();

	void flex_scene_update(); // update any buffers (all guaranteed to be mapped here)
	void flex_scene_sync(); // send any changes to flex (all buffers guaranteed to be unmapped here)
	void flex_initialise_buffers();


	void flex_update_frame();

	void create_bacteria(Vec3 start, Vec3 dir, float stiffness, int segments, float length, int phase, float spiralAngle = 0.0f, float invmass = 1.0f, float give = 0.075f);
	void create_spring(int first, int second, float stiffness, float give = 0.0f);

};

//}

#endif /* CELLARIS_H_ */