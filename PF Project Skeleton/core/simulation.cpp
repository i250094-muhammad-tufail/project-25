#include "simulation.h"
#include "simulation_state.h"
#include "trains.h"
#include "switches.h"
#include "io.h"
#include <cstdlib>
#include <ctime>

// ============================================================================
// SIMULATION.CPP - Implementation of main simulation logic
// ============================================================================

// ----------------------------------------------------------------------------
// INITIALIZE SIMULATION
// ----------------------------------------------------------------------------

void initializeSimulation() {
}

// ----------------------------------------------------------------------------
// SIMULATE ONE TICK
// ----------------------------------------------------------------------------

void simulateOneTick() {
    tickNumber++;

    bool rainSkip = weatherCondition == rainyWeather && tickNumber % 5 == 0;

    spawnTrainsForTick();
    determineAllRoutes();
    queueSwitchFlips();

    if (!rainSkip){

        detectCollisions();
        moveAllTrains();
    }

applyDeferredFlips();
checkArrivals();
updateSignalLights();
updateEmergencyHalt();




}

// ----------------------------------------------------------------------------
// CHECK IF SIMULATION IS COMPLETE
// ----------------------------------------------------------------------------

bool isSimulationComplete() {


    for (int i = 0; i < trainSpawned; i++){
        if(trainState[i] == activeTrain || trainState[i] == waitingTrain || (trainState[i] == inactiveTrain && trainSpawnMoment[i] > tickNumber)){

            return false;
        }


    
    }


    writeMetrics();
    return true;



}
