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

namespace antara::gaming::resources
{
    template<typename UnderlyingResourceManager>
    class system final : public ecs::logic_update_system<system<UnderlyingResourceManager>>
    {
    public:
        using resources_identifier = const char *;
        using TSystem = ecs::logic_update_system<system<UnderlyingResourceManager>>;

        system(entt::registry &registry) noexcept : TSystem::system(registry)
        {
            this->disable();
        }

        void update() noexcept final
        {

        }

        ~system() noexcept final = default;

    public:

        auto load_texture(resources_identifier id)
        {
            return underlying_resource_manager_.load_texture(id);
        }

        auto load_font(resources_identifier id)
        {
            return underlying_resource_manager_.load_font(id);
        }

        auto load_sound(resources_identifier id)
        {
            return underlying_resource_manager_.load_font(id);
        }

    private:
        UnderlyingResourceManager underlying_resource_manager_;
    };
}

REFL_AUTO(template(
                  (typename UnderlyingResourceManager), (antara::gaming::resources::system<UnderlyingResourceManager>)))