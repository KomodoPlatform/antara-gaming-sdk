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

#include <antara/gaming/ecs/virtual.input.system.hpp>

namespace antara::gaming::ecs {
    virtual_input_system::virtual_input_system(entt::registry &registry) noexcept : system(registry) {
        input::virtual_input::init(registry);
    }

    void virtual_input_system::update() noexcept {
        input::virtual_input::update();
    }
}