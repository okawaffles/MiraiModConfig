#pragma once

#include "common.hpp"
#include "chart.hpp"
#include "parser/mesy.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <dirent.h>

namespace Listing
{

// Will return amount of chart mods in sdmc:/miraimods/charts
int GetChartListingCount(size_t *count);

// Gets the chart info at a given index
Result ReadChartAt(uint8_t index, ChartMod *chart);

}