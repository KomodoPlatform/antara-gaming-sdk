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
    class input_system final : public ecs::pre_update_system<input_system>
    {
    public:
        //! Constructors
        input_system(entt::registry &registry, entt::dispatcher &dispatcher, sf::RenderWindow &window) noexcept;

        void update() noexcept final;

    private:
        sf::RenderWindow &window_;
    };
}

REFL_AUTO(type(antara::gaming::sfml::input_system));