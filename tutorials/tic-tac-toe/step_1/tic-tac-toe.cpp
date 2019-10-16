#include <antara/gaming/world/world.app.hpp>
#include <antara/gaming/sfml/graphic.system.hpp>
#include <antara/gaming/sfml/input.system.hpp>
#include <antara/gaming/scenes/scene.manager.hpp>
#include <antara/gaming/math/vector.hpp>
#include <antara/gaming/graphics/component.canvas.hpp>
#include <antara/gaming/graphics/component.layer.hpp>

//! For convenience
using namespace antara::gaming;

struct tic_tac_toe_constants
{
    tic_tac_toe_constants(std::size_t nb_cells_per_row_, std::size_t width_, std::size_t height_) noexcept :
            nb_cells_per_row(nb_cells_per_row_),
            cell_width(width_ / nb_cells_per_row),
            cell_height(height_ / nb_cells_per_row)
    {
    }

    const std::size_t nb_cells_per_row;
    const std::size_t cell_width;
    const std::size_t cell_height;
    const float grid_thickness{20.0f};
};

static inline entt::entity create_grid(entt::registry &registry) noexcept
{
    //! retrieve canvas information
    auto[canvas_width, canvas_height] = registry.ctx<graphics::canvas_2d>().canvas.size;

    //! entity creation
    auto grid_entity = registry.create();

    //! our vertices
    std::vector<geometry::vertex> lines{8 * 4};

    //! retrieve constants information
    auto[nb_cells, cell_width, cell_height, grid_thickness] = registry.ctx<tic_tac_toe_constants>();
    const auto half_thickness = grid_thickness * 0.5f;

    //! our loop to create the grid
    for (std::size_t counter = 0, i = 0; i <= nb_cells; ++i, counter += 4 * 2) {

        //! to avoid narrowing conversion
        auto idx = static_cast<float>(i);

        // First and last ones should be a bit inside, otherwise half of it is out of the screen
        auto offset_x = 0.0f;
        auto offset_y = 0.0f;

        if (i == 0) {
            offset_x += half_thickness;
            offset_y += half_thickness;
        } else if (i == nb_cells) {
            offset_x -= half_thickness;
            offset_y -= half_thickness;
        }

        // Prepare lines

        // Vertical
        lines[counter].pos = {offset_x + idx * cell_width - half_thickness, 0.f};
        lines[counter + 1].pos = {offset_x + idx * cell_width + half_thickness, 0.f};
        lines[counter + 2].pos = {offset_x + idx * cell_width + half_thickness, canvas_height};
        lines[counter + 3].pos = {offset_x + idx * cell_width - half_thickness, canvas_height};

        // Horizontal
        lines[counter + 4].pos = {offset_x + 0, offset_y + idx * cell_height - half_thickness};
        lines[counter + 5].pos = {offset_x + canvas_width, offset_y + idx * cell_height - half_thickness};
        lines[counter + 6].pos = {offset_x + canvas_width, offset_y + idx * cell_height + half_thickness};
        lines[counter + 7].pos = {offset_x + 0, offset_y + idx * cell_height + half_thickness};
    }

    //! assign the vertex array to the grid entity
    registry.assign<geometry::vertex_array>(grid_entity, lines, geometry::vertex_geometry_type::quads);

    //! assign the game_scene tag to the grid_entity (_hs means hashed_string)
    registry.assign<entt::tag<"game_scene"_hs>>(grid_entity);

    //! We want to draw the grid on the most deep layer, here 0.
    registry.assign<graphics::layer<0>>(grid_entity);

    //! we give back our fresh entity
    return grid_entity;
}

class game_scene final : public scenes::base_scene
{
public:
    game_scene(entt::registry &entity_registry) noexcept : base_scene(entity_registry)
    {
        //! Here we retrieve canvas information
        auto[canvas_width, canvas_height] = entity_registry_.ctx<graphics::canvas_2d>().canvas.size.to<math::vec2u>();

        //! Here i set the constants that will be used in the program
        entity_registry_.set<tic_tac_toe_constants>(3ull, canvas_width, canvas_height);

        //! Here i create the grid of the tic tac toe
        grid_entity_ = create_grid(entity_registry_);
    }

    //! This function will not be used, because tic tac toe doesn't need an update every frame.
    void update() noexcept final
    {}

    //! our scene name
    std::string scene_name() noexcept final
    {
        return "game_scene";
    }

    ~game_scene() noexcept final
    {
        //! Here we retrieve the collection of entities from the game scene
        auto view = entity_registry_.view<entt::tag<"game_scene"_hs>>();

        //! Here we iterate the collection and destroy each entities
        entity_registry_.destroy(view.begin(), view.end());

        //! Here we unset the tic tac toe constants
        entity_registry_.unset<tic_tac_toe_constants>();
    }

private:
    //! Our entity representing the tic-tac-toe grid
    entt::entity grid_entity_{entt::null};
};

//! Our game world
struct tic_tac_toe_world : world::app
{
    //! Our game entry point
    tic_tac_toe_world() noexcept
    {
        //! Here we load our graphical system
        auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

        //! Here we load our input system with the window from the graphical system
        system_manager_.create_system<sfml::input_system>(graphic_system.get_window());

        //! Here we load the scenes manager
        auto &scene_manager = system_manager_.create_system<scenes::manager>();

        //! Here we change the current_scene to "game_scene" by pushing it.
        scene_manager.change_scene(std::make_unique<game_scene>(entity_registry_), true);
    }
};

int main()
{
    //! Here we declare our world
    tic_tac_toe_world game;

    //! Here we run the game
    return game.run();
}