#include <imgui.h>
#include <antara/gaming/world/world.app.hpp>
#include <antara/gaming/glfw/graphic.system.hpp>
#include <antara/gaming/glfw/input.system.hpp>

class my_gui_system final : public antara::gaming::ecs::post_update_system<my_gui_system> {
public:
    my_gui_system(entt::registry &registry) : system(registry) {

    }

    void update() noexcept final {
        ImGui::ShowDemoWindow(nullptr);
    }
};

REFL_AUTO(type(my_gui_system));

class my_world : public antara::gaming::world::app {
public:
    my_world() noexcept {
        auto &graphic_system = this->system_manager_.create_system<antara::gaming::glfw::graphic_system>();
        system_manager_.create_system<antara::gaming::glfw::input_system>(graphic_system.get_window());
        system_manager_.create_system<my_gui_system>();
        system_manager_.prioritize_system<my_gui_system, antara::gaming::glfw::graphic_system>();
    }
};

int main() {
    my_world app;
    return app.run();
}