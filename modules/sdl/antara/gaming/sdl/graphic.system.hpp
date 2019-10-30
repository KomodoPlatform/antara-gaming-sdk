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

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <entt/entity/registry.hpp>
#include "antara/gaming/ecs/system.hpp"

namespace antara::gaming::sdl
{
    class graphic_system final : public ecs::post_update_system<graphic_system>
    {
    public:
        graphic_system(entt::registry& registry) noexcept;
        ~graphic_system() noexcept final;
        void update() noexcept final;
    private:
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        SDL_Texture* canvas_texture_;
    };
}

REFL_AUTO(type(antara::gaming::sdl::graphic_system));