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

#include <entt/entity/helper.hpp>
#include <antara/gaming/animation2d/animation.2d.hpp>
#include "antara/gaming/world/world.app.hpp"
#include "antara/gaming/transform/component.position.hpp"
#include "antara/gaming/geometry/component.rectangle.hpp"
#include "antara/gaming/graphics/component.layer.hpp"
#include "antara/gaming/graphics/component.sprite.hpp"
//#include "antara/gaming/sfml/component.drawable.hpp"
#include "antara/gaming/sfml/graphic.system.hpp"
#include "antara/gaming/sfml/input.system.hpp"
#include "antara/gaming/sfml/komodo.intro.scene.hpp"
#include "antara/gaming/scenes/scene.manager.hpp"
#include "antara/gaming/scenes/base.scene.hpp"
#include "antara/gaming/sfml/audio.system.hpp"
#include "antara/gaming/sfml/resources.manager.hpp"

class intro_scene;

using namespace antara::gaming;

class game_scene final : public antara::gaming::scenes::base_scene {
public:
    game_scene(entt::registry &entity_registry, animation2d::anim_system &animation2d_system) noexcept : base_scene(
            entity_registry), animation2d_system_(animation2d_system) {
        auto &window_size = entity_registry.ctx<graphics::canvas_2d>().canvas.size;
        auto text_entity = entity_registry.create();
        entity_registry.assign<graphics::fill_color>(text_entity, graphics::green);
        entity_registry.assign<graphics::text>(text_entity, "Game scene", 144ull);
        entity_registry.assign<transform::position_2d>(text_entity, window_size.x() * 0.5f, window_size.y() * 0.5f);
        entity_registry.assign<entt::tag<"game_scene"_hs>>(text_entity);
        entity_registry.assign<graphics::layer<0>>(text_entity);

        auto another_text_entity = graphics::blueprint_text(entity_registry, {"another text"}, transform::position_2d{900.0f, 900.0f});
        entity_registry.assign<entt::tag<"game_scene"_hs>>(another_text_entity);
        entity_registry.assign<graphics::layer<0>>(another_text_entity);

        auto sprite_entity = graphics::blueprint_sprite(entity_registry,
                                                        graphics::sprite{"tileSand1.png"},
                                                        transform::position_2d{400.f, 400.f},
                                                        graphics::white,
                                                        transform::properties{.scale = 2.f, .rotation = 45.f});
        //entity_registry.assign<graphics::sprite>(sprite_entity, "tileSand1.png");
        entity_registry.assign<entt::tag<"game_scene"_hs>>(sprite_entity);
        entity_registry.assign<graphics::layer<0>>(sprite_entity);


        auto rect_entity = geometry::blueprint_rectangle(entity_registry, math::vec2f{120.f, 120.f},
                                                         graphics::magenta, transform::position_2d(200.f, 200.f));
        entity_registry.assign<entt::tag<"game_scene"_hs>>(rect_entity);
        entity_registry.assign<graphics::layer<3>>(rect_entity);

        animation2d_system_.add_animation("mage_idle", "mage_idle_dir_1.png", 1, 1, 9);
        auto animated_entity = animation2d::blueprint_animation(entity_registry,
                                                                animation2d::anim_component{"mage_idle",
                                                                                            animation2d::anim_component::status::playing,
                                                                                            animation2d::anim_component::seconds(
                                                                                                    0.10f),
                                                                                            1, true},
                                                                transform::position_2d(600.f, 300.f));
        entity_registry.assign<entt::tag<"game_scene"_hs>>(animated_entity);
        entity_registry.assign<graphics::layer<4>>(animated_entity);

        animation2d_system.add_animation("bheet_arrival", "bheet_arrival.png", 12, 7, 80);
        antara::gaming::timer::time_step::reset_lag();
        auto animated2_entity = animation2d::blueprint_animation(entity_registry,
                                                                 animation2d::anim_component{"bheet_arrival",
                                                                                             animation2d::anim_component::status::playing,
                                                                                             animation2d::anim_component::seconds(
                                                                                                     0.10f),
                                                                                             1, true},
                                                                 transform::position_2d(900.f, 300.f));
        entity_registry.assign<entt::tag<"game_scene"_hs>>(animated2_entity);
        entity_registry.assign<graphics::layer<5>>(animated2_entity);

        math::vec2i out;
        math::vec2i out_relative;
        this->dispatcher_.trigger<event::fill_mouse_position>(out);
        this->dispatcher_.trigger<event::fill_mouse_position>(out_relative, true);

        DVLOG_F(loguru::Verbosity_INFO, "before out: [{} : {}], out_relative: [{} : {}]", out.x(), out.y(), out_relative.x(), out_relative.y());

        this->dispatcher_.trigger<event::set_mouse_position>(out + 10);
        this->dispatcher_.trigger<event::set_mouse_position>(out_relative + 10, true);

        this->dispatcher_.trigger<event::fill_mouse_position>(out);
        this->dispatcher_.trigger<event::fill_mouse_position>(out_relative, true);

        DVLOG_F(loguru::Verbosity_INFO, "after out: [{} : {}], out_relative: [{} : {}]", out.x(), out.y(), out_relative.x(), out_relative.y());
        //!
        //auto& anim_cmp = entity_registry.get<animation2d::anim_component>(animated2_entity);
        //anim_cmp.elapsed = animation2d::anim_component::seconds(0.f);
        //anim_cmp.current_frame = 0;
        //anim_cmp.current_status = animation2d::anim_component::playing;
    }

    void update() noexcept final {

    }

    bool on_key_pressed(const antara::gaming::event::key_pressed &) noexcept final {
        return false;
    }

    bool on_key_released(const antara::gaming::event::key_released &) noexcept final {
        return false;
    }

    std::string scene_name() noexcept final {
        return "game_scene";
    }

    ~game_scene() noexcept final {
        auto view = entity_registry_.view<entt::tag<"game_scene"_hs>>();
        entity_registry_.destroy(view.begin(), view.end());
    }

private:
    antara::gaming::animation2d::anim_system &animation2d_system_;
    //antara::gaming::sfml::resources_manager resource_mgr;
};

class my_world : public antara::gaming::world::app {
public:
    my_world() noexcept {
        auto &graphic_system = this->system_manager_.create_system<antara::gaming::sfml::graphic_system>();
        this->entity_registry_.set<antara::gaming::sfml::resources_system>(this->entity_registry_);
        auto &anim_system = this->system_manager_.create_system<antara::gaming::animation2d::anim_system>();
        this->system_manager_.create_system<antara::gaming::sfml::audio_system>();
        this->system_manager_.create_system<antara::gaming::sfml::input_system>(graphic_system.get_window());
        auto &scene_manager = this->system_manager_.create_system<antara::gaming::scenes::manager>();
        scene_manager.change_scene(
                std::make_unique<antara::gaming::sfml::intro_scene>(entity_registry_, [this, &anim_system]() {
                    this->dispatcher_.trigger<antara::gaming::event::change_scene>(
                            std::make_unique<game_scene>(this->entity_registry_, anim_system), false);
                }), true);
    }
};

int main() {
    my_world game_app;
    return game_app.run();
}
