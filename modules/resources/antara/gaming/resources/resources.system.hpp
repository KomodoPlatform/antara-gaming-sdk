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

//! SDK Headers
#include "antara/gaming/event/load.textures.hpp" ///< event::load_textures
#include "antara/gaming/ecs/system.hpp" ///< ecs::logic_update_system<system<UnderlyingResourceManager>>

namespace antara::gaming::resources
{
    template<typename UnderlyingResourceManager>
    class system final : public ecs::logic_update_system<system<UnderlyingResourceManager>>
    {
    public:
        using resources_identifier = const char *;
        using TSystem = ecs::logic_update_system<system<UnderlyingResourceManager>>;

        void on_load_textures(const event::load_textures& evt) noexcept
        {
            for (auto&& current_setting : evt.textures_settings) {
                this->load_texture(current_setting.texture_id.c_str());
            }
        }

        system(entt::registry &registry) noexcept : TSystem::system(registry)
        {
            this->dispatcher_.template sink<event::load_textures>().template connect<&system::on_load_textures>(*this);
            this->disable();
        }

        void update() noexcept final
        {

        }

        ~system() noexcept final = default;

    public:

        template <typename ... Args>
        auto load_texture(Args&& ...args)
        {
            return underlying_resource_manager_.load_texture(std::forward<Args>(args)...);
        }

        auto load_font(resources_identifier id)
        {
            return underlying_resource_manager_.load_font(id);
        }

        auto load_sound(resources_identifier id)
        {
            return underlying_resource_manager_.load_sound(id);
        }

        auto load_music(resources_identifier id)
        {
            return underlying_resource_manager_.load_music(id);
        }

    private:
        UnderlyingResourceManager underlying_resource_manager_;
    };
}

REFL_AUTO(template(
                  (typename UnderlyingResourceManager), (antara::gaming::resources::system<UnderlyingResourceManager>)))