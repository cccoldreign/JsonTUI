#include <gtest/gtest.h> 
#include "JsonRenderer.h"
#include "Table.h"

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
    ASSERT_TRUE(file.is_open()) << "Failed to open file: " << fileEx;

    json j;
    file >> j;

    ASSERT_FALSE(j.empty()) << "JSON file is empty or invalid.";

    file.close();

    ASSERT_FALSE(file.is_open()) << "Failed to close file: " << fileEx;

    TableJson* raw_table = new TableJson();
    std::unique_ptr<JsonRenderer> renderer(raw_table);

    auto screen = ftxui::ScreenInteractive::FitComponent();
    auto component = renderer->render(j, fileEx);

    EXPECT_EQ(raw_table->GetSelected(), 0);

    component->OnEvent(Event::ArrowRight);
    component->OnEvent(Event::ArrowRight);
    component->OnEvent(Event::ArrowRight);

    component->OnEvent(Event::ArrowDown);
    component->OnEvent(Event::ArrowDown);
    component->OnEvent(Event::ArrowDown);
    component->OnEvent(Event::ArrowDown);
    component->OnEvent(Event::Return);

    EXPECT_EQ(raw_table->GetSelected(), 2);


}

