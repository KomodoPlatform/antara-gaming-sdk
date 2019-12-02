//
// Created by Roman Szterg on 25/11/2019.
//

#include <imgui.h>
#include <antara/gaming/world/world.app.hpp>
#include <antara/gaming/sfml/graphic.system.hpp>
#include <antara/gaming/sfml/resources.manager.hpp>
#include <antara/gaming/sfml/input.system.hpp>

using namespace antara::gaming;

class gui_system final : public ecs::post_update_system<gui_system> {
public:
    gui_system(entt::registry &registry) noexcept : system(registry) {
        // Fill store
        {
            for(int i = 0; i < 25; ++i)
                store_items.push_back("Item " + std::to_string(i));
        }
    }

    void update() noexcept final {
        // Store
        {
            ImGui::Begin("Store");

            // Items
            auto& items = store_items;
            auto& target_list = inventory_items;
            for(auto it = items.begin(); it != items.end();) {
                // If button is clicked, remove this item, add it to the other list
                if(ImGui::Button(it->c_str())) {
                    target_list.push_back(*it);
                    it = items.erase(it);
                }
                // Iterate
                else ++it;
            }

            ImGui::End();
        }

        // Inventory
        {
            ImGui::Begin("Inventory");

            // Items
            auto& items = inventory_items;
            auto& target_list = store_items;
            for(auto it = items.begin(); it != items.end();) {
                // If button is clicked, remove this item, add it to the other list
                if(ImGui::Button(it->c_str())) {
                    target_list.push_back(*it);
                    it = items.erase(it);
                }
                // Iterate
                else ++it;
            }

            ImGui::End();
        }
    }

    // Item lists
    std::vector<std::string> store_items;
    std::vector<std::string> inventory_items;
};

REFL_AUTO(type(gui_system))

class my_world : public world::app {
public:
    my_world() noexcept {
        auto &graphic_system = this->system_manager_.create_system<sfml::graphic_system>();
        this->entity_registry_.set<sfml::resources_system>(this->entity_registry_);
        this->system_manager_.create_system<sfml::input_system>(graphic_system.get_window());
        system_manager_.create_system<gui_system>();
        system_manager_.prioritize_system<gui_system, sfml::graphic_system>();
    }
};


int main() {
    my_world world;
    return world.run();
}