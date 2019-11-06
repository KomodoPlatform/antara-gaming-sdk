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

#include <loguru.hpp>
#include <range/v3/numeric.hpp>
#include <range/v3/action/remove_if.hpp>
#include <range/v3/view/filter.hpp>
#include "antara/gaming/ecs/system.manager.hpp"

namespace antara::gaming::ecs {
    system_manager::system_manager(entt::registry &registry, bool susbscribe_to_internal_events) noexcept
            : entity_registry_(
            registry), dispatcher_(this->entity_registry_.ctx<entt::dispatcher>()) {
        LOG_SCOPE_FUNCTION(INFO);
        if (susbscribe_to_internal_events) {
            this->dispatcher_.sink<event::add_base_system>().connect<&system_manager::receive_add_base_system>(*this);
            assert(not dispatcher_.sink<event::add_base_system>().empty());
        }
    }

    std::size_t system_manager::nb_systems(system_type sys_type) const noexcept {
        return systems_[sys_type].size();
    }

    std::size_t system_manager::nb_systems() const noexcept {
        return ranges::accumulate(systems_, size_t{0u}, [](size_t accumulator, auto &&vec) {
            return accumulator + vec.size();
        });
    }

    base_system &
    system_manager::add_system_(system_manager::system_ptr &&system,
                                antara::gaming::ecs::system_type sys_type) noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        DVLOG_F(loguru::Verbosity_INFO, "adding system {} in the system manager.", system->get_name());
        return *systems_[sys_type].emplace_back(std::move(system));
    }

    void system_manager::sweep_systems_() noexcept {
        ranges::for_each(systems_, [](auto &&vec_system) -> void {
            ranges::actions::remove_if(vec_system, &base_system::is_marked);
        });

        this->need_to_sweep_systems_ = false;
    }

    std::size_t system_manager::update() noexcept {
        if (not nb_systems() || not game_is_running)
            return 0u;

        std::size_t nb_systems_updated = 0u;
        timestep_.start_frame();
        nb_systems_updated += update_systems(system_type::pre_update);

        //LCOV_EXCL_START
        while (timestep_.is_update_required()) {
            nb_systems_updated += update_systems(system_type::logic_update);
            timestep_.perform_update();
        }
        //LCOV_EXCL_STOP

        nb_systems_updated += update_systems(system_type::post_update);

        if (need_to_sweep_systems_) {
            sweep_systems_();
        }

        for (auto &&current_sys_vec : systems_)
            for (auto &&current_sys: current_sys_vec)
                current_sys->post_update();

        //LCOV_EXCL_START
        if (not systems_to_add_.empty()) {
            while (not systems_to_add_.empty()) {
                auto sys_type = systems_to_add_.front()->get_system_type_rtti();
                this->add_system_(std::move(systems_to_add_.front()), sys_type);
                systems_to_add_.pop();
            }
        }
        //LCOV_EXCL_STOP

        return nb_systems_updated;
    }

    std::size_t system_manager::update_systems(system_type system_type_to_update) noexcept {
        std::size_t nb_systems_updated = 0ull;
        for (auto &&current_sys : systems_[system_type_to_update] | ranges::views::filter(&base_system::is_enabled)) {
            current_sys->update();
            nb_systems_updated += 1;
        }
        return nb_systems_updated;
    }

    void system_manager::receive_add_base_system(const ecs::event::add_base_system &evt) noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        assert(evt.system_ptr != nullptr);
        ecs::system_type sys_type = evt.system_ptr->get_system_type_rtti();
        if (not game_is_running) {
            this->add_system_(std::move(const_cast<event::add_base_system &>(evt).system_ptr), sys_type);
        } else {
            this->systems_to_add_.push(std::move(const_cast<event::add_base_system &>(evt).system_ptr));
        }
    }

    void system_manager::start() noexcept {
        LOG_SCOPE_FUNCTION(INFO);
        game_is_running = true;
    }

    system_manager::~system_manager() noexcept {
        LOG_SCOPE_FUNCTION(INFO);
    }
}