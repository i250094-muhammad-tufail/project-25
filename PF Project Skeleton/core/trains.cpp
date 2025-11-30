#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include "io.h"
#include <cstdlib>
#include <cmath>

// ============================================================================
// TRAINS.CPP - Train logic
// ============================================================================

// Helper to get Manhattan distance
int getDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
// ----------------------------------------------------------------------------
void spawnTrainsForTick() {
    for (int i = 0; i < train_count; i++) {
        if (train_state[i] == TRAIN_STATE_INACTIVE && train_spawn_time[i] <= current_tick) {
            // Check if spawn tile is occupied
            bool occupied = false;
            for (int j = 0; j < train_count; j++) {
                if (train_state[j] == TRAIN_STATE_ACTIVE && 
                    train_x[j] == train_spawn_x[i] && train_y[j] == train_spawn_y[i]) {
                    occupied = true;
                    break;
                }
            }

            if (!occupied) {
                train_state[i] = TRAIN_STATE_ACTIVE;
                train_x[i] = train_spawn_x[i];
                train_y[i] = train_spawn_y[i];
                train_dir[i] = DIR_NONE; // Will determine direction on first move?
                // Or we need to set initial direction based on track?
                // Let's assume determineNextPosition handles DIR_NONE by looking at track.
            } else {
                // Wait in queue (retry next tick)
                // train_spawn_time[i] is effectively pushed back or we just check again next tick.
                // We don't change spawn_time, just don't activate yet.
            }
        }
    }
}

// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection(int x, int y, int inDir) {
    if (!isInBounds(x, y)) return DIR_NONE;
    char c = grid[y][x];

    // Simple track logic
    if (c == '-' || c == '=') {
        if (inDir == DIR_RIGHT) return DIR_RIGHT;
        if (inDir == DIR_LEFT) return DIR_LEFT;
    } else if (c == '|') {
        if (inDir == DIR_UP) return DIR_UP;
        if (inDir == DIR_DOWN) return DIR_DOWN;
    } else if (c == '/') {
        if (inDir == DIR_RIGHT) return DIR_UP;
        if (inDir == DIR_DOWN) return DIR_LEFT;
        if (inDir == DIR_LEFT) return DIR_DOWN;
        if (inDir == DIR_UP) return DIR_RIGHT;
    } else if (c == '\\') {
        if (inDir == DIR_RIGHT) return DIR_DOWN;
        if (inDir == DIR_UP) return DIR_LEFT;
        if (inDir == DIR_LEFT) return DIR_UP;
        if (inDir == DIR_DOWN) return DIR_RIGHT;
    } else if (c == '+') {
        // Crossing: usually straight, but smart routing might apply?
        // Spec says "Smart Routing at Crossing".
        // But here we just return the default "straight" direction if not handled elsewhere.
        return inDir;
    } else if (c >= 'A' && c <= 'Z') {
        // Switch
        int idx = getSwitchIndex(c);
        int state = getSwitchStateForDirection(idx, inDir);
        // Assume state 0 = straight/left, 1 = diverge/right?
        // Need to know switch geometry.
        // For now, let's assume switches act like '+' or splits.
        // If it's a split, state determines direction.
        // If it's a merge, it just merges.
        
        // This is tricky without knowing the exact switch geometry from the map.
        // But usually:
        // If coming from "stem", state determines branch.
        // If coming from "branch", forced to stem.
        
        // Let's assume standard "straight" vs "turn" logic?
        // Or maybe the map uses specific chars for switch tracks?
        // No, the map has 'A'..'Z' replacing the track char.
        
        // We might need to look at neighbors to infer geometry?
        // Or assume a default behavior.
        
        // Let's assume state 0 = keep direction, state 1 = turn?
        // Or state 0 = primary, state 1 = secondary.
        
        // Let's try: if state == 0, go straight. If state == 1, turn right (relative)?
        // This is a guess.
        
        if (state == 0) return inDir;
        // If state 1, turn?
        // Turn which way?
        // Maybe we need to look at available tracks around?
        
        // Let's assume for now switches are just straight (0) or turn (1).
        // If turn, we need to find valid track.
        
        // Hack: look for valid track in other directions.
        // ...
        return inDir; // Placeholder
    } else if (c == 'S' || c == 'D') {
        return inDir;
    }

    return inDir; // Default keep going
}

// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing(int trainIdx, int x, int y) {
    // Try all 4 directions?
    // Or just straight vs turn?
    // Usually '+' allows straight or turn?
    // Spec: "Smart Routing at Crossing".
    // We should pick the direction that minimizes distance to dest.
    
    int bestDir = train_dir[trainIdx];
    int bestDist = 999999;
    
    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {-1, 0, 1, 0};
    
    // Check possible directions (usually can go any way at +?)
    // Or maybe just straight?
    // "Intersecting routes with crossings"
    // Usually crossing means you continue straight.
    // But maybe "Smart Routing" implies you CAN turn?
    
    // Let's assume you can go any valid direction that has track.
    for (int d = 0; d < 4; d++) {
        // Don't reverse
        if (d == (train_dir[trainIdx] + 2) % 4) continue;
        
        int nx = x + dx[d];
        int ny = y + dy[d];
        
        if (isTrackTile(nx, ny)) {
            int dist = getDistance(nx, ny, train_dest_x[trainIdx], train_dest_y[trainIdx]);
            if (dist < bestDist) {
                bestDist = dist;
                bestDir = d;
            }
        }
    }
    return bestDir;
}

// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------
bool determineNextPosition(int i) {
    if (train_state[i] != TRAIN_STATE_ACTIVE) return false;

    int cx = train_x[i];
    int cy = train_y[i];
    int cdir = train_dir[i];
    
    // If just spawned (DIR_NONE), find valid exit
    if (cdir == DIR_NONE) {
        // Look for any track neighbor
        int dx[4] = {0, 1, 0, -1};
        int dy[4] = {-1, 0, 1, 0};
        for (int d = 0; d < 4; d++) {
            if (isTrackTile(cx + dx[d], cy + dy[d])) {
                cdir = d;
                break;
            }
        }
        if (cdir == DIR_NONE) return false; // Stuck
    }

    // Determine next cell based on current cell type
    char c = grid[cy][cx];
    int nextDir = cdir;
    
    if (c == '+') {
        nextDir = getSmartDirectionAtCrossing(i, cx, cy);
    } else {
        nextDir = getNextDirection(cx, cy, cdir);
    }
    
    // Check switch signal
    if (c >= 'A' && c <= 'Z') {
        int idx = getSwitchIndex(c);
        if (signal_state[idx] == SIGNAL_RED) {
            // Wait
            train_next_x[i] = cx;
            train_next_y[i] = cy;
            train_next_dir[i] = cdir;
            return true;
        }
    }

    // Calculate next coordinates
    int nx = cx;
    int ny = cy;
    if (nextDir == DIR_UP) ny--;
    else if (nextDir == DIR_RIGHT) nx++;
    else if (nextDir == DIR_DOWN) ny++;
    else if (nextDir == DIR_LEFT) nx--;
    
    // Check bounds and track
    if (isTrackTile(nx, ny)) {
        train_next_x[i] = nx;
        train_next_y[i] = ny;
        train_next_dir[i] = nextDir;
        return true;
    } else {
        // Crash or stop?
        // Assume stop/wait if track ends? Or crash?
        // Let's wait.
        train_next_x[i] = cx;
        train_next_y[i] = cy;
        train_next_dir[i] = cdir;
        return false;
    }
}

// ----------------------------------------------------------------------------
// DETERMINE ALL ROUTES (PHASE 2)
// ----------------------------------------------------------------------------
// Fill next positions/directions for all trains.
// ----------------------------------------------------------------------------
void determineAllRoutes() {
    for (int i = 0; i < train_count; i++) {
        if (train_state[i] == TRAIN_STATE_ACTIVE) {
            determineNextPosition(i);
        }
    }
}

// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions() {
    // Reset waiting state for active trains
    // (Actually we don't change state to WAITING, we just revert their move)
    
    // Check for conflicts
    for (int i = 0; i < train_count; i++) {
        if (train_state[i] != TRAIN_STATE_ACTIVE) continue;
        
        for (int j = i + 1; j < train_count; j++) {
            if (train_state[j] != TRAIN_STATE_ACTIVE) continue;
            
            bool conflict = false;
            
            // 1. Same destination tile
            if (train_next_x[i] == train_next_x[j] && train_next_y[i] == train_next_y[j]) {
                conflict = true;
            }
            // 2. Head-on swap (i goes to j's pos, j goes to i's pos)
            else if (train_next_x[i] == train_x[j] && train_next_y[i] == train_y[j] &&
                     train_next_x[j] == train_x[i] && train_next_y[j] == train_y[i]) {
                conflict = true;     
            }
            
            if (conflict) {
                // Resolve using Distance-Based Priority
                int distI = getDistance(train_x[i], train_y[i], train_dest_x[i], train_dest_y[i]);
                int distJ = getDistance(train_x[j], train_y[j], train_dest_x[j], train_dest_y[j]);
                
                if (distI > distJ) {
                    // I has priority (further away), J waits
                    train_next_x[j] = train_x[j];
                    train_next_y[j] = train_y[j];
                    train_next_dir[j] = train_dir[j];
                } else if (distJ > distI) {
                    // J has priority, I waits
                    train_next_x[i] = train_x[i];
                    train_next_y[i] = train_y[i];
                    train_next_dir[i] = train_dir[i];
                } else {
                    // Tie = Crash both?
                    // Spec: "Equal Distance = Both Crash"
                    train_state[i] = TRAIN_STATE_CRASHED;
                    train_state[j] = TRAIN_STATE_CRASHED;
                    total_crashed += 2;
                }
            }
        }
    }
}

// ----------------------------------------------------------------------------
// MOVE ALL TRAINS (PHASE 5)
// ----------------------------------------------------------------------------
// Move trains; resolve collisions and apply effects.
// ----------------------------------------------------------------------------
void moveAllTrains() {
    for (int i = 0; i < train_count; i++) {
        if (train_state[i] == TRAIN_STATE_ACTIVE) {
            // Update switch counters if entered
            if (train_next_x[i] != train_x[i] || train_next_y[i] != train_y[i]) {
                // Moved
                int nx = train_next_x[i];
                int ny = train_next_y[i];
                if (isSwitchTile(nx, ny)) {
                    int idx = getSwitchIndex(grid[ny][nx]);
                    if (idx != -1) {
                        switch_counter[idx]++;
                    }
                }
            }
            
            // Apply move
            train_x[i] = train_next_x[i];
            train_y[i] = train_next_y[i];
            train_dir[i] = train_next_dir[i];
            
            logTrainTrace(i);
        }
    }
}

// ----------------------------------------------------------------------------
// CHECK ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
// ----------------------------------------------------------------------------
void checkArrivals() {
    for (int i = 0; i < train_count; i++) {
        if (train_state[i] == TRAIN_STATE_ACTIVE) {
            if (train_x[i] == train_dest_x[i] && train_y[i] == train_dest_y[i]) {
                train_state[i] = TRAIN_STATE_ARRIVED;
                total_arrived++;
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
    if (!emergency_halt_active) return;
    
    for (int i = 0; i < train_count; i++) {
        if (train_state[i] == TRAIN_STATE_ACTIVE) {
            // Check if in range
            if (abs(train_x[i] - emergency_halt_x) <= emergency_halt_range &&
                abs(train_y[i] - emergency_halt_y) <= emergency_halt_range) {
                // Halt: force next pos to current pos
                train_next_x[i] = train_x[i];
                train_next_y[i] = train_y[i];
                train_next_dir[i] = train_dir[i];
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
    if (emergency_halt_active) {
        emergency_halt_timer--;
        if (emergency_halt_timer <= 0) {
            emergency_halt_active = false;
        }
    }
}