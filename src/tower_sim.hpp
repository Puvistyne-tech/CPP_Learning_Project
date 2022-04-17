#pragma once
#include "aircraft_factory.hpp"
#include "aircraft_manager.hpp"

class Airport;
struct AircraftType;

class TowerSimulation
{
private:
    bool help        = false;
    Airport* airport = nullptr;
    // TASK 1
    AircraftManager aircraft_manager;
    AircraftFactory factory;

    TowerSimulation(const TowerSimulation&)            = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    void create_keystrokes();
    void display_help() const;

    void init_airport();
    [[nodiscard]] std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type) const;
    [[nodiscard]] std::unique_ptr<Aircraft> create_random_aircraft() const;

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
};
