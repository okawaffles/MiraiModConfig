#pragma once

#include "common.hpp"
#include "chart.hpp"
#include <fstream>
#include <sstream>

namespace Listing
{

// Will return amount of chart mods in sdmc:/miraimods/charts
int GetChartListingCount();

// Gets the chart info at a given index
Result ReadChartAt(uint8_t index, ChartMod *chart);

}