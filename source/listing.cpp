#include "listing.hpp"

// i suppose this can be changed if someone really needs more than 128 charts
// however, do 128 custom charts even exist for this game yet?
static std::string charts[128];

#define LISTING_DEBUG
namespace Listing
{

int GetChartListingCount(size_t *count)
{
    DIR *dir;
    struct dirent* ent;

    dir = opendir("sdmc:/miraimods/charts");
    if (dir == NULL)
    {
        std::cout << "miraimods/charts folder does not exist, creating..." << std::endl;
        mkdir("sdmc:/miraimods", 0777);
        mkdir("sdmc:/miraimods/charts", 0777);

        dir = opendir("sdmc:/miraimods/charts");
        if (dir == NULL)
        {
            std::cout << "ERROR: Could not create/open directory:\n-> sdmc:/miraimods/charts <-" << std::endl;
            return -1;
        }
    }

    *count = 0;
    while ((ent = readdir(dir)))
    {
        // if it's not a directory, skip it
        if (ent->d_type != DT_DIR) continue;
        
        // check schema to make sure it's a v1 chart meta file
        std::stringstream path;
        path << "sdmc:/miraimods/charts/" << ent->d_name << "/meta.mesy";

        int mesy_load_rc = MESY::LoadMESY(path.str().c_str());
        if (mesy_load_rc != 0) {
#ifdef LISTING_DEBUG
            std::cout << "Error loading meta.mesy for chart \"" << ent->d_name << "\" with error " << mesy_load_rc << std::endl;
#endif
            continue; // mesy load failure, skip
        }
        std::string schema;
        std::string schema_ver;
        mesy_load_rc += MESY::GetValueByKeyName("mmc_type", &schema);
        std::cout << MESY::GetLastError() << std::endl;
        mesy_load_rc += MESY::GetValueByKeyName("mmc_schema_ver", &schema_ver);
        std::cout << MESY::GetLastError() << std::endl;
        if (mesy_load_rc != 0 || schema != "chart" || schema_ver != "1") {
#ifdef LISTING_DEBUG
            std::cout << "Error getting schema info for chart \"" << ent->d_name << "\" with error " << mesy_load_rc << std::endl;
#endif
            continue; // mesy key-get failure or wrong schema
        }

        count++;
        charts[*count] = ent->d_name;
    }

    closedir(dir);
    return 0;
}

Result ReadChartAt(uint8_t index, ChartMod* chart)
{
    std::stringstream file_path;
    file_path << "sdmc:/miraimods/charts/" << charts[index] << "/meta.mesy";

    std::cout << file_path.str() << std::endl;

    int success = MESY::LoadMESY(file_path.str().c_str());

    if (success != 0)
    {
        std::cout << "ERROR: Could not load meta.mesy!" << std::endl;
        return -1;
    }

    std::string value("null");
    Result rc = 0;

    rc += MESY::GetValueByKeyName("name", &value);
    chart->song_name = value;
    rc += MESY::GetValueByKeyName("artist", &value);
    chart->song_artist = value;
    rc += MESY::GetValueByKeyName("charter", &value);
    chart->chart_author = value;
    rc += MESY::GetValueByKeyName("bpm", &value);
    chart->bpm = value;

    chart->type = CHART_TYPE_BUTTON;
    
    // rc should be zero
    // if it's not, something went wrong, and mmc will ignore it
    return rc;
}

}