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

#include <functional>
#include <SFML/Audio/Sound.hpp>

namespace antara::gaming::sfml
{
    /**
     * @struct component_sound
     * @brief This struct contains the sound and attributes of it such as volume.
     */
    struct component_sound
    {
        /**
         * @brief This function plays the sound.
         */
        void play() {
            is_started = true;
            sound.play();
        }

        sf::Sound sound; ///< This object is SFML's Sound instance which contains the sound data.
        bool is_started{false}; ///< This boolean holds the information if sound started playing. Used later on to check if it should be destroyed or not.
        std::function<void()> on_finish; ///< This is the function which will be called at destruction of the sound when it finishes.
        float volume{100.0f}; ///< This is the volume of the sound, ranged between 0-100.
    };
}