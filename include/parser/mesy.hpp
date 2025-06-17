#pragma once

// Parser for MESY files (Millie's Extremely Stupid... yes)
// This format stores only KV strings
// Each file can have a max of 256 key-value pairs
// example: KEY:{{VALUE}}

#include <sstream>
#include <fstream>
#include <iostream>
#include <string.h>

#define MESY_VERSION "1.0.0"

namespace MESY
{
    
// MESY returns -1 on error regardless of fault.
// Use this function to get a readable reason on why something failed.
const char* GetLastError();

// Read all MESY keys/values into memory.
// This will clear all other MESY keys currently held in memory.
int LoadMESY(const char* filename);

// Clear all
void UnloadMESY();

// Copy all MESY entries held in memory to a specified string array pointer.
void GetAllKVItems(std::string* keys[], std::string* values[]);

// Get a specific value by its corresponding key name
int GetValueByKeyName(std::string name, std::string* value);

} // namespace MESY