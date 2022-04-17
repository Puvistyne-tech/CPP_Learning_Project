//
// Created by puvis on 16/04/2022.
//

#include "aircraft_manager.hpp"
AircraftManager::AircraftManager()
{
    GL::move_queue.emplace(this);
}
void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> aircraft)
{
    assert(aircraft);
    aircrafts.emplace_back(std::move(aircraft));
}
bool AircraftManager::move()
{
    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                   [](std::unique_ptr<Aircraft>& air)
                                   {
                                       try
                                       {
                                           return !air->move();
                                       } catch (const AircraftCrash& crash)
                                       {
                                           std::cerr << crash.what() << std::endl;
                                           return true;
                                       }
                                   }),
                    aircrafts.end());

    return true;
}
