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

//! C System Headers
#include <cstddef> ///< std::size_t

//! C++ System Headers
#include <string> ///< std::string
#include <vector> ///< std::vector

//! Dependencies Headers
#include <entt/entity/entity.hpp>   ///< entt::entity
#include <entt/entity/registry.hpp> ///< entt::registry

//! SDK Headers
#include "antara/gaming/animation2d/antara.animation2d.ranged_anim.hpp" ///< animation2d::ranged_anim
#include "antara/gaming/animation2d/component.animation.2d.hpp"         ///< animation2d::anim_component
#include "antara/gaming/ecs/system.hpp"                                 ///< ecs::logic_update_system
#include "antara/gaming/event/fill.image.properties.hpp"                ///< event::fill_image_properties
#include "antara/gaming/graphics/component.sprite.hpp"                  ///< graphics::sprite

namespace antara::gaming::animation2d
{
    class anim_system final : public ecs::logic_update_system<anim_system>
    {
        //! Private forwarding
        struct anim_internal;

        //! Private typedefs
        using frame_array        = std::vector<graphics::rect>;
        using animation_registry = std::unordered_map<std::string, anim_internal>;

        //! Private data structure
        struct anim_internal
        {
            frame_array frames;
            std::string texture_appearance;
        };

        //! Private member functions
        void on_anim_cmp_create(entt::entity entity, entt::registry& registry, const anim_component& anim_cmp) noexcept;

        //! Private field
        animation_registry animations_;

      public:
        //! Constructors
        explicit anim_system(entt::registry& registry) noexcept;

        //! Public member functions
        void add_animation(
            std::string animation_id, const std::string& texture_appeareance, std::size_t nb_columns, std::size_t nb_lines, std::size_t nb_anims) noexcept;

        void add_animation(const std::string& texture_appeareance, std::size_t nb_columns, std::size_t nb_lines, const ranged_anim& ranged_animation) noexcept;

        void add_animations(
            const std::string& texture_appearance, std::size_t nb_columns, std::size_t nb_lines, const ranged_anim_array& ranged_animations) noexcept;

        void update() noexcept final;
    };
} // namespace antara::gaming::animation2d

REFL_AUTO(type(antara::gaming::animation2d::anim_system));