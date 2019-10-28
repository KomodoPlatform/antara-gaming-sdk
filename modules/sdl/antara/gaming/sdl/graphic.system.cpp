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

#include <SDL2/SDL.h>
#include "antara/gaming/event/fatal.error.hpp"
#include "antara/gaming/sdl/graphic.system.hpp"

namespace antara::gaming::sdl
{
    void graphic_system::update() noexcept
    {
    }

    graphic_system::graphic_system(entt::registry &registry) noexcept : system(registry)
    {
        if (auto res = SDL_Init(SDL_INIT_VIDEO); res < 0) {
            this->dispatcher_.trigger<event::fatal_error>(std::error_code(res, std::generic_category()));
        }
    }
}