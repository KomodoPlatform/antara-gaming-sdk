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

#include <antara/gaming/sdl/input.system.hpp>
#include <antara/gaming/event/quit.game.hpp>

#if defined(IMGUI_AND_SDL_ENABLED)

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#endif


namespace antara::gaming::sdl {
    input_system::input_system(entt::registry &registry, SDL_Window *window) : system(registry), window_(window) {

    }

    void input_system::update() noexcept {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
#if defined(IMGUI_AND_SDL_ENABLED)
            ImGui_ImplSDL2_ProcessEvent(&event);
#endif
            if (event.type == SDL_QUIT)
                this->dispatcher_.trigger<event::quit_game>(0);
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window_))
                this->dispatcher_.trigger<event::quit_game>(0);
        }
#if defined(IMGUI_AND_SDL_ENABLED)
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window_);
        ImGui::NewFrame();
#endif
    }
}