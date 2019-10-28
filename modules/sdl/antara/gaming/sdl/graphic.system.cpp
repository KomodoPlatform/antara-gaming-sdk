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

#include <iostream>
#include <SDL2/SDL.h>
#include <antara/gaming/event/quit.game.hpp>
#include "antara/gaming/graphics/component.canvas.hpp"
#include "antara/gaming/event/fatal.error.hpp"
#include "antara/gaming/sdl/graphic.system.hpp"
#include "antara/gaming/sdl/sdl.error.hpp"

namespace
{
    using namespace antara::gaming;

    SDL_Window *create_window(entt::registry &registry) noexcept
    {
        auto &canvas_2d = registry.ctx<graphics::canvas_2d>();
        SDL_Window *window = nullptr;
        auto[screen_width, screen_height] = canvas_2d.window.size.to<math::vec2i>();
        auto[screen_pos_x, screen_pos_y] = canvas_2d.window.position.to<math::vec2i>();
        SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");
        Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
        if (canvas_2d.is_fullscreen) {
            flags |= SDL_WINDOW_FULLSCREEN;
        }
        window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, flags);
        return window;
    }
}


namespace antara::gaming::sdl
{
    void graphic_system::update() noexcept
    {
        SDL_Event ev;
        SDL_PollEvent(&ev);
        if (ev.type == SDL_QUIT)
        {
            this->dispatcher_.trigger<event::quit_game>(0);
        }
    }

    graphic_system::graphic_system(entt::registry &registry) noexcept : system(registry)
    {
        if (auto res = SDL_Init(SDL_INIT_EVERYTHING); res < 0) {
            this->dispatcher_.trigger<event::fatal_error>(std::error_code(res, sdl::sdl_error_category()));
        }
        window_ = create_window(registry);
        assert(window_ != nullptr);
    }

    graphic_system::~graphic_system() noexcept
    {
        if (window_ != nullptr) {
            SDL_DestroyWindow(window_);
        }
        SDL_Quit();
    }
}