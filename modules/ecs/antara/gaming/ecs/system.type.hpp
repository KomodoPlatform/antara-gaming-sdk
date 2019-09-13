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

#include <st/type.hpp>

namespace antara::gaming::ecs
{
    enum system_type
    {
        pre_update,
        logic_update,
        post_update,
        size
    };

    using st_system_pre_update = st::type<ecs::system_type, struct system_pre_update_tag>;
    using st_system_logic_update = st::type<system_type, struct system_logic_update_tag>;
    using st_system_post_update = st::type<system_type, struct system_post_update_tag>;
}