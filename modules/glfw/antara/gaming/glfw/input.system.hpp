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

#include "antara/gaming/ecs/system.hpp"
#include <GLFW/glfw3.h>

namespace antara::gaming::glfw
{
    class input_system final : public ecs::pre_update_system<input_system>
    {
      public:
        explicit input_system(entt::registry& registry, GLFWwindow* window);
        ~input_system() noexcept final = default;
        void update() noexcept final;

      private:
        GLFWwindow* window_;
    };
} // namespace antara::gaming::glfw

REFL_AUTO(type(antara::gaming::glfw::input_system))