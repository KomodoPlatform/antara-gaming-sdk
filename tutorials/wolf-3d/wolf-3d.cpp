#include <cmath>
#include <antara/gaming/scenes/scene.manager.hpp>
#include <antara/gaming/sfml/graphic.system.hpp>
#include <antara/gaming/sfml/input.system.hpp>
#include <antara/gaming/sfml/resources.manager.hpp>
#include <antara/gaming/graphics/component.layer.hpp>
#include <antara/gaming/world/world.app.hpp>
#include <antara/gaming/ecs/virtual.input.system.hpp>
#include <iostream>
#include <SFML/Window/Keyboard.hpp>

// For convenience
using namespace antara::gaming;
using namespace std::string_literals;

using st_direction = st::type<math::vec2f, struct st_direction_tag>;
using st_bobbing = st::type<float, struct bobbin_tag>;
using st_plane = st::type<math::vec2f, struct plane_tag>;

struct wolf_constants
{
    const std::size_t tex_width{256};
    const std::size_t tex_height{256};
    const float darkness_distance{8.0f};
    const float fov_degrees{103.0f};
    const float fov{fov_degrees / 100.0f};
    const float mouse_sensitivity{0.00125};
    const float minimap_zoom{0.5f};
    const float movement_speed{5.0f};
    const float max_brightness{90.0f};
    const math::vec2i wall_texture_indexes[6] = {
            math::vec2i{0, 0}, // 0
            math::vec2i{2, 0}, // 1
            math::vec2i{3, 2}, // 2
            math::vec2i{0, 2}, // 3
            math::vec2i{1, 0}, // 4
            math::vec2i{1, 1}, // 5
    };

    const int world_map[24][24] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 4, 0, 0, 3, 0, 0, 0, 5, 0, 0, 0, 1, 1, 1},
            {1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 4, 0, 0, 3, 0, 0, 0, 5, 0, 0, 0, 1, 1, 1},
            {1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 5, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 2, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 5, 0, 3, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 5, 5, 5, 2, 0, 0, 0, 2, 5, 5, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 5, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 5, 0, 5, 3, 0, 3, 0, 3, 0, 0, 0, 1},
            {1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 0, 0, 5, 0, 0, 0, 0, 3, 0, 0, 0, 1},
            {1, 0, 0, 0, 3, 0, 0, 2, 2, 2, 3, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 2, 0, 5, 4, 4, 4, 0, 0, 5, 0, 5, 1},
            {1, 0, 0, 2, 2, 2, 2, 0, 0, 0, 3, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 2, 0, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 5, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 0, 5, 0, 0, 1},
            {1, 4, 4, 4, 0, 4, 4, 0, 4, 0, 0, 0, 2, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 1},
            {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 2, 5, 5, 0, 4, 0, 3, 0, 0, 0, 0, 1},
            {1, 4, 0, 0, 0, 0, 5, 0, 4, 2, 2, 2, 2, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 1},
            {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 3, 0, 1, 1},
            {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 1, 1},
            {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 3, 0, 1, 1},
            {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
};

namespace
{
    math::vec2f get_texture_offset(wolf_constants &constants, const math::vec2i &tex_idx)
    {
        return math::vec2f(float(tex_idx.x() * (constants.tex_width + 2) + 1),
                           float(tex_idx.y() * (constants.tex_height + 2) + 1));
    }

    math::vec2f get_texture_offset(wolf_constants &constants, const int type)
    {
        return get_texture_offset(constants, constants.wall_texture_indexes[type]);
    }

    float magnitude(const math::vec2f &a)
    {
        return std::sqrt(a.x() * a.x() + a.y() * a.y());
    }

    void set_color(geometry::vertex &v, std::uint8_t value)
    {
        v.pixel_color.r = v.pixel_color.g = v.pixel_color.b = value;
    }

    /*void set_color(geometry::vertex &v, std::uint8_t value, std::uint8_t alpha)
    {
        set_color(v, value);
        v.pixel_color.a = alpha;
    }*/

    void set_brightness(geometry::vertex &v, const float distance, const float max_distance,
                        const float brightness_cap = 90.0f)
    {
        const float darkness = std::max(std::min(brightness_cap * distance / max_distance, brightness_cap), 0.0f);
        const float brightness = brightness_cap - darkness;
        set_color(v, std::uint8_t(brightness));
    }
}

class raycast_system final : public ecs::post_update_system<raycast_system>
{
private:
    void perform_raycast()
    {
        auto &canvas = entity_registry_.ctx<graphics::canvas_2d>();
        auto &constants = entity_registry_.ctx<wolf_constants>();
        auto size = canvas.canvas.size.to<math::vec2i>();
        const auto width = size.x();
        const auto height = size.y();
        const auto &plane = entity_registry_.get<st_plane>(player_entity).value();
        const auto &pos = entity_registry_.get<transform::position_2d>(player_entity);
        auto &dir = entity_registry_.get<st_direction>(player_entity).value();

        int idx_vx = 0;
        for (int x = 0; x < width; ++x) {
            //! X-coordinate in camera space
            const float camera_x = 2.0f * float(x) / width - 1;

            //! Ray direction
            const math::vec2f ray_dir{dir.x() + plane.x() * camera_x, dir.y() + plane.y() * camera_x};

            //! Length of ray from one x or y-side to next x or y-side
            const math::vec2f delta_dist{std::abs(1 / ray_dir.x()), std::abs(1 / ray_dir.y())};

            //! Calculate step and initial sideDist

            //! Length of ray from current position to next x or y-side
            math::vec2f side_dist;

            //! What direction to step in x or y-direction (either +1 or -1)
            math::vec2i step;
            //! Which box of the map we're in
            auto map_pos = pos.to<math::vec2i>();

            //! Shot a ray
            shot_ray(ray_dir, delta_dist, side_dist, step, map_pos);

            //! Perform DDA algorithm
            int side = perform_dda(constants, delta_dist, side_dist, step, map_pos);

            //! Calculate distance projected on camera direction (Euclidean distance will give fish-eye effect!)
            const float perp_wall_dist{
                    (side == 0) ? std::fabs((map_pos.x() - pos.x() + (1.f - step.x()) / 2) / ray_dir.x())
                                : std::fabs((map_pos.y() - pos.y() + (1.f - step.y()) / 2) / ray_dir.y())};

            //! Prepare current wall into the vertices
            prepare_wall(constants, height, x, idx_vx, ray_dir, map_pos, side, perp_wall_dist);
            idx_vx += 2;
        }
        entity_registry_.assign_or_replace<geometry::vertex_array>(wall_entity, wall_lines, geometry::lines,
                                                                   "csgo.png");
    }

    std::tuple<float, float>
    prepare_wall(wolf_constants &constants, int height, int x, int idx_vx, const math::vec2f &ray_dir,
                 const math::vec2i &map_pos, int side, float perp_wall_dist)
    {
        auto &pos = entity_registry_.get<transform::position_2d>(player_entity);
        const float bobbing_y_offset = entity_registry_.get<st_bobbing>(player_entity).value();
        // Calculate height of line to draw on screen
        const float line_height = std::abs(float(height) / perp_wall_dist);

        const float draw_start = bobbing_y_offset - line_height * 0.5f + float(height) * 0.5f;
        const float draw_end = bobbing_y_offset + line_height * 0.5f + float(height) * 0.5f;

        // Calculate value of wall_x
        float wall_x; // Where exactly the wall was hit
        if (side == 0) wall_x = pos.y() + perp_wall_dist * ray_dir.y();
        else wall_x = pos.x() + perp_wall_dist * ray_dir.x();
        wall_x -= std::floor((wall_x));

        // X coordinate on the texture
        std::size_t tex_x = wall_x * constants.tex_width;
        if (side == 0 && ray_dir.x() > 0) tex_x = constants.tex_width - tex_x - 1;
        if (side == 1 && ray_dir.y() < 0) tex_x = constants.tex_height - tex_x - 1;

        // Prepare wall line
        {
            auto offset = get_texture_offset(constants, constants.world_map[map_pos.x()][map_pos.y()]);
            wall_lines[idx_vx + 0].texture_pos = math::vec2f(float(tex_x), 0.f) + offset;
            wall_lines[idx_vx + 1].texture_pos = math::vec2f(float(tex_x), float(constants.tex_height)) + offset;
            wall_lines[idx_vx + 0].pos = math::vec2f(float(x), float(draw_start));
            wall_lines[idx_vx + 1].pos = math::vec2f(float(x), float(draw_end));

            const float distance = magnitude(math::vec2f(map_pos.x() - pos.x() + (side == 1 ? wall_x : 0),
                                                         map_pos.y() - pos.y() + (side == 0 ? wall_x : 0)));
            set_brightness(wall_lines[idx_vx + 0], distance, constants.darkness_distance);
            set_brightness(wall_lines[idx_vx + 1], distance, constants.darkness_distance);
        }
        return std::make_tuple(wall_x, draw_end); //! in case someone want to code a floor one day he will need it.
    }

    int perform_dda(const wolf_constants &constants,
                    const math::vec2f &delta_dist,
                    math::vec2f &side_dist,
                    const math::vec2i &step,
                    math::vec2i &map_pos) const
    {
        auto &world_map = constants.world_map;
        // Perform DDA
        int side{0}; // Was a NS or a EW wall hit?
        int hit{0}; // Was there a wall hit?

        while (hit == 0) {
            // Jump to next map square, OR in X-direction, OR in Y-direction
            if (side_dist.x() < side_dist.y()) {
                side_dist.x_ref() += delta_dist.x();
                map_pos.x_ref() += step.x();
                side = 0;
            } else {
                side_dist.y_ref() += delta_dist.y();
                map_pos.y_ref() += step.y();
                side = 1;
            }

            // Check if ray has hit a wall
            if (world_map[map_pos.x()][map_pos.y()] > 0) hit = 1;
        }
        return side;
    }

    void shot_ray(const math::vec2f &ray_dir, const math::vec2f &delta_dist, math::vec2f &side_dist, math::vec2i &step,
                  const math::vec2i &map_pos) const
    {
        auto &pos = entity_registry_.get<transform::position_2d>(player_entity);
        // X-direction
        if (ray_dir.x() < 0) {
            step.x_ref() = -1;
            side_dist.x_ref() = (pos.x() - map_pos.x()) * delta_dist.x();
        } else {
            step.x_ref() = 1;
            side_dist.x_ref() = (map_pos.x() + 1.0f - pos.x()) * delta_dist.x();
        }

        // Y-direction
        if (ray_dir.y() < 0) {
            step.y_ref() = -1;
            side_dist.y_ref() = (pos.y() - map_pos.y()) * delta_dist.y();
        } else {
            step.y_ref() = 1;
            side_dist.y_ref() = (map_pos.y() + 1.0f - pos.y()) * delta_dist.y();
        }
    }

public:
    explicit raycast_system(entt::registry &registry) noexcept : system(registry)
    {
        entity_registry_.assign<graphics::layer_1>(wall_entity);
        disable();
    }

    void set_player(entt::entity entity) noexcept
    {
        player_entity = entity;
    }

    void update() noexcept final
    {
        if (not entity_registry_.valid(player_entity)) return;
        perform_raycast();
    }

private:
    // Variables
    //math::vec2f plane{0.f, entity_registry_.ctx<wolf_constants>().fov};

    entt::entity wall_entity{entity_registry_.create()};
    entt::entity player_entity{entt::null};
    std::vector<geometry::vertex> wall_lines{static_cast<std::vector<geometry::vertex>::size_type>(
                                                     entity_registry_.ctx<graphics::canvas_2d>().canvas.size.to<math::vec2i>().x() *
                                                     2)};
};

REFL_AUTO(type(raycast_system));

class player_system final : public ecs::logic_update_system<player_system>
{
public:
    player_system(entt::registry &registry) noexcept : system(registry)
    {
        entity_registry_.assign<transform::position_2d>(player_, 22.f, 12.f);
        entity_registry_.assign<st_direction>(player_, st_direction{{-1.f, 0.f}});
        entity_registry_.assign<st_plane>(player_, st_plane{{0.f, entity_registry_.ctx<wolf_constants>().fov}});
        entity_registry_.assign<st_bobbing>(player_, 0.f);
        this->dispatcher_.sink<event::mouse_moved>().connect<&player_system::on_mouse_moved>(*this);
    }

    void on_mouse_moved(const event::mouse_moved &evt) noexcept
    {
        const auto &constants = entity_registry_.ctx<wolf_constants>();
        math::vec2i curr{math::vec2f{evt.x, evt.y}.to<math::vec2i>()};
        //move_player_camera(constants.mouse_sensitivity)
    }

    void update() noexcept final
    {
        const float dt = timer::time_step::get_fixed_delta_time();
        total_timer_ += dt;
        const auto[_, height] = entity_registry_.ctx<graphics::canvas_2d>().canvas.size;
        auto &constants = entity_registry_.ctx<wolf_constants>();

        bool up = input::virtual_input::is_held("move_forward");
        bool down = input::virtual_input::is_held("move_down");
        bool right = input::virtual_input::is_held("move_right");
        bool left = input::virtual_input::is_held("move_left");

        math::vec2f input_dir{float(right - left), float(up - down)};

        bobbing(dt, height, input_dir);
        move_player(dt, constants, input_dir);
    }


private:
    void move_player_camera(const float amount, const float dt)
    {
        auto &plane = entity_registry_.get<st_plane>(player_).value();
        auto &dir_player = entity_registry_.get<st_direction>(player_).value();
        const float rot_speed = amount * dt; // Constant value is in radians/second

        // Both camera direction and camera plane must be rotated
        const float old_dir_x = dir_player.x();
        dir_player.x_ref() = dir_player.x() * std::cos(-rot_speed) - dir_player.y() * std::sin(-rot_speed);
        dir_player.y_ref() = old_dir_x * std::sin(-rot_speed) + dir_player.y() * std::cos(-rot_speed);
        const float old_plane_x = plane.x();
        plane.x_ref() = plane.x() * std::cos(-rot_speed) - plane.y() * std::sin(-rot_speed);
        plane.y_ref() = old_plane_x * std::sin(-rot_speed) + plane.y() * std::cos(-rot_speed);
    };

    void bobbing(const float dt, const float height, const math::vec2f &input_dir) noexcept
    {
        auto &bobbing_y_offset = entity_registry_.get<st_bobbing>(player_).value();
        bool moving = input_dir.x() != 0 || input_dir.y() != 0;
        if (moving) walking_timer_ += dt;
        bobbing_y_offset = height * (moving ? 0.008 : 0.004) * std::sin(14.0f * walking_timer_ + 2.0f * total_timer_);
    }

    void move_player(const float dt, const wolf_constants &constants, const math::vec2f &input_dir) const noexcept
    {
        const auto &dir_player = entity_registry_.get<st_direction>(player_).value();
        auto &player_pos = entity_registry_.get<transform::position_2d>(player_);
        float move_speed = dt * constants.movement_speed;
        if (input_dir.x() != 0.f && input_dir.y() != 0.f) {
            move_speed /= std::sqrt(2);
        }

        auto move = [&player_pos, &constants](const math::vec2f vec) {
            static const float dist_multiplier = 12.0f;
            auto curr_pos(player_pos.to<math::vec2i>());
            math::vec2i future_pos(int(player_pos.x() + dist_multiplier * vec.x()),
                                   int(player_pos.y() + dist_multiplier * vec.y()));
            if (constants.world_map[future_pos.x()][curr_pos.y()] == 0) player_pos.x_ref() += vec.x();
            if (constants.world_map[curr_pos.x()][future_pos.y()] == 0) player_pos.y_ref() += vec.y();
        };

        if (input_dir.y() != 0) {
            auto dir = math::vec2f(dir_player.x(), dir_player.y()) * input_dir.y();
            move(dir * move_speed);
        }

        // Strafe left or right
        if (input_dir.x() != 0) {
            auto dir = math::vec2f{dir_player.y(), -dir_player.x()} * input_dir.x();
            move(dir * move_speed);
        }
    }

public:
    [[nodiscard]] entt::entity get_player() const noexcept
    {
        return player_;
    }

private:
    entt::entity player_{entity_registry_.create()};
    float walking_timer_{0.f};
    float total_timer_{0.f};
    //math::vec2i mouse_prev_pos{};
};

REFL_AUTO(type(player_system));

// Game Scene
class game_scene final : public scenes::base_scene
{
public:
    explicit game_scene(entt::registry &registry, ecs::system_manager &system_manager) noexcept : base_scene(registry),
                                                                                                  system_manager_(
                                                                                                          system_manager)
    {
        auto &p_system = system_manager_.create_system<player_system>();
        system_manager_.get_system<raycast_system>().set_player(p_system.get_player());
        system_manager_.enable_system<raycast_system>();
    }

    // Scene name
    std::string scene_name() noexcept final
    {
        return "game_scene";
    }

    // Update the game every tick
    void update() noexcept final
    {
    }

private:
    ecs::system_manager &system_manager_;
};

// Game world
struct wolf3d_world : world::app
{
    // Game entry point
    wolf3d_world() noexcept
    {
        //! Set wolf constants
        entity_registry_.set<wolf_constants>();

        //! Load ray cast system
        system_manager_.create_system<raycast_system>();

        //! Load the graphical system
        auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

        //! Load the resources system
        entity_registry_.set<sfml::resources_system>(entity_registry_);

        //! Load the input system with the window from the graphical system
        system_manager_.create_system<sfml::input_system>(graphic_system.get_window());

        // Create virtual input system
        system_manager_.create_system<ecs::virtual_input_system>();

        // Create virtual input system
        input::virtual_input::create("move_forward", {input::key::z, input::key::up}, {});
        input::virtual_input::create("move_left", {input::key::q, input::key::left}, {});
        input::virtual_input::create("move_down", {input::key::s, input::key::down}, {});
        input::virtual_input::create("move_right", {input::key::d, input::key::right}, {});


        //! Load the scenes manager
        auto &scene_manager = system_manager_.create_system<scenes::manager>();

        //! Change the current_scene to "game_scene" by pushing it.
        scene_manager.change_scene(std::make_unique<game_scene>(entity_registry_, system_manager_), true);
    }
};

int main()
{
    // Declare the world
    wolf3d_world game;

    // Run the game
    return game.run();
}