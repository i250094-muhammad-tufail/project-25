#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include<iostream>
#include<string>
#include <cstring>
#include <cstdio>
#include <cstdlib>

using namespace std;

// ============================================================================

// IO.CPP - Level I/O and logging

// ============================================================================

// ----------------------------------------------------------------------------

// LOAD LEVEL FILE

// ----------------------------------------------------------------------------

// Load a .lvl file into global state.

// ----------------------------------------------------------------------------

bool loadLevelFile(const char* loadlevel) {

ifstream file;

file.open(loadlevel);

if(!file.is_open())

{

printf("Error in opening the file %s\n",loadlevel);

return false;

}

initializeSimulationState();

string line;

int y=0;

bool gridread=false;

bool trainread=false;

bool switchread=false;

while(getline(file,line))

{

if(line.empty())

continue;

if(line.find("GRID:")!=string::npos)
{
gridread=true;
trainread=false;
switchread=false;
continue;
}
else if (line.find("TRAINS:")!=string::npos)
{
gridread=false;
trainread=true;
switchread=false;
continue;
}
    else if(line.find("SWITCHES:")!=string::npos)
    {
       gridread=false;
       trainread=false;
       switchread=true;
       continue; 
    }
    if(line.find("WEATHER:")!=string::npos)
    {
        if(line.find("RAIN")!=string::npos)
        {
            weatherCondition=rainyWeather;
        }
   else if(line.find("FOG")!=string::npos)
   {

    weatherCondition=foggyWeather;
   }
   else
   {
    weatherCondition=normWeather;
   }
   continue;
    }
if(line.find("SEED:")!=string::npos)
{
    if (line.length() > 6) {
        string seed=line.substr(6);
        int seeds=stoi(seed);
        srand(seeds);
    }
    continue;
}
if(gridread)
{
    if(y<maxHieght)
    {
        if(line.length()>gridWidth)
        {
            gridWidth=line.length();
        }
    for (int x=0; x<line.length() && x < maxWidth;x++)
    {
        Grid[y][x]=line[x];
    }
    y++;
    gridHeight=y;
}
}
else if(trainread){
    if(trainSpawned<maxTrain)
    {
        int id,s1,s2,d1,d2,time;
        if(sscanf(line.c_str(),"%d %d %d %d %d %d",&id,&s1,&s2,&d1,&d2,&time)==6){
           train_numberplate[trainSpawned]=id;
           trainSpawned_x[trainSpawned]=s1;
           trainSpawned_y[trainSpawned]=s2;
           trainDestination_x[trainSpawned]=d1;
           trainDestination_y[trainSpawned]=d2;
           trainSpawnMoment[trainSpawned]=time;
           trainState[trainSpawned]=inactiveTrain;
           trainSpawned++;
        }
    }
}
else if(switchread)
{
    if(switchCount<maxSwitches)
    {
        char c;
        int s1,s2,p;
        char typestring[20];
        if(sscanf(line.c_str(),"%c %d %d %s %d",&c,&s1,&s2,typestring,&p)==5)
        {
            int idx=getSwitchIndex(c);
            if(idx!=-1)
            {
                switchPosition_x[idx]=s1;
                switchPosition_y[idx]=s2;
                switchThreshold[idx]=p;
                if(strcmp(typestring,"GLOBAL")==0)
                {
                    switchType[idx]=switchGlobal;
                }
                else{
                    switchType[idx]=switchLocal;
                }
                switchCount++;
            }
        }
    }
}
}
file.close();
return true;
}
// ----------------------------------------------------------------------------
// INITIALIZE LOG FILES
// ----------------------------------------------------------------------------
// Create/clear CSV logs with headers.
// ----------------------------------------------------------------------------
void initializeLogFiles() {
ofstream trace("out/trace.csv");
trace<<"tick,train_id,x,y,direction,state"<<endl;
trace.close();
ofstream switches("out/switches.csv");
switches<<"tick,switch_id,state,counter"<<endl;
switches.close();
ofstream signals("out/signals.csv");
signals<<"tick,switch_id,color"<<endl;
signals.close();
}


// ----------------------------------------------------------------------------
// LOG TRAIN TRACE
// ----------------------------------------------------------------------------
// Append tick, train id, position, direction, state to trace.csv.
// ----------------------------------------------------------------------------
void logTrainTrace(int id)
 {
ofstream trace("out/trace.csv",ios ::app);
trace<<tickNumber<<","<<train_numberplate[id]<<","<<trainPosition_x[id]<<","<<trainPosition_y[id]<<","<<trainDirection[id]<<","<<trainState[id]<<endl;
trace.close();
}

// ----------------------------------------------------------------------------
// LOG SWITCH STATE
// ----------------------------------------------------------------------------
// Append tick, switch id/mode/state to switches.csv.
// ----------------------------------------------------------------------------
void logSwitchState(int switchindex)
 {
ofstream switches("out/switches.csv",ios::app);
char c='A'+switchindex;
switches<<tickNumber<<","<<c<<","<<switchState[switchindex]<<","<<switch_tranCounter[switchindex]<<endl;
switches.close();
}

// ----------------------------------------------------------------------------
// LOG SIGNAL STATE
// ----------------------------------------------------------------------------
// Append tick, switch id, signal color to signals.csv.
// ----------------------------------------------------------------------------
void logSignalState(int switchindex) {
    ofstream signals("out/signals.csv",ios:: app);
    char c='A'+switchindex;
    const char* color="GREEN";
    if(signalColor[switchindex]==yellowSig)
    color="YELLOW";
    else if(signalColor[switchindex]==redSig)
    color="RED";
    signals<<tickNumber<<","<<c<<","<<color<<endl;
    signals.close();
}

// ----------------------------------------------------------------------------
// WRITE FINAL METRICS
// ----------------------------------------------------------------------------
// Write summary metrics to metrics.txt.
// ----------------------------------------------------------------------------
void writeMetrics() {
ofstream metrics("out/metrics.txt");
  metrics<<"Total Arrived: "<<arrivedTrain<<endl;
    metrics<<"Total Crashed: "<<crashedTrain<<endl;
    metrics<<"Simulation ticks: "<<tickNumber<<endl;
    metrics.close();
}