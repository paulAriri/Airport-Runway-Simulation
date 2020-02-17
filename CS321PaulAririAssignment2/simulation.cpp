#include <iostream>
#include "simulation.h"
using namespace std;
using namespace simulationspace;


boolSource::boolSource(double prob)
{
	probability = prob;
}

bool boolSource::queryCall()
{
	return (rand() < (probability * 32767));
}

Runway::Runway(int limit)
{
	queueLimit = limit;
	landRequests = takeoffRequests = 0;
	landings = takeoffs = 0;
	deniedLandings = deniedTakeoffs = 0;
	queuedLandings = queuedTakeoffs = 0;
	landWaitTime = takeOffWaitTime = idleTime = 0;
	crashed = 0;
}

bool Runway::canLand(Plane& curr)
{
	bool result;
	if (landing.size() < queueLimit) {
		landing.push(curr);
		result = true;
	}
	else result = false;

	landRequests++;

	if (result != true)
		deniedLandings++;
	else queuedLandings++;

	return result;
}

bool Runway::canDepart(Plane& curr)
{
	bool result;
	if (takeoff.size() < queueLimit) {
		takeoff.push(curr);
		result = true;
	}
	else result = false;

	takeoffRequests++;

	if (result != true)
		deniedTakeoffs++;
	else queuedTakeoffs++;

	return result;
}

bool Runway::hasCrashed(int setTime) {
	if (landWaitTime > setTime) {
		return true;
	}
	else {
		return false;
	}
}

RunwayActive Runway::activity(int time, int setTime, Plane& move)
{
	RunwayActive inProgress = idle;

	if (!landing.empty() && !hasCrashed(setTime)) {
			move = landing.front();
			landWaitTime += time - move.started();
			landings++;
			inProgress = land;
			landing.pop();
	}
	else if (!takeoff.empty()) {
		move = takeoff.front();
		takeOffWaitTime += time - move.started();
		takeoffs++;
		inProgress = takeOff;
		takeoff.pop();
	}
	else {
		idleTime++;
		inProgress = idle;
	}

	if(hasCrashed(setTime)) crashed++;

	return inProgress;
}

void Runway::finishSim(int time) const
{
	cout << "Airport simulation has concluded after " << time << " seconds" << endl << endl;
	cout << "The number of planes that took off in the simulated time: " << takeoffs << endl;
	cout << "The number of planes that landed in the simulated time: " << landings << endl;
	cout << "The average time that a plane spent in the takeoff queue: " << ((float)takeOffWaitTime) / ((float)takeoffs) << " seconds" << endl;
	cout << "The average time that a plane spent in the landing queue: " << ((float)landWaitTime) / ((float)landings) << " seconds" << endl;
	cout << "The number of planes that crashed because they ran out of fuel before they could land: " << crashed;
}

Plane::Plane()
{
	flightNum = -1;
	startClock = -1;
	status = null;
}

Plane::Plane(int num, int time, planeStatus state)
{
	flightNum = num;
	startClock = time;
	status = state;

	cout << "***********************************************" << endl;
	cout << "Plane " << num << " is ready to ";

	if (status == arriving)
		cout << "land." << endl;
	else cout << "take off." << endl;
}

void Plane::deny() const
{
	cout << "Plane number " << flightNum;

	if (status == arriving)
		cout << " can not land in the runway at the moment because landing queue is full" << endl;
	else
		cout << " can not takeoff in the runway at the moment because takeoff queue is full" << endl;
}

void Plane::land(int time) const
{
	int waitTime = (time - startClock);

	cout << waitTime << " time units after the takeoff simulation started, Plane number " << flightNum << " landed" << endl << endl;
}

void Plane::takeOff(int time) const
{
	int waitTime = time - startClock;

	cout << waitTime << " time units after the takeoff simulation started, Plane number " << flightNum << " tookoff" << endl << endl;
}

int Plane::started()
{
	startClock = 0;
	return startClock;
}
