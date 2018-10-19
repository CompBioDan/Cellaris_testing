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

#include "scenetime.h"


// pointer to single instance
SceneTime* SceneTime::mpInstance = nullptr;

// shared pointer to the delegated class
std::shared_ptr<TimeStepper> SceneTime::mpTimeStepper;

// @return a pointer to the scene time object
SceneTime* SceneTime::instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new SceneTime;
		mpTimeStepper.reset();
		std::atexit(destroy);
	}
	return mpInstance;
}

// default scene time constructor
SceneTime::SceneTime()
	:
	m_start_time(0.0)
{
	// make sure there is only a single instance 
	assert(mpInstance == nullptr);
}

// destroys the current SceneTime instance - called before program exit to avoid a memory leak
void SceneTime::destroy()
{
	if (mpInstance)
	{
		delete mpInstance;
		mpInstance = nullptr;
	}
}

// get the simulation time step
double SceneTime::get_time_step() const
{
	return mpTimeStepper->GetNextTimeStep();
}

// increment simulation by a single time step
void SceneTime::increment_time_one_step()
{
	mpTimeStepper->AdvanceOneTimeStep();
}

// get the number of time steps that have elapsed since start of the simulation
unsigned SceneTime::get_time_steps_elapsed() const
{
	return mpTimeStepper->GetTotalTimeStepsTaken();
}

// get the simulation time
double SceneTime::get_time() const
{
	if (mpTimeStepper)
	{
		return mpTimeStepper->GetTime();
	}
	return m_start_time;
}

// set the start time of the simulation
void SceneTime::set_start_time(double startTime)
{
	m_start_time = startTime;
}

double SceneTime::get_start_time() const
{
	return m_start_time;
}

double SceneTime::get_end_time() const
{
	return mpTimeStepper->GetEndTime();
}

// set the end time of the simulation and number of timesteps
void SceneTime::set_end_time_and_number_of_time_steps(double endTime, unsigned totalTimeStepsInSimulation)
{
	mpTimeStepper.reset(new TimeStepper(m_start_time, endTime, (endTime - m_start_time) / totalTimeStepsInSimulation));
}

void SceneTime::reset_end_time_and_number_of_time_steps(const double& rEndTime, const unsigned& rNumberOfTimeStepsInThisRun)
{
	/** start time is the timestepper current time*/
	m_start_time = mpTimeStepper->GetTime();

	assert(rEndTime > m_start_time);

	/** reset the machinery that works out the time */
	mpTimeStepper.reset(new TimeStepper(m_start_time, rEndTime, (rEndTime - m_start_time) / rNumberOfTimeStepsInThisRun));
}

bool SceneTime::has_finished() const
{
	return(mpTimeStepper->IsTimeAtEnd());
}

double SceneTime::check_timestepper() const
{
	std::cout << "Timestepper start: " << mpTimeStepper->GetStart() << '\n';
	std::cout << "Timestepper end: " << mpTimeStepper->GetEndTime() << '\n';
	std::cout << "Timestepper dt: " << mpTimeStepper->GetNextTimeStep() << '\n';

	return 0;

}


