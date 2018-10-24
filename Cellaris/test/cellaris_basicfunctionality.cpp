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
#include "../lib/flex/core/NvFlex.h"
#include "../src/cellaris/cellaris.h"
#include "../src/cellaris/cell/cell.h"
//#include "../src/flex/buffers.h"


#include "../lib/flex/core/NvFlexExt.h"
#include "../lib/flex/maths/maths.h"
#include <ctime>

#include <iostream>

NvFlexSolver* g_flex; // Instance of the NvFlexSolver
NvFlexLibrary* g_flexLib; // Instance of flex library
NvFlexParams g_params;
NvFlexTimers g_timers;

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

SimBuffers* g_buffers;

void MapBuffers(SimBuffers* buffers) // Maps the sim data buffers from the CPU for GPU usage
{
	buffers->positions.map();
	//buffers->restPositions.map();
	//buffers->velocities.map();
	//buffers->phases.map();
	//buffers->densities.map();
	//buffers->anisotropy1.map();
	//buffers->anisotropy2.map();
	//buffers->anisotropy3.map();
	//buffers->normals.map();
	//buffers->diffusePositions.map();
	//buffers->diffuseVelocities.map();
	//buffers->diffuseIndices.map();
	//buffers->smoothPositions.map();
	//buffers->activeIndices.map();

	//// convexes
	//buffers->shapeGeometry.map();
	//buffers->shapePositions.map();
	//buffers->shapeRotations.map();
	//buffers->shapePrevPositions.map();
	//buffers->shapePrevRotations.map();
	//buffers->shapeFlags.map();

	//buffers->rigidOffsets.map();
	//buffers->rigidIndices.map();
	//buffers->rigidMeshSize.map();
	//buffers->rigidCoefficients.map();
	//buffers->rigidRotations.map();
	//buffers->rigidTranslations.map();
	//buffers->rigidLocalPositions.map();
	//buffers->rigidLocalNormals.map();

	//buffers->springIndices.map();
	//buffers->springLengths.map();
	//buffers->springStiffness.map();

	//// inflatables
	//buffers->inflatableTriOffsets.map();
	//buffers->inflatableTriCounts.map();
	//buffers->inflatableVolumes.map();
	//buffers->inflatableCoefficients.map();
	//buffers->inflatablePressures.map();

	//buffers->triangles.map();
	//buffers->triangleNormals.map();
	//buffers->uvs.map();
}

void UnmapBuffers(SimBuffers* buffers) // Following CPU mapping/writting to the buffer it then unmaps the buffer to allow asynchronous access to GPU
{
	// particles
	buffers->positions.unmap();
	buffers->restPositions.unmap();
	buffers->velocities.unmap();
	buffers->phases.unmap();
	buffers->densities.unmap();
	buffers->anisotropy1.unmap();
	buffers->anisotropy2.unmap();
	buffers->anisotropy3.unmap();
	buffers->normals.unmap();
	buffers->diffusePositions.unmap();
	buffers->diffuseVelocities.unmap();
	buffers->diffuseIndices.unmap();
	buffers->smoothPositions.unmap();
	buffers->activeIndices.unmap();

	// convexes
	buffers->shapeGeometry.unmap();
	buffers->shapePositions.unmap();
	buffers->shapeRotations.unmap();
	buffers->shapePrevPositions.unmap();
	buffers->shapePrevRotations.unmap();
	buffers->shapeFlags.unmap();

	// rigids
	buffers->rigidOffsets.unmap();
	buffers->rigidIndices.unmap();
	buffers->rigidMeshSize.unmap();
	buffers->rigidCoefficients.unmap();
	buffers->rigidRotations.unmap();
	buffers->rigidTranslations.unmap();
	buffers->rigidLocalPositions.unmap();
	buffers->rigidLocalNormals.unmap();

	// springs
	buffers->springIndices.unmap();
	buffers->springLengths.unmap();
	buffers->springStiffness.unmap();

	// inflatables
	buffers->inflatableTriOffsets.unmap();
	buffers->inflatableTriCounts.unmap();
	buffers->inflatableVolumes.unmap();
	buffers->inflatableCoefficients.unmap();
	buffers->inflatablePressures.unmap();

	// triangles
	buffers->triangles.unmap();
	buffers->triangleNormals.unmap();
	buffers->uvs.unmap();

}

SimBuffers* AllocBuffers(NvFlexLibrary* lib)
{
	return new SimBuffers(lib);
}

void DestroyBuffers(SimBuffers* buffers) // Destroy the buffers following completion of the simulation
{
	// particles
	buffers->positions.destroy();
	buffers->restPositions.destroy();
	buffers->velocities.destroy();
	buffers->phases.destroy();
	buffers->densities.destroy();
	buffers->anisotropy1.destroy();
	buffers->anisotropy2.destroy();
	buffers->anisotropy3.destroy();
	buffers->normals.destroy();
	buffers->diffusePositions.destroy();
	buffers->diffuseVelocities.destroy();
	buffers->diffuseIndices.destroy();
	buffers->smoothPositions.destroy();
	buffers->activeIndices.destroy();

	// convexes
	buffers->shapeGeometry.destroy();
	buffers->shapePositions.destroy();
	buffers->shapeRotations.destroy();
	buffers->shapePrevPositions.destroy();
	buffers->shapePrevRotations.destroy();
	buffers->shapeFlags.destroy();

	// rigids
	buffers->rigidOffsets.destroy();
	buffers->rigidIndices.destroy();
	buffers->rigidMeshSize.destroy();
	buffers->rigidCoefficients.destroy();
	buffers->rigidRotations.destroy();
	buffers->rigidTranslations.destroy();
	buffers->rigidLocalPositions.destroy();
	buffers->rigidLocalNormals.destroy();

	// springs
	buffers->springIndices.destroy();
	buffers->springLengths.destroy();
	buffers->springStiffness.destroy();

	// inflatables
	buffers->inflatableTriOffsets.destroy();
	buffers->inflatableTriCounts.destroy();
	buffers->inflatableVolumes.destroy();
	buffers->inflatableCoefficients.destroy();
	buffers->inflatablePressures.destroy();

	// triangles
	buffers->triangles.destroy();
	buffers->triangleNormals.destroy();
	buffers->uvs.destroy();

	delete buffers;
}


//using namespace Cellaris;

int main()
{

	int start_s = clock();

	// Set up cellaris simulation (bounds, times etc)
	cellaris sim;

	double simulation_end_time = 52.0, simulation_start_time = 0.0;
	int number_time_steps = 52000, num_cells = 1, sampling_count = 1000;
	double dt = (simulation_end_time - simulation_start_time) / number_time_steps;
	myVec3f sim_bounds = myVec3f(2.0f, 2.0f, 2.0f);
	double cell_radius = 0.1;

	// TESTING THE SETUP
	sim.set_start_and_end_times(simulation_start_time,simulation_end_time);
	//sim.set_end_time_and_timesteps(simulation_end_time, time_steps);
	sim.set_times_and_steps(simulation_start_time, simulation_end_time, dt);
	sim.set_num_simulated_cells(num_cells);

	sim.set_output_directory("C:/Users/BCL/Documents/GitHub/Cellaris_testing/Cellaris/out/celldata.txt");
	sim.set_sampling_timestep(sampling_count);
	std::cout << "Set output directory: " << sim.get_output_directory() << '\n';

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
	std::uniform_real_distribution<> rand_x(0.0 + cell_radius * 3, sim_bounds.pos.x - cell_radius * 3);
	std::uniform_real_distribution<> rand_y(0.0 + cell_radius * 3, sim_bounds.pos.y - cell_radius * 3);
	std::uniform_real_distribution<> rand_z(0.0 + cell_radius * 3, sim_bounds.pos.z - cell_radius * 3);
	std::uniform_real_distribution<> cc_length(10, 12);
	std::uniform_real_distribution<> random_birth(-8, -4);

	for (int i = 0; i < num_cells; i++)
	{
		Cell* cell = new Cell;

		myVec3d position; 
		position.pos.x = rand_x(gen); position.pos.y = rand_y(gen); position.pos.z = rand_z(gen);

		cell->set_cell_position(position);
		cell->set_birth_time(random_birth(gen));
		//cell->set_birth_time(0.0);
		cell->set_cell_id(i);
		cell->set_cell_radius(cell_radius);
		cell->set_cell_cycle_length(cc_length(gen));
		//cell->set_cell_cycle_length(10.0);

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


	/* Flex Initialisation */
	//if (g_flex) // If instance of solver already exists remove it
	//{
	//	std::cout << "GFLEX EXISTS" << '\n';

	//	if (g_buffers)
	//		DestroyBuffers(g_buffers);

	//	/*NvFlexDestroySolver(g_flex);
	//	g_flex = NULL;*/
	//}
	// alloc buffers
	g_buffers = AllocBuffers(g_flexLib);

	// map during initialization
	MapBuffers(g_buffers);

	/*g_buffers->positions.resize(0);
	g_buffers->velocities.resize(0);
	g_buffers->phases.resize(0);

	g_buffers->rigidOffsets.resize(0);
	g_buffers->rigidIndices.resize(0);
	g_buffers->rigidMeshSize.resize(0);
	g_buffers->rigidRotations.resize(0);
	g_buffers->rigidTranslations.resize(0);
	g_buffers->rigidCoefficients.resize(0);
	g_buffers->rigidLocalPositions.resize(0);
	g_buffers->rigidLocalNormals.resize(0);

	g_buffers->springIndices.resize(0);
	g_buffers->springLengths.resize(0);
	g_buffers->springStiffness.resize(0);
	g_buffers->triangles.resize(0);
	g_buffers->triangleNormals.resize(0);
	g_buffers->uvs.resize(0);

	g_buffers->shapeGeometry.resize(0);
	g_buffers->shapePositions.resize(0);
	g_buffers->shapeRotations.resize(0);
	g_buffers->shapePrevPositions.resize(0);
	g_buffers->shapePrevRotations.resize(0);
	g_buffers->shapeFlags.resize(0);*/

	// free buffers
	//DestroyBuffers(g_buffers);


	sim.evolve();

	std::cout << "Cells in simulation at end: " << sim.get_number_cells() << '\n';

	sim.destroy_scene_time();

	
	// the code you wish to time goes here
	int stop_s = clock();

	std::cout << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 <<'\n';

	std::cin.ignore();


	return 0;
}

