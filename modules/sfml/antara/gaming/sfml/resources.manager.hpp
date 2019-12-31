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

#include <filesystem>
#include <utility>
#include <entt/core/hashed_string.hpp>
#include "antara/gaming/core/real.path.hpp"
#include "antara/gaming/resources/resources.system.hpp"
#include "antara/gaming/sfml/resources.loader.hpp"

namespace antara::gaming::sfml
{
    class resources_manager
    {
    public:
        template<typename TLoader, typename TCache, typename ... TArgs>
        static auto load(TCache &cache, const char *id, TArgs &&...args)
        {
            const auto identifier = entt::hashed_string::value(id);
            if (contains<TCache>(cache, id)) {
                return get<TCache>(cache, id);
            }
            return cache.template load<TLoader>(identifier, std::forward<TArgs>(args)...);
        }

        template<typename TCache>
        static auto get(TCache &cache, const char *id)
        {
            const auto identifier = entt::hashed_string::value(id);
            return cache.handle(identifier);
        }

        template<typename TCache>
        static auto contains(TCache &cache, const char *id) noexcept
        {
            const auto identifier = entt::hashed_string::value(id);
            return cache.contains(identifier);
        }

        texture_handle load_texture(const char *resource_id, bool smooth = true);

        font_handle load_font(const char *resource_id);

        sound_handle load_sound(const char *resource_id);

        music_handle load_music(const char* resource_id);

    private:
        const std::filesystem::path assets_path_{antara::gaming::core::assets_real_path()};
        std::filesystem::path musics_path_{assets_path_ / "musics"};
        std::filesystem::path textures_path_{assets_path_ / "textures"};
        std::filesystem::path fonts_path_{assets_path_ / "fonts"};
        std::filesystem::path sounds_path{assets_path_ / "sounds"};
        textures_cache textures_cache_;
        musics_cache musics_cache_;
        sounds_cache sounds_cache_;
        fonts_cache fonts_cache_;
    };
    
    using resources_system = resources::system<resources_manager>;
}