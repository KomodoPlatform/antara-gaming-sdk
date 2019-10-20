#include <vector>
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
    tic_tac_toe_constants(std::size_t nb_cells_per_axis_, std::size_t width_, std::size_t height_) noexcept :
            nb_cells_per_axis(nb_cells_per_axis_),
            cell_width(width_ / nb_cells_per_axis),
            cell_height(height_ / nb_cells_per_axis)
    {
    }

    const std::size_t nb_cells_per_axis;
    const std::size_t cell_width;
    const std::size_t cell_height;
    const float grid_thickness{20.0f};
};

enum cell_state
{
    empty,
    player_x = 1,
    player_y = 2
};

//! Contains all the function that will be used for logic  and factory
namespace
{
    //! Factory for creating a tic-tac-toe grid
    entt::entity create_grid(entt::registry &registry) noexcept
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

            //! first and last ones should be a bit inside, otherwise half of it is out of the screen
            auto offset_x = 0.0f;
            auto offset_y = 0.0f;

            if (i == 0) {
                offset_x += half_thickness;
                offset_y += half_thickness;
            } else if (i == nb_cells) {
                offset_x -= half_thickness;
                offset_y -= half_thickness;
            }

            //! prepare lines

            //! vertical
            lines[counter + 0].pos = {offset_x + idx * cell_width - half_thickness, 0.f};
            lines[counter + 1].pos = {offset_x + idx * cell_width + half_thickness, 0.f};
            lines[counter + 2].pos = {offset_x + idx * cell_width + half_thickness, canvas_height};
            lines[counter + 3].pos = {offset_x + idx * cell_width - half_thickness, canvas_height};

            //! horizontal
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

    std::vector<cell_state> create_board(std::size_t nb_cells_per_axis)
    {
        std::vector<cell_state> board(nb_cells_per_axis * nb_cells_per_axis, cell_state::empty);
        return board;
    }

    void create_x(entt::registry &entity_registry, std::size_t row, std::size_t column) noexcept
    {
        auto[nb_cells, cell_width, cell_height, grid_thickness] = entity_registry.ctx<tic_tac_toe_constants>();
        const auto half_box_side = static_cast<float>(std::fmin(cell_width, cell_height) * 0.25f);
        const auto center_x = static_cast<float>(cell_width * 0.5 + column * cell_width);
        const auto center_y = static_cast<float>(cell_height * 0.5 + row * cell_height);

        auto x_entity = entity_registry.create();
        std::vector<geometry::vertex> lines{2 * 4};

        for (auto &&current_vertex: lines) current_vertex.pixel_color = graphics::magenta;

        const auto half_thickness = grid_thickness * 0.5f;

        // Top-left to Bottom-right
        lines[0].pos = {center_x - half_box_side - half_thickness, center_y - half_box_side};
        lines[1].pos = {center_x - half_box_side + half_thickness, center_y - half_box_side};
        lines[2].pos = {center_x + half_box_side + half_thickness, center_y + half_box_side};
        lines[3].pos = {center_x + half_box_side - half_thickness, center_y + half_box_side};


        // Top-right to Bottom-left
        lines[4].pos = {center_x + half_box_side - half_thickness, center_y - half_box_side};
        lines[5].pos = {center_x + half_box_side + half_thickness, center_y - half_box_side};
        lines[6].pos = {center_x - half_box_side + half_thickness, center_y + half_box_side};
        lines[7].pos = {center_x - half_box_side - half_thickness, center_y + half_box_side};

        entity_registry.assign<geometry::vertex_array>(x_entity, lines, geometry::vertex_geometry_type::quads);
        entity_registry.assign<entt::tag<"game_scene"_hs>>(x_entity);
        entity_registry.assign<entt::tag<"player_x"_hs>>(x_entity);
        entity_registry.assign<graphics::layer<1>>(x_entity);
    }

    void create_o(entt::registry &entity_registry, std::size_t row, std::size_t column) noexcept
    {
        auto constants = entity_registry.ctx<tic_tac_toe_constants>();
        const auto half_box_side = static_cast<float>(std::fmin(constants.cell_width, constants.cell_height) * 0.25f);
        const auto center_x = static_cast<float>(constants.cell_width * 0.5 + column * constants.cell_width);
        const auto center_y = static_cast<float>(constants.cell_height * 0.5 + row * constants.cell_height);

        auto o_entity = entity_registry.create();
        entity_registry.assign<geometry::circle>(o_entity, half_box_side);
        entity_registry.assign<graphics::fill_color>(o_entity, graphics::transparent);
        entity_registry.assign<graphics::outline_color>(o_entity, constants.grid_thickness, graphics::cyan);
        entity_registry.assign<transform::position_2d>(o_entity,
                                                       center_x,
                                                       center_y);

        entity_registry.assign<entt::tag<"game_scene"_hs>>(o_entity);
        entity_registry.assign<graphics::layer<1>>(o_entity);
    }
}

class tic_tac_toe_logic final : public ecs::logic_update_system<tic_tac_toe_logic>
{
public:
    ~tic_tac_toe_logic() noexcept final = default;

    void update() noexcept final
    {}

    void on_mouse_button_pressed(const event::mouse_button_pressed &evt) noexcept
    {
        if (current_game_state_ == running) {
            //! Retrieve game constants.
            auto constants = entity_registry_.ctx<tic_tac_toe_constants>();

            //! Play one turn of the Tic-Tac-Toe
            play_turn(evt.y / constants.cell_height, evt.x / constants.cell_width);
        } else {
            //! Reset the game
        }
    }

    tic_tac_toe_logic(entt::registry &registry, entt::entity grid_entity, std::vector<cell_state> board) noexcept
            : system(registry), grid_entity_(grid_entity), board_(std::move(board))
    {
        //! stateless system
        this->disable();

        //! subscribe to mouse_button event
        this->dispatcher_.sink<event::mouse_button_pressed>().connect<&tic_tac_toe_logic::on_mouse_button_pressed>(
                *this);
    }

private:
    //! Game logic
    void play_turn(std::size_t row, std::size_t column) noexcept
    {
        //! Retrieve constants
        auto constants = entity_registry_.ctx<tic_tac_toe_constants>();

        //! Which cell is clicked ?
        std::size_t index = row * constants.nb_cells_per_axis + column;

        //! Cell is available ?
        if (index < board_.size() && board_[index] == cell_state::empty) {

            //! change state of the cell to the current player
            board_[index] = static_cast<cell_state>(player_turn_);

            //! create x or o based on the current player
            player_turn_ == x ? create_x(entity_registry_, row, column) : create_o(entity_registry_, row, column);

            //! switch player
            player_turn_ = (player_turn_ == player::x) ? player::o : player::x;
        }
    }

private:
    //! Private enums
    enum game_state
    {
        running,
        player_x_won = 1,
        player_y_won = 2,
        tie,
        quit
    };

    enum player
    {
        x = 1,
        o = 2
    };

private:
    //! Private members variable
    entt::entity grid_entity_{entt::null};
    std::vector<cell_state> board_;
    game_state current_game_state_{game_state::running};
    player player_turn_{player::x};
};

//! Give a name to our system
REFL_AUTO(type(tic_tac_toe_logic));

class game_scene final : public scenes::base_scene
{
public:
    game_scene(entt::registry &entity_registry) noexcept : base_scene(entity_registry)
    {
        //! Retrieve canvas information
        auto[canvas_width, canvas_height] = entity_registry_.ctx<graphics::canvas_2d>().canvas.size.to<math::vec2u>();

        //! Set the constants that will be used in the program
        auto &tictactoe_constants = entity_registry_.set<tic_tac_toe_constants>(3ull, canvas_width, canvas_height);

        //! Create the grid of the tic tac toe
        auto grid_entity = create_grid(entity_registry_);

        //! Create the board of the tic tac toe
        auto board = create_board(tictactoe_constants.nb_cells_per_axis);

        //! Create our logic game system and give the fresh grid entity and the fresh board.
        this->system_manager_.create_system<tic_tac_toe_logic>(grid_entity, board);
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
        //! Retrieve the collection of entities from the game scene
        auto view = entity_registry_.view<entt::tag<"game_scene"_hs>>();

        //! Iterate the collection and destroy each entities
        entity_registry_.destroy(view.begin(), view.end());

        //! Unset the tic tac toe constants
        entity_registry_.unset<tic_tac_toe_constants>();
    }

private:
    ecs::system_manager system_manager_{entity_registry_};
};

//! Our game world
struct tic_tac_toe_world : world::app
{
    //! Our game entry point
    tic_tac_toe_world() noexcept
    {
        //! Load our graphical system
        auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

        //! Load our input system with the window from the graphical system
        system_manager_.create_system<sfml::input_system>(graphic_system.get_window());

        //! Load the scenes manager
        auto &scene_manager = system_manager_.create_system<scenes::manager>();

        //! Change the current_scene to "game_scene" by pushing it.
        scene_manager.change_scene(std::make_unique<game_scene>(entity_registry_), true);
    }
};

int main()
{
    //! Declare our world
    tic_tac_toe_world game;

    //! Run the game
    return game.run();
}