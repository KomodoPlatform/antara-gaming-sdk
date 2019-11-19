#include <antara/gaming/scenes/scene.manager.hpp>
#include <antara/gaming/sfml/graphic.system.hpp>
#include <antara/gaming/sfml/input.system.hpp>
#include <antara/gaming/sfml/resources.manager.hpp>
#include <antara/gaming/graphics/component.layer.hpp>
#include <antara/gaming/world/world.app.hpp>

// For convenience
using namespace antara::gaming;
using namespace std::string_literals;

struct wolf_constants {
    const std::size_t tex_width{256};
    const std::size_t tex_height{256};
    const float darkness_distance{8.0f};
    const float fov_degrees{103.0f};
    const float fov{fov_degrees / 100.0f};
    const float mouse_sensitivity{0.00125};
    const float minimap_zoom{0.5f};
    const float movement_speed{5.0f};
    const float max_brightness{90.0f};
    const math::vec2i floor_texture_index{0, 0};
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
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 1},
            {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
};

namespace {
    math::vec2f get_texture_offset(wolf_constants &constants, const math::vec2i &tex_idx) {
        return math::vec2f(float(tex_idx.x() * (constants.tex_width + 2) + 1),
                           float(tex_idx.y() * (constants.tex_height + 2) + 1));
    }

    math::vec2f get_texture_offset(wolf_constants &constants, const int type) {
        return get_texture_offset(constants, constants.wall_texture_indexes[type]);
    }
}

class raycast_system final : public ecs::post_update_system<raycast_system> {
public:
    explicit raycast_system(entt::registry &registry) noexcept : system(registry) {
        this->entity_registry_.assign<graphics::layer_0>(wall_entity);
    }

    void update() noexcept final {
        auto &canvas = this->entity_registry_.ctx<graphics::canvas_2d>();
        auto &constants = this->entity_registry_.ctx<wolf_constants>();
        auto &worldMap = constants.world_map;
        auto[width, height] = canvas.canvas.size.to<math::vec2i>();
        for (int x = 0, idx_vx = 0; x < width; ++x, idx_vx += 2) {
            const float cameraX = 2.0f * x / width - 1; // X-coordinate in camera space

            const float rayDirX = dirX + planeX * cameraX;
            const float rayDirY = dirY + planeY * cameraX;

            // Length of ray from one x or y-side to next x or y-side
            const float deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
            const float deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

            // Calculate step and initial sideDist
            // Length of ray from current position to next x or y-side
            float sideDistX;
            float sideDistY;

            // What direction to step in x or y-direction (either +1 or -1)
            int stepX;
            int stepY;

            // Which box of the map we're in
            int mapX = posX;
            int mapY = posY;

            // X-direction
            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (posX - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0f - posX) * deltaDistX;
            }

            // Y-direction
            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (posY - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0f - posY) * deltaDistY;
            }

            // Perform DDA
            int side{0}; // Was a NS or a EW wall hit?
            int hit = 0; // Was there a wall hit?
            float perpWallDist;

            while (hit == 0) {
                // Jump to next map square, OR in X-direction, OR in Y-direction
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }

                // Check if ray has hit a wall
                if (worldMap[mapX][mapY] > 0) hit = 1;
            }

            // Calculate distance projected on camera direction (Euclidean distance will give fish-eye effect!)
            if (side == 0) {
                perpWallDist = std::fabs((mapX - posX + (1 - stepX) / 2) / rayDirX);
            } else {
                perpWallDist = std::fabs((mapY - posY + (1 - stepY) / 2) / rayDirY);
            }

            // Calculate height of line to draw on screen
            const float lineHeight = std::abs(height / perpWallDist);

            const float drawStart = bobbing_y_offset - lineHeight * 0.5f + height * 0.5f;
            const float drawEnd = bobbing_y_offset + lineHeight * 0.5f + height * 0.5f;

            // Calculate value of wallX
            float wallX; // Where exactly the wall was hit
            if (side == 0) wallX = posY + perpWallDist * rayDirY;
            else wallX = posX + perpWallDist * rayDirX;
            wallX -= floor((wallX));

            // X coordinate on the texture
            std::size_t texX = wallX * constants.tex_width;
            if (side == 0 && rayDirX > 0) texX = constants.tex_width - texX - 1;
            if (side == 1 && rayDirY < 0) texX = constants.tex_height - texX - 1;

            // Prepare wall line
            {
                auto offset = get_texture_offset(constants, worldMap[mapX][mapY]);
                wall_lines[idx_vx + 0].texture_pos = math::vec2f(float(texX), 0.f) + offset;
                wall_lines[idx_vx + 1].texture_pos = math::vec2f(float(texX), float(constants.tex_height)) + offset;
                wall_lines[idx_vx + 0].pos = math::vec2f(float(x), float(drawStart));
                wall_lines[idx_vx + 1].pos = math::vec2f(float(x), float(drawEnd));
            }
        }
        this->entity_registry_.assign_or_replace<geometry::vertex_array>(wall_entity, wall_lines, geometry::lines,
                                                                         "csgo.png");
    }

private:
    // Variables
    float posX{22};
    float posY{12};  // X and Y start position
    float dirX{-1};
    float dirY{0}; // Initial direction vector
    float planeX{0};
    float planeY{this->entity_registry_.ctx<wolf_constants>().fov};

    //TODO: move it elsewhere
    float bobbing_y_offset{0.f};
    entt::entity wall_entity{this->entity_registry_.create()};

    std::vector<geometry::vertex> wall_lines{static_cast<std::vector<geometry::vertex>::size_type>(
                                                     this->entity_registry_.ctx<graphics::canvas_2d>().canvas.size.to<math::vec2i>().x() *
                                                     2)};
};

REFL_AUTO(type(raycast_system));

// Game Scene
class game_scene final : public scenes::base_scene {
public:
    explicit game_scene(entt::registry &registry) noexcept : base_scene(registry) {
    }

    // Scene name
    std::string scene_name() noexcept final {
        return "game_scene";
    }

private:
    // Update the game every tick
    void update() noexcept final {
    }
};

// Game world
struct wolf3d_world : world::app {
    // Game entry point
    wolf3d_world() noexcept {
        // Load the graphical system
        entity_registry_.set<wolf_constants>();
        system_manager_.create_system<raycast_system>();
        auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

        // Load the resources system
        entity_registry_.set<sfml::resources_system>(entity_registry_);

        // Load the input system with the window from the graphical system
        system_manager_.create_system<sfml::input_system>(graphic_system.get_window());

        // Load the scenes manager
        auto &scene_manager = system_manager_.create_system<scenes::manager>();

        // Change the current_scene to "game_scene" by pushing it.
        scene_manager.change_scene(std::make_unique<game_scene>(entity_registry_), true);
    }
};

int main() {
    // Declare the world
    wolf3d_world game;

    // Run the game
    return game.run();
}