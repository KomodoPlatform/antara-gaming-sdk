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

#include <iostream>
#include <SFML/Audio.hpp>
#include "antara/gaming/sfml/audio.system.hpp"

namespace antara::gaming::sfml
{
    audio_system::audio_system(entt::registry &registry, entt::dispatcher &dispatcher) noexcept : system(registry,
                                                                                                         dispatcher)
    {
        this->dispatcher_.sink<play_sound_event>().connect<&audio_system::receive_sound_event>(*this);
    }

    void audio_system::update() noexcept
    {
        this->entity_registry_.view<component_sound>().each([this](auto &&entity, component_sound &cmp_sound) {
            if (cmp_sound.sound.getStatus() == sf::Sound::Stopped) {
                cmp_sound.on_finish();
                this->entity_registry_.destroy(entity);
            }
        });
    }

    void audio_system::receive_sound_event(const play_sound_event &evt) noexcept
    {
        auto sound_entity = this->entity_registry_.create();
        auto &cmp_sound = this->entity_registry_.assign<component_sound>(sound_entity);

        if (auto resource = const_cast<play_sound_event &>(evt).resource_mgr->load_sound(evt.sound_id); resource) {
            cmp_sound.sound.setBuffer(resource.get());
            cmp_sound.sound.play();
            cmp_sound.on_finish = std::move(evt.on_finish);
        }
    }

    play_sound_event::play_sound_event() noexcept : sound_id{nullptr}, resource_mgr{nullptr}, on_finish([](){})
    {

    }

    play_sound_event::play_sound_event(const char *sound_id_, resources_manager *resources_manager_,
                                       std::function<void()> on_finish) noexcept
            : sound_id(sound_id_),
              resource_mgr(
                      resources_manager_),
              on_finish(std::move(on_finish))
    {

    }
}