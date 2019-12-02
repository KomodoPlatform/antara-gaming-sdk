//
// Created by Roman Szterg on 25/11/2019.
//

#include <imgui.h>

#include <thread>
#include <chrono>

#include <antara/gaming/world/world.app.hpp>
#include <antara/gaming/sfml/graphic.system.hpp>
#include <antara/gaming/sfml/resources.manager.hpp>
#include <antara/gaming/sfml/input.system.hpp>

using namespace antara::gaming;
using namespace std::chrono_literals;

class gui_system final : public ecs::post_update_system<gui_system> {
    struct item {
        int id;
        int quantity;
        int price;
        std::string name;
        std::string description;
    };

    struct inventory {
        int balance{0};
        int pending_balance{0};
        std::unordered_map<int, item> items;
    };

public:
    gui_system(entt::registry &registry) noexcept : system(registry) {
        // Fill store
        {
            int id = -1;
            ++id; store.items[id] = {id, 5, 1, "Roma", "Sometimes nothing beats a classic. We make ours the same way the original Italian masterpiece was prepared for Queen Margherita. A simple classic pie layered with fresh made whole milk mozzarella and finished with cold pressed olive oil fruttato drizzle and organic basil leaf chiffonade."};
            ++id; store.items[id] = {id, 5, 2, "5-Points", "Named after one of the oldest neighborhoods in Denver, The 5-Points is our homage to the diversity of this historic district. We use our house red sauce and layer this cheese pizza with 5 delectable varieties. Mozzarella, provolone, muenster, cheddar and creamy ricotta impastata cheese, finished with fresh basil chiffonade."};
            ++id; store.items[id] = {id, 5, 2, "The Boulder", "Our classic Colorado Pizza done veggie style, with green peppers, red onion, white button mushrooms, black olives and roma tomato."};
            ++id; store.items[id] = {id, 5, 2, "Twin Sisters", "Named after the Colorado peaks of the same name, this pie is a classic beauty. We layer a pizza with house cut premium imported Italian pepperoni and imported soppressata salami."};
            ++id; store.items[id] = {id, 5, 3, "Durango", "We take our classic pie and layer mozzarella and provolone and then add a wonderful garlic and fennel Italian sausage and white button mushrooms."};
            ++id; store.items[id] = {id, 5, 3, "The Baby Doe", "Baby Doe Tabor was a 1st generation Irish American and one of Colorado's most colorful historic figures. A true \"Rags to Riches...back to Rags\" kind of gal. As such we created a pizza that showed her humble roots as well as her panache for the extravagant. We layer mozzarella and provolone, then add thin sliced oven roasted organic red and yukon gold potatoes, thick cut applewood bacon, red onions and fresh picked rosemary. We finish it with a white truffle oil drizzle and Parmigiano-Reggiano. NO SAUCE- Roasted garlic & olive oil base."};
            ++id; store.items[id] = {id, 5, 3, "Kebler", "Colorado is home to one of the largest Aspen groves in the world, a single sprawling mass connected via a single root system located in Kebler Pass. After seeing those leaves in autumn we knew we had to name a pizza after it. So we take a pie cover it in our house pesto pomodoro sauce, mozzarella and add garlic fennel Italian sausage, green and tri-colored bell peppers."};
            ++id; store.items[id] = {id, 5, 4, "The Pueblo", "Pueblo is not only known as a a southern neighbor but also a haven for some of the state's best chili producers. We take a pie and spread a layer of creamy neufchatel cheese and fire roasted poblano peppers then cover in smoked gouda and mozzarella and finish with roasted corn."};
            ++id; store.items[id] = {id, 5, 4, "The Greek", "A base of EVOO, roasted garlic and Greek oregano is covered in a layer of mozzarella. We add artichoke hearts, organic spinach, imported Mykonos kalamata olives, imported Sicilian sun dried tomatoes, red onions and finished with fresh feta crumble."};
            ++id; store.items[id] = {id, 5, 4, "Molly Brown", "This pizza is just gold! Our house pomodoro covered in mozzarella and aged Tilamook sharp cheddar with applewood smoked bacon, roma tomatoes, roasted garlic and fresh organic spinach."};
            ++id; store.items[id] = {id, 5, 5, "Clyde & Chauncey", "The infamous \"Don of Denver\" and his brother ran their criminal enterprises out of a restaurant in North Denver where their mama would hand roll meatballs that were nearly as famous as her sons. So it only seemed right we named this classic after those two meatball mafiosi. We start with our classic pie and add a mozzarella and provolone blend then cover in sliced house made and hand rolled meatballs(70%beef/30%pork) and caramelized onions. We finish the pie with grated Parmigiano-Reggiano and fresh Italian parsley."};
            ++id; store.items[id] = {id, 5, 5, "1876", "We layer a pie with our red sauce, mozzarella and provolone and then add tri-colored bell peppers, white button mushrooms, red onions, black olives and imported pepperoni."};
            ++id; store.items[id] = {id, 5, 6, "Rankin Kelly", "El Paso County's 1st sheriff was a character legends were built on. Rough and formidable on the outside and surprisingly soft and tender on the inside, Rankin was a man to be admired. We realized we had a pizza just like him! We take our classic pie and layer globs of fresh ricotta impastata then cover with mozzarella and provolone blend and top with sliced house made meatballs, garlic & fennel Italian sausage and organic spinach."};
            ++id; store.items[id] = {id, 5, 6, "BLT", "We love the sandwich so much we had to make a pie to pay homage. We start with a roasted garlic & olive oil base then cover in mozzarella and provolone. We layer juicy Organic heirloom tomatoes and thick cut apple-wood bacon on top and finish with Organic arugula and a balsamic reduction drizzle."};
            ++id; store.items[id] = {id, 5, 7, "Mt Massive.", "In an attempt to woo the carnivores out there, we layer our classic pie with sliced house made meatballs, imported pepperoni, Italian sausage, spiral cut ham and thick cut applewood bacon."};
        }
    }

    ~gui_system() {
        // Wait for transactions
        for(auto& t : transaction_threads) t.join();
    }

    void display_balance(const inventory& inv, bool show_pending_count = false) {
        if(show_pending_count) ImGui::Text("Pending Transactions: %d", pending_transaction_count);
        ImGui::Text(std::string(std::string("Balance: %d") +
            std::string(inv.pending_balance == 0 ? "" : std::string(inv.pending_balance > 0 ? "(+" : "(") + "%d)")).c_str(),
                inv.balance, inv.pending_balance);
    }

    bool user_has_enough_funds(int price) {
        return user.balance >= price;
    }

    void complete_transaction(const int price) {
        user.pending_balance += price;
        store.pending_balance -= price;
        --pending_transaction_count;
    }

    void buy(const int price) {
        ++pending_transaction_count;

        // Drop from user balance
        user.balance -= price;
        user.pending_balance -= price;

        // Increase the store balance
        store.pending_balance += price;
        store.balance += price;

        // Simulate pending
        transaction_threads.emplace_back([this, price]{
            std::this_thread::sleep_for(2s);
            complete_transaction(price);
        });
    }

    void transfer(item& store_item) {
        // Pay
        buy(store_item.price);

        // Lower the stock
        --store_item.quantity;

        // Give the item
        // If user already has this item, just increase the quantity
        if(user.items.find(store_item.id) != user.items.end()) {
            ++user.items[store_item.id].quantity;
        }
            // Else, add this item, set quantity as 1
        else {
            user.items[store_item.id] = store_item;
            user.items[store_item.id].quantity = 1;
        }
    }

    void update() noexcept final {
        // Inventory
        {
            ImGui::Begin("Inventory");

            display_balance(user);

            ImGui::Separator();

            // Items
            auto& items = user.items;
            auto& target_list = store.items;
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
                display_balance(store, true);

                auto& items = store.items;

                // Left
                static int curr_item_id = items.begin()->second.id;
                static int selected = 0;

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
                        ImGui::Text("Unit Price: %d %s", curr_item.price, currency_name.c_str());
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::EndChild();

                bool has_stock = curr_item.quantity > 0;
                bool has_funds = user_has_enough_funds(curr_item.price);
                if (ImGui::Button(std::string(
                        !has_funds ? std::string("Not enough funds (" + std::to_string(curr_item.price) + " " + currency_name + ")") :
                        !has_stock ? "Out of stock" :
                        ("Buy 1 for " + std::to_string(curr_item.price) + " " + currency_name)).c_str())) {
                    if(has_stock && has_funds) {
                        transfer(curr_item);
                    }
                }
                ImGui::SameLine();
                ImGui::EndGroup();
            }
            ImGui::End();
        }
    }

    // Item lists
    inventory store{0};
    inventory user{60};
    std::string currency_name{"RICK"};

    int pending_transaction_count{0};
    std::vector<std::thread> transaction_threads;
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