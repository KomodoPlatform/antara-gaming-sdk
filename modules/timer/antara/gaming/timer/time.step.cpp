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

#include "antara/gaming/timer/time.step.hpp"

namespace antara::gaming::timer
{
    std::chrono::nanoseconds time_step::fps_ = _60fps;
    float time_step::fixed_delta_time{std::chrono::duration<float, std::ratio<1>>(fps_).count()};
    void time_step::start() noexcept
    {
        start_ = clock::now();
    }

    void time_step::start_frame() noexcept
    {
        auto deltaTime = clock::now() - start_;
        start_ = clock::now();
        lag_ += std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);
    }

    bool time_step::is_update_required() const noexcept
    {
        return (lag_ >= fps_);
    }

    void time_step::perform_update() noexcept
    {
        lag_ -= fps_;
    }

    void time_step::change_fps(std::chrono::nanoseconds new_fps_rate)
    {
        fps_ = new_fps_rate;
        fixed_delta_time = std::chrono::duration<float, std::ratio<1>>(fps_).count();
    }

    float time_step::get_fixed_delta_time() noexcept
    {
        return fixed_delta_time;
    }
}