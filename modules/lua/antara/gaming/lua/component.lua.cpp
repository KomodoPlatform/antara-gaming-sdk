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

#include <utility>
#include "antara/gaming/lua/component.lua.hpp"

namespace antara::gaming::lua
{
    component_script::component_script(std::string script_, std::string table_name_) noexcept : script(std::move(script_)),
                                                                                                table_name(std::move(table_name_))
    {

    }

    component_script::component_script() noexcept : script(""), table_name("")
    {

    }
}