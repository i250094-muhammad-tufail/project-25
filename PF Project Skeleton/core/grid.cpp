#include "grid.h"
#include "simulation_state.h"

// ============================================================================
// GRID.CPP - Grid utilities
// ============================================================================

// ----------------------------------------------------------------------------
// Check if a position is inside the grid.
// ----------------------------------------------------------------------------
// Returns true if x,y are within bounds.
// ----------------------------------------------------------------------------
bool isInBounds(int x, int y) {
    if (x >= 0 && x < gridWidth && y >= 0 && y < gridHeight){
        return true;
    }
    else
     return false;
}

// ----------------------------------------------------------------------------
// Check if a tile is a track tile.
// ----------------------------------------------------------------------------
// Returns true if the tile can be traversed by trains.
// ----------------------------------------------------------------------------
bool isTrackTile(int x, int y) {
    int check = isInBounds(x,y);
    if(check == 0){
        return false;
    }

    char tileType = Grid[y][x];
    if(tileType == ' ' || tileType == '#'){
        return false;

    }
    return true;

}



// ----------------------------------------------------------------------------
// Check if a tile is a switch.
// ----------------------------------------------------------------------------
// Returns true if the tile is 'A'..'Z'.
// ----------------------------------------------------------------------------
bool isSwitchTile(int x, int y) {
    int check = isInBounds(x,y);
    if(check == 0){
        return false;
    }

    char tileType = Grid[y][x];
    if(tileType >= 'A' && tileType <= 'Z'){
        return true;

}

    else
     return false;
}

// ----------------------------------------------------------------------------
// Get switch index from character.
// ----------------------------------------------------------------------------
// Maps 'A'..'Z' to 0..25, else -1.
// ----------------------------------------------------------------------------
int getSwitchIndex(char switchIndex) {
    if (switchIndex >= 'A' && switchIndex <= 'Z'){
        int value = switchIndex - 'A';
        return value;
    }
    return -1;

}

// ----------------------------------------------------------------------------
// Check if a position is a spawn point.
// ----------------------------------------------------------------------------
// Returns true if x,y is a spawn.
// ----------------------------------------------------------------------------
bool isSpawnPoint(int x, int y) {
    int check = isInBounds(x,y);
    if(check == 0){
        return false;
    }
    char tileCheck = Grid[y][x];
    if(tileCheck == 's' || tileCheck == 'S'){
        return true;
    }
    else
     return false;
}

// ----------------------------------------------------------------------------
// Check if a position is a destination.
// ----------------------------------------------------------------------------
// Returns true if x,y is a destination.
// ----------------------------------------------------------------------------
bool isDestinationPoint(int x, int y) {
    int check_a = isInBounds(x,y);
    if(check_a == 0){
        return false;
    }
    char check = Grid[y][x];
    if(check == 'd' || check == 'D'){
        return true;
    }
    else
        return false;
}

// ----------------------------------------------------------------------------
// Toggle a safety tile.
// ----------------------------------------------------------------------------
// Returns true if toggled successfully.
// ----------------------------------------------------------------------------
bool toggleSafetyTile(int x, int y) {
    int check = isInBounds(x,y);
    if(check == 0){
        return false;
    }
    char tileType = Grid[y][x];
    if (tileType == ' '){
        Grid[y][x] = '=';
        return true;

    }

    if (tileType == '='){
        Grid[y][x] = ' ';
        return true;
    }

return false;
}
