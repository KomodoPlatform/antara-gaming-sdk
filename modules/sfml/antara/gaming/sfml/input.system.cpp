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

#include <SFML/Window/Event.hpp>
#include <antara/gaming/graphics/component.canvas.hpp>

#if defined(IMGUI_AND_SFML_ENABLED)

#    include <imgui-SFML.h>

#endif

#include "antara/gaming/event/key.pressed.hpp"
#include "antara/gaming/event/key.released.hpp"
#include "antara/gaming/event/mouse.button.pressed.hpp"
#include "antara/gaming/event/mouse.button.released.hpp"
#include "antara/gaming/event/mouse.moved.hpp"
#include "antara/gaming/event/quit.game.hpp"
#include "antara/gaming/event/window.resized.hpp"
#include "antara/gaming/sfml/input.system.hpp"
#include "graphic.system.hpp"
#include <antara/gaming/timer/time.step.hpp>

namespace antara::gaming::input
{
    bool
    is_key_pressed(input::key key) noexcept
    {
        return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
    }

    bool
    is_mouse_button_pressed(input::mouse_button button) noexcept
    {
        return sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(button));
    }

    math::vec2i
    get_mouse_position(bool relative_to_the_window) noexcept
    {
        if (not relative_to_the_window)
        {
            auto [x, y] = sf::Mouse::getPosition();
            return {x, y};
        }
        // TODO: handle the window
        return {0, 0};
    }
} // namespace antara::gaming::input

namespace antara::gaming::sfml
{
    void
    input_system::on_fill_mouse_position(const event::get_mouse_position& evt) noexcept
    {
        if (not evt.relative_to_the_window)
        {
            auto [x, y] = sf::Mouse::getPosition();
            evt.pos     = math::vec2i{x, y};
            return;
        }
        auto [x, y] = sf::Mouse::getPosition(this->window_);
        evt.pos     = math::vec2i{x, y};
    }


    void
    input_system::on_set_mouse_position(const event::set_mouse_position& evt) noexcept
    {
        if (not evt.relative_to_the_window)
        {
            sf::Mouse::setPosition(sf::Vector2i{evt.to.x(), evt.to.y()});
        }
        else
        {
            sf::Mouse::setPosition(sf::Vector2i{evt.to.x(), evt.to.y()}, this->window_);
        }
    }

    input_system::input_system(entt::registry& registry, sf::RenderWindow& window) noexcept : system(registry), window_(window)
    {
        this->dispatcher_.sink<event::get_mouse_position>().connect<&input_system::on_fill_mouse_position>(*this);
        this->dispatcher_.sink<event::set_mouse_position>().connect<&input_system::on_set_mouse_position>(*this);
    }

    auto
    input_system::translate_window_coord(const int x, const int y) const
    {
        // Translate Window pixel position to coordinate using the view
        auto window_pos = window_.mapPixelToCoords(sf::Vector2i(x, y));

        // Translate position of RT, if half of it is out of the screen for example, still find the correct position
        auto& rt_sprite = this->entity_registry_.ctx<sf::Sprite>();
        auto  translated_coord =
            window_pos +
            sf::Vector2f((rt_sprite.getGlobalBounds().width - window_.getSize().x) * 0.5f, (rt_sprite.getGlobalBounds().height - window_.getSize().y) * 0.5f);

        // Scale the position to RT level
        sf::Vector2i rt_pos(translated_coord.x / rt_sprite.getScale().x, translated_coord.y / rt_sprite.getScale().y);

        // Translate RT pixel position to coordinate using the view
        auto& rt       = this->entity_registry_.ctx<sf::RenderTexture>();
        auto  rt_coord = rt.mapPixelToCoords(rt_pos);

        return std::make_tuple(rt_coord, window_pos);
    }

    void
    input_system::update() noexcept
    {
        sf::Event evt{};
        while (window_.pollEvent(evt))
        {
#if defined(IMGUI_AND_SFML_ENABLED)
            ImGui::SFML::ProcessEvent(evt);
#endif
            switch (evt.type)
            {
            case sf::Event::Closed:
                this->dispatcher_.trigger<event::quit_game>(0);
                break;
            case sf::Event::Resized:
            {
                auto& canvas                         = this->entity_registry_.ctx<graphics::canvas_2d>();
                auto&& [window_width, window_height] = canvas.window.size;
                window_width                         = evt.size.width;
                window_height                        = evt.size.height;
                canvas.reset_canvas();
                this->dispatcher_.trigger<event::window_resized>();
            }
            break;
            case sf::Event::LostFocus:
                break;
            case sf::Event::GainedFocus:
                break;
            case sf::Event::TextEntered:
                break;
            case sf::Event::KeyPressed:
                this->dispatcher_.trigger<event::key_pressed>(
                    static_cast<input::key>(evt.key.code), evt.key.alt, evt.key.control, evt.key.shift, evt.key.system);
                break;
            case sf::Event::KeyReleased:
                this->dispatcher_.trigger<event::key_released>(
                    static_cast<input::key>(evt.key.code), evt.key.alt, evt.key.control, evt.key.shift, evt.key.system);
                break;
            case sf::Event::MouseWheelMoved:
                break;
            case sf::Event::MouseWheelScrolled:
                break;
            case sf::Event::MouseButtonPressed:
            {
                auto [rt_coord, window_pos] = translate_window_coord(evt.mouseButton.x, evt.mouseButton.y);
                this->dispatcher_.trigger<event::mouse_button_pressed>(
                    static_cast<input::mouse_button>(evt.mouseButton.button), rt_coord.x, rt_coord.y, window_pos.x, window_pos.y);
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                auto [rt_coord, window_pos] = translate_window_coord(evt.mouseButton.x, evt.mouseButton.y);
                this->dispatcher_.trigger<event::mouse_button_released>(
                    static_cast<input::mouse_button>(evt.mouseButton.button), rt_coord.x, rt_coord.y, window_pos.x, window_pos.y);
                break;
            }
            case sf::Event::MouseMoved:
            {
                auto [rt_coord, window_pos] = translate_window_coord(evt.mouseMove.x, evt.mouseMove.y);
                this->dispatcher_.trigger<event::mouse_moved>(rt_coord.x, rt_coord.y, window_pos.x, window_pos.y);
                break;
            }
            case sf::Event::MouseEntered:
                break;
            case sf::Event::MouseLeft:
                break;
            case sf::Event::JoystickButtonPressed:
                break;
            case sf::Event::JoystickButtonReleased:
                break;
            case sf::Event::JoystickMoved:
                break;
            case sf::Event::JoystickConnected:
                break;
            case sf::Event::JoystickDisconnected:
                break;
            case sf::Event::TouchBegan:
                break;
            case sf::Event::TouchMoved:
                break;
            case sf::Event::TouchEnded:
                break;
            case sf::Event::SensorChanged:
                break;
            case sf::Event::Count:
                break;
            }
        }
#if defined(IMGUI_AND_SFML_ENABLED)
        sf::Time delta_time = sf::seconds(timer::time_step::get_fixed_delta_time());
        ImGui::SFML::Update(window_, delta_time);
#endif
    }

} // namespace antara::gaming::sfml
