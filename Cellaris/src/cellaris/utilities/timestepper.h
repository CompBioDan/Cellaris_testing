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

#ifndef TIMESTEPPER_H_
#define TIMESTEPPER_H_

#include <vector>
#include <climits>
#include <cassert>
#include <cmath>
#include <iostream>

/**
*
* timestepper.h is a helper class to provide a robust way to handle time loops
*
* An incrementing integer counter is used to calculate the current simulation time and
* is then used to ensure that we have a correct end time
*
*
**/


class TimeStepper
{

public:

	/**
	* Create a new time stepper over the requried scene simulation interval.
	* Units for the time are not specified here, but values should have consistent units
	*
	* @param startTime: the start time of the interval
	* @param endTime: the finishing time of the interval
	* @param dt: the timestep we want to use in the simulation
	*
	**/
	TimeStepper(double startTime, double endTime, double dt);

	/**
	* Progress one time step in the simulation, updating the number of steps taken, and the simulation time
	**/
	void AdvanceOneTimeStep();

	/**
	* Retrieve the current scene time
	**/
	double GetTime();

	/**
	*  Retrieve the simulation time at the next step
	**/
	double GetNextTime();

	/**
	* Retrieve the end time set for the scene
	**/
	double GetEndTime();

	/**
	* Check whether we are at the end of the scene
	**/
	bool IsTimeAtEnd() const;

	/**
	* Retrieve the number of timesteps taken so far in the scene
	**/
	unsigned GetTotalTimeStepsTaken() const;

	/**
	* Retrieve the size of the next timestep (currently fixed but can be used in future to retrieve adaptive timesteps)
	**/
	double GetNextTimeStep();

	double GetStart();



private:

	/** Scene start time */
	double mStart;

	/** Scene end time */
	double mEnd;

	/** Scene timestep */
	double mDt;

	/** Number of time steps taken during scene */
	unsigned mTotalTimeStepsTaken;

	/** Current scene time */
	double mTime;

	/** Scene time at next timestep */
	double mNextTime;

	/** Calculate the scene time following timestep */
	double CalculateNextTime();

};

#endif /*TIMESTEPPER_H_*/