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

#include "antara/gaming/core/safe.refl.hpp"
#include "antara/gaming/input/keyboard.hpp"

namespace antara::gaming::event
{
    /**
     * @class key_pressed
     * @brief triggered when pressing a key on the keyboard.
     *
     * @verbatim embed:rst:leading-asterisk
     *      .. note::
     *         This class is automatically reflected for scripting systems such as lua, python.
     * @endverbatim
     */
    struct key_pressed
    {
        /**
         * constructor with args
         * @param key_ represents the keyboard key currently pressed
         * @param alt_ is true if the alt key on the keyboard is pressed
         * @param control_ is true if the keyboard control key is pressed
         * @param shift_ is true if the keyboard shift_ key is pressed
         * @param system_ is true if the keyboard system key is pressed
         *
         * @verbatim embed:rst:leading-asterisk
         *      .. note::
         *         Principal Constructor.
         * @endverbatim
         *
         * Example:
         * @code{cpp}
         *          #include <entt/entity/registry.hpp>
         *          #include <entt/dispatcher/dispatcher.hpp>
         *          #include <antara/gaming/ecs/system.manager.hpp>
         *
         *          int main()
         *          {
         *              entt::registry entity_registry;
         *              entt::dispatcher& dispatcher{registry.set<entt::dispatcher>()};
         *              dispatcher.trigger<key_pressed>(input::key::a, false, false, false, false);
         *          }
         * @endcode
         */
        key_pressed(input::key key_, bool alt_,
                    bool control_,
                    bool shift_,
                    bool system_) noexcept;

        /**
         * default constructor (for scripting systems convenience)
         */
        key_pressed() noexcept;


        antara::gaming::input::key key; ///< key pressed
        bool alt{false}; ///< is alt pressed at the same time.
        bool control{false}; ///< is ctrl pressed at the same time.
        bool shift{false}; ///< is shift pressed at the same time.
        bool system{false}; ///< is system pressed at the same time.
    };
}

REFL_AUTO(type(antara::gaming::event::key_pressed), field(key), field(alt), field(control), field(shift), field(system))