#include "listing.hpp"

// i suppose this can be changed if someone really needs more than 128 charts
// however, do 128 custom charts even exist for this game yet?
static std::string* charts[128];

namespace Listing
{

int GetChartListingCount()
{
    // right now i'm only testing with 1 chart so i just need to return 1
    // i will have it actually read later
    return 1;
}

Result ReadChartAt(uint8_t index, ChartMod* chart)
{
    std::stringstream file_path;
    file_path << "sdmc:/miraimods/charts/" << charts[index];

    std::ifstream chart_file(file_path.str());
    // continue here!
    return 0;
}

}