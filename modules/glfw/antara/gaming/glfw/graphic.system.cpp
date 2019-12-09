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

#include <iostream>

#include <glad/glad.h>
#include <antara/gaming/graphics/component.canvas.hpp>

#if defined(IMGUI_AND_GLFW_ENABLED)

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#endif

#include "antara/gaming/core/api.scaling.hpp"
#include "antara/gaming/event/quit.game.hpp"
#include "antara/gaming/glfw/graphic.system.hpp"

namespace {
    void glfw_error_callback(int error, const char *description) {
        std::cerr << "[ERROR] GLFW error: " << error << ", " << description << std::endl;
    }

    void teardown(GLFWwindow *window) {
#if defined(IMGUI_AND_GLFW_ENABLED)
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
#endif
        if (window != nullptr) { glfwDestroyWindow(window); }
        glfwTerminate();
    }

    void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        auto &entity_registry = *static_cast<entt::registry *>(glfwGetWindowUserPointer(window));
        auto &canvas_2d = entity_registry.ctx<antara::gaming::graphics::canvas_2d>();
        canvas_2d.window.size.set_xy(width, height);
        glViewport(0, 0, width, height);
    }
}

namespace antara::gaming::glfw {
    void graphic_system::update() noexcept {
        //! Draw here
#if defined(IMGUI_AND_GLFW_ENABLED)
        ImGui::Render();
#endif
        glClearColor(0, 0, 0, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

#if defined(IMGUI_AND_GLFW_ENABLED)
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

        glfwSwapBuffers(window_);
    }

    graphic_system::graphic_system(entt::registry &registry) : system(registry) {
#ifdef _WIN32
        SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit()) {
            std::cerr << "[ERROR] Couldn't initialize GLFW\n";
            this->dispatcher_.trigger<event::quit_game>(-1);
            return;
        } else {
            std::cout << "[INFO] GLFW initialized\n";
        }

        glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);


        std::string glsl_version = "";
#ifdef __APPLE__
        // GL 3.2 + GLSL 150
        glsl_version = "#version 150";
        glfwWindowHint( // required on Mac OS
                GLFW_OPENGL_FORWARD_COMPAT,
                GL_TRUE
        );
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(
                GLFW_OPENGL_PROFILE,
                GLFW_OPENGL_CORE_PROFILE
        );

#elif __linux__
        // GL 3.2 + GLSL 150
    glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(
                GLFW_OPENGL_PROFILE,
                GLFW_OPENGL_CORE_PROFILE
        );

#elif _WIN32
    // GL 3.0 + GLSL 130
    glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

        float &factor = this->entity_registry_.set<st_high_dpi_factor>(1.0f).value();
#ifdef _WIN32
        // if it's a HighDPI monitor, try to scale everything
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    std::cout << "[INFO] Monitor scale: " << xscale << "x" << yscale << std::endl;
    if (xscale > 1 || yscale > 1)
    {
        factor = xscale;
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    }
#elif __APPLE__
        // to prevent 1200x800 from becoming 2400x1600
        // and some other weird resizings
        //glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif
        auto &canvas_2d = this->entity_registry_.ctx<graphics::canvas_2d>();
        auto[real_width, real_height] = canvas_2d.window.size.to<math::vec2i>() /
                                        math::vec2f{core::get_scaling_factor().first,
                                                    core::get_scaling_factor().second}.to<math::vec2i>();
        canvas_2d.window.size.set_xy(real_width, real_height);
        window_ = glfwCreateWindow(
                real_width,
                real_height,
                canvas_2d.window_title.c_str(),
                nullptr,
                nullptr
        );
        if (!window_) {
            std::cerr << "[ERROR] Couldn't create a GLFW window\n";
            teardown(nullptr);
            this->dispatcher_.trigger<event::quit_game>(-1);
            return;
        }
        glfwSetWindowUserPointer(window_, &this->entity_registry_);
        glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
        glfwMakeContextCurrent(window_);

        //! VSync
        if (canvas_2d.vsync) {
            glfwSwapInterval(1);
        }

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cerr << "[ERROR] Couldn't initialize GLAD" << std::endl;
            teardown(window_);
            this->dispatcher_.trigger<event::quit_game>(-1);
            return;
        } else {
            std::cout << "[INFO] GLAD initialized\n";
        }

        std::cout << "[INFO] OpenGL renderer: "
                  << glGetString(GL_RENDERER)
                  << std::endl;

        std::cout << "[INFO] OpenGL from glad "
                  << GLVersion.major << "." << GLVersion.minor
                  << std::endl;

        int actualWindowWidth, actualWindowHeight;
        glfwGetWindowSize(window_, &actualWindowWidth, &actualWindowHeight);
        std::cout << "[INFO] Window size: " << actualWindowWidth << "x" << actualWindowHeight << std::endl;
        glViewport(0, 0, actualWindowWidth, actualWindowHeight);

#if defined(IMGUI_AND_GLFW_ENABLED)
        //IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window_, true);
        ImGui_ImplOpenGL3_Init(glsl_version.c_str());
        auto &style = ImGui::GetStyle();
        style.ScaleAllSizes(factor);
#endif

    }

    graphic_system::~graphic_system() noexcept {
        teardown(window_);
    }
}

