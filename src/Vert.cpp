#include "Vert.h"
#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include <nlohmann/json.hpp>

using namespace ftxui;
using json = nlohmann::json;

JsonRenderer2::JsonRenderer2(bool flag1)
    : flag1_(flag1) {}

Component JsonRenderer2::render(const nlohmann::json& json, std::string& filePath) {
   

    size_t max_length = find_max_length(json);
    size_t max_key_length = 0;
    for (const auto& item : json) {
        for (const auto& [key, value] : item.items()) {
            if (value.is_string()) {
                max_key_length = std::max(max_key_length, value.get<std::string>().length());
            }
        }
    }

    auto screen = ScreenInteractive::FitComponent();

    auto container = Container::Vertical({});
    if(flag1_ == true){
        color_table("", json, 0, container, max_length, max_key_length);
    } else {
        black_table("", json, 0, container, max_length, max_key_length);
    }


    auto containers = Container::Vertical({
                    Container::Vertical({
                                container
                            }) | frame | vscroll_indicator,
                    }) | borderHeavy //| size(HEIGHT, LESS_THAN, 30) 
                    //| size(WIDTH, EQUAL, max_length + max_key_length + 8);
;
    return containers;
}