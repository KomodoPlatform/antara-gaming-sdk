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

#include <SFML/Graphics/RenderTexture.hpp>
#include <entt/entity/helper.hpp>
#include <antara/gaming/sfml/component.drawable.hpp>
#include <antara/gaming/sfml/graphic.system.hpp>
#include "tic.tac.toe.constants.hpp"
#include "tic.tac.toe.components.hpp"
#include "tic.tac.toe.factory.hpp"
#include "game.scene.hpp"

namespace tictactoe::example
{
    game_scene::game_scene(entt::registry &entity_registry) noexcept : base_scene(entity_registry)
    {
        this->dispatcher_.sink<antara::gaming::event::canvas_resized>().connect<&game_scene::on_canvas_resized_event>(*this);
        prepare_constants();
        tic_tac_toe_factory::create_grid_entity(entity_registry);
        tic_tac_toe_factory::create_board(entity_registry);
    }

    void game_scene::update() noexcept
    {

    }

    void game_scene::check_condition()
    {
        auto player_as_win_functor = [this]() {
            std::size_t row_count{0u};
            std::size_t column_count{0u};
            std::size_t diag1_count{0u};
            std::size_t diag2_count{0u};
            bool player_won{false};
            this->entity_registry_.view<board_component>().each([&, this](board_component &board_cmp) {
                auto constants = this->entity_registry_.ctx<tic_tac_toe_constants>();
                for (std::size_t i = 0; i < constants.nb_cells; ++i) {
                    for (std::size_t j = 0; j < constants.nb_cells; ++j) {
                        if (board_cmp.board[i * constants.nb_cells + j] ==
                            static_cast<cell_state>(this->player_turn_)) {
                            row_count++;
                        }

                        if (board_cmp.board[j * constants.nb_cells + i] ==
                            static_cast<cell_state>(this->player_turn_)) {
                            column_count++;
                        }
                    }
                    if (row_count >= constants.nb_cells || column_count >= constants.nb_cells) {
                        player_won = true;
                        return;
                    }
                    row_count = 0;
                    column_count = 0;
                    if (board_cmp.board[i * constants.nb_cells + i] == static_cast<cell_state>(this->player_turn_)) {
                        diag1_count++;
                    }

                    if (board_cmp.board[i * constants.nb_cells + constants.nb_cells - i - 1] ==
                        static_cast<cell_state>(this->player_turn_)) {
                        diag2_count++;
                    }
                }

                player_won = diag1_count >= constants.nb_cells || diag2_count >= constants.nb_cells;
            });
            return player_won;
        };

        auto tie_functor = [this]() {
            bool tie{false};
            this->entity_registry_.view<board_component>().each([&tie](board_component &board_cmp) {
                tie = std::count(begin(board_cmp.board), end(board_cmp.board), cell_state::empty) == 0;
            });
            return tie;
        };
        if (player_as_win_functor()) {
            this->current_state_ = static_cast<game_state>(this->player_turn_);
            this->entity_registry_.view<antara::gaming::sfml::vertex_array, entt::tag<"grid"_hs>>().less(
                    [this](antara::gaming::sfml::vertex_array &array_cmp) {
                        for (std::size_t count = 0; count < array_cmp.drawable.getVertexCount(); ++count) {
                            array_cmp.drawable[count].color =
                                    this->player_turn_ == player::x ? sf::Color::Red : sf::Color::Blue;
                        }
                    });
        } else if (tie_functor()) {
            this->entity_registry_.view<antara::gaming::sfml::vertex_array, entt::tag<"grid"_hs>>().less(
                    [](antara::gaming::sfml::vertex_array &array_cmp) {
                        for (std::size_t count = 0; count < array_cmp.drawable.getVertexCount(); ++count) {
                            array_cmp.drawable[count].color = sf::Color::Magenta;
                        }
                    });
            this->current_state_ = game_state::tie;
        }
    }


    void game_scene::on_click_cell(std::size_t row, std::size_t column) noexcept
    {
        auto player_functor = [this, row, column]() {
            this->entity_registry_.view<board_component>().each([this, row, column](board_component &board_cmp) {
                auto constants = this->entity_registry_.ctx<tic_tac_toe_constants>();
                auto index = row * constants.nb_cells + column;
                if (index < board_cmp.board.size() &&
                    board_cmp.board[index] == cell_state::empty) {
                    board_cmp.board[index] = static_cast<cell_state>(this->player_turn_);
                    if (this->player_turn_ == x) {
                        tic_tac_toe_factory::create_x(entity_registry_, row, column);
                    } else {
                        tic_tac_toe_factory::create_o(entity_registry_, row, column);
                    }
                    check_condition();
                    this->player_turn_ = this->player_turn_ == player::x ? o : x;
                }
            });
        };

        if (this->current_state_ == game_state::running) {
            player_functor();
        }
    }

    bool game_scene::on_mouse_button_pressed(const antara::gaming::event::mouse_button_pressed &pressed) noexcept
    {
        if (this->current_state_ == game_state::running) {
            auto constants = entity_registry_.ctx<tic_tac_toe_constants>();

            auto& gfx = system_manager_.get_system<antara::gaming::sfml::graphic_system>();
            std::cout << "Pressed: " << pressed.x << " " << pressed.y << std::endl;
            auto world_pos = gfx.translate_mouse_pos(pressed.x, pressed.y);
            std::cout << "World: " << world_pos.x << " " << world_pos.y << std::endl;

            on_click_cell(world_pos.y / constants.cell_height, world_pos.x / constants.cell_width);
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
        auto canvas_size = this->entity_registry_.ctx<sf::RenderTexture>().getSize();
        this->entity_registry_.set<tic_tac_toe_constants>(3ull, canvas_size.x, canvas_size.y);
    }

    void game_scene::on_canvas_resized_event(const antara::gaming::event::canvas_resized &) noexcept
    {
        entity_registry_.unset<tic_tac_toe_constants>();
        prepare_constants();
        tic_tac_toe_factory::reset_grid(entity_registry_);
        tic_tac_toe_factory::reset_x(entity_registry_);
    }
}