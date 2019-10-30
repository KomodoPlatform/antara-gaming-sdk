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

#include <SDL2/SDL_error.h>
#include "antara/gaming/sdl/sdl.error.hpp"

const char *antara::gaming::sdl::sdl_error_category::name() const noexcept
{
    return "sdl_error";
}

std::string antara::gaming::sdl::sdl_error_category::message(int) const noexcept
{
    return SDL_GetError();
}
