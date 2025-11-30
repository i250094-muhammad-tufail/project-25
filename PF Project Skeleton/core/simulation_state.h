#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

// ============================================================================
// SIMULATION_STATE.H - Global constants and state
// ============================================================================
// Global constants and arrays used by the game.
// ============================================================================

// ----------------------------------------------------------------------------
// GRID CONSTANTS
// ----------------------------------------------------------------------------
const int maxWidth = 80;
const int maxHieght = 80;


// ----------------------------------------------------------------------------
// TRAIN CONSTANTS
// ----------------------------------------------------------------------------
const int maxTrain = 20;

const int inactiveTrain = 0;
const int activeTrain = 1;
const int crashTrian = 2;
const int waitingTrain = 3;
const int reachedTrian = 4;

const int rightDirection = 0;
const int leftDirection = 1;
const int upDirection = 2;
const int downDirection = 3;
const int directionless = -1;



// ----------------------------------------------------------------------------
// SWITCH CONSTANTS
// ----------------------------------------------------------------------------
const int maxSwitches = 26;
const int switchGlobal = 0;
const int switchLocal = 1;


// ----------------------------------------------------------------------------
// WEATHER CONSTANTS
// ----------------------------------------------------------------------------
const int normWeather = 0;
const int rainyWeather = 1;
const int foggyWeather = 2;

extern int weatherCondition;
extern int weatherTimer;


// ----------------------------------------------------------------------------
// SIGNAL CONSTANTS
// ----------------------------------------------------------------------------

const int greenSig = 0;
const int yellowSig = 1;
const int redSig = 2;

extern int signalColor [maxSwitches];

// ----------------------------------------------------------------------------
// GLOBAL STATE: GRID
// ----------------------------------------------------------------------------
extern int gridWidth;
extern int gridHeight;
extern char Grid[maxHieght][maxWidth];

// ----------------------------------------------------------------------------
// GLOBAL STATE: TRAINS
// ----------------------------------------------------------------------------
extern int trainSpawned;
extern int trainPosition_x [maxTrain];
extern int trainPosition_y [maxTrain];
extern int trainDirection [maxTrain];
extern int trainState [maxTrain];
extern int trainSpawned_x [maxTrain];
extern int trainSpawned_y [maxTrain];
extern int trainDestination_x [maxTrain];
extern int trainDestination_y [maxTrain];
extern int train_numberplate [maxTrain];
extern int trainSpawnMoment [maxTrain];
extern int traind_next_x [maxTrain];
extern int traind_next_y [maxTrain];
extern int traind_next_direction [maxTrain];

// ----------------------------------------------------------------------------
// GLOBAL STATE: SWITCHES (A-Z mapped to 0-25)
// ----------------------------------------------------------------------------
extern int switchCount;
extern int switchPosition_x [maxSwitches];
extern int switchPosition_y [maxSwitches];
extern int switchThreshold [maxSwitches];
extern int switchState [maxSwitches];
extern int switch_tranCounter [maxSwitches];
extern int switchType [maxSwitches];
extern bool switchFlip [maxSwitches];


// ----------------------------------------------------------------------------
// GLOBAL STATE: SPAWN POINTS
// ----------------------------------------------------------------------------
const int maxSpawnPoints = 20;
extern int spawnPosition_x [maxSpawnPoints];
extern int spawnPosition_y [maxSpawnPoints];
extern int spawnID [maxSpawnPoints];

// ----------------------------------------------------------------------------
// GLOBAL STATE: DESTINATION POINTS
// ----------------------------------------------------------------------------
const int maxDestinationPoints = 20;
extern int destinationPosition_x [maxDestinationPoints];
extern int destinatoinPosition_y [maxDestinationPoints];
extern int destinationID [maxDestinationPoints];


// ----------------------------------------------------------------------------
// GLOBAL STATE: SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
extern int tickNumber;
//extern int seed;

// ----------------------------------------------------------------------------
// GLOBAL STATE: METRICS
// ----------------------------------------------------------------------------
extern int arrivedTrain;
extern int crashedTrain;

// ----------------------------------------------------------------------------
// GLOBAL STATE: EMERGENCY HALT
// ----------------------------------------------------------------------------
extern bool Halt;
extern int haltTotalTime;
extern int haltPositionX;
extern int haltPositionY;
extern int haltRadius;

// ----------------------------------------------------------------------------
// INITIALIZATION FUNCTION
// ----------------------------------------------------------------------------
// Resets all state before loading a new level.
void initializeSimulationState();

#endif
