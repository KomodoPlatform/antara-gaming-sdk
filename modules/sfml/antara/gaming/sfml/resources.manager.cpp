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

#include "antara/gaming/sfml/resources.manager.hpp"

namespace antara::gaming::sfml
{

    texture_handle resources_manager::load_texture(const char *resource_id, bool smooth)
    {
        auto handle = resources_manager::load<textures_loader, textures_cache>(textures_cache_, resource_id,
                                                                        (textures_path_ / resource_id).string());
        handle->setSmooth(smooth);
        return handle;
    }

    font_handle resources_manager::load_font(const char *resource_id)
    {
        return resources_manager::load<fonts_loader, fonts_cache>(fonts_cache_, resource_id,
                                                                  (fonts_path_ / resource_id).string());
    }

    sound_handle resources_manager::load_sound(const char *resource_id)
    {
        return resources_manager::load<sounds_loader, sounds_cache>(sounds_cache_, resource_id,
                                                                    (sounds_path / resource_id).string());
    }
}