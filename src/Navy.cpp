#include "Navy.h"

#include <memory>  // for allocator, __shared_ptr_access, shared_ptr
#include <string>  // for string, basic_string
#include <vector>  // for vector
#include <fstream>
#include <iostream>

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Radiobox, Horizontal, Menu, Renderer, Tab
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp" // for Element, separator, hbox, operator|, border

#include <nlohmann/json.hpp>

using namespace ftxui;
using json = nlohmann::json;

ftxui::Component JsonRenderer3::render(const json& json__, std::string& filePath) {

    current = json__;

    print_root_keys(json__, menu_entries_, menu_values_);

    auto buttons = 
        Container::Horizontal({
                Container::Vertical({
                    Menu(&menu_entries_, &value_),
                }),
                Renderer([&] {return separatorHeavy();}),
                Container::Vertical({
                    Menu(&menu_values_, &value_),
                })
            }) | border | CatchEvent([ & ](Event event) {
      if (event == Event::Backspace) {
            if (!history.empty()) {
                history.pop_back();
                if (!history.empty()) {
                    current = json__;
                    for (const auto& pointer : history) {
                        current = current[pointer];
                    }
                } else {
                    current = json__;
                }
                menu_entries_.clear();
                menu_values_.clear();
                print_root_keys(current, menu_entries_, menu_values_);
                select = value_;
            }
        return false;

      } else if (event == Event::ArrowDown) {
            if(select != menu_entries_.size() - 1){
                select += 1;
            }
        return false;

      } else if (event == Event::ArrowUp) {
            if(select != 0){
                select -= 1;
            }
        return false;

      } else if (event == Event::Return) {
            if (current.is_object() && current.contains(menu_entries_[select]) && (current.at(menu_entries_[select]).is_primitive() == false)) {

                history.push_back(json::json_pointer("/" + menu_entries_[select]));
                current = current.at(menu_entries_[select]);
                menu_entries_.clear();
                menu_values_.clear();

                print_root_keys(current, menu_entries_, menu_values_);
                select = value_;
            } else if (current.is_array() && std::stoi(menu_entries_[select]) < current.size() && !current.at(std::stoi(menu_entries_[select])).is_primitive()){

                history.push_back(json::json_pointer("/" + menu_entries_[select]));
                current = current.at(std::stoi(menu_entries_[select]));
                menu_entries_.clear();
                menu_values_.clear();

                print_root_keys(current, menu_entries_, menu_values_);
                select = value_;
            }
        return false;
      }
      return false;
    });

    return buttons;
}
