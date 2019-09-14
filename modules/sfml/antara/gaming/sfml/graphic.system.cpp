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

#include "antara/gaming/sfml/graphic.system.hpp"

namespace antara::gaming::sfml
{
    graphic_system::graphic_system(entt::registry &registry, entt::dispatcher &dispatcher) noexcept : system(registry, dispatcher)
    {

    }

    void graphic_system::update() noexcept
    {
        window_.clear();
        //TODO: Write draw components and think about logic to draw correctly
        window_.display();
    }

    sf::RenderWindow &graphic_system::get_window() noexcept
    {
        return window_;
    }
}
