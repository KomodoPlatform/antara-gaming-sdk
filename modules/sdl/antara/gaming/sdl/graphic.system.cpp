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
#ifdef _WIN32
#pragma comment(lib, "Shcore.lib")
#define NOMINMAX
#include <windows.h>
#include <ShellScalingApi.h>
#endif

#include <SDL2/SDL.h>
#include <iostream>
#include "antara/gaming/core/api.scaling.hpp"
#include "antara/gaming/event/quit.game.hpp"
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
        if (!core::is_high_dpi_capable()) {
            SDL_LogWarn(SDL_LOG_CATEGORY_SYSTEM, "Your system is not high dpi capable, physical dpi used instead");
        }
        Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
        if (canvas_2d.is_fullscreen) {
            flags |= SDL_WINDOW_FULLSCREEN;
        }
        auto[real_width, real_height] = canvas_2d.window.size.to<math::vec2i>() /
                                            math::vec2f{core::get_scaling_factor().first,
                                                        core::get_scaling_factor().second}.to<math::vec2i>();
        window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, real_width, real_height, flags);
        return window;
    }

    SDL_Renderer* create_renderer(SDL_Window* window) noexcept
    {
        return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    }

    SDL_Texture* create_texture(entt::registry& registry, SDL_Renderer *renderer) noexcept
    {
        auto [width, height] = registry.ctx<graphics::canvas_2d>().canvas_texture.size;
        return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    }
}


namespace antara::gaming::sdl
{
    void graphic_system::update() noexcept
    {
        SDL_Event ev;
        SDL_PollEvent(&ev);
        if (ev.type == SDL_QUIT) {
            this->dispatcher_.trigger<event::quit_game>(0);
        }

        //! Set target to texture
        SDL_SetRenderTarget(renderer_, canvas_texture_);

        //! Black background
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255 );

        //! Blit the black blackground
        SDL_RenderClear(renderer_);

        //! TODO: Blit everything here on every layer

        //! Set render back to the window
        SDL_SetRenderTarget(renderer_, nullptr);

        //! Copy the whole texture to the renderer
        SDL_RenderCopy(renderer_, canvas_texture_, nullptr, nullptr);

        //! Blit
        SDL_RenderPresent(renderer_);

        //! Flush
        SDL_RenderFlush(renderer_);
    }

    graphic_system::graphic_system(entt::registry &registry) noexcept : system(registry)
    {
#ifdef _WIN32
        SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif
        if (auto res = SDL_Init(SDL_INIT_EVERYTHING); res < 0) {
            this->dispatcher_.trigger<event::fatal_error>(std::error_code(res, sdl::sdl_error_category()));
        }
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
        window_ = create_window(registry);
        assert(window_ != nullptr);
        renderer_ = create_renderer(window_);
        assert(renderer_);
        canvas_texture_ = create_texture(registry, renderer_);
        assert(canvas_texture_);
    }

    graphic_system::~graphic_system() noexcept
    {
        if (window_ != nullptr) {
            SDL_DestroyWindow(window_);
        }
        SDL_Quit();
    }
}