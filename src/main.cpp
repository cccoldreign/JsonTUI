// main.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "JsonRenderer.h"
#include "Table.h"
#include "Vert.h"
#include "Navy.h"

#include <nlohmann/json.hpp>


using json = nlohmann::json;
using namespace ftxui;

int main(int argc, char* argv[]) {
    if (argc > 3 || argc == 1) {
        std::cerr << "Usage: " << argv[0] << " <renderer_flag> <json_file_path>" << std::endl;
        return 1;
    }
    if (argc == 2){
        std::string filePath = argv[1];
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file.n";
            return 1;
        }

        json j;
        file >> j;
        file.close();
        
        std::unique_ptr<JsonRenderer> renderer;
        renderer = std::make_unique<TableJson>();
        auto screen = ScreenInteractive::FitComponent();
        auto component = renderer->render(j, filePath);
        screen.Loop(component);

        return 0;

    } else{
        std::string flag = argv[1];
        std::string filePath = argv[2];
        
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file.n";
            return 1;
        }

        json j;
        file >> j;
        file.close();
        
        std::unique_ptr<JsonRenderer> renderer;
        if (flag == "--table") {
            renderer = std::make_unique<TableJson>();
        } else if (flag == "--vert-c") {
            renderer = std::make_unique<VerticalTab>(true);
        } else if (flag == "--vert-b") {
            renderer = std::make_unique<VerticalTab>(false);
        } else if (flag == "--navy") {
            renderer = std::make_unique<Navy>();
        } else {
            std::cerr << "Invalid flag" << std::endl;
            return 1;
        }

        Screen screen(9, 40) ;
        auto component = renderer->render(j, filePath);
        std::cout << screen.ToString();
        
        Render(screen, component->Render());
        std::cout << screen.ToString();

        return 0;
    }
}
