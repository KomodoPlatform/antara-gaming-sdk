#include <antara/gaming/world/world.app.hpp>
#include <antara/gaming/sfml/graphic.system.hpp>
#include <antara/gaming/sfml/input.system.hpp>

//! For convenience
using namespace antara::gaming;

//! Our game world
struct tic_tac_toe_world : world::app
{
    //! Our game entry point
    tic_tac_toe_world() noexcept
    {
        //! Here we load our graphical system
        auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

        //! Here we load our input system with the window from the graphical system
        system_manager_.create_system<sfml::input_system>(graphic_system.get_window());
    }
};

int main()
{
    //! Here we declare our world
    tic_tac_toe_world game;

    //! Here we run the game
    return game.run();
}