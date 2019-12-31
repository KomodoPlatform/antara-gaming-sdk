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
#include "antara/gaming/graphics/component.text.hpp"

namespace antara::gaming::graphics
{
    entt::entity
    blueprint_text(entt::registry& registry, const text& txt, const transform::position_2d pos, fill_color txt_color) noexcept
    {
        auto text_entity = registry.create();
        registry.assign<fill_color>(text_entity, txt_color);
        registry.assign<text>(text_entity, txt);
        registry.assign<transform::position_2d>(text_entity, pos);
        return text_entity;
    }
} // namespace antara::gaming::graphics