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

//! SDK Headers
#include "antara/gaming/box2d/box2d.system.hpp"
#include "antara/gaming/timer/time.step.hpp" ///< timer::time_step

namespace antara::gaming::box2d {
    box2d_system::box2d_system(entt::registry &registry) noexcept : system(registry) {
    }

    void box2d_system::update() noexcept {
        world_.Step(antara::gaming::timer::time_step::get_fixed_delta_time(), 8, 3);
    }
}