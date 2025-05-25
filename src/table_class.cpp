#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/captured_mouse.hpp>

#include <nlohmann/json.hpp>

using namespace ftxui;
using json = nlohmann::json;


class Table {
private:
    json jsonCur;
    std::string path;

    std::vector<std::string> radiobox = {"integer", "float", "string"};
    int selected = 0;

    struct Selection {
        json current_json;
        int selected_index;
        std::vector<std::string> keys_json;
        std::string valStr;
    };

    Component cont = Container::Horizontal({});
    std::vector<std::string> temp;
    std::vector<Selection> selections;
    int column = 0;
    std::string value;

    void collectKeys(const json& j, std::vector<Selection>& selections) {
        Selection S;
        S.current_json = j;

        if (j.is_object()) {
            for (auto it = j.begin(); it != j.end(); ++it) {
                S.keys_json.push_back(it.key());
            }
            S.selected_index = 0;
            S.valStr = "";
            selections.push_back(S);
            if (!S.keys_json.empty()) {
                auto first_key = S.keys_json[0];
                collectKeys(j[first_key], selections);
            }
        } else if (j.is_array()) {
            for (size_t i = 0; i < j.size(); ++i) {
                S.keys_json.push_back(std::to_string(i));
            }
            S.selected_index = 0;
            S.valStr = "";
            selections.push_back(S);
            if (!j.empty()) {
                collectKeys(j[0], selections);
            }
        } else {
            if (j.is_primitive()) {
                S.valStr = j.dump();
                selections.push_back(S);
                return;
            }
        }
    }

    void FillingTabs() {
        for (auto &it: selections) {
            cont -> Add(Container::Vertical({
                Menu( & it.keys_json, & it.selected_index) | vscroll_indicator | frame 
            }));

            if(it.valStr == ""){
                cont -> Add(Renderer([] {
                return separator();
            }));
        }
        }
        temp.clear();
        temp.push_back(selections.back().valStr);
        cont -> Add(Container::Vertical({
            Menu(&temp, &selections.back().selected_index) | vscroll_indicator | frame,
            Renderer([] {return separator();}),
            Input(&value, "value"),
            Renderer([] {return separator();}),
            Radiobox(&radiobox, &selected)
        }));
    }

    void ChangeDown(int column) {
        if (column < selections.size()) {
            Selection S = selections[column];
            if (S.selected_index < S.keys_json.size() - 1) {
                S.selected_index++;
                std::string key = S.keys_json[S.selected_index];
                json newLine = S.current_json.is_array() ? S.current_json[std::stoi(key)] : S.current_json[key];
                selections.erase(selections.begin() + 1 + column, selections.end());
                collectKeys(newLine, selections);
            }
        }
    }

    void ChangeUp(int column) {
        if (column < selections.size()) {
            Selection S = selections[column];
            if (S.selected_index > 0) {
                S.selected_index--;
                std::string key = S.keys_json[S.selected_index];
                json newLine = S.current_json.is_array() ? S.current_json[std::stoi(key)] : S.current_json[key];
                selections.erase(selections.begin() + 1 + column, selections.end());
                collectKeys(newLine, selections);
            }
        }
    }

    void changeVal(std::string& key__, std::vector<Selection>& sel__, std::string filePath){
        int ik = sel__.size() - 1;
        sel__[ik].valStr = key__;
        int it = sel__.size() - 2;
        json newValue;
        if (selected == 0) {
            try {
                newValue = std::stoi(key__);
            } catch (const std::exception& e) {
                throw std::invalid_argument("Ошибка преобразования строки в число: " + key__);
            }
        } if (selected == 1) {
           try {
                newValue = std::stof(key__);
            } catch (const std::exception& e) {
                throw std::invalid_argument("Ошибка преобразования строки в дробь: " + key__);
            }
        } if(selected == 2){
                newValue = key__;
        }
        if (sel__[it].current_json.is_object()) {
            sel__[it].current_json[sel__[it].keys_json[sel__[it].selected_index]] = newValue;
        } else if (sel__[it].current_json.is_array()) {
            int idx = sel__[it].selected_index;
            if (idx >= 0 && idx < sel__[it].current_json.size()) {
                sel__[it].current_json[idx] = newValue;
            } 
        }
        for (int i = it - 1; i >= 0; i--) {
            json j = {};
            if (sel__[i].current_json.is_object()) {
                j[sel__[i].keys_json[sel__[i].selected_index]] = sel__[i + 1].current_json;
                sel__[i].current_json.update(j, true);
            } 
            else if (sel__[i].current_json.is_array()) {
                int idx = sel__[i].selected_index;
                if (idx >= 0 && idx < sel__[i].current_json.size()) {
                    sel__[i].current_json[idx] = sel__[i + 1].current_json;
                } 
            } 
        }
        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "Failed to open fadsdsadasdasd\n";
        }
        file << sel__[0].current_json.dump(4);
        file.close();

        std::ifstream file2(path);

        if (!file2.is_open()) {
            std::cerr << "Failed to open file.\n";
        }

        json j;
        file2 >> j;

        value = "";

        FillingTabs();
    }

    Component container =
    Container::Horizontal({
      cont
    }) | border | CatchEvent([ & ](Event event) {

      if (event == Event::ArrowDown) {
        if((selections[column].selected_index < selections[column].keys_json.size() - 1) && (column != selections.size() - 1 ))
        {
          cont->DetachAllChildren();
          ChangeDown(column);
          FillingTabs();
        }
        return false;

      } else if (event == Event::ArrowRight) {
        if(column < selections.size() - 1){
          ++column;
        }
        return false;

      } else if (event == Event::ArrowLeft) {
        if(column > 0){
          --column;
        }
        return false;

      } else if (event == Event::ArrowUp) {
        if((selections[column].selected_index > 0) && (column != selections.size() - 1))
        {
          cont->DetachAllChildren();
          ChangeUp(column);
          FillingTabs();
        }
        return false;
      } else if (event.is_character()) {
          return false;
      } else if ( event == Event::Backspace){
          return false;
      } else if ( event == Event::Return){
        if (value != ""){
          cont->DetachAllChildren();
          changeVal(value, selections, path);
        }
          return false;
      }
      return true;
    });

public:

    int GetSelected() const { return selected; }
    std::string GetVal() const {return value; }
    Component getComponent(){ return container; };

    void draw(){
        collectKeys(jsonCur, selections);
        FillingTabs();

        ScreenInteractive screen = ScreenInteractive::FitComponent();
        screen.Loop(container);

    };

    Table(std::string& filePath): path(filePath){
        std::ifstream file(filePath);
        file >> jsonCur;
        file.close();
        draw();
    }

};

int main(){

    std::string p = "/Users/coldreign/Statistic/ex.json";
    Table t(p);

    return 0;
}

