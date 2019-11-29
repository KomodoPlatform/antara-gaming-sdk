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

//! C++ System Headers
#include <functional>
#include <string>

//! SDK Headers
#include "antara/gaming/audio/audio.status.hpp"

namespace antara::gaming::audio {
    struct sound_effect {
        std::string sound_id; ///< The id of the sound.
        status sound_status{status::wait_for_first_run}; ///< The sound status.
        float pitch{1.f}; ///< The sound pitch;
        float volume{100.f}; ///< The sound volume.
        bool loop{false}; ///< Indicates whether the sound is played in a loop.
        std::function<void()> on_finish{[]() {}}; ///< Callback that will be executed once the sound is finished
        float minimum_distance{1.f}; ///< The minimum distance of the sound.
        float attenuation{1.f}; ///< The attenuation factor of the sound.
        bool relative_to_listener{false}; ///< Indicate if the sound is relative to the listener.
        bool recycling{false}; ///< Indicate if the sound will be reused (do not destroy the entity)
    };
}