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

//! C++ System Headers
#include <utility> //! std::move

//! SDK Headers
#include "antara/gaming/ecs/event.add.base.system.hpp"

namespace antara::gaming::ecs
{
    using namespace ecs::event;

    add_base_system::add_base_system(base_system_ptr system_ptr_) noexcept : system_ptr(std::move(system_ptr_))
    {
    }
} // namespace antara::gaming::ecs