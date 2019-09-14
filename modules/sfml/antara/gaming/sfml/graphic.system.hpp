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

#pragma once

#include <refl.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "antara/gaming/ecs/system.hpp"

namespace antara::gaming::sfml
{
    class graphic_system final : public ecs::post_update_system<graphic_system>
    {
    public:
        graphic_system(entt::registry &registry, entt::dispatcher &dispatcher) noexcept;
        void update() noexcept final;

        //! Public getter
        sf::RenderWindow& get_window() noexcept;
    private:
        sf::RenderWindow window_{sf::VideoMode(800, 600), "My window"};
    };
}

REFL_AUTO(type(antara::gaming::sfml::graphic_system));