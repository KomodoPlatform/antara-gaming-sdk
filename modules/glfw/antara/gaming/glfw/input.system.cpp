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

#include <antara/gaming/glfw/input.system.hpp>
#include <antara/gaming/event/quit.game.hpp>

#if defined(IMGUI_AND_GLFW_ENABLED)

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#endif

namespace {
    void window_close_callback(GLFWwindow* window) {
        auto &entity_registry = *static_cast<entt::registry *>(glfwGetWindowUserPointer(window));
        auto& dispatcher = entity_registry.ctx<entt::dispatcher>();
        dispatcher.trigger<antara::gaming::event::quit_game>(0);
    }
}

namespace antara::gaming::glfw {
    input_system::input_system(entt::registry &registry, GLFWwindow *window) : system(registry), window_(window) {
        glfwSetWindowCloseCallback(window, window_close_callback);
    }

    void input_system::update() noexcept {
        glfwPollEvents();
#if defined(IMGUI_AND_GLFW_ENABLED)
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
#endif
    }
}