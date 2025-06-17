#include "listing.hpp"

// i suppose this can be changed if someone really needs more than 128 charts
// however, do 128 custom charts even exist for this game yet?
static std::string charts[128];

namespace Listing
{

int GetChartListingCount()
{
    // right now i'm only testing with 1 chart so i just need to return 1
    // i will have it actually read later
    charts[0] = "BBF";
    return 1;
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