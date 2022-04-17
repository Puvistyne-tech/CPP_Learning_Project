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
//    AircraftManager aircraft_manager;
    std::unique_ptr<AircraftFactory> factory;
    std::unique_ptr<AircraftManager> manager;

    std::string data_path;

    TowerSimulation(const TowerSimulation&)            = delete;
    TowerSimulation& operator=(const TowerSimulation&) = delete;

    void create_keystrokes();
    void display_help() const;

    void init_airport();
    //std::unique_ptr<Aircraft> create_aircraft(const AircraftType& type) const;
    void create_random_aircraft();

public:
    TowerSimulation(int argc, char** argv);
    ~TowerSimulation();

    void launch();
    void display_airline(unsigned int number);
    void display_all_airlines();
};
