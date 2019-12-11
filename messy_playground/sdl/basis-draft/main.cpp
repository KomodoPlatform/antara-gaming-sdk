#include <imgui.h>
#include <antara/gaming/world/world.app.hpp>
#include <antara/gaming/sdl/graphic.system.hpp>
#include <antara/gaming/sdl/input.system.hpp>
#include <antara/gaming/sdl/sdl.opengl.image.loading.hpp>
#include <antara/gaming/core/real.path.hpp>
#include <filesystem>

class my_gui_system final : public antara::gaming::ecs::post_update_system<my_gui_system> {
public:
    my_gui_system(entt::registry &registry) : system(registry) {
        std::filesystem::path p = antara::gaming::core::assets_real_path() / "textures/kmd.png";

        bool res = antara::gaming::sdl::load_image(p, img_);
        assert(res);
    }

    void update() noexcept final {
        static bool res = true;
        ImGui::Image((void*)(intptr_t)(img_.id), ImVec2{static_cast<float>(img_.width), static_cast<float>(img_.height)});
        ImGui::ShowDemoWindow(&res);
    }
private:
    antara::gaming::sdl::opengl_image img_;
};

REFL_AUTO(type(my_gui_system));

class my_world : public antara::gaming::world::app {
public:
    my_world() noexcept {
        auto &graphic_system = this->system_manager_.create_system<antara::gaming::sdl::graphic_system>();
        graphic_system.set_framerate_limit(60);
        system_manager_.create_system<antara::gaming::sdl::input_system>(graphic_system.get_window());
        system_manager_.create_system<my_gui_system>();
        system_manager_.prioritize_system<my_gui_system, antara::gaming::sdl::graphic_system>();
    }
};

int main(int argc, char* argv[])
{
    //void(argc);
    //void(argv);
    my_world app;
    return app.run();
}