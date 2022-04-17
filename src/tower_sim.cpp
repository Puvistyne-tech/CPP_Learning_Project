#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft_factory.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;

TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) }
{
    MediaPath::initialize(argv[0]);
    data_path = help && argc > 2 ? argv[2] : argc > 1 ? argv[1] : "";
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GL::init_gl(argc, argv, "Airport Tower Simulation");

    GL::move_queue.emplace(&aircraft_manager);
    create_keystrokes();
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { create_random_aircraft(); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    // TASK 0
    GL::keystrokes.emplace('i', []() { GL::increase_frames(); });
    GL::keystrokes.emplace('d', []() { GL::decrease_frames(); });
    GL::keystrokes.emplace('p', []() { GL::pause(); });
    GL::keystrokes.emplace('z', []() { GL::ticks_per_sec = std::max(GL::ticks_per_sec - 1u, 1u); });
    GL::keystrokes.emplace('a', []() { GL::ticks_per_sec = std::min(GL::ticks_per_sec + 1u, 180u); });
    for (auto i = 0; i < 8; i++)
    {
        GL::keystrokes.emplace('1' + i, [this, i]() { display_airline(i); });
    }
    GL::keystrokes.emplace('9', [this]() { display_all_airlines(); });
}

void TowerSimulation::display_airline(unsigned number)
{
    assert(number <= 7);
    const std::string& airline = airlines[number];
    const unsigned count       = aircraft_manager.count_aircrafts(airline);
    std::cout << " Aircraft of the line [ " << airline << " ] ::: " << count << std::endl;
}

void TowerSimulation::display_all_airlines()
{
    for (auto i = 0; i < 8; i++)
    {
        display_airline(i);
    }
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keystrokes have meaning:" << std::endl;

    for (const auto& [key, action] : GL::keystrokes)
    {
        std::cout << key << ' ';
    }
    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport = new Airport { one_lane_airport, Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() }, *manager };

    //    GL::display_queue.emplace_back(airport);
    GL::move_queue.emplace(airport);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();
    factory = data_path.empty() ? std::make_unique<AircraftFactory>()
                                : AircraftFactory::LoadTypes(MediaPath { data_path });
    //    factory.init_aircraft_types();

    GL::loop();
}
void TowerSimulation::create_random_aircraft()
{
    assert(airport); // make sure the airport is initialized before creating aircraft
    aircraft_manager.add_aircraft(factory->create_aircraft(airport->get_tower()));
}