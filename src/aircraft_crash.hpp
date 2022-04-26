//
// Created by puvis on 17/04/2022.
//
//

#pragma once
#include "geometry.hpp"
#include <stdexcept>
#include <ostream>

using namespace std;

enum AircraftCrashes
{
    out_of_fuel,
    bad_landing
};

class AircraftCrash : public runtime_error
{
private:
    static string error_mgs(const string& flight_number, const Point3D& position, const Point3D& speed,
                        const AircraftCrashes crashes)
    {
        return flight_number + " crashed #####" + crashes_to_string(crashes) +
               " || position :: " + position.to_string() + " || speed :: " + speed.to_string();
    }
    static std::string crashes_to_string(const AircraftCrashes& crashes) {
        if (crashes == out_of_fuel) return " NO FUEL";
        if (crashes == bad_landing) return " LANDING GEAR NOT DEPLOYED";
        return " CRITICAL REASON";
    }
public:
    AircraftCrash(const std::string& flight_number, const Point3D& position,
                  const Point3D& speed, const AircraftCrashes& crash)
        : runtime_error {error_mgs(flight_number, position, speed, crash)}
    {
        assert(!flight_number.empty());
    }
};

// TOWER_SIM_AIRCRAFT_CRASH_HPP
//
