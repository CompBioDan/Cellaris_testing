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

#include "cellaris.h"


void cellaris::set_dt(double timestep)
{
	dt = timestep;
}

void cellaris::set_simulation_time(double sim_time)
{
	simulation_time = sim_time;
}

void cellaris::set_start_and_end_times(double start_time, double end_time)
{
	scene_start_time = start_time;
	scene_end_time = end_time;
	scene_time->set_start_time(scene_start_time);
}

void cellaris::destroy_scene_time()
{
	scene_time->destroy();
}

/*void cellaris::set_end_time_and_timesteps(double end_time, unsigned int num_timesteps)
{
	scene_time->SetEndTimeAndNumberOfTimeSteps(end_time, num_timesteps);
	scene_end_time = end_time;
}*/

void cellaris::set_scene_upper_bounds(float x, float y, float z)
{
	scene_upper_bounds = Vec3(x, y, z);
}

double cellaris::get_dt()
{
	return dt;
}

double cellaris::get_start_time()
{
	double start_time = scene_time->get_start_time();
	return start_time;
}

Vec3 cellaris::get_scene_upper_bounds()
{
	return scene_upper_bounds;
}

double cellaris::get_simulation_time()
{
	return simulation_time;
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

//void cellaris::destroy()
//{
//	if (instance)
//	{
//		delete instance;
//		instance = nullptr;
//	}
//}

int cellaris::get_number_cells()
{
	return cell_population.getCount();
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
	cell_population.add(cell);
}

//void cellaris::add_cell(Bacteria* cell)
//{
//	cell_population.add(cell);
//}

Cell* cellaris::get_cell(unsigned int cell_id)
{
	if (cell_id > cell_population.getSize())
		return NULL;
	else
		return cell_population[cell_id];
}

//Bacteria* cellaris::get_cell(unsigned int cell_id)
//{
//	if (cell_id > cell_population.getSize())
//		return NULL;
//	else
//		return cell_population[cell_id];
//}


void cellaris::evolve()
{
	// CHECK SIM PARAMS SETUP
	/*std::cout << "Start time in scene_time: " << scene_time->GetStartTime() << '\n';
	std::cout << "End time in scene_time: " << scene_time->GetEndTime() << '\n';
	std::cout << "Timestep in scene_time: " << scene_time->GetTimeStep() << '\n';
	std::cout << "Time elapsed in scene_time: " << scene_time->GetTimeStepsElapsed() << '\n';
	std::cout << "Current time in scene_time: " << scene_time->GetTime() << '\n';
		
	scene_time->CheckTimestepperTimes();*/

	// set up simulation time
	double current_time = scene_time->get_time();

	// set the end time and number of timesteps
	unsigned num_time_steps = (unsigned)((scene_end_time - current_time) / dt + 0.5);

	if (current_time > 0)
	{
		scene_time->reset_end_time_and_number_of_time_steps(scene_end_time, num_time_steps);
	}
	else
	{
		scene_time->set_end_time_and_number_of_time_steps(scene_end_time, num_time_steps);
	}

	// set the current time
	double time_now = scene_time->get_time();

	// correctly age all cells before main solve loop
	//for (int i = 0; i < get_number_cells(); ++i)
	//{
	//	//cell_population[i]->age();
	//}

	//std::cout << "end time: " << scene_time->GetEndTime() << '\n';
	//std::cout << "current time: " <<time_now << '\n';
	std::cout << "at end of simulation? " << scene_time->has_finished() << '\n';

	/*for (int y = 0; y < get_number_cells(); y++)
	{
		std::cout << "Cell position: x= " << cell_population[y]->get_cell_position().pos.x << " y= " << cell_population[y]->get_cell_position().pos.y << " z= " << cell_population[y]->get_cell_position().pos.z << '\n';
	}*/


	// Entering the main Cellaris solve loop
	printf("Entering Cellaris evolve loop..\n\n");

	map_buffers(flex_buffers);
	std::cout << "Initial position = x: " << flex_buffers->positions[0].x << " y: " << flex_buffers->positions[0].y << " z: " << flex_buffers->positions[0].z << '\n';
	unmap_buffers(flex_buffers);

	// Main solve loop
	while (!(scene_time->has_finished()))
	{

		std::cout << "At timestep: " << scene_time->get_time_steps_elapsed() << '\n';

		//* Output simulation population data with set frequency*/
		if ((scene_time->get_time_steps_elapsed() % output_sampling_timestep_multiple) == 0) {
			for (int i = 0; i < get_number_cells(); ++i)
			{
				//outputcellsdata(i,getCell(i));
				//std::cout << "Simulated cell " << simulated_cell_count << '\n';
				basicCellDataWriter(get_cell(i), scene_output_directory);
			}
		}


		flex_update_frame(); // To access 'position' buffer we need buffers to be mapped -> division of cell and creation of new cell requires mapped buffers so change from seperate flex update frame to cellaris update frame with division in there

		//std::cout << "Flex cell particle count: " << NvFlexGetActiveCount(flex_solver) << '\n';

		//flex_initialise_buffers(); // THIS SHOULDN'T BE HERE

		/*std::cout << "x: " << flex_buffers->positions[0].x << " y: " << flex_buffers->positions[0].y << " z: " << flex_buffers->positions[0].z << '\n';*/


		//UpdateCellPopulation();

		scene_time->increment_time_one_step();
	}

	std::cout << "at end of simulation? " << scene_time->has_finished() << '\n';
}

void cellaris::set_cell_positions(int num_cells, int position_flag)
{
	if (position_flag == 0)
	{
		std::uniform_real_distribution<double> rand_x(0.0, get_scene_upper_bounds().x), rand_y(0.0, get_scene_upper_bounds().y), rand_z(0.0, get_scene_upper_bounds().z);
		std::default_random_engine re;

		for (int i = 0; i < num_cells; i++)
		{
			double pos_x = rand_x(re), pos_y = rand_y(re), pos_z = rand_z(re);
			//cell_population[i]->cell_position = myVec3d(pos_x,pos_y,pos_z);
		}
	}
}

void cellaris::set_cell_cycle_length(double cc_length)
{
	cell_cycle_length = cc_length;
}
	 
void cellaris::set_times_and_steps(double start_time, double end_time, double timestep)
{
	int num_timesteps = (end_time - start_time) / timestep;
	scene_time->set_end_time_and_number_of_time_steps(end_time, num_timesteps);
	scene_time->set_start_time(scene_start_time);
	scene_start_time = start_time;
	scene_end_time = end_time;	
	dt = timestep;
}

//void cellaris::set_num_simulated_cells(int cell_count)
//{
//	simulated_cell_count = cell_count;
//}

const std::string cellaris::get_output_directory()
{
	return scene_output_directory;
}

void cellaris::initialise_flex_scene()
{
	/*
	* This function has to follow 'initialise_flex_context' as that function sets up the CUDA context, the flex library, 
	* and creates a descriptor for the flex context
	*/
	printf("Entering flex scene initialisation..\n\n");

	/* if an instance of the flex solver already exists remove it */
	if (flex_solver) 
	{
		if (flex_buffers) /* if the buffers have already been set then destroy previous buffers */
			destroy_buffers(flex_buffers);

		NvFlexDestroySolver(flex_solver); /* destroy the solver instance */
		flex_solver = NULL;
	}

	/* allocate buffers */
	flex_buffers = alloc_buffers(flex_library);

	map_buffers(flex_buffers);

	/* resize buffers */
	flex_buffers->positions.resize(0);
	flex_buffers->velocities.resize(0);
	flex_buffers->phases.resize(0);

	flex_buffers->rigidOffsets.resize(0);
	flex_buffers->rigidIndices.resize(0);
	flex_buffers->rigidMeshSize.resize(0);
	flex_buffers->rigidRotations.resize(0);
	flex_buffers->rigidTranslations.resize(0);
	flex_buffers->rigidCoefficients.resize(0);
	flex_buffers->rigidLocalPositions.resize(0);
	flex_buffers->rigidLocalNormals.resize(0);

	flex_buffers->springIndices.resize(0);
	flex_buffers->springLengths.resize(0);
	flex_buffers->springStiffness.resize(0);
	flex_buffers->triangles.resize(0);
	flex_buffers->triangleNormals.resize(0);
	flex_buffers->uvs.resize(0);

	g_meshSkinIndices.resize(0);
	g_meshSkinWeights.resize(0);

	std::vector<Emitter> g_emitters(1);	// first emitter is the camera 'gun'

	g_emitters.resize(1);
	g_emitters[0].mEnabled = false;
	g_emitters[0].mSpeed = 1.0f;

	flex_buffers->shapeGeometry.resize(0);
	flex_buffers->shapePositions.resize(0);
	flex_buffers->shapeRotations.resize(0);
	flex_buffers->shapePrevPositions.resize(0);
	flex_buffers->shapePrevRotations.resize(0);
	flex_buffers->shapeFlags.resize(0);

	/* flex simulation parameters */
	flex_params.gravity[0] = 0.0f;
	flex_params.gravity[1] = -9.8f;
	flex_params.gravity[2] = 0.0f;

	flex_params.wind[0] = 0.0f;
	flex_params.wind[1] = 0.0f;
	flex_params.wind[2] = 0.0f;

	flex_params.radius = particle_radius;
	flex_params.viscosity = 0.0f;
	flex_params.dynamicFriction = 0.0f;
	flex_params.staticFriction = 0.0f;
	flex_params.particleFriction = 0.0f; // scale friction between particles by default
	flex_params.freeSurfaceDrag = 0.0f;
	flex_params.drag = 0.0f;
	flex_params.lift = 0.0f;
	flex_params.numIterations = 3;
	flex_params.fluidRestDistance = 0.0f;
	flex_params.solidRestDistance = 0.0f;

	flex_params.anisotropyScale = 1.0f;
	flex_params.anisotropyMin = 0.1f;
	flex_params.anisotropyMax = 2.0f;
	flex_params.smoothing = 1.0f;

	flex_params.dissipation = 0.0f;
	flex_params.damping = 0.0f;
	flex_params.particleCollisionMargin = 0.0f;
	flex_params.shapeCollisionMargin = 0.0f;
	flex_params.collisionDistance = 0.0f;
	flex_params.plasticThreshold = 0.0f;
	flex_params.plasticCreep = 0.0f;
	flex_params.fluid = false;
	flex_params.sleepThreshold = 0.0f;
	flex_params.shockPropagation = 0.0f;
	flex_params.restitution = 0.0f;

	flex_params.maxSpeed = FLT_MAX;
	flex_params.maxAcceleration = 100.0f;	// approximately 10x gravity

	flex_params.relaxationMode = eNvFlexRelaxationLocal;
	flex_params.relaxationFactor = 1.0f;
	flex_params.solidPressure = 1.0f;
	flex_params.adhesion = 0.0f;
	flex_params.cohesion = 0.025f;
	flex_params.surfaceTension = 0.0f;
	flex_params.vorticityConfinement = 0.0f;
	flex_params.buoyancy = 1.0f;
	flex_params.diffuseThreshold = 100.0f;
	flex_params.diffuseBuoyancy = 1.0f;
	flex_params.diffuseDrag = 0.8f;
	flex_params.diffuseBallistic = 16;
	flex_params.diffuseSortAxis[0] = 0.0f;
	flex_params.diffuseSortAxis[1] = 0.0f;
	flex_params.diffuseSortAxis[2] = 0.0f;
	flex_params.diffuseLifetime = 2.0f;

	// planes created after particles
	flex_params.numPlanes = 1;

	// NEED TO INITIALIZE A SCENE HERE (I.E INCLUDE PARTICLES, CONSTRAINTS ETC)
	flex_scene_initialize();


	uint32_t number_particles = flex_buffers->positions.size(); /* number of particles in the scene */
	uint32_t max_number_particles = number_particles + num_extra_particles; /* max number of particles possible in the scene */

	// by default solid particles use the maximum radius
	if (flex_params.fluid && flex_params.solidRestDistance == 0.0f)
		flex_params.solidRestDistance = flex_params.fluidRestDistance;
	else
		flex_params.solidRestDistance = flex_params.radius;

	// collision distance with shapes half the radius
	if (flex_params.collisionDistance == 0.0f)
	{
		flex_params.collisionDistance = flex_params.radius*0.5f;

		if (flex_params.fluid)
			flex_params.collisionDistance = flex_params.fluidRestDistance*0.5f;
	}

	// default particle friction to 10% of shape friction
	if (flex_params.particleFriction == 0.0f)
		flex_params.particleFriction = flex_params.dynamicFriction*0.1f;

	// add a margin for detecting contacts between particles and shapes
	if (flex_params.shapeCollisionMargin == 0.0f)
		flex_params.shapeCollisionMargin = flex_params.collisionDistance*0.5f;


	// calculate particle bounds
	Vec3 particleLower, particleUpper;
	get_particle_bounds(particleLower, particleUpper);

	// update bounds
	scene_lower_bounds = Min(scene_lower_bounds, particleLower);
	scene_upper_bounds = Max(scene_upper_bounds, particleUpper);

	scene_lower_bounds -= flex_params.collisionDistance;
	scene_upper_bounds += flex_params.collisionDistance;

	// update collision planes to match flexs
	Vec3 up = Normalize(Vec3(-wave_floor_tilt, 1.0f, 0.0f));

	(Vec4&)flex_params.planes[0] = Vec4(up.x, up.y, up.z, 0.0f);
	(Vec4&)flex_params.planes[1] = Vec4(0.0f, 0.0f, 1.0f, -scene_lower_bounds.z);
	(Vec4&)flex_params.planes[2] = Vec4(1.0f, 0.0f, 0.0f, -scene_lower_bounds.x);
	(Vec4&)flex_params.planes[3] = Vec4(-1.0f, 0.0f, 0.0f, scene_upper_bounds.x);
	(Vec4&)flex_params.planes[4] = Vec4(0.0f, 0.0f, -1.0f, scene_upper_bounds.z);
	(Vec4&)flex_params.planes[5] = Vec4(0.0f, -1.0f, 0.0f, scene_upper_bounds.y);

	wave_plane = flex_params.planes[2][3];

	flex_buffers->normals.resize(0);
	flex_buffers->normals.resize(max_number_particles);

	// main create method for the Flex solver
	flex_solver = NvFlexCreateSolver(flex_library, max_number_particles, max_diffuse_particles, max_neighbours_per_particle);

	// create active indices (just a contiguous block for the demo)
	flex_buffers->activeIndices.resize(flex_buffers->positions.size());
	for (int i = 0; i < flex_buffers->activeIndices.size(); ++i)
		flex_buffers->activeIndices[i] = i;

	// resize particle buffers to fit
	flex_buffers->positions.resize(max_number_particles);
	flex_buffers->velocities.resize(max_number_particles);
	flex_buffers->phases.resize(max_number_particles);

	flex_buffers->densities.resize(max_number_particles);
	flex_buffers->anisotropy1.resize(max_number_particles);
	flex_buffers->anisotropy2.resize(max_number_particles);
	flex_buffers->anisotropy3.resize(max_number_particles);

	// save rest positions
	flex_buffers->restPositions.resize(flex_buffers->positions.size());
	for (int i = 0; i < flex_buffers->positions.size(); ++i)
		flex_buffers->restPositions[i] = flex_buffers->positions[i];

	flex_initialise_buffers();

	// unmap so we can start transferring data to GPU
	unmap_buffers(flex_buffers);

	//-----------------------------
	// Send data to Flex
	NvFlexSetParams(flex_solver, &flex_params);
	NvFlexSetParticles(flex_solver, flex_buffers->positions.buffer, number_particles);
	NvFlexSetVelocities(flex_solver, flex_buffers->velocities.buffer, number_particles);
	NvFlexSetNormals(flex_solver, flex_buffers->normals.buffer, number_particles);
	NvFlexSetPhases(flex_solver, flex_buffers->phases.buffer, flex_buffers->phases.size());
	NvFlexSetRestParticles(flex_solver, flex_buffers->restPositions.buffer, flex_buffers->restPositions.size());
	NvFlexSetActive(flex_solver, flex_buffers->activeIndices.buffer, number_particles);

	// springs
	if (flex_buffers->springIndices.size())
	{
		assert((flex_buffers->springIndices.size() & 1) == 0);
		assert((flex_buffers->springIndices.size() / 2) == flex_buffers->springLengths.size());

		NvFlexSetSprings(flex_solver, flex_buffers->springIndices.buffer, flex_buffers->springLengths.buffer, flex_buffers->springStiffness.buffer, flex_buffers->springLengths.size());
	}

	// rigids
	if (flex_buffers->rigidOffsets.size())
	{
		NvFlexSetRigids(flex_solver, flex_buffers->rigidOffsets.buffer, flex_buffers->rigidIndices.buffer, flex_buffers->rigidLocalPositions.buffer, flex_buffers->rigidLocalNormals.buffer, flex_buffers->rigidCoefficients.buffer, flex_buffers->rigidRotations.buffer, flex_buffers->rigidTranslations.buffer, flex_buffers->rigidOffsets.size() - 1, flex_buffers->rigidIndices.size());
	}

	// inflatables
	if (flex_buffers->inflatableTriOffsets.size())
	{
		NvFlexSetInflatables(flex_solver, flex_buffers->inflatableTriOffsets.buffer, flex_buffers->inflatableTriCounts.buffer, flex_buffers->inflatableVolumes.buffer, flex_buffers->inflatablePressures.buffer, flex_buffers->inflatableCoefficients.buffer, flex_buffers->inflatableTriOffsets.size());
	}

	// dynamic triangles
	if (flex_buffers->triangles.size())
	{
		NvFlexSetDynamicTriangles(flex_solver, flex_buffers->triangles.buffer, flex_buffers->triangleNormals.buffer, flex_buffers->triangles.size() / 3);
	}

	// collision shapes
	if (flex_buffers->shapeFlags.size())
	{
		NvFlexSetShapes(
			flex_solver,
			flex_buffers->shapeGeometry.buffer,
			flex_buffers->shapePositions.buffer,
			flex_buffers->shapeRotations.buffer,
			flex_buffers->shapePrevPositions.buffer,
			flex_buffers->shapePrevRotations.buffer,
			flex_buffers->shapeFlags.buffer,
			int(flex_buffers->shapeFlags.size()));
	}


	//map_buffers(flex_buffers);

	//std::cout << "Flex initialised positions:- x = " << flex_buffers->positions[0].x << ", y = " << flex_buffers->positions[0].y << ", z = " << flex_buffers->positions[0].z << '\n';
}

cellaris::SimBuffers* cellaris::alloc_buffers(NvFlexLibrary* lib)
{
	return new SimBuffers(lib);
}

void cellaris::map_buffers(SimBuffers* buffers)
{
	buffers->positions.map();
	buffers->restPositions.map();
	buffers->velocities.map();
	buffers->phases.map();
	buffers->densities.map();
	buffers->anisotropy1.map();
	buffers->anisotropy2.map();
	buffers->anisotropy3.map();
	buffers->normals.map();
	buffers->diffusePositions.map();
	buffers->diffuseVelocities.map();
	buffers->diffuseIndices.map();
	buffers->smoothPositions.map();
	buffers->activeIndices.map();

	// convexes
	buffers->shapeGeometry.map();
	buffers->shapePositions.map();
	buffers->shapeRotations.map();
	buffers->shapePrevPositions.map();
	buffers->shapePrevRotations.map();
	buffers->shapeFlags.map();

	buffers->rigidOffsets.map();
	buffers->rigidIndices.map();
	buffers->rigidMeshSize.map();
	buffers->rigidCoefficients.map();
	buffers->rigidRotations.map();
	buffers->rigidTranslations.map();
	buffers->rigidLocalPositions.map();
	buffers->rigidLocalNormals.map();

	buffers->springIndices.map();
	buffers->springLengths.map();
	buffers->springStiffness.map();

	// inflatables
	buffers->inflatableTriOffsets.map();
	buffers->inflatableTriCounts.map();
	buffers->inflatableVolumes.map();
	buffers->inflatableCoefficients.map();
	buffers->inflatablePressures.map();

	buffers->triangles.map();
	buffers->triangleNormals.map();
	buffers->uvs.map();
}

void cellaris::unmap_buffers(SimBuffers* buffers)
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

void cellaris::destroy_buffers(SimBuffers* buffers)
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

void ErrorCallback(NvFlexErrorSeverity, const char* msg, const char* file, int line)
{
	printf("Flex: %s - %s:%d\n", msg, file, line);
	//g_Error = true;
	//assert(0); asserts are bad for TeamCity
}

void cellaris::intialise_flex_context()
{
	/* This initialise function selects the graphics card to use with the flex library and solver. 
	 * Creates an optimized CUDA context for flex and sets on the calling thread
	 * Initialize flex and allocates a flex descriptor (desc)
	 * Initialize the flex library (flex_library) */
	printf("Initializing flex context..\n\n");

	if (graphics_device == -1) // use the physX gpu selected from the NVIDIA control panel
		graphics_device = NvFlexDeviceGetSuggestedOrdinal(); // 

	/* Create an optimized CUDA context for Flex and set it on the 
	 * calling thread. This is an optional call, it is fine to use 
	 * a regular CUDA context, although creating one through this API
	 * is recommended for best performance. */
	bool success = NvFlexDeviceCreateCudaContext(graphics_device);

	if (!success) // catch if we fail to create the CUDA solving context
	{
		printf("Error creating CUDA context.\n");
		exit(-1);
	}

	/* Initialise flex with descriptor (desc) */
	NvFlexInitDesc desc;
	desc.deviceIndex = graphics_device;
	desc.enableExtensions = true; 
	desc.renderDevice = 0;
	desc.renderContext = 0;
	desc.computeType = eNvFlexCUDA;

	/* initialise the Flex library, note that no CUDA methods should be called before this 
	 * point to ensure we get the device context we want */
	flex_library = NvFlexInit(NV_FLEX_VERSION, ErrorCallback, &desc);

	if (flex_library == NULL) // catch if we could not successfully initialize flex instance
	{
		printf("Could not initialize Flex, exiting.\n");
		exit(-1);
	}

	/* store graphics card device name */
	strcpy(device_name, NvFlexGetDeviceName(flex_library)); 
	printf("Graphics card for FleX: %s\n\n", device_name);

	/* PUT SCENE SETUP HERE TO CHECK */
	initialise_flex_scene();

}

void cellaris::flex_shutdown()
{
	printf("Shutting down flex..\n\n");

	/* method for destorying buffers, flex solver and flex libary following simulation */
	destroy_buffers(flex_buffers); // free buffers

	NvFlexDestroySolver(flex_solver); // destroy flex solver instance
	NvFlexShutdown(flex_library); // destroy flex library instance
}

//void cellaris::error_callback(NvFlexErrorSeverity, const char* msg, const char* file, int line)
//{
//	printf("Flex: %s - %s:%d\n", msg, file, line);
//	g_Error = true;
//	//assert(0); asserts are bad for TeamCity
//}

void cellaris::get_particle_bounds(Vec3& lower, Vec3& upper)
{
	lower = Vec3(FLT_MAX);
	upper = Vec3(-FLT_MAX);

	for (int i = 0; i < flex_buffers->positions.size(); ++i)
	{
		lower = Min(Vec3(flex_buffers->positions[i]), lower);
		upper = Max(Vec3(flex_buffers->positions[i]), upper);
	}
}

void cellaris::flex_update_frame()
{
	static double lastTime;

	// real elapsed frame time
	double frameBeginTime = GetSeconds();

	real_dt = float(frameBeginTime - lastTime);
	lastTime = frameBeginTime;

	// scene update
	double waitBeginTime = GetSeconds();

	map_buffers(flex_buffers);

	//std::cout << "x: " << flex_buffers->positions[0].x << " y: " << flex_buffers->positions[0].y << " z: " << flex_buffers->positions[0].z << '\n';

	double waitEndTime = GetSeconds();

	// UPDATE CAMERA IN FLEX_DEMO

	flex_scene_update();

	// Undergo check for cell growth
	for (int i = 0; i < get_number_cells(); ++i)
	{
		if (cell_population[i]->ready_to_grow())
		{
			//cell_population[i]->grow_cell();
			cell_growth(i);
			std::cout << "Ready to grow" << '\n';
		}
	}

	// Undergo check for cell division
	//for (int i = 0; i < get_number_cells(); ++i)
	//{
	//	if (cell_population[i]->ready_to_divide())
	//	{
	//		cell_division(i);
	//	}
	//}

	// RENDER IN FLEX_DEMO

	unmap_buffers(flex_buffers);

	// Flex update
	double updateBeginTime = GetSeconds();

	// send any particle updates to the solver
	NvFlexSetParticles(flex_solver, flex_buffers->positions.buffer, flex_buffers->positions.size());
	NvFlexSetVelocities(flex_solver, flex_buffers->velocities.buffer, flex_buffers->velocities.size());
	NvFlexSetPhases(flex_solver, flex_buffers->phases.buffer, flex_buffers->phases.size());
	NvFlexSetActive(flex_solver, flex_buffers->activeIndices.buffer, flex_buffers->activeIndices.size());

	flex_scene_sync();

	// tick solver
	NvFlexSetParams(flex_solver, &flex_params);
	NvFlexUpdateSolver(flex_solver, dt, number_substeps, false); 

	flex_frame++;

	// read back base particle data
	// Note that flexGet calls don't wait for the GPU, they just queue a GPU copy 
	// to be executed later.
	// When we're ready to read the fetched buffers we'll Map them, and that's when
	// the CPU will wait for the GPU flex update and GPU copy to finish.
	NvFlexGetParticles(flex_solver, flex_buffers->positions.buffer, flex_buffers->positions.size());
	NvFlexGetVelocities(flex_solver, flex_buffers->velocities.buffer, flex_buffers->velocities.size());
	NvFlexGetNormals(flex_solver, flex_buffers->normals.buffer, flex_buffers->normals.size());

	// readback triangle normals
	if (flex_buffers->triangles.size())
		NvFlexGetDynamicTriangles(flex_solver, flex_buffers->triangles.buffer, flex_buffers->triangleNormals.buffer, flex_buffers->triangles.size() / 3);

	// readback rigid transforms
	if (flex_buffers->rigidOffsets.size())
		NvFlexGetRigidTransforms(flex_solver, flex_buffers->rigidRotations.buffer, flex_buffers->rigidTranslations.buffer);

	//if (!g_interop)
	//{
	//	// if not using interop then we read back fluid data to host
	//	if (g_drawEllipsoids)
	//	{
	//		NvFlexGetSmoothParticles(flex_solver, flex_buffers->smoothPositions.buffer, flex_buffers->smoothPositions.size());
	//		NvFlexGetAnisotropy(flex_solver, flex_buffers->anisotropy1.buffer, flex_buffers->anisotropy2.buffer, flex_buffers->anisotropy3.buffer);
	//	}

	//	// read back diffuse data to host
	//	if (g_drawDensity)
	//		NvFlexGetDensities(flex_solver, flex_buffers->densities.buffer, flex_buffers->positions.size());

	//	if (g_diffuseRenderBuffers.mNumDiffuseParticles)
	//	{
	//		NvFlexGetDiffuseParticles(flex_solver, flex_buffers->diffusePositions.buffer, flex_buffers->diffuseVelocities.buffer, flex_buffers->diffuseIndices.buffer);
	//	}
	//}

	double updateEndTime = GetSeconds();

}

void cellaris::flex_scene_sync()
{

}

void cellaris::flex_scene_update()
{

	//std::cout << "Flex particle positions:- x = " << flex_buffers->positions[0].x << ", y = " << flex_buffers->positions[0].y << ", z = " << flex_buffers->positions[0].z << '\n';

}

//void cellaris::flex_scene_initialize()
//{
//	/*
//	* This function effectively sets up the initial state of the flex simulation scene, including the
//	* allocation of particles and constraints, as well as any global flex parameter changes.
//	*/
//
//	const float mRadius = 0.05f; // Radius of cells forming the fluid
//	const float radius = mRadius;
//	const float restDistance = mRadius * 0.65f; // 0.0325 (ratio of radius: rest distance ~ 2:1)
//
//	int dx = 1, dy = 1, dz = 1; // number of cells in each direction
//
//	for (int x = 0; x < dx; ++x)
//	{
//		for (int y = 0; y < dy; ++y)
//		{
//			for (int z = 0; z < dz; ++z)
//			{
//
//				//Vec3 position = Vec3(0.0f, restDistance, 0.0f) + Vec3(float(x), float(y), float(z))*(radius * 4);
//				Vec3 position = Vec3(1.0f, 1.0f, 1.0f);
//				flex_buffers->positions.push_back(Vec4(position.x, position.y, position.z, 1.0f));
//				flex_buffers->velocities.push_back(Vec3(0.0f));
//				flex_buffers->phases.push_back(NvFlexMakePhase(0, eNvFlexPhaseSelfCollide | eNvFlexPhaseFluid));
//			}
//		}
//	}
//
//
//	scene_lower_bounds = Vec3(0.0f, 0.0f, 0.0f);
//	scene_upper_bounds = Vec3(scene_upper_bounds.x, scene_upper_bounds.y, scene_upper_bounds.z);
//
//	number_substeps = 2;
//
//	flex_params.fluid = true;
//	flex_params.radius = radius;
//	flex_params.fluidRestDistance = restDistance;
//	flex_params.dynamicFriction = 0.f;
//	flex_params.restitution = 0.001f;
//
//	flex_params.numIterations = 3;
//	flex_params.relaxationFactor = 1.0f;
//
//	flex_params.smoothing = 0.4f;
//	flex_params.anisotropyScale = 3.0f / radius;
//
//	flex_params.viscosity = 0.001f;
//	flex_params.cohesion = 0.1f;
//	flex_params.vorticityConfinement = 80.0f;
//	flex_params.surfaceTension = 0.0f;
//	flex_params.gravity[1] = 0.0f;
//	flex_params.numPlanes = 6;
//
//	// limit velocity to CFL condition
//	flex_params.maxSpeed = 0.5f*radius*number_substeps / dt;
//
//	max_diffuse_particles = 0;
//
//	//g_waveFrequency = 1.0f;
//	//g_waveAmplitude = 2.0f;
//	//g_waveFloorTilt = 0.0f;
//
//	//// draw options		
//	//g_drawPoints = true;
//	//g_drawEllipsoids = false;
//	//g_drawDiffuse = true;
//}

void cellaris::flex_scene_initialize()
{
	///*
	//* This function effectively sets up the initial state of the flex simulation scene, including the
	//* allocation of particles and constraints, as well as any global flex parameter changes.
	//*/

	//// Limit on max number of particles/springs in the simulation
	////g_numExtraParticles = 1000000;
	///////////////////////////////////////////////////////////////

	//float length = 0.4f; // Length of the 'rod' cells
	//int numbersegments = 5;
	//float mVelocity = 5.0f;
	///////////////////////////////////////////////////////////////

	////// Simulation parameters/////////////////////////////////////
	////g_params.numIterations = 5;
	////g_params.radius = radius;
	////g_params.dynamicFriction = 0.2f;
	////g_params.dissipation = 0.001f;
	////g_params.sleepThreshold = g_params.radius*0.2f;
	////g_params.relaxationFactor = 1.0f;
	////g_params.restitution = 0.0f;
	////g_params.shapeCollisionMargin = 0.01f;
	////g_params.particleCollisionMargin = g_params.radius*0.05f;
	////g_params.gravity[1] = 0.0f;
	////g_params.numPlanes = 6;
	////g_lightDistance *= 0.5f;
	////g_drawPoints = true;
	////g_drawRopes = true;
	////g_drawSprings = true;
	////dt = 1 / (double)60;
	////mFrame = 0;


	//// Cell population parameters
	//int cellCount = 1; // starting cell count
	//int numSprings = 0; // number of springs in the scene
	//int maxCellParticles = 6; // Max number of particles in each cell
	//int numExtraParticles = 0; // Number of extra particles to add to the simulation
	//int sceneParticleCount = 0; // Number of particles in the scene (used for initialising buffer tranfer holder
	//						/////////////////////////////////////////////////////////////

	//// Create the initial cells//////////////////////////////////
	//int offsetCounter = 0; // Loop variable for setting first buffer index of cell particle data
	//int number_springser = 0; // Loop variable for setting the first buffer index of cell spring data
	//numbersegments = 2;
	//for (int i = 0; i < cellCount; ++i)
	//{
	///*	Bacteria bac;
	//	bac.particle_offset = offsetCounter;
	//	bac.spring_offset = number_springser;
	//	bac.number_particles = int(ceil(length / radius)) + 1;
	//	bac.number_springs = 0;
	//	bac.cell_type = 1;
	//	bac.cell_age = 0.0;
	//	bac.cell_id = i;
	//	bac.grow = 0;
	//	bac.divide = 0;
	//	bac.cell_velocity = 5.0f;*/


	//	create_bacteria(Vec3(Randf(scene_lower_bounds.x + length, scene_upper_bounds.x - length), Randf(scene_lower_bounds.y + length, scene_upper_bounds.y - length), Randf(scene_lower_bounds.z + length, scene_upper_bounds.z - length)), Vec3(1.0f, 0.0f, 0.0f), 0.25f, int(ceil(length / flex_params.radius)), length, NvFlexMakePhase(0, eNvFlexPhaseSelfCollide));
	//	//g_bac.push_back(bac);

	///*	offsetCounter += bac.number_particles;
	//	number_springser += bac.number_springs;

	//	sceneParticleCount += bac.number_particles;*/

	//}
	float length = 0.4f;
	//create_bacteria(Vec3(Randf(scene_lower_bounds.x + length, scene_upper_bounds.x - length), Randf(scene_lower_bounds.y + length, scene_upper_bounds.y - length), Randf(scene_lower_bounds.z + length, scene_upper_bounds.z - length)), Vec3(1.0f, 0.0f, 0.0f), 0.25f, int(ceil(length / particle_radius)), length, NvFlexMakePhase(0, eNvFlexPhaseSelfCollide));
	create_bacteria(Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), 0.25f, int(ceil(length / particle_radius)), length, NvFlexMakePhase(0, eNvFlexPhaseSelfCollide));

	//flex_buffers->positions[0].x = 1.0f;
}

void cellaris::create_bacteria(Vec3 start, Vec3 dir, float stiffness, int segments, float length, int phase, float spiralAngle, float invmass, float give)
{
	flex_buffers->positions.push_back(Vec4(start.x, start.y, start.z, invmass));
	flex_buffers->velocities.push_back(0.0f);
	flex_buffers->phases.push_back(phase);//int(g_buffers->positions.size()));

	Vec3 left, right;
	BasisFromVector(dir, &left, &right);

	float segmentLength = length / segments;
	Vec3 spiralAxis = dir;
	float spiralHeight = spiralAngle / (2.0f*kPi)*(length / segments);

	if (spiralAngle > 0.0f)
		dir = left;

	Vec3 p = start;

	for (int i = 0; i < segments; ++i)
	{
		int prev = int(flex_buffers->positions.size()) - 1;

		p += dir * segmentLength;

		// rotate 
		if (spiralAngle > 0.0f)
		{
			p += spiralAxis * spiralHeight;

			dir = RotationMatrix(spiralAngle, spiralAxis)*dir;
		}

		flex_buffers->positions.push_back(Vec4(p.x, p.y, p.z, 1.0f));
		flex_buffers->velocities.push_back(0.0f);
		flex_buffers->phases.push_back(phase);

		// stretch
		create_spring(prev, prev + 1, stiffness, give);

		// bending spring
		if (i > 0)
		{
			create_spring(prev - 1, prev + 1, stiffness*0.5f, give);
		}
	}
}

void cellaris::create_spring(int first, int second, float stiffness, float give)
{
	flex_buffers->springIndices.push_back(first);
	flex_buffers->springIndices.push_back(second);
	flex_buffers->springLengths.push_back((1.0f + give)*Length(Vec3(flex_buffers->positions[first]) - Vec3(flex_buffers->positions[second])));
	flex_buffers->springStiffness.push_back(stiffness);
}

//void cellaris::flex_initialise_buffers()
//{
//
//	//for (int y = 0; y < get_number_cells(); y++)
//	//{
//	//	std::cout << "Cell position: x= " << cell_population[y]->get_cell_position().pos.x << " y= " << cell_population[y]->get_cell_position().pos.y << " z= " << cell_population[y]->get_cell_position().pos.z << '\n';
//	//}
//
//	//for (int c_count = 0; c_count < get_number_cells(); c_count++)
//	//{
//
//
//	//	std::cout << "buffer size: " << flex_buffers->positions.size() << '\n';
//	//	/*flex_buffers->positions[c_count] = Vec4(cell_population[c_count]->get_cell_position().pos.x, cell_population[c_count]->get_cell_position().pos.y, cell_population[c_count]->get_cell_position().pos.z, 1.0f);*/
//	//	//flex_buffers->positions[c_count] = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
//	//	flex_buffers->positions[c_count].x = cell_population[c_count]->get_cell_position().pos.x;
//	//	//flex_buffers->positions[c_count].y = cell_population[c_count]->get_cell_position().pos.y;
//	//	//flex_buffers->positions[c_count].z = cell_population[c_count]->get_cell_position().pos.z;
//	//	//flex_buffers->positions[c_count].w = 1.0f; // inverse mass
//
//	//	//flex_buffers->velocities[c_count] = 0.0f;
//
//	//	//flex_buffers->phases[c_count] = NvFlexMakePhase(0, eNvFlexPhaseSelfCollide);
//	//}
//
//
//}

void cellaris::cell_division(int cell_id)
{
	printf("Entering cell division loop..\n\n");

	Cell* daughter_cell = cell_population[cell_id]->divide();
	//Bacteria* daughter_cell = cell_population[cell_id]->divide();
	daughter_cell->set_cell_id(get_number_cells());

	/** Set daughter cell position (random position around mother cell) */
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> azim(-pi, pi);
	std::uniform_real_distribution<> zen(0.0, pi);
	int conflict = 1;

	//myVec3d daughter_position = get_cell_position();
	//myVec3d mother_position = cell_population[i]->get_cell_position();
	myVec3d mother_position = myVec3d(flex_buffers->positions[cell_id].x, flex_buffers->positions[cell_id].y, flex_buffers->positions[cell_id].z);
	myVec3d daughter_position;

	while (conflict != 0)
	{
		conflict = 1;

		daughter_position.pos.x = mother_position.pos.x + (particle_radius*cos(azim(gen)*sin(zen(gen))));
		daughter_position.pos.y = mother_position.pos.y + (particle_radius*sin(azim(gen)*sin(zen(gen))));
		daughter_position.pos.z = mother_position.pos.z + (particle_radius*cos(zen(gen)));

		/** Check new daughter position doesn't conflict with current cells */
		for (int cell_check = 0; cell_check < get_number_cells(); cell_check++)
		{
			double distance = sqrt(pow(daughter_position.pos.x - flex_buffers->positions[cell_id].x, 2) + pow(daughter_position.pos.y - flex_buffers->positions[cell_id].y, 2) + pow(daughter_position.pos.z - flex_buffers->positions[cell_id].z, 2));

			if (distance < particle_radius) {
				conflict++;
			}
			if (daughter_position.pos.x < scene_lower_bounds.x || daughter_position.pos.x > scene_upper_bounds.x) {
				conflict++;
			}
			if (daughter_position.pos.y < scene_lower_bounds.y || daughter_position.pos.y > scene_upper_bounds.y) {
				conflict++;
			}
			if (daughter_position.pos.z < scene_lower_bounds.z || daughter_position.pos.z > scene_upper_bounds.z) {
				conflict++;
			}
		}

		if (conflict == 1)
			conflict = 0;

	}


	flex_buffers->positions[get_number_cells()] = Vec4(daughter_position.pos.x, daughter_position.pos.y, daughter_position.pos.z, 1.0f);
	flex_buffers->velocities[get_number_cells()] = 0.0f;
	flex_buffers->phases[get_number_cells()] =  NvFlexMakePhase(0, eNvFlexPhaseSelfCollide);
	//daughter_cell->set_cell_position(daughter_position);

	add_cell(daughter_cell);
	//simulated_cell_count++;



}

/* 
Initialise the buffers for flex -> effectively we are creating the initial particle and constraint setups for our specified cell types here 

For single particle cells this is a simple loop through number of cells

For bacteria we need to know the number of particles and constraints in each cell
*/
void cellaris::flex_initialise_buffers()
{
	//// INITIALIZE FOR SINGLE PARTICLE CELLS
	///** Set daughter cell position (random position around mother cell) */
	//std::random_device rd;  //Will be used to obtain a seed for the random number engine
	//std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	//std::uniform_real_distribution<> positionx(scene_lower_bounds.x, scene_upper_bounds.x);
	//std::uniform_real_distribution<> positiony(scene_lower_bounds.y, scene_upper_bounds.y);
	//std::uniform_real_distribution<> positionz(scene_lower_bounds.z, scene_upper_bounds.z);

	//std::cout << "Flex buffer size: " << flex_buffers->positions.size() << '\n';

	//for (int cc = 0; cc < cell_population.getSize(); cc++)
	//{
	//	std::cout << "cc = " << cc << " pop size = " << cell_population.getSize() << '\n';
	//	flex_buffers->positions[cc] = Vec4(positionx(gen), positiony(gen), positionz(gen), 1.0f);
	//	flex_buffers->phases[cc] = NvFlexMakePhase(0, eNvFlexPhaseSelfCollide);
	//	flex_buffers->velocities[cc] = 0.0f;
	//}



	//// INITIALISE FOR BACTERIA CELLS
	//for (int cc = 0; cc < cell_population.getSize(); cc++)
	//{
	//	std::cout << "cc = " << cc << " pop size = " << cell_population.getSize() << '\n';
	//	flex_buffers->positions[cc*cell_population[cc]->get_particle_offset()] = Vec4(positionx(gen), positiony(gen), positionz(gen), 1.0f);
	//	flex_buffers->phases[cc] = NvFlexMakePhase(0, eNvFlexPhaseSelfCollide);
	//	flex_buffers->velocities[cc] = 0.0f;
	//}
}

//unsigned cellaris::get_simulated_cell_count()
//{
//
//	return simulated_cell_count;
//
//}

void cellaris::cell_growth(int cellID)
{
	int cellFirstParticleIndex;
	int cellFirstSpringIndex;

	// Resize the interim buffers to zero to be rebuilt
	int numParticleGrowth = 0;
	particles_holder.resize(0);
	velocities_holder.resize(0);
	phases_holder.resize(0);

	springstiffness_holder.resize(0);
	springlengths_holder.resize(0);
	springindices_holder.resize(0);

	// Looping variable for rebuilding the active particle and spring buffers
	int numActiveParticles = 0;
	int numActiveSprings = 0;

	// Copy cells into relevant holders from the buffers
	for (int cell_counter = 0; cell_counter < get_number_cells(); cell_counter++)
	{
		// Index of first particle in cell
		cellFirstParticleIndex = cell_population[cell_counter]->get_flex_particle_buffer_offset() - numParticleGrowth;
		cellFirstSpringIndex = cell_population[cell_counter]->get_flex_spring_buffer_offset() - (2 * numParticleGrowth);

		int numberParticlesInCell = cell_population[cell_counter]->get_flex_particle_count();
		int numberSpringsInCell = cell_population[cell_counter]->get_flex_spring_count();
		int endParticleIndex = (cellFirstParticleIndex + numberParticlesInCell) - 1;

		// Create temporary buffers for the particles in the simulation
		for (int j = 0; j < numberParticlesInCell; ++j)
		{
			particles_holder.push_back(flex_buffers->positions[cellFirstParticleIndex + j]);
			velocities_holder.push_back(flex_buffers->velocities[cellFirstParticleIndex + j]);
			phases_holder.push_back(flex_buffers->phases[cellFirstParticleIndex + j]);
		}

		for (int j = 0; j < numberSpringsInCell; ++j)
		{
			springstiffness_holder.push_back(flex_buffers->springStiffness[cellFirstSpringIndex + j]);
			springlengths_holder.push_back(flex_buffers->springLengths[cellFirstSpringIndex + j]);
		}

		for (int j = 0; j < numberSpringsInCell * 2; ++j)
		{
			springindices_holder.push_back(flex_buffers->springIndices[cellFirstSpringIndex * 2 + j] + numParticleGrowth);
		}


		numActiveParticles += numberParticlesInCell;
		numActiveSprings += numberSpringsInCell;

		particleSpacing.x = flex_buffers->positions[endParticleIndex].x - flex_buffers->positions[endParticleIndex - 1].x;
		particleSpacing.y = flex_buffers->positions[endParticleIndex].y - flex_buffers->positions[endParticleIndex - 1].y;
		particleSpacing.z = flex_buffers->positions[endParticleIndex].z - flex_buffers->positions[endParticleIndex - 1].z;

		// If cell is flagged for growth then add a new particle to the end of the bacteria and add to the buffer holders
		if (cell_counter == cellID)
		{
			// Set the particle to end of the current cell 
			particles_holder.push_back(Vec4(flex_buffers->positions[endParticleIndex].x + (particleSpacing.x), flex_buffers->positions[endParticleIndex].y + (particleSpacing.y), flex_buffers->positions[endParticleIndex].z + (particleSpacing.z), 1.0f));
			velocities_holder.push_back(flex_buffers->velocities[endParticleIndex]);
			phases_holder.push_back(flex_buffers->phases[endParticleIndex]);

			// spring 1 (new to end)
			springstiffness_holder.push_back(0.25f);
			springlengths_holder.push_back(Length(Vec3(flex_buffers->positions[endParticleIndex]) - Vec3(particles_holder[numActiveParticles])));
			springindices_holder.push_back(numParticleGrowth + (endParticleIndex));
			springindices_holder.push_back(numActiveParticles);

			// spring 2 (new to end -1)
			springstiffness_holder.push_back(0.25f);
			springlengths_holder.push_back(Length(Vec3(flex_buffers->positions[endParticleIndex - 1]) - Vec3(particles_holder[numActiveParticles])));
			springindices_holder.push_back(numParticleGrowth + (endParticleIndex - 1));
			springindices_holder.push_back(numActiveParticles);

			// Increment cell particle and spring counters
			/* For a bacterium the growth involves adding a single particle */
			cell_population[cell_counter]->set_flex_particle_count(cell_population[cell_counter]->get_flex_particle_count() + 1);

			/* We add an additional two springs for each growth step */
			cell_population[cell_counter]->set_flex_spring_count(cell_population[cell_counter]->get_flex_spring_count() + 2);
			//g_bac[cell_counter].number_particles += 1;
			//g_bac[cell_counter].number_springs += 2;

			for (int cc = cell_counter + 1; cc < get_number_cells(); cc++)
			{
				cell_population[cell_counter]->set_flex_particle_count(cell_population[cell_counter]->get_flex_particle_count() + 1);

				/* We add an additional two springs for each growth step */
				cell_population[cell_counter]->set_flex_spring_count(cell_population[cell_counter]->get_flex_spring_count() + 2);
			}

			numActiveParticles += 1;
			numActiveSprings += 2;
			//grow = 0;
			numParticleGrowth += 1;

		}

		//g_bac[cell_counter].grow = 0;

	}

	// Assign the holders to the FleX buffers
	flex_buffers->positions.assign(&particles_holder[0], particles_holder.size());
	flex_buffers->velocities.assign(&velocities_holder[0], velocities_holder.size());
	flex_buffers->phases.assign(&phases_holder[0], phases_holder.size());

	flex_buffers->springIndices.assign(&springindices_holder[0], springindices_holder.size());
	flex_buffers->springLengths.assign(&springlengths_holder[0], springlengths_holder.size());
	flex_buffers->springStiffness.assign(&springstiffness_holder[0], springstiffness_holder.size());

	// Extend the active indices to include any new added particles
	flex_buffers->activeIndices.resize(numActiveParticles);
	for (int i = 0; i < numActiveParticles; ++i)
		flex_buffers->activeIndices[i] = i;

}

void cellaris::DrawShapes()
{
	
}

