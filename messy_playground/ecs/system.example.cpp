#include <iostream>
#include <antara/gaming/world/world.app.hpp>

using namespace antara::gaming;

class pre_concrete_system final : public ecs::pre_update_system<pre_concrete_system>
{
public:
    //! Here the constructor can take other additional arguments but the first two are mandatory
    pre_concrete_system(entt::registry &registry) noexcept : system(registry)
    {

    }

    void update() noexcept final
    {
        std::cout << "nb_iteration: " << (++nb_iteration) << "\n";
        if (nb_iteration == 10u) {
            this->dispatcher_.trigger<antara::gaming::event::quit_game>(0);
        }
    }

    ~pre_concrete_system() noexcept final = default;
private:
    std::size_t nb_iteration{0u};
};

REFL_AUTO(type(pre_concrete_system))

class my_world : public world::app
{
public:
    my_world() noexcept
    {
        //! Here we don't need to add any parameters for the constructor
        //! because the mandatory parameters are forwarded by default
        this->system_manager_.create_system<pre_concrete_system>();
    }
};

int main()
{
    my_world world;
    return world.run();
}