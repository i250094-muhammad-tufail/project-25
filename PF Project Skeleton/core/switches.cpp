#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"

// ============================================================================
// SWITCHES.CPP - Switch management
// ============================================================================

// ----------------------------------------------------------------------------
// UPDATE SWITCH COUNTERS
// ----------------------------------------------------------------------------
// Increment counters for trains entering switches.
// ----------------------------------------------------------------------------
void updateSwitchCounters() {
    for (int i = 0 ; i < trainSpawned; i ++){
        if(trainState[i] = activeTrain){

            int x = trainPosition_x[i];
            int y = trainPosition_y[i];
            if (isSwitchTile(x, y)){
                char switchchar = Grid[y][x];
                int switchIdnx = getSwitchIndex(switchchar);
                if (switchIdnx != -1){

                    switch_tranCounter[switchIdnx]++;
                }

            }

        }


    }


}

// ----------------------------------------------------------------------------
// QUEUE SWITCH FLIPS
// ----------------------------------------------------------------------------
// Queue flips when counters hit K.
// ----------------------------------------------------------------------------
void queueSwitchFlips() {
    for (int i = 0; i < switchCount; i++){
        if(switch_tranCounter[i] >= switchThreshold[i]);{
            switchFlip[i] = true;
            switch_tranCounter[i] = 0;

        }
    }



}

// ----------------------------------------------------------------------------
// APPLY DEFERRED FLIPS
// ----------------------------------------------------------------------------
// Apply queued flips after movement.
// ----------------------------------------------------------------------------
void applyDeferredFlips() {
    for (int i = 0; i < switchCount; i++){
    if (switchFlip[i]){
        switchState[i] = !switchState[i];
        switchFlip[i] = false;
        logSwitchState(i);

    }
}

}

// ----------------------------------------------------------------------------
// UPDATE SIGNAL LIGHTS
// ----------------------------------------------------------------------------
// Update signal colors for switches.
// ----------------------------------------------------------------------------
void updateSignalLights() {

for (int i = 0 ; i < switchCount; i++){
    bool onspot = false;
    for (int k = 0; k < trainSpawned; k++){
        if(trainState[k] != inactiveTrain && trainPosition_x[k] == switchPosition_x[i] && trainPosition_y[k] == switchPosition_y[i]){
            onspot = true;
            break;
        }

    }


    if(onspot){
        signalColor[i] = redSig;


    }

    else{
        signalColor[i] = greenSig;

    }

    logSignalState(i);


}




}

// ----------------------------------------------------------------------------
// TOGGLE SWITCH STATE (Manual)
// ----------------------------------------------------------------------------
// Manually toggle a switch state.
// ----------------------------------------------------------------------------
void toggleSwitchState(int switchIndex) {

    if(switchIndex >= 0 && switchIndex < switchCount){
        switchState[switchIndex] = !switchState[switchIndex];
        logSwitchState(switchIndex);
    }



}

// ----------------------------------------------------------------------------
// GET SWITCH STATE FOR DIRECTION
// ----------------------------------------------------------------------------
// Return the state for a given direction.
// ----------------------------------------------------------------------------
int getSwitchStateForDirection(int switchIndex, int direction) {

    if(switchIndex < 0 || switchIndex >= switchCount){

        return 0;
    }

    if(switchType[switchIndex] == switchGlobal){
        return switchState[switchIndex];    
    }

    else
        return switchState[switchIndex];

}