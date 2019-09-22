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

#include <entt/entity/helper.hpp>
#include "game.scene.hpp"
#include "intro.scene.hpp"

game_scene::game_scene(entt::registry &entity_registry, entt::dispatcher &dispatcher_) noexcept : base_scene(entity_registry,
                                                                                                             dispatcher_)
{
}

void game_scene::update() noexcept
{

}

bool game_scene::on_key_released(const antara::gaming::event::key_released &) noexcept
{
    return false;
}

bool game_scene::on_key_pressed(const antara::gaming::event::key_pressed &evt) noexcept
{
    if (evt.key == antara::gaming::input::key::space) {
        this->dispatcher_.trigger<antara::gaming::event::change_scene>(
                std::make_unique<intro_scene>(this->entity_registry_, this->dispatcher_), false);
    }
    return false;
}

std::string game_scene::scene_name() noexcept
{
    return "game_scene";
}

game_scene::~game_scene() noexcept
{
    auto view = entity_registry_.view<entt::tag<"game_scene"_hs>>();
    entity_registry_.destroy(view.begin(), view.end());
}
