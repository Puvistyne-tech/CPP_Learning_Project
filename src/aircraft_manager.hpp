//
// Created by puvis on 16/04/2022.
//

#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"

#include <memory>
#include <ostream>
#include <vector>

class Aircraft;

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;

public:
    AircraftManager();                       // Base Constructor
    ~AircraftManager() override = default;   // Destructor
    AircraftManager(const AircraftManager&) = delete;
    AircraftManager& operator=(const AircraftManager&) = delete;

    void add_aircraft(std::unique_ptr<Aircraft>);
    bool move() override;
    unsigned count_aircrafts(const std::string_view&);
    unsigned get_required_fuel();

};
