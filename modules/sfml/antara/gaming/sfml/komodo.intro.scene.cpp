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

#include <utility>
#include "antara/gaming/sfml/komodo.intro.scene.hpp"

namespace antara::gaming::sfml
{
    intro_scene::intro_scene(entt::registry &entity_registry, entt::dispatcher &dispatcher,
                             intro_scene::on_finish_functor on_finish_functor) noexcept : base_scene(entity_registry,
                                                                                                     dispatcher),
                                                                                          on_finish_functor_(std::move(
                                                                                                  on_finish_functor))
    {

    }

    void intro_scene::update() noexcept
    {
        if (intro_finished) {
            on_finish_functor_();
        } else {
            //! update intro here.
        }
    }

    std::string intro_scene::scene_name() noexcept
    {
        return "intro_scene";
    }

    bool intro_scene::on_key_pressed([[maybe_unused]] const event::key_pressed &evt) noexcept
    {
        intro_finished = true;
        return true;
    }
}