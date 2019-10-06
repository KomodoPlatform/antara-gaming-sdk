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

#include "world.hpp"
#include "intro.scene.hpp"

flappy_world::flappy_world() noexcept
{
    auto &graphic_system = this->system_manager_.create_system<antara::gaming::sfml::graphic_system>();
    this->system_manager_.create_system<antara::gaming::sfml::input_system>(graphic_system.get_window());
    auto &scene_manager = this->system_manager_.create_system<antara::gaming::scenes::manager>();
    scene_manager.change_scene(std::make_unique<intro_scene>(entity_registry_), true);
}
