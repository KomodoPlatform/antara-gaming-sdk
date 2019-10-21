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

struct reset_event
{

};

//! Contains all the function that will be used for logic  and factory
namespace
{
    //! Factory for creating a tic-tac-toe grid
    entt::entity create_grid(entt::registry &registry) noexcept
    {
        //! Retrieve canvas information
        auto[canvas_width, canvas_height] = registry.ctx<graphics::canvas_2d>().canvas.size;

        //! Entity creation
        auto grid_entity = registry.create();

        //! Our vertices
        std::vector<geometry::vertex> lines{8 * 4};

        //! Retrieve constants information
        auto[nb_cells, cell_width, cell_height, grid_thickness] = registry.ctx<tic_tac_toe_constants>();
        const auto half_thickness = grid_thickness * 0.5f;

        //! Our loop to create the grid
        for (std::size_t counter = 0, i = 0; i <= nb_cells; ++i, counter += 4 * 2) {

            //! To avoid narrowing conversion
            auto idx = static_cast<float>(i);

            //! First and last ones should be a bit inside, otherwise half of it is out of the screen
            auto offset_x = 0.0f;
            auto offset_y = 0.0f;

            if (i == 0) {
                offset_x += half_thickness;
                offset_y += half_thickness;
            } else if (i == nb_cells) {
                offset_x -= half_thickness;
                offset_y -= half_thickness;
            }

            //! Prepare lines

            //! Vertical
            lines[counter + 0].pos = {offset_x + idx * cell_width - half_thickness, 0.f};
            lines[counter + 1].pos = {offset_x + idx * cell_width + half_thickness, 0.f};
            lines[counter + 2].pos = {offset_x + idx * cell_width + half_thickness, canvas_height};
            lines[counter + 3].pos = {offset_x + idx * cell_width - half_thickness, canvas_height};

            //! Horizontal
            lines[counter + 4].pos = {offset_x + 0, offset_y + idx * cell_height - half_thickness};
            lines[counter + 5].pos = {offset_x + canvas_width, offset_y + idx * cell_height - half_thickness};
            lines[counter + 6].pos = {offset_x + canvas_width, offset_y + idx * cell_height + half_thickness};
            lines[counter + 7].pos = {offset_x + 0, offset_y + idx * cell_height + half_thickness};
        }

        //! Assign the vertex array to the grid entity
        registry.assign<geometry::vertex_array>(grid_entity, lines, geometry::vertex_geometry_type::quads);

        //! Assign the game_scene tag to the grid_entity (_hs means hashed_string)
        registry.assign<entt::tag<"game_scene"_hs>>(grid_entity);

        //! We want to draw the grid on the most deep layer, here 0.
        registry.assign<graphics::layer<0>>(grid_entity);

        //! We give back our fresh entity
        return grid_entity;
    }

    std::vector<cell_state> create_board(std::size_t nb_cells_per_axis)
    {
        std::vector<cell_state> board(nb_cells_per_axis * nb_cells_per_axis, cell_state::empty);
        return board;
    }

    void create_x(entt::registry &entity_registry, std::size_t row, std::size_t column) noexcept
    {
        auto[_, cell_width, cell_height, grid_thickness] = entity_registry.ctx<tic_tac_toe_constants>();
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

        auto o_entity = geometry::blueprint_circle(entity_registry, half_box_side, graphics::transparent,
                                                   transform::position_2d(center_x, center_y),
                                                   graphics::outline_color(constants.grid_thickness, graphics::cyan));

        entity_registry.assign<entt::tag<"game_scene"_hs>>(o_entity);
        entity_registry.assign<graphics::layer<1>>(o_entity);
    }
}

class tic_tac_toe_logic final : public ecs::logic_update_system<tic_tac_toe_logic>
{
public:

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
            this->dispatcher_.trigger<reset_event>();
        }
    }

    tic_tac_toe_logic(entt::registry &registry, entt::entity grid_entity, std::vector<cell_state> board) noexcept
            : system(registry), grid_entity_(grid_entity), board_(std::move(board))
    {
        //! Stateless system
        this->disable();

        //! Subscribe to mouse_button event
        this->dispatcher_.sink<event::mouse_button_pressed>().connect<&tic_tac_toe_logic::on_mouse_button_pressed>(
                *this);
    }

    ~tic_tac_toe_logic() noexcept final
    {
        this->dispatcher_.sink<event::mouse_button_pressed>().disconnect(*this);
    }

private:
    //! Game logic
    [[nodiscard]] bool did_current_player_win_the_game() const noexcept
    {
        std::size_t row_count{0u}, column_count{0u}, diag1_count{0u}, diag2_count{0u};
        auto[nb_cells, cell_width, cell_height, _] = entity_registry_.ctx<tic_tac_toe_constants>();
        for (std::size_t i = 0; i < nb_cells; ++i) {
            for (std::size_t j = 0; j < nb_cells; ++j) {
                //! Check rows
                if (board_[i * nb_cells + j] == static_cast<cell_state>(player_turn_))
                    row_count++;

                //! Check columns
                if (board_[j * nb_cells + i] == static_cast<cell_state>(player_turn_))
                    column_count++;
            }

            //! Check condition
            if (row_count >= nb_cells || column_count >= nb_cells) {
                return true;
            }

            //! Reset rows and columns
            row_count = 0u, column_count = 0u;

            //! Diag1 count
            if (board_[i * nb_cells + i] == static_cast<cell_state>(player_turn_))
                diag1_count++;

            //! Second diag count
            if (board_[i * nb_cells + nb_cells - i - 1] == static_cast<cell_state>(player_turn_))
                diag2_count++;
        }

        //! Condition
        return diag1_count >= nb_cells || diag2_count >= nb_cells;
    }

    [[nodiscard]] bool is_tie() const noexcept
    {
        return std::count(begin(board_), end(board_), cell_state::empty) == 0;
    }

    void check_winning_condition() noexcept
    {
        auto make_screen = [this](graphics::color clr_winner,
                                  entt::entity entity) {
            auto &array_cmp = this->entity_registry_.get<geometry::vertex_array>(entity);
            for (auto &v : array_cmp.vertices) v.pixel_color = clr_winner;
            entity_registry_.replace<geometry::vertex_array>(entity, array_cmp.vertices, array_cmp.geometry_type);
        };

        auto make_player_win_screen = [this, make_screen](entt::entity entity) {
            auto winning_color = player_turn_ == player::x ? graphics::magenta : graphics::cyan;
            make_screen(winning_color, entity);
        };

        auto make_tie_screen = [make_screen](entt::entity entity) {
            make_screen(graphics::yellow, entity);
        };

        if (did_current_player_win_the_game()) {
            current_game_state_ = static_cast<game_state>(player_turn_);
            make_player_win_screen(grid_entity_);
        } else if (is_tie()) {
            current_game_state_ = game_state::tie;
            make_tie_screen(grid_entity_);
        }
    }

    void play_turn(std::size_t row, std::size_t column) noexcept
    {
        //! Retrieve constants
        auto constants = entity_registry_.ctx<tic_tac_toe_constants>();

        //! Which cell is clicked ?
        std::size_t index = row * constants.nb_cells_per_axis + column;

        //! Cell is available ?
        if (index < board_.size() && board_[index] == cell_state::empty) {

            //! Change state of the cell to the current player
            board_[index] = static_cast<cell_state>(player_turn_);

            //! Create x or o based on the current player
            player_turn_ == x ? create_x(entity_registry_, row, column) : create_o(entity_registry_, row, column);

            //! Check winning condition
            check_winning_condition();

            //! Switch player
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
        //! Subscribe to reset event
        this->dispatcher_.sink<reset_event>().connect<&game_scene::on_reset_event>(*this);
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


    //! Our scene name
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

    //! Callback
    void on_reset_event(const reset_event &) noexcept
    {
        entt::registry &registry = this->entity_registry_;
        this->~game_scene();
        new(this) game_scene(registry);
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