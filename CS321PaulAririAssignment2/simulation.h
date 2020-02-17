#pragma once
#include <iostream>
#include <queue>
#ifndef SIMULATION_H
#define SIMULATION_H

namespace simulationspace {

	class boolSource {
	private:
		double probability;
	public:
		boolSource(double prob = 0.5);
		bool queryCall();
	};

	enum planeStatus { null, arriving, departing };
	class Plane {
	public:
		Plane();
		Plane(int num, int time, planeStatus status);
		void deny() const;
		void land(int time) const;
		void takeOff(int time) const;
		int started();

	private:
		int flightNum;
		int startClock;
		planeStatus status;
	};

	enum RunwayActive { idle, land, takeOff };
	class Runway {
	public:
		Runway(int limit);
		bool canLand(Plane &curr);
		bool canDepart(Plane &curr);
		bool hasCrashed(int setTime);
		RunwayActive activity(int time, int setTime, Plane &move);
		void finishSim(int time) const;

	private:
		std::queue<Plane> landing;
		std::queue<Plane> takeoff;
		int queueLimit;
		int landRequests, takeoffRequests;
		int landings, takeoffs;
		int queuedLandings, queuedTakeoffs;
		int deniedLandings, deniedTakeoffs;
		int landWaitTime, takeOffWaitTime, idleTime;
		int crashed;
	};
}

#endif
