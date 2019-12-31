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

#include "antara/gaming/sfml/audio.system.hpp"
#include "antara/gaming/audio/component.music.hpp"
#include "antara/gaming/audio/component.sound.effect.hpp"
#include "antara/gaming/sfml/component.audio.hpp"
#include <SFML/Audio.hpp>

namespace
{
    using namespace antara::gaming;

    void
    on_music_construct([[maybe_unused]] entt::entity entity, entt::registry& registry, audio::music& music) noexcept
    {
        auto& resources_system = registry.ctx<sfml::resources_system>();
        auto  handle           = resources_system.load_music(music.music_id.c_str());
        handle->setPitch(music.pitch);
        handle->setVolume(music.volume);
        handle->setLoop(music.loop);
        handle->setAttenuation(music.attenuation);
        handle->setMinDistance(music.minimum_distance);
        handle->setRelativeToListener(music.relative_to_listener);
        switch (music.music_status)
        {
        case audio::wait_for_first_run:
            break;
        case audio::stopped:
            handle->stop();
            break;
        case audio::paused:
            handle->pause();
            break;
        case audio::playing:
            if (handle->getStatus() != sf::Sound::Playing)
            {
                handle->play();
            }
            break;
        }
    }

    void
    on_sound_effect_construct(entt::entity entity, entt::registry& registry, audio::sound_effect& snd) noexcept
    {
        auto&      resources_system = registry.ctx<sfml::resources_system>();
        auto       handle           = resources_system.load_sound(snd.sound_id.c_str());
        sf::Sound& sfml_sound       = registry.assign_or_replace<sfml::component_sound>(entity).sound;
        sfml_sound.setBuffer(handle.get());
        sfml_sound.setPitch(snd.pitch);
        sfml_sound.setVolume(snd.volume);
        sfml_sound.setAttenuation(snd.attenuation);
        sfml_sound.setLoop(snd.loop);
        sfml_sound.setMinDistance(snd.minimum_distance);
        sfml_sound.setRelativeToListener(snd.relative_to_listener);
        switch (snd.sound_status)
        {
        case audio::stopped:
            sfml_sound.stop();
            break;
        case audio::paused:
            sfml_sound.pause();
            break;
        case audio::playing:
            sfml_sound.play();
            break;
        case audio::wait_for_first_run:
            assert(sfml_sound.getStatus() != sf::Sound::Playing);
            break;
        }
    }
} // namespace

namespace antara::gaming::sfml
{
    audio_system::audio_system(entt::registry& registry) noexcept : system(registry)
    {
        this->entity_registry_.on_construct<audio::sound_effect>().connect<on_sound_effect_construct>();
        this->entity_registry_.on_replace<audio::sound_effect>().connect<on_sound_effect_construct>();
        this->entity_registry_.on_construct<audio::music>().connect<on_music_construct>();
        this->entity_registry_.on_replace<audio::music>().connect<on_music_construct>();
    }

    void
    audio_system::update() noexcept
    {
        auto snd_effect_functor = [this](auto entity, component_sound& sfml_sound, audio::sound_effect& snd_effect) {
            if (snd_effect.sound_status != audio::wait_for_first_run and sfml_sound.sound.getStatus() == sf::Sound::Stopped and not snd_effect.loop)
            {
                snd_effect.on_finish();
                if (not snd_effect.recycling)
                {
                    this->entity_registry_.destroy(entity);
                }
            }
        };
        this->entity_registry_.view<sfml::component_sound, audio::sound_effect>().each(snd_effect_functor);
    }
} // namespace antara::gaming::sfml