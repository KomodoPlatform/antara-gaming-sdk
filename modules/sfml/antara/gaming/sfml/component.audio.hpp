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

#include <SFML/Audio/Sound.hpp>
#include <functional>

namespace antara::gaming::sfml
{
    /**
     * @struct component_sound
     * @brief This struct contains the sound and attributes of it such as volume.
     */
    struct component_sound
    {
        sf::Sound sound; ///< This object is SFML's Sound instance which contains the sound data.
    };
} // namespace antara::gaming::sfml