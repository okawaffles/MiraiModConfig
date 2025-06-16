#pragma once

#include "common.hpp"

enum ChartType {
    CHART_TYPE_TOUCH = 0,
    CHART_TYPE_BUTTON = 1,
};

// Most of these are strings since they will
// be written as strings in pv_db.txt regardless,
// so conversion doesn't matter.
struct ChartMod
{
    std::string song_name;
    std::string song_artist;
    std::string chart_author;
    std::string bpm;
    ChartType type;
};