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
#include "antara/gaming/scenes/change.scene.event.hpp"
#include "game.scene.hpp"
#include "intro.scene.hpp"

intro_scene::intro_scene(entt::registry &entity_registry) noexcept : base_scene(entity_registry)
{
}

void intro_scene::update() noexcept
{

}

bool intro_scene::on_key_pressed(const antara::gaming::event::key_pressed &evt) noexcept
{
    if (evt.key == antara::gaming::input::key::space) {
        this->dispatcher_.trigger<antara::gaming::event::change_scene>(std::make_unique<game_scene>(this->entity_registry_), false);
    }
    return true;
}

bool intro_scene::on_key_released(const antara::gaming::event::key_released &) noexcept
{
    return false;
}

std::string intro_scene::scene_name() noexcept
{
    return "intro_scene";
}

intro_scene::~intro_scene() noexcept
{
    auto view = entity_registry_.view<entt::tag<"intro_scene"_hs>>();
    entity_registry_.destroy(view.begin(), view.end());
}
