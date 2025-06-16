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


static const char* last_error = NULL;
static std::string mesy_keys[256];
static std::string mesy_vals[256];

namespace MESY
{
    
// MESY returns -1 on error regardless of fault.
// Use this function to get a readable reason on why something failed.
const char* GetLastError()
{
    return last_error;
}

// Read all MESY keys/values into memory.
// This will clear all other MESY keys currently held in memory.
int LoadMESY(const char* filename)
{
    mesy_keys->clear();
    mesy_vals->clear();
    last_error = "";

    std::ifstream mesyfile(filename);
    if (!mesyfile.is_open()) {
        last_error = "Could not open file.";
        return -1;
    }

    size_t entry = 0;
    std::string line;
    while (std::getline(mesyfile, line))
    {
        if (line.empty() || line.find_first_not_of(' ') != std::string::npos || line[0] == '>') continue; // skip empty lines and comments

        if (entry > 255)
        {
            last_error = "Too many entries in MESY file. MESY only supports up to 256 entries. The first 256 entries are available, but the rest will be ignored.";
            return -2;
        }

        std::stringstream entry_name;
        std::stringstream entry_value;

        std::stringstream to_compare;
        size_t ptr = 0;
        int strcmp_fail = 1;
        
        // read until we find :{{ or hit EOL
        while (strcmp_fail)
        {
            if (ptr > line.length() - 2)
            {
                last_error = "Reached EOL with no \":{{\"";
                return -1;
            }

            to_compare.str("");
            to_compare << line.c_str()[ptr] << line.c_str()[ptr+1] << line.c_str()[ptr + 2];
        
            strcmp_fail = strcmp(to_compare.str().c_str(), ":{{");

            if (strcmp_fail) entry_name << line.c_str()[ptr];

            ptr++;
        }
        // adjust pointer after we get the property key
        ptr += 2;
        strcmp_fail = 1;
        
        while (strcmp_fail)
        {
            if (ptr > line.length() - 1)
            {
                last_error = "Reached EOL with no \"}}\"";
                return -1;
            }

            to_compare.str("");
            to_compare << line.c_str()[ptr] << line.c_str()[ptr+1];
            
            strcmp_fail = strcmp(to_compare.str().c_str(), "}}");

            if (strcmp_fail) entry_value << line.c_str()[ptr];

            ptr++;
        }

        // reached }}, add key/value to array
        mesy_keys[entry] = entry_name.str();
        mesy_vals[entry] = entry_value.str();

        entry++;
    }

    return 0;
}

// Clear all
void UnloadMESY()
{
    mesy_keys->clear();
    mesy_vals->clear();
    last_error = "";
}

// Copy all MESY entries held in memory to a specified string array pointer.
void GetAllKVItems(std::string* keys[], std::string* values[])
{
    for (size_t i = 0; i < mesy_keys->length(); i++)
    {
        *keys[i]   = mesy_keys[i];
        *values[i] = mesy_vals[i];
    }
}

// Get a specific value by its corresponding key name
int GetValueByKeyName(std::string name, std::string* value)
{
    size_t item = 0;
    int compare_fail = 1;
    while (compare_fail)
    {
        if (item >= mesy_keys->length()) 
        {
            std::stringstream t;
            t << "item index of " << item << " is larger than key count of " << mesy_keys->length();
            last_error = t.str().c_str();
            return -1;
        }

        compare_fail = strcmp(mesy_keys[item].c_str(), name.c_str());

        if (!compare_fail) 
        {
            *value = mesy_vals[item];
            return 0;
        }

        item++;
    }

    last_error = "compare succeeded without return? this is unintended behavior.";
    return -1;
}

} // namespace MESY