# Cellaris

Cellaris is an open-source, multi-scale agent-based modelling framework to allow for the particle-constraint based modelling of complex cellular population dynamics using GPU optimised physics solving (via the FleX engine)

Cellaris is developed as part of the Biocompute lab, in collaboration with BrisSynBio, School of Biological Sciences, University of Bristol

see: https://biocomputelab.github.io/index.html

Cellaris is developed using C++ in Microsoft Visual Studio, Windows 10

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

What things you need to install the software and how to install them

```
Microsoft Visual Studio (Target platform version = 10.0.17134.0, Platform Toolset = Visual Studio 2015 (v140))
```

### Installing

Currently the Cellaris toolkit is easiest to develop with inside of Microsoft Visual Studio, this may change with a standalone executable or compiling software.

In Visual Studio:
```
File - Open - Project/Solution - Cellaris.sln
```

The Cellaris software comes with a simple example simulation with spherical particle cells within a cube domain.

The default example simulation is found in cellaris_basicfunctionality.cpp


## Cellaris simulation setup

A Cellaris simulation consists of a 'test' main, consisting of the following key components. 

### Set up the cellaris simulation

To include the cellaris default setup you include the following

```
cellaris sim;
```

with the cellaris class holding the default parameters and the simulation variabled and solvers. 

The simulation times are set as follows:

```
sim.set_times_and_steps(simulation_start_time, simulation_end_time, dt);
```

The simulation output directory is set as follows:

```
sim.set_output_directory("C:/path_to_output_file/Output.txt");
```

with the output sampling rate set with:

```
sim.set_sampling_timestep(sampling_count);
```

where **sampling_count** is the number of timesteps between outputs.

The default scene bounds form a cube (1x1x1). This can be alternatively set with the following:

```
sim.set_scene_upper_bounds(float xbound, float ybound, float zbound);
```

To add cells to the simulation you need to define a **Cell** class based on the default single particle set up

``` 
Cell* cell = new Cell;
```
 
which needs to be set up with a **birth_time**, **particle_buffer_size** and a **cell_cycle_length**

```
cell->set_birth_time(double birth_time);
cell->set_cell_id(int id);
cell->set_flex_particle_buffer_offset(int particle_count);
cell->set_cell_cycle_length(double cell_cycle_length);
```

These cells can be added to the simulation with 

```
sim.add_cell(cell);
```

### Set up the FleX instance

To use the flex library for solving the simulation cell interactions we need to initialize the API with its CUDA based physics solver.

```
sim.initialise_flex_context();
```

This means we are able to utilize the efficiency of the CUDA solving of physics for the simulation.

Now we just need to evole the simulation 

```
sim.evolve();
```

Which will run the simulation according to the times we allocated previously. 

### Shutdown the FleX instance

We need to be careful about shutting down the FleX instance to avoid clashes when repeating the simulation.

``` 
sim.flex_shutdown();
```

### Shutdown the Cellaris scene_time

We need to delete our instance of scene_time to prevent a memory leak

``` 
sim.destroy_scene_time();
```


## Authors

* **Daniel Ward** - daniel.ward@bristol.ac.uk


## License



