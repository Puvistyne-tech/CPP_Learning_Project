//
// Created by puvis on 16/04/2022.
//
#include "aircraft_manager.hpp"

#include <algorithm>
#include <numeric>

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
    std::sort(aircrafts.begin(), aircrafts.end(),
              [](std::unique_ptr<Aircraft>& a, std::unique_ptr<Aircraft>& b)
              {
                  if (a->has_terminal() == b->has_terminal())
                      return a->getFuel() <= b->getFuel();
                  return a->has_terminal();
              });

    aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                   [this](std::unique_ptr<Aircraft>& air)
                                   {
                                       try
                                       {
                                           return !air->move();
                                       } catch (const AircraftCrash& crash)
                                       {
                                           std::cerr << crash.what() << std::endl;
                                           crash_count++;
                                           return true;
                                       }
                                   }),
                    aircrafts.end());

    return true;
}
unsigned AircraftManager::count_aircrafts(const std::string_view& line)
{
    return std::count_if(aircrafts.begin(), aircrafts.end(),
                         [line](const std::unique_ptr<Aircraft>& a)
                         { return (a->get_flight_num().rfind(line, 0) == 0); });
}

unsigned AircraftManager::get_required_fuel() {
    return std::accumulate(aircrafts.begin(), aircrafts.end(), 0,
                           [](unsigned x, const std::unique_ptr<Aircraft>& aircrft){
                               return aircrft->is_low_on_fuel() && aircrft->is_circling() ? aircrft->get_missing_fuel() + x : x;
                           });
}

void AircraftManager::display_crash_number() const {
    std::cout << crash_count << " aircraft(s) have crashed so far." << std::endl;
}