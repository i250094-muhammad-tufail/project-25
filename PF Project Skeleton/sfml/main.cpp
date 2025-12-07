#include "app.h"
#include "../core/simulation_state.h"
#include "../core/simulation.h"
#include "../core/io.h"
#include <iostream>
using namespace std;

// ============================================================================
// MAIN.CPP - Entry point of the application (NO CLASSES)
// ============================================================================

// ----------------------------------------------------------------------------
// MAIN ENTRY POINT
// ----------------------------------------------------------------------------
// This function is the main entry point of the application. It handles command
// line arguments to specify the level file to load, loads the level file using
// loadLevelFile, initializes the simulation system, initializes the SFML
// application window, prints control instructions to the console, runs the
// main application loop, cleans up resources, and prints final simulation
// statistics. Returns 0 on success, 1 on error (e.g., failed to load level
// file or initialize application).
// ----------------------------------------------------------------------------
int main(int argc, char* argv[]) {

    const char* level = "data/levels/complex_network.lvl";
    if(argc > 1){
        level = argv[1];
    }

    cout<<"Loading Level.........."<<level<<endl;

    if (!loadLevelFile(level)) {
        cout << "Failed to load level: " << level << endl;
        return 1;
    }

    int app = initializeApp();
    if (app == 0){
        cout<<"Failed to initialize the app:(";
        return 1;
    }


    cout << "Simulation successfully initialized." <<endl;
    cout << "Grid Size: " << gridWidth << "x" << gridHeight << endl;
    cout << "Trains Loaded: " << trainSpawned << endl;
    cout << "Switches Loaded: " << switchCount << endl;
    cout << "Controls are:" << endl;
    cout << " Use  SPACE: Pause/Resume" << endl;
    cout << "  .: Shows Step one tick" << endl;
    cout << "  Use  Left Click: Toggle Safety Tile" <<endl;
    cout << "Press Right Click: Toggle Switch" <<endl;
    cout << " Use the Middle Drag: Pan Camera" << endl;
    cout << " Use the mouse Scroll: Zoom" <<endl;
    cout << " Press ESC: Exit" <<endl;



runApp();
cleanupApp();
writeMetrics();


cout<<"Simulation complete :).....wrote the results to matrics.txt......thanks for indulging BTW";





}