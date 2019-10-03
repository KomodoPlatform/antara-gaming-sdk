#include <antara/gaming/world/world.app.hpp>

using namespace antara::gaming;

class my_world : public world::app
{
public:
    my_world() noexcept
    {

    }
};

int main()
{
    my_world world;
    return world.run();
}