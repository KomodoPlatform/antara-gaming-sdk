#include <random>
#include <antara/gaming/scenes/scene.manager.hpp>
#include <antara/gaming/sfml/graphic.system.hpp>
#include <antara/gaming/sfml/input.system.hpp>
#include <antara/gaming/sfml/resources.manager.hpp>
#include <antara/gaming/world/world.app.hpp>

// For convenience
using namespace antara::gaming;
using namespace std::string_literals;

// Game Scene
class game_scene final : public scenes::base_scene {
public:
    game_scene(entt::registry &registry) noexcept : base_scene(registry) {
    }

    // Scene name
    std::string scene_name() noexcept final {
        return "game_scene";
    }

private:
    // Update the game every tick
    void update() noexcept final {
    }
};

// Game world
struct flappy_bird_world : world::app {
    // Game entry point
    flappy_bird_world() noexcept {
        // Load the graphical system
        auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

        // Load the resources system
        entity_registry_.set<sfml::resources_system>(entity_registry_);

        // Load the input system with the window from the graphical system
        system_manager_.create_system<sfml::input_system>(graphic_system.get_window());

        // Load the scenes manager
        auto &scene_manager = system_manager_.create_system<scenes::manager>();

        // Change the current_scene to "game_scene" by pushing it.
        scene_manager.change_scene(std::make_unique<game_scene>(entity_registry_), true);
    }
};

int main() {
    // Declare the world
    flappy_bird_world game;

    // Run the game
    return game.run();
}