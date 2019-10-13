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

#pragma once

#include <antara/gaming/scenes/change.scene.event.hpp>
#include <antara/gaming/scenes/base.scene.hpp>
#include <antara/gaming/event/canvas.resized.hpp>

namespace tictactoe::example
{
    class game_scene final : public antara::gaming::scenes::base_scene
    {
    public:
        game_scene(entt::registry &entity_registry) noexcept;

        void update() noexcept final;

        bool on_mouse_button_pressed(const antara::gaming::event::mouse_button_pressed &pressed) noexcept final;

        std::string scene_name() noexcept final;

        ~game_scene() noexcept final;

        void reset() noexcept;

    private:
        //! Game logic
        void play_one_turn(std::size_t row, std::size_t column) noexcept;

        [[nodiscard]] bool is_current_player_win_the_game() const noexcept;
        [[nodiscard]] bool is_tie_game() const noexcept;

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

        void on_click_cell(std::size_t row, std::size_t column) noexcept;

        game_state current_state_{running};
        player player_turn_{player::x};

        void check_condition();

        void prepare_constants() noexcept;
    };
}