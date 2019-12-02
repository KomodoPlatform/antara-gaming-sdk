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
    struct item {
        int id{0};
        std::string name;
        std::string description;
        int quantity{0};
        float price{0.f};
    };

public:
    gui_system(entt::registry &registry) noexcept : system(registry) {
        // Fill store
        {
            int id = 0;
            store_items.push_back({id++, "Burgundy", "Méo-Camuzet ''Vosne Romanée Les Chaumes'' Burgundy, France 2009", 5, 1900});
            store_items.push_back({id++, "Saint-Émilion", "Château Vieux “Château des Combes” Grand Cru Classé Saint-Émilion, France 2011", 5, 390});
            store_items.push_back({id++, "Entre Deux-Mers", "Château Labatut, Cuvée Prestige,Bordeaux Supérieur Entre Deux-Mers, France 2009", 5, 1900});
            store_items.push_back({id++, "Médoc", "Château la Pirouette, Cru Bourgeois AOC, Médoc, France 2009", 5, 260});
            store_items.push_back({id++, "Saint-Julien", "Château Léoville Las Cases, Grand Cru Classé, Saint-Julien, France 2004", 5, 2850});
            store_items.push_back({id++, "Moulis", "L’Héritage De Chasse Spleen, AOC Moulis, Moulis, France 2003", 5, 950});
            store_items.push_back({id++, "Margaux", "Château Giscours, La Siréne de Giscours, Grand Cru Classé, 2008", 5, 950});
            store_items.push_back({id++, "Pessac-Léognan", "Château Haut Brion, A.O.C. Pessac-Léognan, 1er Grand Cru Classé, Pessac-Léognan, France 1999", 5, 7000});
            store_items.push_back({id++, "Pauillac", "La Rose Bel Air, Grand Vin de Bordeaux, Pauillac, France 2008", 5, 390});
            store_items.push_back({id++, "Cahors", "Château du Cédre “Le Cédre” AOC Cahors, France 2006", 5, 950});
            store_items.push_back({id++, "Rhône", "Mourvedre, Grenache, Syrah, Counoise, Cinsault, Rhône, France 2010", 5, 1850});
            store_items.push_back({id++, "Tuscany", "Cecchi “Chianti” Sangiovese, DOCG,Tuscany, Italy 2013", 5, 320});
            store_items.push_back({id++, "Bolgheri Superiore", "Tenuta dell’Ornellaia “Ornellaia” Cabernet Sauvignon, Merlot, Cabernet Franc, P.Verdot Bolgheri Superiore, DOC, Bolgheri, Tuscany 2010", 5, 1900});
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
                ImGui::PushID(it->id);

                // If button is clicked, remove this item, add it to the other list
                if(ImGui::Button(it->name.c_str())) {
                    target_list.push_back(*it);
                    it = items.erase(it);
                }
                // Iterate
                else ++it;

                ImGui::PopID();
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
                ImGui::PushID(it->id);

                // If button is clicked, remove this item, add it to the other list
                if(ImGui::Button(it->name.c_str())) {
                    target_list.push_back(*it);
                    it = items.erase(it);
                }
                // Iterate
                else ++it;

                ImGui::PopID();
            }

            ImGui::End();
        }
    }

    // Item lists
    std::vector<item> store_items;
    std::vector<item> inventory_items;
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