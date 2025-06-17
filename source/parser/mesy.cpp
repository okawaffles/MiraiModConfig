#include "parser/mesy.hpp"

static const char* last_error = NULL;
static std::map<std::string, std::string> pairs;

#define MESY_DEBUG

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
    pairs.clear();

    std::ifstream mesyfile(filename);
    if (!mesyfile.is_open()) {
        last_error = "File not found.";
        return -1;
    }

    size_t entry = 0;
    size_t cur_line = 0;
    std::string line;

    while (std::getline(mesyfile, line))
    {
        cur_line++;
        if (line.c_str()[0] == '>' || line.find_first_not_of(" \t\n\r\f\v") == std::string::npos) {continue;}
        if (entry > 255)
        {
            last_error = "Too many entries in MESY file. MESY only supports up to 256 entries.";
            return -1;
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
#ifdef MESY_DEBUG
            std::cout << "AT LINE" << cur_line << ": " << line << std::endl;
#endif
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
        while (true)
        {
            if (ptr+1 >= line.length()) 
            {
#ifdef MESY_DEBUG
                std::cout << "AT LINE" << cur_line << ": " << line << std::endl;
#endif
                last_error = "Reached EOL with no \"}}\"";
                return -1;
            }

            to_compare.clear();
            to_compare.str("");
            to_compare << line[ptr] << line[ptr+1];
            strcmp_fail = strcmp(to_compare.str().c_str(), "}}");
            
            if (strcmp_fail == 0) break;

            entry_value << line[ptr];
            ptr++;
        }

        // reached }}, add key/value to array
        pairs[entry_name.str()] = entry_value.str();
        
#ifdef MESY_DEBUG
        std::cout << "NEW ENTRY" << entry << ": " << entry_name.str() << " => " << entry_value.str() << std::endl;
#endif

        entry++;
    }

    return 0;
}

// Clear all
void UnloadMESY()
{
    pairs.clear();
    last_error = "";
}

// Copy all MESY entries held in memory to a specified string array pointer.
void GetAllKVItems(std::string* keys[], std::string* values[])
{
    // fix later
    return;
    // for (size_t i = 0; i < pairs.size(); i++)
    // {
    //     *keys[i]   = pairs[i];
    //     *values[i] = mesy_vals[i];
    // }
}

// Get a specific value by its corresponding key name
int GetValueByKeyName(std::string name, std::string* value)
{
#ifdef MESY_DEBUG
    std::cout << "SCAN FOR KEY: " << name << std::endl;
#endif
    
    if (pairs.count(name) == 0)
    { 
        last_error = "No key exists by provided name.";
        return -1;
    }

    *value = pairs[name];
    return 0;
}

} // namespace MESY