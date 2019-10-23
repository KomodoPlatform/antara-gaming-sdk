#include <antara/gaming/world/world.app.hpp>
#include <antara/gaming/sfml/graphic.system.hpp>
#include <antara/gaming/sfml/input.system.hpp>

//! For convenience
using namespace antara::gaming;

//! Game world
struct flappy_bird_world : world::app
{
    //! Our game entry point
    flappy_bird_world() noexcept
    {
        //! Load our graphical system
        auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

        //! Load our input system with the window from the graphical system
        system_manager_.create_system<sfml::input_system>(graphic_system.get_window());
    }
};

int main()
{
    //! Declare our world
    flappy_bird_world game;

    //! Run the game
    return game.run();
}