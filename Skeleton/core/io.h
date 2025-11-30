#ifndef IO_H
#define IO_H

// ============================================================================
// IO.H - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LEVEL LOADING
// ----------------cccciwdhfiurgui------------------------------------------------------------
// Load a .lvl file.
bool loadLevelFile(const char* filename);

// ----------------------------------------------------------------------------
// LOGGING
// ----------------------------------------------------------------------------
// Create/clear log files.
void initializeLogFiles();

// Append train movement to trace.csv.
void logTrainTrace(int id);

// Append switch state to switches.csv.
void logSwitchState(int switchIdx);

// Append signal state to signals.csv.
void logSignalState(int switchIDX);

// Write final metrics to metrics.txt.
void writeMetrics();

#endif
