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
#include <antara/gaming/core/safe.refl.hpp>
#include <antara/gaming/world/world.app.hpp>

class example_system final : public antara::gaming::ecs::post_update_system<example_system>
{
public:
    example_system(entt::registry& entity_registry, entt::dispatcher& dispatcher) noexcept : system(entity_registry,
                                                                                                  dispatcher)
    {
        //! Here you can initialize your system, adding entities etc
    }

    void update() noexcept final
    {
        //! Your game logic here
        nb_iteration += 1;
        std::cout << "nb_iteration: " << nb_iteration << "\n";
        if (nb_iteration == 10ull) {
            std::cout << "Maximum iteration reached, leaving game now\n";
            this->dispatcher_.trigger<antara::gaming::event::quit_game>(0);
        }
    }

private:
    std::size_t nb_iteration{0ull};
};

REFL_AUTO(type(example_system));

class my_world_example : public antara::gaming::world::app
{
public:
    my_world_example() noexcept
    {
        this->system_manager_.create_system<example_system>(); //! Here we load our system to use it.
    }
};

int main()
{
    my_world_example world;
    return world.run();
}