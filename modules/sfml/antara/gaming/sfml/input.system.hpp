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

#include <SFML/Graphics/RenderWindow.hpp>
#include "antara/gaming/core/safe.refl.hpp"
#include "antara/gaming/ecs/system.hpp"
#include "antara/gaming/event/get.mouse.position.hpp"
#include "antara/gaming/event/set.mouse.position.hpp"

namespace antara::gaming::sfml
{
    class input_system final : public ecs::pre_update_system<input_system>
    {
    public:
        //! Constructors
        input_system(entt::registry &registry, sf::RenderWindow &window) noexcept;

        void update() noexcept final;

        void on_fill_mouse_position(const event::get_mouse_position& evt) noexcept;
        void on_set_mouse_position(const event::set_mouse_position& evt) noexcept;
    private:
        [[nodiscard]] auto translate_window_coord(int x,  int y) const;

        sf::RenderWindow &window_;
    };
}

REFL_AUTO(type(antara::gaming::sfml::input_system));