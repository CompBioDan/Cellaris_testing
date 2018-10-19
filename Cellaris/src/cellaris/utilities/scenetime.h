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

#ifndef SCENETIME_H_
#define SCENETIME_H_

#include "timestepper.h"
#include <memory>
#include <cmath>
#include <cassert>

/**
*
* We define a 'Scene time' object to store the simulation time.
* This object uses a singleton pattern to make sure we have a globally consistent time value.
*
* Start time, end time and number of time steps must be set before the time can be incremented and returned.
*
* This class is used in conjunction with the IncrementTimeOneStep() and GetTime() calls.
*
**/

class SceneTime {

public:

	/**
	* @return a pointer to the scene time object
	*/
	static SceneTime* instance();

	/** Sets the end time and number of timesteps - called after SetStartTime() but before other methods */
	void set_end_time_and_number_of_time_steps(double endTime, unsigned totalSimulationTimeSteps);

	void reset_end_time_and_number_of_time_steps(const double& rEndTime, const unsigned& rNumberOfTimeStepsInThisRun);

	/** @return the timestep for the simulation, set in earlier calls */
	double get_time_step() const;

	/** Increments the simulation time by a single time step */
	void increment_time_one_step();

	/** @returns the number of timesteps that have elapsed in simulation thus far */
	unsigned get_time_steps_elapsed() const;

	/** @returns the simulation time */
	double get_time() const;

	/** Destroys the current SceneTime instance */
	static void destroy();

	/** Set the simulation start time */
	void set_start_time(double startTime);

	/** @return the simulation start time */
	double get_start_time() const;

	/** @return the simulation end time */
	double get_end_time() const;

	/** Has the simulation finished running */
	bool has_finished() const;

	double check_timestepper() const;

protected:

	/** default scene time constructor -> must set start time, end time and number of timesteps before using object */
	SceneTime();

private:

	/** Pointer to the singleton instance of this class */
	static SceneTime* mpInstance;

	/** Delegate all time stepping to a TimeStepper class */
	static std::shared_ptr<TimeStepper> mpTimeStepper;

	/** Stores time at which simulation started */
	double m_start_time;


};

#endif /*SCENETIME_H_*/