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

#include <chrono>
#include "antara/gaming/timer/fps.hpp"

namespace antara::gaming::timer
{
    class time_step
    {
    public:
        static void start() noexcept;

        static void start_frame() noexcept;

        [[nodiscard]] bool is_update_required() const noexcept;
        
        [[nodiscard]] float get_interpolation() const noexcept;

        static void perform_update() noexcept;

        static void change_fps(std::chrono::nanoseconds new_fps_rate);

        static float get_fixed_delta_time() noexcept;

        static void reset_lag() noexcept;

    private:
        static std::chrono::nanoseconds fps_;
        static float fixed_delta_time;
        using clock = std::chrono::steady_clock;
        static std::chrono::nanoseconds lag_;
        static clock::time_point start_;
    };
}