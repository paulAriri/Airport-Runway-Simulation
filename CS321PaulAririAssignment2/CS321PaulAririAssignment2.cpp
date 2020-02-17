// CS321PaulAririAssignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "simulation.h"
using namespace simulationspace;
using namespace std;


int main()
{
    int runTime;
    int limit;
    int landTime, takeoffTime;
    int crashed = 0;
    int setTime, LloopTime, TloopTime, loopTime;
    int flightNum = 0;
    double landingRate, takeoffRate, avgLand, avgTakeoff;

    cout << "***********************************************************************" << endl;
    cout << "***  This program simulates a small airport with only one runway.   ***" << endl;
    cout << "***  At each unit of time, one plane can land or depart!            ***" << endl;
    cout << "***  Planes that crash will only show --Plane # ready to land--     ***" << endl;
    cout << "***********************************************************************" << endl << endl;

    cout << "The amount of time needed for one plane to land? ";
    cin >> landTime;
    cout << endl;

    cout << "The amount of time needed for one plane to takeoff? ";
    cin >> takeoffTime;
    cout << endl;

    cout << "How many planes can be waiting in the queue to land or take off at any time? ";
    cin >> limit;
    cout << endl;

    cout << "What is the total length of time to be simulated? ";
    cin >> runTime;
    cout << endl;

    cout << "The maximum amount of time that a plane can stay in the landing queue without running out of fuel and crashing? ";
    cin >> setTime;
    cout << endl;

    cout << "The average amount of time between arrival of planes to the landing queue? ";
    cin >> avgLand;
    cout << endl;
 
    cout << "The average amount of time between arrival of planes to the takeoff queue? ";
    cin >> avgTakeoff;
    cout << endl;

    bool acceptable = false;
    LloopTime = TloopTime = loopTime = 0;

    do {
        landingRate = 1 / avgLand;
        cout << "Expected number of arrivals per unit time: " << landingRate << endl;
        cout << endl;

        takeoffRate = 1 / avgTakeoff;
        cout << "Expected number of departures per unit time: " << takeoffRate << endl;
        cout << endl;

        if (landingRate < 0.0 || takeoffRate < 0.0)
            cerr << "These rates must be positive." << endl;
        else acceptable = true;

    } while (!acceptable);

    boolSource lRates(landingRate);
    boolSource tRates(takeoffRate);

    Runway airport(limit);

    for (int time = 0; time < runTime; time++) {
        if(lRates.queryCall()){
            LloopTime += landTime + avgLand;
            Plane currPlane(flightNum++, time, arriving);
            
            if (airport.canLand(currPlane) != true)
                currPlane.deny();
        }
        else if(tRates.queryCall()){
            TloopTime += takeoffTime + avgTakeoff;
            Plane currPlane(flightNum++, time, departing);

            if (airport.canDepart(currPlane) != true)
                currPlane.deny();
        }

        Plane flyingPlane;
        loopTime = LloopTime + TloopTime;

        switch (airport.activity(loopTime, setTime, flyingPlane)) {
        case land: flyingPlane.land(LloopTime); break;
        case takeOff: flyingPlane.takeOff(TloopTime); break;
        }

        if (loopTime > runTime) {
            break;
        }
    }
    
    airport.finishSim(runTime);
}