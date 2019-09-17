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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <refl.hpp>

#pragma clang diagnostic pop
#include <SFML/Graphics/RenderWindow.hpp>
#include "meta/sequence/list.hpp"
#include "antara/gaming/ecs/component.window.infos.hpp"
#include "antara/gaming/ecs/system.hpp"

namespace antara::gaming::sfml
{
    class graphic_system final : public ecs::post_update_system<graphic_system>
    {
    public:
        graphic_system(entt::registry &registry, entt::dispatcher &dispatcher) noexcept;

        void update() noexcept final;

        template <size_t Layer, typename DrawableType>
        void draw() noexcept;

        template <size_t Layer, typename... DrawableType>
        void draw(doom::meta::list<DrawableType...>) noexcept;

        template <size_t...Is>
        void draw_each_layers(std::index_sequence<Is...>) noexcept;

        void draw_each_layers() noexcept;

        //! Public getter
        sf::RenderWindow &get_window() noexcept;

    private:
        ecs::component_window &window_component_{entity_registry_.ctx<ecs::component_window>()};
        sf::RenderWindow window_{sf::VideoMode(window_component_.width, window_component_.height),
                                 window_component_.title};
    };
}

REFL_AUTO(type(antara::gaming::sfml::graphic_system));