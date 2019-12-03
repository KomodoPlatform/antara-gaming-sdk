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

//! C++ System Headers
#include <ostream> ///< std::ostream

//! SDK Headers
#include "antara/gaming/core/safe.refl.hpp" ///< REFL_AUTO
#include "antara/gaming/math/vector.hpp" ///< math::vec2f
#include "antara/gaming/graphics/component.color.hpp" ///< graphics::fill_color

namespace antara::gaming::graphics {
    struct rectangle {
        math::vec2f size; //! width/height
        math::vec2f position;

        bool operator==(const rectangle &rhs) const {
            return size == rhs.size &&
                   position == rhs.position;
        }

        bool operator!=(const rectangle &rhs) const {
            return !(rhs == *this);
        }

        //! pos of the rectangle
        friend std::ostream &operator<<(std::ostream &os, const rectangle &rectangle) {
            os << "size: " << rectangle.size << " position: " << rectangle.position;
            return os;
        }
    };

    struct canvas_2d {
        enum scale_mode {
            none,
            stretch,
            crop,
            fit
        };

        rectangle window{.size = math::vec2f{1920.f, 1080.f},
                .position = math::vec2f::scalar(0.f)};
        rectangle canvas{.size = math::vec2f{1920.f, 1080.f},
                .position = math::vec2f::scalar(0.f)};
        rectangle canvas_texture;
        bool custom_canvas_width{true};
        bool custom_canvas_height{true};
        bool native_desktop_mode{false};
        bool is_fullscreen{false};
        bool vsync{false};
        bool mouse_visible{true};
        bool mouse_grabbed{false};
        scale_mode current_scaling_mode{fit};
        rectangle view_port;
        std::string window_title{"Antara Gaming SDK"};
        fill_color background_color{graphics::black};
        math::vec2f canvas_texture_scaling{1.0f, 1.0f};

        void reset_canvas() noexcept {
            auto&&[window_width, window_height] = window.size;
            auto&&[canvas_width, canvas_height] = canvas.size;
            auto&&[canvas_texture_width, canvas_texture_height] = canvas_texture.size;
            if (custom_canvas_width && custom_canvas_height) {
                canvas_texture_width = canvas_width;
                canvas_texture_height = canvas_height;
            } else if (custom_canvas_width) {
                canvas_texture_width = canvas_width;
                canvas_texture_height = canvas_width * window_height / window_width;
            } else if (custom_canvas_height) {
                canvas_texture_width = canvas_height * window_width / window_height;
                canvas_texture_height = canvas_height;
            } else {
                canvas_texture_width = window_width;
                canvas_texture_height = window_height;
            }

            view_port.position = math::vec2f::scalar(0.f);
            view_port.size = math::vec2f{window_width, window_height};

            switch (current_scaling_mode) {
                case none:
                    break;
                case stretch:
                    canvas_texture_scaling = math::vec2f{window_width / canvas_texture_width,
                                                         window_height / canvas_texture_height};
                    break;
                case crop:
                    canvas_texture_scaling = math::vec2f::scalar(
                            std::max(window_width / canvas_texture_width, window_height / canvas_texture_height));
                    break;
                case fit:
                    canvas_texture_scaling = math::vec2f::scalar(
                            std::min(window_width / canvas_texture_width, window_height / canvas_texture_height));
                    break;
            }
            canvas_texture.position = math::vec2f{window_width * 0.5f, window_height * 0.5f};
        }

        bool operator==(const canvas_2d &rhs) const {
            return window == rhs.window &&
                   canvas == rhs.canvas &&
                   current_scaling_mode == rhs.current_scaling_mode &&
                   view_port == rhs.view_port &&
                   window_title == rhs.window_title &&
                   background_color == rhs.background_color;
        }

        friend inline std::ostream &operator<<(std::ostream &os, const canvas_2d &d) {
            os << "window: " << d.window << " canvas: " << d.canvas << " canvas_texture: " << d.canvas_texture
               << " custom_canvas_width: " << d.custom_canvas_width << " custom_canvas_height: "
               << d.custom_canvas_height << " native_desktop_mode: " << d.native_desktop_mode << " is_fullscreen: "
               << d.is_fullscreen << " current_scaling_mode: " << d.current_scaling_mode << " view_port: "
               << d.view_port << " window_title: " << d.window_title << " background_color: " << d.background_color
               << " canvas_texture_scaling: " << d.canvas_texture_scaling;
            return os;
        }

        bool operator!=(const canvas_2d &rhs) const {
            return !(rhs == *this);
        }


        canvas_2d() = default;

        canvas_2d(const canvas_2d &other) = default;

        canvas_2d &operator=(const canvas_2d &other) = default;
    };
}

REFL_AUTO(type(antara::gaming::graphics::rectangle), field(size), field(position))
REFL_AUTO(type(antara::gaming::graphics::canvas_2d), field(window), field(canvas), field(current_scaling_mode),
          field(view_port), field(window_title), field(background_color))