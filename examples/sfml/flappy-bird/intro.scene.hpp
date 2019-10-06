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

#include <entt/entity/helper.hpp>
#include "antara/gaming/scenes/base.scene.hpp"

class intro_scene final : public antara::gaming::scenes::base_scene
{
public:
    intro_scene(entt::registry &entity_registry) noexcept;

    void update() noexcept final;

    bool on_key_pressed(const antara::gaming::event::key_pressed &evt) noexcept final;

    bool on_key_released(const antara::gaming::event::key_released &) noexcept final;

    std::string scene_name() noexcept final;

    ~intro_scene() noexcept final;
};
