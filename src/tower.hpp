#pragma once

#include "waypoint.hpp"

#include <algorithm>
#include <unordered_map>
#include <utility>
#include <vector>
#include <map>


class Airport;
class Aircraft;

class Tower
{
private:
//    using AircraftToTerminal      = std::vector<std::pair<const Aircraft*, size_t>>;
//    using AircraftAndTerminalIter = AircraftToTerminal::iterator;

    // TASK_0 C-6
    // We can use a map or an unordered map, in order to retrieve the terminal index from the Aircraft in
    // O(1).
    using AircraftToTerminal = std::map<const Aircraft*, size_t>;

    Airport& airport;
    // aircrafts may reserve a terminal
    // if so, we need to save the terminal number in order to liberate it when the craft leaves
    AircraftToTerminal reserved_terminals = {};

    WaypointQueue get_circle() const;

//    AircraftAndTerminalIter find_craft_and_terminal(const Aircraft& aircraft)
//    {
//        return std::find_if(reserved_terminals.begin(), reserved_terminals.end(),
//                            [&aircraft](const auto& x) { return x.first == &aircraft; });
//    }

public:
    Tower(Airport& airport_) : airport { airport_ } {}

    // produce instructions for aircraft
    WaypointQueue get_instructions(Aircraft& aircraft);
    void arrived_at_terminal(const Aircraft& aircraft);
};
