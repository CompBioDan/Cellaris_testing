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
	static SceneTime* Instance();

	/** Sets the end time and number of timesteps - called after SetStartTime() but before other methods */
	void SetEndTimeAndNumberOfTimeSteps(double endTime, unsigned totalSimulationTimeSteps);

	void ResetEndTimeAndNumberOfTimeSteps(const double& rEndTime, const unsigned& rNumberOfTimeStepsInThisRun);

	/** @return the timestep for the simulation, set in earlier calls */
	double GetTimeStep() const;

	/** Increments the simulation time by a single time step */
	void IncrementTimeOneStep();

	/** @returns the number of timesteps that have elapsed in simulation thus far */
	unsigned GetTimeStepsElapsed() const;

	/** @returns the simulation time */
	double GetTime() const;

	/** Destroys the current SceneTime instance */
	static void Destroy();

	/** Set the simulation start time */
	void SetStartTime(double startTime);

	/** @return the simulation start time */
	double GetStartTime() const;

	/** @return the simulation end time */
	double GetEndTime() const;

	/** Has the simulation finished running */
	bool HasFinished() const;

	double CheckTimestepperTimes() const;

protected:

	/** default scene time constructor -> must set start time, end time and number of timesteps before using object */
	SceneTime();

private:

	/** Pointer to the singleton instance of this class */
	static SceneTime* mpInstance;

	/** Delegate all time stepping to a TimeStepper class */
	static std::shared_ptr<TimeStepper> mpTimeStepper;

	/** Stores time at which simulation started */
	double mStartTime;


};

#endif /*SCENETIME_H_*/