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
using st_tile_size = st::type<float, struct tile_size_tag>;

inline constexpr float RAD2DEG = 57.295779513082320876798154814105f;
inline constexpr std::size_t map_width = 24ull;
inline constexpr std::size_t map_height = 24ull;
struct wolf_constants
{
    const std::size_t tex_width{256};
    const std::size_t tex_height{256};
    const float darkness_distance{8.0f};
    const float fov_degrees{103.0f};
    const float fov{fov_degrees / 100.0f};
    const float mouse_sensitivity{0.00125};
    const float minimap_zoom{0.5f};
    const float movement_speed{3.5f};
    const math::vec2i wall_texture_indexes[6] = {
            math::vec2i{0, 0}, // 0
            math::vec2i{2, 0}, // 1
            math::vec2i{3, 2}, // 2
            math::vec2i{0, 2}, // 3
            math::vec2i{1, 0}, // 4
            math::vec2i{1, 1}, // 5
    };

    const int world_map[map_width][map_height] = {
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
    inline constexpr math::vec2f UNIT_UP(0.f, -1.f);
    inline constexpr math::vec2f UNIT_UP_RIGHT(1.f, -1.f);
    inline constexpr math::vec2f UNIT_UP_LEFT(-1.f, -1.f);
    inline constexpr math::vec2f UNIT_DOWN(0.f, 1.f);
    inline constexpr math::vec2f UNIT_DOWN_RIGHT(1.f, 1.f);
    inline constexpr math::vec2f UNIT_DOWN_LEFT(-1.f, 1.f);
    inline constexpr math::vec2f UNIT_RIGHT(1.f, 0.f);
    inline constexpr math::vec2f UNIT_LEFT(-1.f, 0.f);

    math::vec2f angle_to_vec(const float &degree)
    {
        if (degree == 180) return UNIT_DOWN;
        if (degree == 90) return UNIT_RIGHT;
        if (degree == 270) return UNIT_LEFT;
        if (degree == 0 || degree == 360) return UNIT_UP;
        if (degree == 45) return UNIT_UP_RIGHT;
        if (degree == 135) return UNIT_DOWN_RIGHT;
        if (degree == 225) return UNIT_DOWN_LEFT;
        if (degree == 315) return UNIT_UP_LEFT;

        const float rad = (-degree + 90.0f) / RAD2DEG;
        return math::vec2f(cosf(rad), -sinf(rad));
    }

    static float vec_to_angle(const math::vec2f &vec)
    {
        if (vec.x() == 0.0f && vec.y() == 0.0f) return 0.0f;
        const float absx = std::abs(vec.x()), absy = std::abs(vec.y());
        const float a = absx > absy ? absy / absx : absx / absy;
        const float s = a * a;
        float r = ((-0.0464964749f * s + 0.15931422f) * s - 0.327622764f) * s * a + a;

        if (absy > absx) r = 1.57079637f - r;
        if (vec.x() < 0) r = 3.14159274f - r;
        if (vec.y() < 0) r = -r;

        float ang = r * RAD2DEG + 90.0f;
        if (ang < 0.0f) ang += 360.0f;
        else if (ang > 360.0f) ang -= 360.0f;
        return ang + 90;
    }

    math::vec2f get_texture_offset(const wolf_constants &constants, const math::vec2i &tex_idx)
    {
        return math::vec2f(float(tex_idx.x() * (constants.tex_width + 2) + 1),
                           float(tex_idx.y() * (constants.tex_height + 2) + 1));
    }

    math::vec2f get_texture_offset(const wolf_constants &constants, const int type)
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

    void set_color(geometry::vertex &v, std::uint8_t value, std::uint8_t alpha)
    {
        set_color(v, value);
        v.pixel_color.a = alpha;
    }

    void set_brightness(geometry::vertex &v, const float distance, const float max_distance,
                        const float brightness_cap = 90.0f)
    {
        const float darkness = std::max(std::min(brightness_cap * distance / max_distance, brightness_cap), 0.0f);
        const float brightness = brightness_cap - darkness;
        set_color(v, std::uint8_t(brightness));
    }
}

class background_system final : public ecs::post_update_system<background_system>
{
public:
    explicit background_system(entt::registry &registry, entt::entity player) noexcept : system(registry),
                                                                                         player_entity_(player)
    {
        entity_registry_.assign<graphics::layer_0>(floor_);
        entity_registry_.assign<graphics::layer_0>(sky_);
    }

    void update() noexcept final
    {
        if (not entity_registry_.valid(player_entity_)) return;
        const float bobbing_y_offset = entity_registry_.get<st_bobbing>(player_entity_).value();
        auto &canvas = entity_registry_.ctx<graphics::canvas_2d>();
        const auto[w, h] = canvas.canvas.size;

        prepare_sky(bobbing_y_offset, w, h);
        prepare_floor(bobbing_y_offset, w, h);
    }

private:
    void prepare_floor(const float bobbing_y_offset, const float w, const float h) const noexcept
    {
        std::vector<geometry::vertex> floor_vertices(4);
        floor_vertices[0].pos = {0.0f, bobbing_y_offset + h * 0.5f};
        floor_vertices[1].pos = {w, bobbing_y_offset + h * 0.5f};
        floor_vertices[2].pos = {w, bobbing_y_offset + h * 1.1f};
        floor_vertices[3].pos = {0.0f, bobbing_y_offset + h * 1.1f};
        graphics::color c{27, 24, 24};
        floor_vertices[2].pixel_color = floor_vertices[3].pixel_color = c;
        float decrease = 0.f;
        c.r = c.r * decrease;
        c.g = c.g * decrease;
        c.b = c.b * decrease;
        floor_vertices[0].pixel_color = floor_vertices[1].pixel_color = c;
        entity_registry_.assign_or_replace<geometry::vertex_array>(floor_, floor_vertices, geometry::quads);
    }

    void prepare_sky(const float bobbing_y_offset, const float w, const float h) const noexcept
    {
        std::vector<geometry::vertex> sky_vertices(4);
        sky_vertices[0].pos = {0.0f, bobbing_y_offset};
        sky_vertices[1].pos = {w, bobbing_y_offset};
        sky_vertices[2].pos = {w, bobbing_y_offset + h * 0.51f};
        sky_vertices[3].pos = {0.0f, bobbing_y_offset + h * 0.51f};
        sky_vertices[2].pixel_color = sky_vertices[3].pixel_color = graphics::color{21, 12, 7};
        sky_vertices[0].pixel_color = sky_vertices[1].pixel_color = graphics::color{3, 6, 16};
        entity_registry_.assign_or_replace<geometry::vertex_array>(sky_, sky_vertices, geometry::quads);
    }

private:
    entt::entity floor_{entity_registry_.create()};
    entt::entity sky_{entity_registry_.create()};
    entt::entity player_entity_;
};

REFL_AUTO(type(background_system));

class minimap_system final : public ecs::post_update_system<minimap_system>
{
public:
    minimap_system(entt::registry &registry, entt::entity player_entity) noexcept: system(registry),
                                                                                   player_entity_(player_entity)
    {
        load_minimap_textures();
        math::vec2u compass_inner_shadow_texture_size;
        dispatcher_.trigger<event::fill_image_properties>("compass_inner_shadow.png",
                                                          compass_inner_shadow_texture_size);
        minimap_height_ = compass_inner_shadow_texture_size.y();

        auto &constants = entity_registry_.ctx<wolf_constants>();


        math::vec2u minimap_size{static_cast<unsigned int>(minimap_height_ / constants.minimap_zoom),
                                 static_cast<unsigned int>(minimap_height_ / constants.minimap_zoom)};

        create_minimap_rt_contents(minimap_size);

        entity_registry_.assign<graphics::render_texture_2d>(
                minimap_,
                "minimap_rt",
                minimap_size,
                graphics::drawable_registry{
                        {"0_minimap_tiles", graphics::drawable_info{.entity = minimap_tiles_, .dt = graphics::d_vertex_array}},
                        {"1_minimap_fov",   graphics::drawable_info{.entity = minimap_fov_, .dt = graphics::d_vertex_array}},
                        {"2_minimap_arrow", graphics::drawable_info{.entity = compass_arrow_, .dt = graphics::d_sprite}}
                });

        transform::position_2d minimap_position = create_minimap_circle(registry);
        create_compass(registry, minimap_position);
    }

    void update() noexcept final
    {
        if (not entity_registry_.valid(player_entity_)) return;
        const auto &player_dir = entity_registry_.get<st_direction>(player_entity_).value();
        const float tile_size = entity_registry_.get<st_tile_size>(minimap_tiles_).value();
        const auto &constants = entity_registry_.ctx<wolf_constants>();
        const auto &player_pos = entity_registry_.get<transform::position_2d>(player_entity_);
        math::vec2f minimap_player_pos{player_pos.y() * tile_size, player_pos.x() * tile_size};
        const float minimap_player_dir_angle{-vec_to_angle(player_dir)};
        const auto &minimap_rt_size = entity_registry_.get<graphics::render_texture_2d>(minimap_).size;

        update_minimap_rotation(player_dir);
        update_tiles(constants, tile_size);
        update_fov(constants, tile_size, minimap_player_pos, minimap_player_dir_angle, minimap_rt_size);
        update_minimap_arrow(minimap_player_pos, minimap_player_dir_angle);
        update_minimap_circle(constants, minimap_rt_size, tile_size, player_pos);
    }

private:
    transform::position_2d create_minimap_circle(entt::registry &registry) const
    {
        auto &canvas = entity_registry_.ctx<graphics::canvas_2d>();
        auto[_, height] = canvas.canvas.size;
        auto minimap_position = transform::position_2d{10 + minimap_height_,
                                                       height - minimap_height_ * 0.5f - 10};
        geometry::blueprint_circle(minimap_, registry, minimap_height_ * 0.5f,
                                   graphics::fill_color{255, 255, 255, 200},
                                   minimap_position, true);
        entity_registry_.assign<graphics::layer_2>(minimap_);
        return minimap_position;
    }

    void create_minimap_rt_contents(const math::vec2u &minimap_size) noexcept
    {
        const float tile_size = static_cast<float>(minimap_size.y()) / map_width;
        std::vector<geometry::vertex> minimap_tiles_vertices(map_width * map_height * 4);
        minimap_tiles_ = entity_registry_.create();
        entity_registry_.assign<st_tile_size>(minimap_tiles_, st_tile_size{tile_size});
        entity_registry_.assign<geometry::vertex_array>(minimap_tiles_, minimap_tiles_vertices, geometry::quads,
                                                        "csgo.png");

        std::vector<geometry::vertex> minimap_fov_vertices(3);
        minimap_fov_ = entity_registry_.create();
        entity_registry_.assign<geometry::vertex_array>(minimap_fov_, minimap_fov_vertices, geometry::triangles);

        compass_arrow_ = graphics::blueprint_sprite(entity_registry_, graphics::sprite{"compass_arrow.png"},
                                                    math::vec2f{}, graphics::white,
                                                    transform::properties{.scale= math::vec2f{1.0f, 1.5f}});
    }

    void load_minimap_textures() const noexcept
    {
        std::vector<event::loading_settings> settings = {{"compass.png"},
                                                         {"compass_inner_shadow.png"},
                                                         {"compass_ring.png"},
                                                         {"compass_arrow.png"}};
        dispatcher_.trigger<event::load_textures>(settings);
    }

    void create_compass(entt::registry &registry, const transform::position_2d &minimap_position) noexcept
    {
        auto compass_inner_shadow = graphics::blueprint_sprite(registry, graphics::sprite{"compass_inner_shadow.png"},
                                                               minimap_position);
        entity_registry_.assign<graphics::layer_3>(compass_inner_shadow);

        compass_ = graphics::blueprint_sprite(registry, graphics::sprite{"compass.png"}, minimap_position);
        entity_registry_.assign<graphics::layer_4>(compass_);

        auto compass_ring = graphics::blueprint_sprite(registry, graphics::sprite{"compass_ring.png"},
                                                       minimap_position);
        entity_registry_.assign<graphics::layer_5>(compass_ring);
    }

    void update_minimap_rotation(const math::vec2f &player_dir) const noexcept
    {
        auto &circle_props = entity_registry_.get<transform::properties>(minimap_);
        auto &compass_props = entity_registry_.get<transform::properties>(compass_);
        circle_props.rotation = vec_to_angle(player_dir);
        compass_props.rotation = circle_props.rotation;
        entity_registry_.replace<transform::properties>(minimap_, circle_props);
        entity_registry_.replace<transform::properties>(compass_, circle_props);
    }

    void update_tiles(const wolf_constants &constants, const float tile_size) const noexcept
    {
        auto &vertices = entity_registry_.get<geometry::vertex_array>(minimap_tiles_).vertices;
        for (std::size_t m_x = 0, idx = 0; m_x < map_width; ++m_x) {
            for (std::size_t m_y = 0; m_y < map_height; ++m_y, idx += 4) {
                const int type = constants.world_map[m_y][m_x];

                vertices[idx + 0].pos = {m_x * tile_size, m_y * tile_size};
                vertices[idx + 1].pos = {(m_x + 1) * tile_size, m_y * tile_size};
                vertices[idx + 2].pos = {(m_x + 1) * tile_size, (m_y + 1) * tile_size};
                vertices[idx + 3].pos = {m_x * tile_size, (m_y + 1) * tile_size};

                // Texture
                const auto offset = get_texture_offset(constants, type);
                vertices[idx + 0].texture_pos = {offset.x(), offset.y()};
                vertices[idx + 1].texture_pos = {offset.x() + constants.tex_width, offset.y()};
                vertices[idx + 2].texture_pos = {offset.x() + constants.tex_width, offset.y() + constants.tex_height};
                vertices[idx + 3].texture_pos = {offset.x(), offset.y() + constants.tex_height};

                const float darkness = 150;
                std::uint8_t color = type == 0 ? std::uint8_t(darkness) : 255;
                for (int i = 0; i < 4; ++i) set_color(vertices[idx + i], color);
            }
        }
        this->entity_registry_.replace<geometry::vertex_array>(minimap_tiles_, vertices, geometry::quads, "csgo.png");
    }

    void update_fov(const wolf_constants &constants, const float tile_size, const math::vec2f &minimap_player_pos,
                    const float minimap_player_dir_angle, const math::vec2u &minimap_rt_size) const noexcept
    {
        auto &fov_vertices = entity_registry_.get<geometry::vertex_array>(minimap_fov_).vertices;

        // Angles
        const float fov_arm_dist = tile_size * constants.darkness_distance * (minimap_height_ / minimap_rt_size.x()) /
                                   constants.minimap_zoom;
        const math::vec2f left_fov_vec =
                angle_to_vec(minimap_player_dir_angle - constants.fov_degrees * 0.5f) * fov_arm_dist;
        const math::vec2f right_fov_vec =
                angle_to_vec(minimap_player_dir_angle + constants.fov_degrees * 0.5f) * fov_arm_dist;

        // Positions
        fov_vertices[0].pos = minimap_player_pos;
        fov_vertices[1].pos = fov_vertices[0].pos + left_fov_vec;
        fov_vertices[2].pos = fov_vertices[0].pos + right_fov_vec;

        // Character point is visible
        const unsigned char color = 255;
        set_color(fov_vertices[0], color, 60);

        // Then it goes invisible towards the end
        for (int i = 1; i <= 2; ++i) set_color(fov_vertices[i], color, 0);

        this->entity_registry_.replace<geometry::vertex_array>(minimap_fov_, fov_vertices, geometry::triangles);
    }

    void
    update_minimap_arrow(const math::vec2f &minimap_player_pos, const float minimap_player_dir_angle) const noexcept
    {
        auto &arrow_props = entity_registry_.get<transform::properties>(compass_arrow_);
        arrow_props.rotation = minimap_player_dir_angle;
        entity_registry_.replace<transform::properties>(compass_arrow_, arrow_props);
        entity_registry_.replace<transform::position_2d>(compass_arrow_, minimap_player_pos);
    }

    void update_minimap_circle(const wolf_constants &constants, const math::vec2u &minimap_rt_size,
                               const float tile_size, const math::vec2f &player_pos) const noexcept
    {

        const math::vec2f minimap_pos_offset = -(math::vec2f{float(map_width), float(map_height)} * 0.5f +
                                                 math::vec2f{-player_pos.y(), -player_pos.x()}) * tile_size;

        const math::vec2i minimap_rect_size(int(constants.minimap_zoom * minimap_rt_size.x()),
                                            int(constants.minimap_zoom * minimap_rt_size.y()));

        auto left = std::min(
                std::max(minimap_pos_offset.x() + (1 - constants.minimap_zoom) * 0.5f * minimap_rt_size.x(), 0.0f),
                float(minimap_rt_size.x() - minimap_rect_size.x()));
        auto right = std::min(
                std::max(minimap_pos_offset.y() + (1 - constants.minimap_zoom) * 0.5f * minimap_rt_size.y(), 0.0f),
                float(minimap_rt_size.y() - minimap_rect_size.y()));
        auto[width, height] = minimap_rect_size.to<math::vec2f>();
        graphics::rect rect{.pos = {left, right}, .size = {width, height}};
        auto &circle = entity_registry_.get<geometry::circle>(minimap_);
        circle.circle_texture_props = geometry::circle_texture{.native_size = false, .texture_rec = rect};
        entity_registry_.replace<geometry::circle>(minimap_, circle);
    }

private:
    entt::entity player_entity_;
    entt::entity compass_{entt::null};
    entt::entity compass_arrow_{entt::null};
    entt::entity minimap_tiles_{entt::null};
    entt::entity minimap_fov_{entt::null};
    entt::entity minimap_{entity_registry_.create()};
    float minimap_height_;
};

REFL_AUTO(type(minimap_system));

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
        const auto &plane = entity_registry_.get<st_plane>(player_entity_).value();
        const auto &pos = entity_registry_.get<transform::position_2d>(player_entity_);
        auto &dir = entity_registry_.get<st_direction>(player_entity_).value();

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
        auto &pos = entity_registry_.get<transform::position_2d>(player_entity_);
        const float bobbing_y_offset = entity_registry_.get<st_bobbing>(player_entity_).value();
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
        auto &pos = entity_registry_.get<transform::position_2d>(player_entity_);
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
    explicit raycast_system(entt::registry &registry, entt::entity player) noexcept : system(registry),
                                                                                      player_entity_(player)
    {
        entity_registry_.assign<graphics::layer_1>(wall_entity);
    }

    void update() noexcept final
    {
        if (not entity_registry_.valid(player_entity_)) return;
        perform_raycast();
    }

private:
    entt::entity wall_entity{entity_registry_.create()};
    entt::entity player_entity_;
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
        dispatcher_.sink<event::mouse_moved>().connect<&player_system::on_mouse_moved>(*this);
        dispatcher_.trigger<event::get_mouse_position>(mouse_prev_pos, true);
    }

    void on_mouse_moved(const event::mouse_moved &evt) noexcept
    {
        const auto &constants = entity_registry_.ctx<wolf_constants>();
        math::vec2i curr{math::vec2f{evt.x, evt.y}.to<math::vec2i>()};
        move_player_camera(constants.mouse_sensitivity * (float(curr.x() - mouse_prev_pos.x())), 1);
        mouse_prev_pos = curr;
    }

    void update() noexcept final
    {
        const auto[width, height] = entity_registry_.ctx<graphics::canvas_2d>().canvas.size;
        mouse_prev_pos = math::vec2i{int(width / 2), int(height / 2)};
        dispatcher_.trigger<event::set_mouse_position>(mouse_prev_pos, true);
        const float dt = timer::time_step::get_fixed_delta_time();
        total_timer_ += dt;

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
        bobbing_y_offset = height * (moving ? 0.016 : 0.008) * std::sin(15.0f * walking_timer_ + 2.0f * total_timer_);
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
    math::vec2i mouse_prev_pos{};
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
        //! Create player system
        auto &p_system = system_manager_.create_system<player_system>();

        //! Load Background system (Sky, Ground)
        system_manager.create_system<background_system>(p_system.get_player());

        //! Load Raycast system
        system_manager.create_system<raycast_system>(p_system.get_player());

        //! Load minimap system
        system_manager.create_system<minimap_system>(p_system.get_player());
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
        //! Load the resources system
        entity_registry_.set<sfml::resources_system>(entity_registry_);

        //! Set wolf constants
        entity_registry_.set<wolf_constants>();

        //! Load the graphical system
        auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

        //! Load the input system with the window from the graphical system
        system_manager_.create_system<sfml::input_system>(graphic_system.get_window());

        //! Create virtual input system
        system_manager_.create_system<ecs::virtual_input_system>();

        //! Create virtual inputs
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