#ifndef NAVY_H
#define NAVY_H

#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include <nlohmann/json.hpp>

using namespace ftxui;
using json = nlohmann::json;

#include "JsonRenderer.h"

class Navy : public JsonRenderer {
public:
    void print_value(const json& j, std::vector<std::string>& menu, std::vector<std::string>& value) {
        if (j.is_primitive()) {
            value.push_back(to_string(j));
        }
        if(j.is_object()){
            value.push_back(" { object }");
        }
        if(j.is_array()){
            value.push_back(" [ array ]");
        }
    }
    void print_root_keys(const json& j, std::vector<std::string>& menu, std::vector<std::string>& value) {
        if (j.is_object()) {
            for (auto it = j.begin(); it != j.end(); ++it) {
                menu.push_back(it.key());
                print_value(it.value(), menu, value);
            } 
        } else if (j.is_array()) {
            for (size_t i = 0; i < j.size(); i++) {
                menu.push_back(std::to_string(i));
                print_value(j.at(i), menu, value);
            }
        }
    }
    ftxui::Component render(const nlohmann::json& json__, std::string& filePath) override;

    protected:
    int value_ = 0;
    int select = 0;

    json current;
    
    std::vector<json::json_pointer> history;
    std::vector<std::string> menu_values_;
    std::vector<std::string> menu_entries_;
};

#endif 