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
        int id;
        std::string name;
        std::string description;
        int quantity;
        int price;
    };

public:
    gui_system(entt::registry &registry) noexcept : system(registry) {
        // Fill store
        {
            int id = -1;
            ++id; store_items[id] = {id, "Burgundy", "Méo-Camuzet ''Vosne Romanée Les Chaumes'' Burgundy, France 2009", 5, 1900};
            ++id; store_items[id] = {id, "Saint-Émilion", "Château Vieux “Château des Combes” Grand Cru Classé Saint-Émilion, France 2011", 5, 390};
            ++id; store_items[id] = {id, "Entre Deux-Mers", "Château Labatut, Cuvée Prestige,Bordeaux Supérieur Entre Deux-Mers, France 2009", 5, 1900};
            ++id; store_items[id] = {id, "Médoc", "Château la Pirouette, Cru Bourgeois AOC, Médoc, France 2009", 5, 260};
            ++id; store_items[id] = {id, "Saint-Julien", "Château Léoville Las Cases, Grand Cru Classé, Saint-Julien, France 2004", 5, 2850};
            ++id; store_items[id] = {id, "Moulis", "L’Héritage De Chasse Spleen, AOC Moulis, Moulis, France 2003", 5, 950};
            ++id; store_items[id] = {id, "Margaux", "Château Giscours, La Siréne de Giscours, Grand Cru Classé, 2008", 5, 950};
            ++id; store_items[id] = {id, "Pessac-Léognan", "Château Haut Brion, A.O.C. Pessac-Léognan, 1er Grand Cru Classé, Pessac-Léognan, France 1999", 5, 7000};
            ++id; store_items[id] = {id, "Pauillac", "La Rose Bel Air, Grand Vin de Bordeaux, Pauillac, France 2008", 5, 390};
            ++id; store_items[id] = {id, "Cahors", "Château du Cédre “Le Cédre” AOC Cahors, France 2006", 5, 950};
            ++id; store_items[id] = {id, "Rhône", "Mourvedre, Grenache, Syrah, Counoise, Cinsault, Rhône, France 2010", 5, 1850};
            ++id; store_items[id] = {id, "Tuscany", "Cecchi “Chianti” Sangiovese, DOCG,Tuscany, Italy 2013", 5, 320};
            ++id; store_items[id] = {id, "Bolgheri Superiore", "Tenuta dell’Ornellaia “Ornellaia” Cabernet Sauvignon, Merlot, Cabernet Franc, P.Verdot Bolgheri Superiore, DOC, Bolgheri, Tuscany 2010", 5, 1900};
        }
    }

    void update() noexcept final {
        // Inventory
        {
            ImGui::Begin("Inventory");

            ImGui::Text("Balance: %d", inventory_balance);

            ImGui::Separator();

            // Items
            auto& items = inventory_items;
            auto& target_list = store_items;
            for(auto it = items.begin(); it != items.end(); ++it) {
                auto& item = it->second;
                ImGui::PushID(item.id);

                if(ImGui::Button(std::string(std::to_string(item.quantity) + "x " + item.name).c_str())) {}

                ImGui::PopID();
            }

            ImGui::End();
        }


        // Store
        {
            ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
            if (ImGui::Begin("Store")) {
                ImGui::Text("Balance: %d", store_balance);

                auto& items = store_items;
                auto& target_items = inventory_items;
                // Left
                static int selected = 0;
                static int curr_item_id = items.begin()->second.id;

                ImGui::BeginChild("left pane", ImVec2(150, 0), true);
                int i = 0;
                for (auto it = items.begin(); it != items.end(); ++it, ++i) {
                    auto& item = it->second;
                    if (ImGui::Selectable(item.name.c_str(), selected == i)) {
                        selected = i;
                        curr_item_id = item.id;
                    }
                }
                ImGui::EndChild();
                ImGui::SameLine();

                // Right
                auto& curr_item = items[curr_item_id];
                ImGui::BeginGroup();
                ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
                ImGui::TextWrapped("%s  x%d", curr_item.name.c_str(), curr_item.quantity);
                ImGui::Separator();
                if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
                    if (ImGui::BeginTabItem("Description")) {
                        ImGui::TextWrapped("%s", curr_item.description.c_str());
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Details")) {
                        ImGui::Text("ID: %d", curr_item.id);
                        ImGui::Text("Quantity: %d", curr_item.quantity);
                        ImGui::Text("Unit Price: %d", curr_item.price);
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::EndChild();

                bool has_stock = curr_item.quantity > 0;
                bool has_funds = inventory_balance >= curr_item.price;
                if (ImGui::Button(std::string(
                        !has_funds ? std::string("Not enough funds (" + std::to_string(curr_item.price) + " " + currency_name + ")") :
                        !has_stock ? "Out of stock" :
                        ("Buy 1 for " + std::to_string(curr_item.price) + " " + currency_name)).c_str())) {
                    if(has_stock && has_funds) {
                        // Pay
                        inventory_balance -= curr_item.price;
                        store_balance += curr_item.price;

                        // Lower the stock
                        --curr_item.quantity;

                        // Give the item
                        // If user already has this item, just increase the quantity
                        if(target_items.find(curr_item.id) != target_items.end()) {
                            ++target_items[curr_item.id].quantity;
                        }
                        // Else, add this item, set quantity as 1
                        else {
                            target_items[curr_item.id] = curr_item;
                            target_items[curr_item.id].quantity = 1;
                        }
                    }
                }
                ImGui::SameLine();
                ImGui::EndGroup();
            }
            ImGui::End();
        }
    }

    // Item lists
    int store_balance{0};
    int inventory_balance{20000};
    std::string currency_name{"RICK"};
    std::unordered_map<int, item> store_items;
    std::unordered_map<int, item> inventory_items;
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