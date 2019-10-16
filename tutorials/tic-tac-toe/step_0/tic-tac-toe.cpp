/******************************************************************************
 * Copyright © 2013-2019 The Komodo Platform Developers.                      *
 *                                                                            *
 * See the AUTHORS, DEVELOPER-AGREEMENT and LICENSE files at                  *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * Komodo Platform software, including this file may be copied, modified,     *
 * propagated or distributed except according to the terms contained in the   *
 * LICENSE file                                                               *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/

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
        auto &graphic_system = this->system_manager_.create_system<sfml::graphic_system>();

        //! Here we load our input system with the window from the graphical system
        this->system_manager_.create_system<sfml::input_system>(graphic_system.get_window());
    }
};

int main()
{
    //! Here we declare our world
    tic_tac_toe_world game;

    //! Here we run the game
    return game.run();
}