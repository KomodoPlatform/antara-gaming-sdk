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

#include <antara/gaming/world/world.app.hpp>
#include <antara/gaming/scenes/scene.manager.hpp>
#include <antara/gaming/sdl/graphic.system.hpp>

class my_world : public antara::gaming::world::app
{
public:
    my_world() noexcept
    {
        auto &graphic_system = this->system_manager_.create_system<antara::gaming::sdl::graphic_system>();
        //this->entity_registry_.set<antara::gaming::sfml::resources_system>(this->entity_registry_);
        //this->system_manager_.create_system<antara::gaming::sfml::audio_system>();
        //this->system_manager_.create_system<antara::gaming::sfml::input_system>(graphic_system.get_window());

    }
};

int main()
{
    my_world game_app;
    return game_app.run();
}


/*
#include <SDL2/SDL.h>

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    //SDL_Event ev;
    while (true)
    {
        SDL_PollEvent(&ev);
        if (ev.type == SDL_QUIT)
        {
            break;
        }
    }
    SDL_Quit();
}*/