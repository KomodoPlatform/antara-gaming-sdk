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
#include <pipes/transform.hpp>
#include <pipes/push_back.hpp>
#include <antara/gaming/geometry/component.vertex.hpp>
#include <antara/gaming/graphics/component.color.hpp>
#include <antara/gaming/graphics/component.canvas.hpp>
#include <antara/gaming/math/vector.hpp>
#include "tic.tac.toe.constants.hpp"
#include "tic.tac.toe.components.hpp"
#include "tic.tac.toe.factory.hpp"
#include "game.scene.hpp"

using namespace antara::gaming;

namespace tictactoe::example
{
    game_scene::game_scene(entt::registry &entity_registry) noexcept : base_scene(entity_registry)
    {
        prepare_constants();
        tic_tac_toe_factory::create_grid_entity(entity_registry);
        tic_tac_toe_factory::create_board(entity_registry);
    }

    void game_scene::update() noexcept
    {}

    void game_scene::check_condition()
    {
        auto make_screen = [this](graphics::color clr_winner,
                                  geometry::vertex_array &array_cmp,
                                  entt::entity entity) {
            std::vector<geometry::vertex> vertices;
            auto f = [&clr_winner](geometry::vertex vertex) { vertex.pixel_color = clr_winner; return vertex; };
            array_cmp.vertices >>= pipes::transform(f) >>= pipes::push_back(vertices);
            entity_registry_.replace<geometry::vertex_array>(entity, vertices, array_cmp.geometry_type);
        };

        auto make_player_win_screen = [this, make_screen](entt::entity entity, geometry::vertex_array &array_cmp) {
            auto winning_color = player_turn_ == player::x ? graphics::magenta : graphics::cyan;
            make_screen(winning_color, array_cmp, entity);
        };

        auto make_tie_screen = [make_screen](entt::entity entity, geometry::vertex_array &array_cmp) {
            make_screen(graphics::yellow, array_cmp, entity);
        };

        if (is_current_player_win_the_game()) {
            current_state_ = static_cast<game_state>(player_turn_);
            entity_registry_.view<geometry::vertex_array, entt::tag<"grid"_hs>>().less(make_player_win_screen);
        } else if (is_tie_game()) {
            entity_registry_.view<geometry::vertex_array, entt::tag<"grid"_hs>>().less(make_tie_screen);
            current_state_ = game_state::tie;
        }
    }

    void game_scene::on_click_cell(std::size_t row, std::size_t column) noexcept
    {
        if (this->current_state_ == game_state::running) {
            play_one_turn(row, column);
        }
    }

    bool game_scene::on_mouse_button_pressed(const event::mouse_button_pressed &pressed) noexcept
    {
        if (this->current_state_ == game_state::running) {
            auto constants = entity_registry_.ctx<tic_tac_toe_constants>();
            on_click_cell(pressed.y / constants.cell_height, pressed.x / constants.cell_width);
        } else {
            reset();
        }
        return true;
    }

    std::string game_scene::scene_name() noexcept
    {
        return "game_scene";
    }

    game_scene::~game_scene() noexcept
    {
        auto view = entity_registry_.view<entt::tag<"game_scene"_hs>>();
        entity_registry_.destroy(view.begin(), view.end());
        entity_registry_.unset<tic_tac_toe_constants>();
    }

    void game_scene::reset() noexcept
    {
        entt::registry &registry = this->entity_registry_;
        this->~game_scene();
        new(this) game_scene(registry);
    }

    void game_scene::prepare_constants() noexcept
    {
        auto[canvas_width, canvas_height] = entity_registry_.ctx<graphics::canvas_2d>().canvas.size.to<math::vec2u>();
        this->entity_registry_.set<tic_tac_toe_constants>(3ull, canvas_width, canvas_height);
    }

    void game_scene::play_one_turn(std::size_t row, std::size_t column) noexcept
    {
        this->entity_registry_.view<board_component>().each([this, row, column](board_component &board_cmp) {
            auto constants = this->entity_registry_.ctx<tic_tac_toe_constants>();
            auto index = row * constants.nb_cells + column;
            if (index < board_cmp.board.size() && board_cmp.board[index] == cell_state::empty) {
                board_cmp.board[index] = static_cast<cell_state>(player_turn_);
                player_turn_ == x ? tic_tac_toe_factory::create_x(entity_registry_, row, column)
                                  : tic_tac_toe_factory::create_o(entity_registry_, row, column);
                check_condition();
                player_turn_ = player_turn_ == player::x ? o : x;
            }
        });
    }

    bool game_scene::is_current_player_win_the_game() const noexcept
    {
        bool player_won{false};
        auto winning_condition_functor = [&player_won, this](board_component &board_cmp) {
            std::size_t row_count{0u}, column_count{0u}, diag1_count{0u}, diag2_count{0u};
            auto[nb_cells, cell_width, cell_height] = this->entity_registry_.ctx<tic_tac_toe_constants>();
            for (std::size_t i = 0; i < nb_cells; ++i) {
                for (std::size_t j = 0; j < nb_cells; ++j) {
                    if (board_cmp.board[i * nb_cells + j] == static_cast<cell_state>(this->player_turn_))
                        row_count++;

                    if (board_cmp.board[j * nb_cells + i] == static_cast<cell_state>(this->player_turn_))
                        column_count++;
                }
                if (row_count >= nb_cells || column_count >= nb_cells) {
                    player_won = true;
                    return;
                }
                row_count = 0u, column_count = 0u;
                if (board_cmp.board[i * nb_cells + i] == static_cast<cell_state>(this->player_turn_))
                    diag1_count++;

                if (board_cmp.board[i * nb_cells + nb_cells - i - 1] == static_cast<cell_state>(player_turn_))
                    diag2_count++;
            }

            player_won = diag1_count >= nb_cells || diag2_count >= nb_cells;
        };

        this->entity_registry_.view<board_component>().each(winning_condition_functor);
        return player_won;
    }

    bool game_scene::is_tie_game() const noexcept
    {
        bool tie{false};
        entity_registry_.view<board_component>().each([&tie](board_component &board_cmp) {
            tie = std::count(begin(board_cmp.board), end(board_cmp.board), cell_state::empty) == 0;
        });
        return tie;
    }
}