#include <antara/gaming/world/world.app.hpp>

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

#include "antara/gaming/world/world.app.hpp"
#include "antara/gaming/sfml/graphic.system.hpp"
#include "antara/gaming/sfml/input.system.hpp"

class my_world : public antara::gaming::world::app
{
public:
    my_world() noexcept
    {
        auto& graphic_system = this->system_manager_.create_system<antara::gaming::sfml::graphic_system>();
        this->system_manager_.create_system<antara::gaming::sfml::input_system>(graphic_system.get_window());
    }
};

int main()
{
    my_world game_app;
    return game_app.run();
}