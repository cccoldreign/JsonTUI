#ifndef VERT_H
#define VERT_H

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

class VerticalTab : public JsonRenderer {
private:
    std::vector<Color> colors = {
        Color::RGB(0, 0, 0),
        Color::RGB(255, 0, 0),     // Красный
        Color::RGB(0, 128, 0),    // Зеленый
        Color::RGB(238, 130, 238),// Розовый
        Color::RGB(0, 0, 255),    // Синий
        Color::RGB(255, 165, 0),  // Оранжевый
        Color::RGB(75, 0, 130),   // Индиго
        Color::RGB(255, 0, 0),     // Красный
        Color::RGB(127, 255, 212),// Аквамариновый
        Color::RGB(255, 192, 203), // Розовый
        Color::RGB(0, 0, 0),
        Color::RGB(255, 0, 0),     // Красный
        Color::RGB(0, 128, 0),    // Зеленый
        Color::RGB(238, 130, 238),// Розовый
        Color::RGB(0, 0, 255),    // Синий
        Color::RGB(255, 165, 0),  // Оранжевый
        Color::RGB(75, 0, 130),   // Индиго
        Color::RGB(255, 0, 0),     // Красный
        Color::RGB(127, 255, 212),// Аквамариновый
        Color::RGB(255, 192, 203), // Розовый
        Color::RGB(0, 0, 0),
        Color::RGB(255, 0, 0),     // Красный
        Color::RGB(0, 128, 0),    // Зеленый
        Color::RGB(238, 130, 238),// Розовый
        Color::RGB(0, 0, 255),    // Синий
        Color::RGB(255, 165, 0),  // Оранжевый
        Color::RGB(75, 0, 130),   // Индиго
        Color::RGB(255, 0, 0),     // Красный
        Color::RGB(127, 255, 212),// Аквамариновый
        Color::RGB(255, 192, 203), // Розовый
        Color::RGB(0, 0, 0),
        Color::RGB(255, 0, 0),     // Красный
        Color::RGB(0, 128, 0),    // Зеленый
        Color::RGB(238, 130, 238),// Розовый
        Color::RGB(0, 0, 255),    // Синий
        Color::RGB(255, 165, 0),  // Оранжевый
        Color::RGB(75, 0, 130),   // Индиго
        Color::RGB(255, 0, 0),     // Красный
        Color::RGB(127, 255, 212),// Аквамариновый
        Color::RGB(255, 192, 203), // Розовый
    };

    MenuEntryOption Colored(ftxui::Color c) {
    MenuEntryOption option;
    option.transform = [c](EntryState state) {
        state.label = (state.active ? "> " : "  ") + state.label;
        Element e = text(state.label) | color(c);
        if (state.focused)
        e = e | inverted;
        if (state.active)
        e = e | bold;
        return e;
    };
    return option;
    }

    size_t find_max_length(const nlohmann::json &j) {
        size_t max_length = 0;
        for(auto &element : j.items()){
            if(element.value().is_string()){
                max_length = std::max(max_length, element.value().get<std::string>().size());
            }
            else if(element.value().is_object() || element.value().is_array()){
                max_length = std::max(max_length, find_max_length(element.value()));
            }
        }
        return max_length;
    }

    void black_table(const std::string& key, const json& value, int indent_level, 
                            Component& container, size_t& max_length, size_t& max_key_length) {
        std::string str_in;
        for (int i = 0; i < indent_level; ++i) {
            str_in += " ";
        }
        int  size__ = key.length();
        if(!value.is_primitive()){
            if (!key.empty()) {
                container->Add(
                    MenuEntry(str_in + key, 
                    Colored(Color::White)) //| size(WIDTH, GREATER_THAN, max_key_length + indent_level)
                );
            }
            if ((value.is_object() || value.is_array()) && !key.empty()) {
                container->Add(Renderer([&] {return separatorHeavy();}));
            }
            }else{
                container->Add(Container::Horizontal({
                    MenuEntry(str_in + key,
                    Colored(Color::White)), //| size(WIDTH, EQUAL, max_key_length + indent_level + 4),
                    Renderer([&] {return separatorHeavy();}),
                    MenuEntry(to_string(value),
                    Colored(Color::Yellow1)),// | size(WIDTH, EQUAL, max_length + indent_level + 4)
                }));
                container->Add(Renderer([&] {return separatorHeavy();}));
            }
        if (value.is_object() || value.is_array()) {
            for (const auto& [key, val] : value.items()) {
                black_table(key, val, indent_level + 1, container, max_length, max_key_length);
            }
        } 
    }
    void color_table(const std::string& key, const json& value, int indent_level, 
                    Component& container, size_t& max_length, size_t& max_key_length) {
        std::string str_in;
        for (int i = 0; i < indent_level; ++i) {
            str_in += " ";
        }
        int  size__ = key.length();
        if(!value.is_primitive()){
            if (!key.empty()) {
                container->Add(
                    MenuEntry(str_in + key, 
                    Colored(colors[indent_level]))//  | size(WIDTH, GREATER_THAN, max_key_length + indent_level)
                );
            }
                if ((value.is_object() || value.is_array()) && !key.empty()) {
                    container->Add(Renderer([&] {return separatorHeavy();}));
                }
            }else{
                container->Add(Container::Horizontal({
                    MenuEntry(str_in + key,
                    Colored(colors[indent_level])), // | size(WIDTH, GREATER_THAN, max_key_length + indent_level),
                    Renderer([&] {return separatorHeavy();}),
                    MenuEntry(to_string(value),
                    Colored(Color::Yellow1)) // | size(WIDTH, GREATER_THAN, max_key_length + indent_level)
                }));
                container->Add(Renderer([&] {return separatorHeavy();}));
        }
        if (value.is_object() || value.is_array()) {
            for (const auto& [key, val] : value.items()) {
                color_table(key, val, indent_level + 1, container, max_length, max_key_length);
            }
        } 
    }
public:
    bool flag1_;

    VerticalTab(bool flag1);

    ftxui::Component render(const nlohmann::json& json, std::string& filePath) override;
};

#endif 