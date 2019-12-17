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
#include <glad/glad.h>
#include <antara/gaming/graphics/component.canvas.hpp>

#if defined(IMGUI_AND_SDL_ENABLED)

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#ifdef _WIN32

#include <imgui_impl_win32.h>

#endif

#endif

#include "antara/gaming/core/api.scaling.hpp"
#include "antara/gaming/event/quit.game.hpp"
#include "antara/gaming/sdl/graphic.system.hpp"
#include "antara/gaming/sdl/sdl.timer.hpp"

namespace {

}

namespace antara::gaming::sdl {

//The application time based timer
    void graphic_system::update() noexcept {
        //The frames per second timer

        render();
        //If frame finished early
        int frame_ticks = timer_.getTicks();
        int fps_cap_ms = 1000 / fps_cap_;
        if (frame_ticks < fps_cap_ms) {
            //Wait remaining time
            SDL_Delay(fps_cap_ms - frame_ticks);
        }

        //Start cap timer
        timer_.start();
    }

    void graphic_system::render() noexcept {
#if defined(IMGUI_AND_SDL_ENABLED)
        ImGui::Render();
        ImGuiIO &io = ImGui::GetIO();
        glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
#endif
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
#if defined(IMGUI_AND_SDL_ENABLED)
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            SDL_Window *backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
            SDL_HideWindow(backup_current_window);
        }
#endif

        SDL_GL_SwapWindow(window_);
    }

    graphic_system::graphic_system(entt::registry &registry) : system(registry) {
#if _WIN32
        ImGui_ImplWin32_EnableDpiAwareness();
#endif
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
            this->dispatcher_.trigger<event::quit_game>(-1);
            return;
        }

#if __APPLE__
        // GL 3.2 Core + GLSL 150
        const char *glsl_version = "#version 150";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#elif __linux__
        // GL 3.2 Core + GLSL 150
        const char* glsl_version = "#version 150";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#else
        // GL 3.0 + GLSL 130
        const char *glsl_version = "#version 130";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

        auto &canvas_2d = this->entity_registry_.ctx<graphics::canvas_2d>();
        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                                                          SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN);
        auto[real_width, real_height] = canvas_2d.window.size.to<math::vec2i>() /
                                        math::vec2f{core::get_scaling_factor().first,
                                                    core::get_scaling_factor().second}.to<math::vec2i>();
        canvas_2d.window.size.set_xy(real_width, real_height);
        window_ = SDL_CreateWindow(canvas_2d.window_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   1,
                                   1, window_flags);
        gl_context_ = SDL_GL_CreateContext(window_);
        SDL_GL_MakeCurrent(window_, gl_context_);
        if (canvas_2d.vsync) {
            SDL_GL_SetSwapInterval(1); // Enable vsync
        }

        bool err = gladLoadGL() == 0;
        assert(!err);

#if defined(IMGUI_AND_SDL_ENABLED)
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
        io.ConfigViewportsNoAutoMerge = true;
        io.ConfigViewportsNoDefaultParent = true;
        ImGui::StyleColorsDark();
        ImGuiStyle &style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        ImGui_ImplSDL2_InitForOpenGL(window_, gl_context_);
        ImGui_ImplOpenGL3_Init(glsl_version);
#endif
    }

    graphic_system::~graphic_system() noexcept {
#if defined(IMGUI_AND_SDL_ENABLED)
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
#endif

        SDL_GL_DeleteContext(gl_context_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }
}

