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
#include "antara/gaming/event/quit.game.hpp"
#include "antara/gaming/event/mouse.button.pressed.hpp"
#include "antara/gaming/event/mouse.button.released.hpp"
#include "antara/gaming/event/mouse.moved.hpp"
#include "antara/gaming/event/key.released.hpp"
#include "antara/gaming/event/key.pressed.hpp"
#include "antara/gaming/event/window.resized.hpp"
#include "antara/gaming/sfml/input.system.hpp"
#include "graphic.system.hpp"

namespace antara::gaming::sfml
{
    input_system::input_system(entt::registry &registry, sf::RenderWindow &window) noexcept
            : system(registry), window_(window)
    {

    }

    auto input_system::translate_window_coord(const int x, const int y) const {
        // Translate Window pixel position to coordinate using the view
        auto window_pos = window_.mapPixelToCoords(sf::Vector2i(x, y));

        // Translate position of RT, if half of it is out of the screen for example, still find the correct position
        auto& rt_sprite = this->entity_registry_.ctx<sf::Sprite>();
        auto translated_coord = window_pos + sf::Vector2f((rt_sprite.getGlobalBounds().width - window_.getSize().x) * 0.5f, (rt_sprite.getGlobalBounds().height - window_.getSize().y) * 0.5f);

        // Scale the position to RT level
        sf::Vector2i rt_pos(translated_coord.x / rt_sprite.getScale().x, translated_coord.y / rt_sprite.getScale().y);

        // Translate RT pixel position to coordinate using the view
        auto& rt = this->entity_registry_.ctx<sf::RenderTexture>();
        auto rt_coord = rt.mapPixelToCoords(rt_pos);

        return std::make_tuple(rt_coord, window_pos);
    }

    void input_system::update() noexcept
    {
        sf::Event evt{};
        while (window_.pollEvent(evt)) {
            switch (evt.type) {
                case sf::Event::Closed:
                    this->dispatcher_.trigger<event::quit_game>(0);
                    break;
                case sf::Event::Resized: {
                    auto& canvas = this->entity_registry_.ctx<graphics::canvas_2d>();
                    auto &&[window_width, window_height] = canvas.window.size;
                    window_width = evt.size.width;
                    window_height = evt.size.height;
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
                    this->dispatcher_.trigger<event::key_pressed>(static_cast<input::key>(evt.key.code), evt.key.alt,
                                                                  evt.key.control, evt.key.shift, evt.key.system);
                    break;
                case sf::Event::KeyReleased:
                    this->dispatcher_.trigger<event::key_released>(static_cast<input::key>(evt.key.code), evt.key.alt,
                                                                   evt.key.control, evt.key.shift, evt.key.system);
                    break;
                case sf::Event::MouseWheelMoved:
                    break;
                case sf::Event::MouseWheelScrolled:
                    break;
                case sf::Event::MouseButtonPressed: {
                    auto [rt_coord, window_pos] = translate_window_coord(evt.mouseButton.x, evt.mouseButton.y);
                    this->dispatcher_.trigger<event::mouse_button_pressed>(
                            static_cast<input::mouse_button>(evt.mouseButton.button), rt_coord.x, rt_coord.y, window_pos.x, window_pos.y);
                    break;
                }
                case sf::Event::MouseButtonReleased: {
                    auto [rt_coord, window_pos] = translate_window_coord(evt.mouseButton.x, evt.mouseButton.y);
                    this->dispatcher_.trigger<event::mouse_button_released>(
                            static_cast<input::mouse_button>(evt.mouseButton.button), rt_coord.x, rt_coord.y, window_pos.x, window_pos.y);
                    break;
                }
                case sf::Event::MouseMoved: {
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
    }
}

