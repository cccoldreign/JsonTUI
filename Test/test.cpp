#include <gtest/gtest.h> 
#include "JsonRenderer.h"
#include "Table.h"
// #include "/Users/coldreign/JsonTUI/src/Table.cpp"
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <nlohmann/json.hpp>

using namespace ftxui;
using json = nlohmann::json;

TEST(TestFileCheck, Change){
    std::string fileEx = "/Users/coldreign/JsonTUI/Example/Example.json";
    std::ifstream file(fileEx);
    TableJson table;
    ASSERT_TRUE(file.is_open()) << "Failed to open file: " << fileEx;

    json j;
    file >> j;

    ASSERT_FALSE(j.empty()) << "JSON file is empty or invalid.";

    file.close();

    ASSERT_FALSE(file.is_open()) << "Failed to close file: " << fileEx;

    std::unique_ptr<JsonRenderer> renderer = std::make_unique<TableJson>();
    auto screen = ftxui::ScreenInteractive::FitComponent();
    auto component = renderer->render(j, fileEx);

    // Проверка приватной переменной через публичный метод
    EXPECT_EQ(table.GetSelected(), 0); // Проверяем значение приватной переменной
    component->OnEvent(Event::ArrowDown);
    component->OnEvent(Event::ArrowDown);
    component->OnEvent(Event::Return);
}

