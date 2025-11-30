#ifndef IO_H
#define IO_H

// ============================================================================
// IO.H - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LEVEL LOADING
// ----------------------------------------------------------------------------
// Load a .lvl file.
bool loadLevelFile(const char* loadlevel);

// ----------------------------------------------------------------------------
// LOGGING
// ----------------------------------------------------------------------------
// Create/clear log files.
void initializeLogFiles();

// Append train movement to trace.csv.
void logTrainTrace(int id);

// Append switch state to switches.csv.
void logSwitchState(int switchindex);

// Append signal state to signals.csv.
void logSignalState(int switchindex);

// Write final metrics to metrics.txt.
void writeMetrics();

#endif
