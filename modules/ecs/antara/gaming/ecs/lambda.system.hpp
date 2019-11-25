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
#include "antara/gaming/ecs/system.hpp"


namespace antara::gaming::ecs
{
    struct ftor
    {
        std::function<void()> on_create{nullptr};
        std::function<void()> on_update{nullptr};
        std::function<void()> on_destruct{nullptr};
        std::function<void()> on_post_update{nullptr};
    };

    template<typename SystemType>
    class lambda_system final : public ecs::system<lambda_system<SystemType>, SystemType>
    {
        using TSystem = ecs::system<lambda_system<SystemType>, SystemType>;
        ftor lambda_contents_;
        std::string lambda_name_{""};
    public:
        lambda_system(entt::registry &registry, ftor lambda_contents, std::string lambda_name = "") noexcept
                : TSystem::system(registry),
                  lambda_contents_(std::move(lambda_contents)),
                  lambda_name_(
                          std::move(lambda_name))
        {
            if (lambda_contents_.on_create != nullptr) {
                lambda_contents_.on_create();
            }
        }

        void update() noexcept final
        {
            if (lambda_contents_.on_update != nullptr) {
                lambda_contents_.on_update();
            }
        }

        ~lambda_system() noexcept
        {
            if (lambda_contents_.on_destruct != nullptr) {
                lambda_contents_.on_destruct();
            }
        }

        void post_update() noexcept final
        {
            if (lambda_contents_.on_post_update != nullptr) {
                lambda_contents_.on_post_update();
            }
        }
    };

    using lambda_post_system = lambda_system<ecs::st_system_post_update>;
    using lambda_pre_system = lambda_system<ecs::st_system_pre_update>;
    using lambda_logic_system = lambda_system<ecs::st_system_logic_update>;
}

REFL_AUTO(template(
                  (typename SystemType), (antara::gaming::ecs::lambda_system<SystemType>)))
