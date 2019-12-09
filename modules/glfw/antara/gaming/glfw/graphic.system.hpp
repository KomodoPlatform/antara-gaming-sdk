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

#include <GLFW/glfw3.h>
#include "antara/gaming/ecs/system.hpp"

namespace antara::gaming::glfw {
    using st_high_dpi_factor = st::type<float, struct high_dpi_factor>;

    class graphic_system final : public ecs::post_update_system<graphic_system> {
    public:
        explicit graphic_system(entt::registry &registry);

        ~graphic_system() noexcept final;

        void update() noexcept final;

        [[nodiscard]] GLFWwindow *get_window() const noexcept { return window_; }

    private:
        GLFWwindow *window_{nullptr};
    };
}

REFL_AUTO(type(antara::gaming::glfw::graphic_system))