//
// Created by puvis on 16/04/2022.
//

#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"

#include <memory>
#include <ostream>
#include <vector>

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;

public:
    //    aircraft_manager();//Constructeur base
    //    ~aircraft_manager();//Destructeur
    //    aircraft_manager(const aircraft_manager&)=delete;
    //    aircraft_manager& operator=(const aircraft_manager&)=delete;

    void add(std::unique_ptr<Aircraft> aircraft);
    bool move() override;
};
