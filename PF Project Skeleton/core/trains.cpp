#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <cstdlib>

// ============================================================================
// TRAINS.CPP - Train logic
// ============================================================================


// Storage for planned moves (for collisions).

// Previous positions (to detect switch entry).

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
// ----------------------------------------------------------------------------
void spawnTrainsForTick() {
    for(int i = 0; i < trainSpawned; i++){
        if(trainState[i] == inactiveTrain && trainSpawnMoment[i] <= tickNumber){
            bool onspot = false;
            int j = 0;
            while( j < trainSpawned){
                if (trainState[j] == activeTrain && trainPosition_x[j] == trainSpawned_x[i] && trainPosition_y[j] == trainSpawned_y[i]){

                    onspot = true;
                    break;                    
                }

                j++;
            }


            if(!onspot){
                trainState[i] = activeTrain;
                trainPosition_x[i] = trainSpawned_x[i];
                trainPosition_y[i] = trainSpawned_y[i];
                trainDirection[i] = directionless;  
            }

        }


    }



}

// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------
bool determineNextPosition(int i) {
    if(trainState[i] != activeTrain){
        return false;
    }

int currentX = trainPosition_x[i];
int currentY = trainPosition_y[i];

if (!isInBounds(currentX, currentY)) return false;

int currentDirection = trainDirection[i];

    if(currentDirection == directionless){
        int dx [4] {0,1,0,-1};
        int dy [4] {-1,0,1,0};
        for(int s = 0; s < 4; s++){
            if(isTrackTile(currentX + dx[s], currentY + dy[s])){
                currentDirection = s;
                break;

            }

        }
        if (currentDirection == directionless){
            traind_next_x[i] = currentX;
            traind_next_y[i] = currentY;
            traind_next_direction[i] = directionless;
            return false;
        }

    }


    char c = Grid[currentY][currentX];
    int nextDirection = currentDirection;


    if(c == '+'){
        nextDirection = getSmartDirectionAtCrossing(i, currentX, currentY); 


    }

    else{
        nextDirection = getNextDirection(currentX, currentY, currentDirection);
    }


    if(c >= 'A' && c <= 'Z'){
        int index = getSwitchIndex(c);
        if(signalColor[index] == redSig){
            traind_next_x[i] = currentX;
            traind_next_y[i] = currentY;
            traind_next_direction[i] = currentDirection;
            return true;

        }


    }

    int nextX = currentX;
    int nextY = currentY;

    if(nextDirection == upDirection)
        nextY--;
    else if (nextDirection == rightDirection)
        nextX++;

    else if (nextDirection == downDirection)
        nextY++;

    else if (nextDirection == leftDirection) 
        nextX--;


    if (isTrackTile(nextX, nextY)){
        
    
    traind_next_x[i] = nextX;
    traind_next_y[i] = nextY;
    traind_next_direction[i] = nextDirection;
    return true;
    }

    else{

        traind_next_x[i] = currentX;
        traind_next_y[i] = currentY;
        traind_next_direction[i] = currentDirection;
        return false;



    }





}

// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection(int x, int y, int inDir ) {

    if(!isInBounds(x,y)){
        return directionless;
    }

    char s = Grid[y][x];
    if (s == '-' || s == '='){
        if (inDir == rightDirection)
            return rightDirection;
        if (inDir == leftDirection)
            return leftDirection;
    }

    else if (s == '|'){
        if (inDir == upDirection)
            return upDirection;
        if (inDir == downDirection)
            return downDirection;
    }



    else if (s == '/'){
        if (inDir == rightDirection)
            return upDirection;
        if (inDir == downDirection)
            return leftDirection;
        if (inDir == leftDirection)
            return downDirection;
        if (inDir == upDirection)
            return rightDirection;
    }

     else if (s == '\\'){
        if (inDir == rightDirection)
            return downDirection;
        if (inDir == upDirection)
            return leftDirection;
        if (inDir == leftDirection)
            return upDirection;
        if (inDir == downDirection)
            return rightDirection;
    }

    else if (s == '+'){
        return inDir;        
    
    }


    else if (s >= 'A' && s <= 'Z'){
        
        int index = getSwitchIndex(s);
        int state = getSwitchStateForDirection(index, inDir);


    if(state == 0)
        return inDir;
    
    }

    else if (s == 's' || s == 'd'){
        return inDir;
    }

    return inDir;    

}


//Siddeeq(me) defined function for the distance between two points;
    int getDistance (int x1,int y1,int x2,int y2){
        return abs(x1 - x2) + abs (y1- y2);
    }




// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing(int trainIndex, int x, int y) {

    int bestDirection = trainDirection[trainIndex];
    int bestDistance = 9998889;

    int dx [4] {0,1,0,-1};
    int dy [4] {-1,0,1,0};

    for (int z = 0; z <4; z++){
        if (z == (trainDirection[trainIndex] + 2) % 4)
            continue;

    int nextX = x + dx[z];
    int nextY = y + dy[z];

        if(isTrackTile(nextX, nextY)){
            int distance = getDistance(nextX, nextY, trainDestination_x[trainIndex], trainDestination_y[trainIndex]);
            if(distance < bestDistance){
                    bestDistance = distance;
                    bestDirection = z;


            }

        }



    }
    return bestDirection;

}

// ----------------------------------------------------------------------------
// DETERMINE ALL ROUTES (PHASE 2)
// ----------------------------------------------------------------------------
// Fill next positions/directions for all trains.
// ----------------------------------------------------------------------------
void determineAllRoutes() {

    for (int i = 0; i < trainSpawned; i++){
        if(trainState[i] == activeTrain){
            determineNextPosition(i);
        }

    }



}

// ----------------------------------------------------------------------------
// MOVE ALL TRAINS (PHASE 5)
// ----------------------------------------------------------------------------
// Move trains; resolve collisions and apply effects.
// ----------------------------------------------------------------------------
void moveAllTrains() {
    for (int i = 0; i < trainSpawned; i++){
        if(trainState[i] == activeTrain){
            if(traind_next_x[i] != trainPosition_x[i] || traind_next_y[i] != trainPosition_y[i]){


                int nextX = traind_next_x[i];
                int nextY = traind_next_y[i];
                if(isSwitchTile(nextX,nextY)){
                    int index = getSwitchIndex(Grid[nextY][nextX]);
                    if (index != -1){
                        switch_tranCounter[index]++;
                    }
                }

            }

            trainPosition_x[i] = traind_next_x[i];
            trainPosition_y[i] = traind_next_y[i];
            trainDirection[i] = traind_next_direction[i];

            
        }

    }


}

// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions() {

for (int i = 0; i < trainSpawned; i++){
    if(trainState[i] != activeTrain)
    continue;

    for(int j = i+1; j < trainSpawned; j++){
        if(trainState[i] != activeTrain)
            continue;



        bool colision = false;
        if(traind_next_x[i] == traind_next_x[j] && traind_next_y[i] == traind_next_y[j]){

            colision = true;

        }


        else if (traind_next_x[i] == trainPosition_x[j] && traind_next_y[i] == trainPosition_y[j] && 
    traind_next_x[j] == trainPosition_x[i] && traind_next_y[j] == trainPosition_y[i] ){
            colision = true;


        }


        if (colision){
            int destinationK = getDistance(trainPosition_x[i], trainPosition_y[i], trainDestination_x[i], trainDestination_y[i]);

            int destinationL = getDistance(trainPosition_x[j], trainPosition_y[j], trainDestination_x[j], trainDestination_y[j]);
        
        
        if(destinationK > destinationL){
            traind_next_x[j] = trainPosition_x[j];
            traind_next_y[j] = trainPosition_y[j];
            traind_next_direction[j] = trainDirection[j];


        }
        
        
        else if(destinationL > destinationK){
            traind_next_x[i] = trainPosition_x[i];
            traind_next_y[i] = trainPosition_y[i];
            traind_next_direction[i] = trainDirection[i];


        }
        
        else{

            trainState[i] = crashTrian;
            trainState[j] = crashTrian;
            crashedTrain += 2;


        }
        
        
        
        
        }

}


}


}

// ----------------------------------------------------------------------------
// CHECK ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
// ----------------------------------------------------------------------------
void checkArrivals() {

    for (int i = 0; i < trainSpawned; i++){

        if(trainState[i] == activeTrain){
            if(trainPosition_x[i] == trainDestination_x[i] && trainPosition_y[i] == trainDestination_y[i]){

                trainState[i] = reachedTrian;
                arrivedTrain++;
                

            }


        }



    }






}

// ----------------------------------------------------------------------------
// APPLY EMERGENCY HALT
// ----------------------------------------------------------------------------
// Apply halt to trains in the active zone.
// ----------------------------------------------------------------------------
void applyEmergencyHalt() {
    if(!Halt)
        return;

    for(int i = 0; i < trainSpawned; i++){
        if(trainState[i] == activeTrain){

            if(abs(trainPosition_x[i] - haltPositionX) <= haltRadius && abs(trainPosition_y [i]- haltPositionY) <= haltRadius ){
                traind_next_x[i] = trainPosition_x[i];
                traind_next_y[i] = trainPosition_y[i];
                traind_next_direction[i] = trainDirection[i];


            }


        }


    }



}

// ----------------------------------------------------------------------------
// UPDATE EMERGENCY HALT
// ----------------------------------------------------------------------------
// Decrement timer and disable when done.
// ----------------------------------------------------------------------------
void updateEmergencyHalt() {
    if(Halt){
        haltTotalTime--;
        if(haltTotalTime <= 0){
            Halt = false;
        }
    
    }


}
