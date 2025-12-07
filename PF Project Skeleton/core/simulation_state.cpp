#include "simulation_state.h"
#include <cstring>

// ============================================================================
// SIMULATION_STATE.CPP - Global state definitions
// ============================================================================

// ----------------------------------------------------------------------------
// GRID
// ----------------------------------------------------------------------------
int gridWidth = 0;
int gridHeight = 0;
char Grid[maxHieght][maxWidth];

// ----------------------------------------------------------------------------
// TRAINS
// ----------------------------------------------------------------------------
int trainSpawned = 0;
int trainPosition_x [maxTrain];
int trainPosition_y [maxTrain];
int trainDirection [maxTrain];
int trainState [maxTrain];
int trainSpawned_x [maxTrain];
int trainSpawned_y [maxTrain];
int trainDestination_x [maxTrain];
int trainDestination_y [maxTrain];
int train_numberplate [maxTrain];
int trainSpawnMoment [maxTrain];
int traind_next_x [maxTrain];
int traind_next_y [maxTrain];
int traind_next_direction [maxTrain];

// ----------------------------------------------------------------------------
// SWITCHES
// ----------------------------------------------------------------------------
int switchCount = 0;
int switchPosition_x [maxSwitches];
int switchPosition_y [maxSwitches];
int switchThreshold [maxSwitches];
int switchState [maxSwitches];
int switch_tranCounter [maxSwitches];
int switchType [maxSwitches];
bool switchFlip [maxSwitches];


// ----------------------------------------------------------------------------
// SPAWN AND DESTINATION POINTS
// ----------------------------------------------------------------------------

int spawnPosition_x [maxSpawnPoints];
int spawnPosition_y [maxSpawnPoints];
int spawnID [maxSpawnPoints];

int destinationPosition_x [maxDestinationPoints];
int destinatoinPosition_y [maxDestinationPoints];
int destinationID [maxDestinationPoints];


// ----------------------------------------------------------------------------
// SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
int weatherCondition = normWeather;
int signalColor [maxSwitches];
int weatherTimer = 0;
int tickNumber = 0;
int spawnInterval = 5;
// ----------------------------------------------------------------------------
// METRICS
// ----------------------------------------------------------------------------
int arrivedTrain = 0;
int crashedTrain = 0;
// ----------------------------------------------------------------------------
// EMERGENCY HALT
// ----------------------------------------------------------------------------
bool Halt = false;
int haltTotalTime = 0;
int haltPositionX = 0;
int haltPositionY = 0;
int haltRadius = 0;

// ============================================================================
// INITIALIZE SIMULATION STATE
// ============================================================================
// ----------------------------------------------------------------------------
// Resets all global simulation state.
// ----------------------------------------------------------------------------
// Called before loading a new level.
// ----------------------------------------------------------------------------
void initializeSimulationState() {
gridWidth = 0;
gridHeight = 0;

for(int i = 0; i < maxHieght; i++){
    for(int j = 0; j < maxWidth; j++){
        Grid[i][j] = ' ';       
}

}

trainSpawned = 0;
int train_Var = 0;
while (train_Var < maxTrain){
    trainPosition_x [train_Var] = 0;
    trainPosition_y [train_Var] = 0;
    trainState[train_Var] = inactiveTrain;
    trainDirection [train_Var] = directionless;
    traind_next_x[train_Var] = 0;
    traind_next_y[train_Var] = 0;
    traind_next_direction[train_Var] = directionless;
    train_Var++;
}


switchCount = 0;
int switch_Var = 0;
while (switch_Var < maxSwitches){
    switchFlip [switch_Var] = false;
    switchState [switch_Var] = 0;
    signalColor [switch_Var] = greenSig;
    switch_tranCounter [switch_Var] = 0;
    switch_Var++;
}


Halt = false;
weatherCondition = normWeather;
weatherTimer = 0;
 

arrivedTrain = 0;
crashedTrain = 0;

tickNumber = 0;
}
