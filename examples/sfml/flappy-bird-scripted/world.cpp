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

#include <antara/gaming/sfml/lua.system.hpp>
#include "world.hpp"
#include "antara/gaming/sfml/komodo.intro.scene.hpp"
#include "antara/gaming/lua/lua.system.hpp"
#include "antara/gaming/scenes/scene.manager.hpp"
#include "antara/gaming/sfml/graphic.system.hpp"
#include "antara/gaming/sfml/input.system.hpp"

flappy_world::flappy_world() noexcept
{
    auto &lua_scripting_system = this->system_manager_.create_system<antara::gaming::lua::scripting_system>();
    this->system_manager_.create_system<antara::gaming::sfml::lua_system>(lua_scripting_system.get_state_ptr());
    auto &graphic_system = this->system_manager_.create_system<antara::gaming::sfml::graphic_system>();
    this->system_manager_.create_system<antara::gaming::sfml::input_system>(graphic_system.get_window());
    auto &scene_manager = this->system_manager_.create_system<antara::gaming::scenes::manager>();
    //lua_scripting_system.load_scripted_system("scenes_system.lua");
    scene_manager.change_scene(std::make_unique<antara::gaming::sfml::intro_scene>(entity_registry_, dispatcher_, [&lua_scripting_system, this]() {
        lua_scripting_system.load_scripted_system("scenes_system.lua");
        this->system_manager_.mark_system<antara::gaming::scenes::manager>();
    }), true);
}
