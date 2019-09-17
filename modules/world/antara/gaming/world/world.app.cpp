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

#if defined(__EMSCRIPTEN__) && defined(EMSCRIPTEN_ONLY_WEB)
#include <emscripten.h>
#endif

#include "antara/gaming/config/config.game.hpp"
#include "antara/gaming/event/start.game.hpp"
#include "antara/gaming/world/world.app.hpp"

//LCOV_EXCL_START
void emscripten_antara_loop(void *world)
{
    static_cast<antara::gaming::world::app *>(world)->process_one_frame();
}
//LCOV_EXCL_STOP
namespace antara::gaming::world
{
    //! Constructor
    app::app() noexcept
    {
        this->entity_registry_.set<config::game_cfg>();
        dispatcher_.sink<event::quit_game>().connect<&app::receive_quit_game>(*this);
    }

    //! Public callbacks
    void app::receive_quit_game(const event::quit_game &evt) noexcept
    {
        this->is_running_ = false;
        this->game_return_value_ = evt.return_value_;
        //LCOV_EXCL_START
#if defined(__EMSCRIPTEN__) && defined(EMSCRIPTEN_ONLY_WEB)
        emscripten_cancel_main_loop();
#endif
        //LCOV_EXCL_STOP
    }

    int app::run() noexcept
    {
        if (!system_manager_.nb_systems()) {
            return this->game_return_value_;
        }
        this->dispatcher_.trigger<event::start_game>();
        this->is_running_ = true;
        //LCOV_EXCL_START
#if defined(__EMSCRIPTEN__) && defined(EMSCRIPTEN_ONLY_WEB)
        emscripten_set_main_loop_arg(emscripten_antara_loop, this, 0, 1);
#else
        //LCOV_EXCL_STOP
        while (this->is_running_) {
            process_one_frame();
        }
#endif
        return this->game_return_value_;
    }

    void app::process_one_frame()
    {
        this->system_manager_.update();
    }
}