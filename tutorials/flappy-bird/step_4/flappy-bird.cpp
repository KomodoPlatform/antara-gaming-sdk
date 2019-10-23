#include <antara/gaming/world/world.app.hpp>
#include <antara/gaming/sfml/graphic.system.hpp>
#include <antara/gaming/sfml/input.system.hpp>
#include <antara/gaming/scenes/scene.manager.hpp>
#include <antara/gaming/math/vector.hpp>
#include <antara/gaming/graphics/component.canvas.hpp>
#include <antara/gaming/graphics/component.layer.hpp>
#include <random>
#include <iostream>

//! For convenience
using namespace antara::gaming;

namespace {
    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    float random_float(float lower, float higher) {
        std::uniform_real_distribution<float> dist(lower, higher);
        return dist(gen);
    }
}

struct flappy_bird_constants {
    // Pipes
    const float gap_height{200.f};
    const float column_min{0.2f};
    const float column_max{0.8f};
    const float column_thickness{100.f};
    const float column_distance{400.f};
    const std::size_t column_count{6};
    const graphics::color pipe_color{92, 181, 61};
    const graphics::outline_color pipe_outline_color{2.0f, graphics::color{76, 47, 61}};
    const float scroll_speed{200.f};

    // Background
    const float ground_thickness{100.0f};
    const float grass_thickness{20.0f};
    const graphics::color background_color{82, 189, 199};
    const graphics::color ground_color{220, 209, 143};
    const graphics::color grass_color{132, 227, 90};
    const graphics::outline_color grass_outline_color{2.0f, graphics::color{76, 47, 61}};
};

// A Flappy Bird column which has two pipes
struct column {
    //! Entities representing the Flappy Bird pipes
    entt::entity top_pipe{entt::null};
    entt::entity bottom_pipe{entt::null};
};

//! Contains all the function that will be used for logic  and factory
namespace {
    // Factory for pipes, requires to know if it's a top one, position x of the column, and the gap starting position Y
    entt::entity create_pipe(entt::registry &registry, bool is_top, float pos_x, float gap_start_pos_y) {
        // Retrieve constants
        const auto canvas_height = registry.ctx<graphics::canvas_2d>().canvas.size.y();
        const auto constants = registry.ctx<flappy_bird_constants>();

        // Top pipe is at Y: 0 and bottom pipe is at canvas_height, bottom of the canvas
        transform::position_2d pos{pos_x, is_top ? 0.f : canvas_height};

        // Size X is the column thickness,
        // Size Y is the important part.
        // If it's a top pipe, gap_start_pos_y should be bottom of the rectangle
        //  So half size should be gap_start_pos_y since center of the rectangle is at 0.
        // If it's the bottom pipe, top of the rectangle will be at gap_start_pos_y + gap_height
        //  So half size should be canvas_height - (gap_start_pos_y + gap_height)
        // Since these are half-sizes, and the position is at the screen border, we multiply these sizes by two
        math::vec2f size{constants.column_thickness,
            is_top ?
                gap_start_pos_y * 2.0f :
                (canvas_height - (gap_start_pos_y + constants.gap_height)) * 2.0f};

        auto pipe = geometry::blueprint_rectangle(registry, size, constants.pipe_color, pos, constants.pipe_outline_color);

        registry.assign<graphics::layer<6>>(pipe);

        return pipe;
    }

    // Returns a random gap start position Y
    float get_random_gap_start_pos(entt::registry &registry) {
        //! Retrieve constants
        const auto canvas_height = registry.ctx<graphics::canvas_2d>().canvas.size.y();
        const auto constants = registry.ctx<flappy_bird_constants>();

        float top_limit = canvas_height * constants.column_min;
        float bottom_limit = canvas_height * constants.column_max - constants.gap_height;

        return random_float(top_limit, bottom_limit);
    }

    // Factory to create single column
    void create_column(entt::registry &registry, float pos_x) noexcept {
        // Create a fresh entity for a new column
        auto entity_column = registry.create();

        // Get a random gap start position Y, between pipes
        float gap_start_pos_y = get_random_gap_start_pos(registry);

        // Create pipes, is_top variable is false for bottom one
        auto top_pipe = create_pipe(registry, true, pos_x, gap_start_pos_y);
        auto bottom_pipe = create_pipe(registry, false, pos_x, gap_start_pos_y);

        // Make a column from these two pipes and mark it as "column"
        registry.assign<column>(entity_column, top_pipe, bottom_pipe);
        registry.assign<entt::tag<"column"_hs>>(entity_column);
    }

    //! Factory for creating a Flappy Bird columns
    void create_columns(entt::registry &registry) noexcept {
        //! Retrieve constants
        const auto canvas_width = registry.ctx<graphics::canvas_2d>().canvas.size.x();
        const auto constants = registry.ctx<flappy_bird_constants>();

        const float column_pos_offset = canvas_width;

        // Create the columns
        for(std::size_t i = 0; i < constants.column_count; ++i) {
            // Horizontal position (X) increases for every column, keeping the distance
            float pos_x = column_pos_offset + i * constants.column_distance;

            create_column(registry, pos_x);
        }
    }

    //! Factory for creating a Flappy Bird background
    void create_background(entt::registry &registry) noexcept {
        //! Retrieve constants
        const auto[canvas_width, canvas_height] = registry.ctx<graphics::canvas_2d>().canvas.size;
        const auto constants = registry.ctx<flappy_bird_constants>();

        // Create Sky
        {
            // Sky is whole canvas so position is middle of it
            transform::position_2d pos{canvas_width * 0.5f, canvas_height * 0.5f};

            // And the size is full canvas
            math::vec2f size{canvas_width, canvas_height};

            auto sky = geometry::blueprint_rectangle(registry, size, constants.background_color, pos);
            registry.assign<graphics::layer<5>>(sky);
        }

        // Create Ground
        {
            // Ground expands to whole canvas width so position is middle of it,
            // But position Y is at bottom of the screen so it's full canvas_height minus half of the ground thickness
            transform::position_2d pos{canvas_width * 0.5f, canvas_height - constants.ground_thickness * 0.5f};

            // Size X is full canvas but the height is defined in constants
            math::vec2f size{canvas_width, constants.ground_thickness};

            auto ground = geometry::blueprint_rectangle(registry, size, constants.ground_color, pos);
            registry.assign<graphics::layer<7>>(ground);
        }

        // Create Grass
        {
            // Ground expands to whole canvas width so position is middle of it,
            // But position Y is at top of the ground, so it's canvas height minus ground thickness
            transform::position_2d pos{canvas_width * 0.5f, canvas_height - constants.ground_thickness};

            // Size X is full canvas but the height is defined in constants
            // We also make it a bit longer by adding the thickness of the outline to hide the outline at sides
            math::vec2f size{canvas_width + constants.grass_outline_color.thickness * 2.0f, constants.grass_thickness};

            auto grass = geometry::blueprint_rectangle(registry, size, constants.grass_color, pos, constants.grass_outline_color);
            registry.assign<graphics::layer<8>>(grass);
        }
    }
}

class column_logic final : public ecs::logic_update_system<column_logic> {
public:
    column_logic(entt::registry &registry) noexcept : system(registry) { }

    void update() noexcept final {
        auto& registry = entity_registry_;

        //! Retrieve constants
        const auto constants = registry.ctx<flappy_bird_constants>();

        // Loop all columns
        auto view = registry.view<column>();

        for(auto entity : view) {
            auto& col = registry.get<column>(entity);

            // Move pipes, and check if they are out of the screen
            bool col_out_of_screen = move_pipe(registry, col.top_pipe) || move_pipe(registry, col.bottom_pipe);

            // If column is out of the screen
            if(col_out_of_screen) {
                // Remove this column
                registry.destroy(col.top_pipe);
                registry.destroy(col.bottom_pipe);
                registry.destroy(entity);

                // Create a new column at far end
                create_column(registry, furthest_pipe_position(registry) + constants.column_distance);
            }
        }
    }

private:
    // Find the most far pipe's position X
    float furthest_pipe_position(entt::registry &registry) {
        float furthest = 0.f;

        auto view = registry.view<column>();

        for(auto entity : view) {
            auto& col = registry.get<column>(entity);
            float x = entity_registry_.get<transform::position_2d>(col.top_pipe).x();
            if(x > furthest) furthest = x;
        }

        return furthest;
    }

    // Move the pipe and return if it's out of the screen
    bool move_pipe(entt::registry &registry, entt::entity pipe) {
        // Retrieve constants
        const auto constants = registry.ctx<flappy_bird_constants>();


        // Get current position of the pipe
        auto pos = entity_registry_.get<transform::position_2d>(pipe);

        // Shift pos X to left by scroll_speed but multiplying with dt because we do this so many times a second,
        // Delta time makes sure that it's applying over time, so in one second it will move scroll_speed pixels
        auto new_pos_x = pos.x() - constants.scroll_speed * timer::time_step::get_fixed_delta_time();

        // Set the new position value
        registry.assign_or_replace<transform::position_2d>(pipe, new_pos_x, pos.y());

        // Return the info about if this pipe is out of the screen
        return new_pos_x < -constants.column_thickness * 2.0f;
    }
};

//! Give a name to our system
REFL_AUTO(type(column_logic));

class game_scene final : public scenes::base_scene {
public:
    game_scene(entt::registry &entity_registry) noexcept : base_scene(entity_registry) {
        //! Set the constants that will be used in the program
        entity_registry_.set<flappy_bird_constants>();

        //! Create the columns
        create_columns(entity_registry_);
        create_background(entity_registry_);
    }

    //! Update the game every tick
    void update() noexcept final {
    }

    //! Scene name
    std::string scene_name() noexcept final {
        return "game_scene";
    }

    ~game_scene() noexcept final {
        //! Retrieve the collection of entities from the game scene
        auto view = entity_registry_.view<entt::tag<"game_scene"_hs>>();

        //! Iterate the collection and destroy each entities
        entity_registry_.destroy(view.begin(), view.end());

        //! Unset the tic tac toe constants
        entity_registry_.unset<flappy_bird_constants>();
    }
};

//! Game world
struct flappy_bird_world : world::app {
    //! Game entry point
    flappy_bird_world() noexcept {
        //! Load our graphical system
        auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

        //! Load our input system with the window from the graphical system
        system_manager_.create_system<sfml::input_system>(graphic_system.get_window());

        //! Load the scenes manager
        auto &scene_manager = system_manager_.create_system<scenes::manager>();

        //! Change the current_scene to "game_scene" by pushing it.
        scene_manager.change_scene(std::make_unique<game_scene>(entity_registry_), true);

        //! Create column logic system
        this->system_manager_.create_system<column_logic>();
    }
};

int main() {
    // Use current time as seed for random generator
    std::srand(std::time(nullptr));

    //! Declare our world
    flappy_bird_world game;

    //! Run the game
    return game.run();
}