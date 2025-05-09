#include <gtest/gtest.h> 
#include "JsonRenderer.h"
#include "Table.h"

#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <nlohmann/json.hpp>

using namespace ftxui;
using json = nlohmann::json;

TEST(TestFileCheck, TypeChange){
    std::string fileEx = std::__fs::filesystem::path(__FILE__).parent_path().string() + "/../Example/Example.json";
    std::ifstream file(fileEx);
    ASSERT_TRUE(file.is_open()) << "Failed to open file: " << fileEx;

    json j;
    file >> j;

    ASSERT_FALSE(j.empty()) << "JSON file is empty or invalid.";

    file.close();

    ASSERT_FALSE(file.is_open()) << "Failed to close file: " << fileEx;

    TableJson* table = new TableJson();
    std::unique_ptr<JsonRenderer> renderer(table);

    auto screen = ftxui::ScreenInteractive::FitComponent();
    auto component = renderer->render(j, fileEx);

    EXPECT_EQ(table->GetSelected(), 0);

    component->OnEvent(Event::ArrowRight);
    component->OnEvent(Event::ArrowRight);
    component->OnEvent(Event::ArrowRight);
    EXPECT_EQ(table->GetSelected(), 0);
    component->OnEvent(Event::ArrowDown);
    EXPECT_EQ(table->GetSelected(), 0);
    component->OnEvent(Event::ArrowDown);
    EXPECT_EQ(table->GetSelected(), 0);
    component->OnEvent(Event::ArrowDown);
    EXPECT_EQ(table->GetSelected(), 0);
    component->OnEvent(Event::Return);
    EXPECT_EQ(table->GetSelected(), 1);
    component->OnEvent(Event::ArrowDown);
    component->OnEvent(Event::Return);
    EXPECT_EQ(table->GetSelected(), 2);
    component->OnEvent(Event::ArrowUp);
    component->OnEvent(Event::Return);
    EXPECT_EQ(table->GetSelected(), 1);
}

TEST(TestFileCheck, changeValInt){
    std::string fileEx = std::__fs::filesystem::path(__FILE__).parent_path().string() + "/../Example/Example.json";
    std::ifstream file(fileEx);
    ASSERT_TRUE(file.is_open()) << "Failed to open file: " << fileEx;

    json j;
    file >> j;

    ASSERT_FALSE(j.empty()) << "JSON file is empty or invalid.";

    file.close();

    ASSERT_FALSE(file.is_open()) << "Failed to close file: " << fileEx;

    TableJson* table = new TableJson();
    std::unique_ptr<JsonRenderer> renderer(table);

    auto screen = ftxui::ScreenInteractive::FitComponent();
    auto component = renderer->render(j, fileEx);

    component->OnEvent(Event::ArrowRight);
    component->OnEvent(Event::ArrowRight);
    component->OnEvent(Event::ArrowRight);

    component->OnEvent(Event::ArrowDown);

    component->OnEvent(Event::Character("1"));
    
    EXPECT_EQ(table->GetVal(), "1") << "value is not equal to input";
    
    component->OnEvent(Event::Return);


    file.open(fileEx);
    ASSERT_TRUE(file.is_open()) << "Failed to open file: " << fileEx;
    int check = 1;
    file >> j;
    ASSERT_TRUE(j["Globus"]["hohm"]["ffsa"].is_number()) << "Is not number.";
    EXPECT_EQ(j["Globus"]["hohm"]["ffsa"], check);
    
    file.close();
    ASSERT_FALSE(file.is_open()) << "Failed to close file: " << fileEx;

}

TEST(TestFileCheck, changeValFloat){
    std::string fileEx = std::__fs::filesystem::path(__FILE__).parent_path().string() + "/../Example/Example.json";
    std::ifstream file(fileEx);
    ASSERT_TRUE(file.is_open()) << "Failed to open file: " << fileEx;

    json j;
    file >> j;

    ASSERT_FALSE(j.empty()) << "JSON file is empty or invalid.";

    file.close();

    ASSERT_FALSE(file.is_open()) << "Failed to close file: " << fileEx;

    TableJson* table = new TableJson();
    std::unique_ptr<JsonRenderer> renderer(table);

    auto screen = ftxui::ScreenInteractive::FitComponent();
    auto component = renderer->render(j, fileEx);

    component->OnEvent(Event::ArrowRight);
    component->OnEvent(Event::ArrowRight);
    component->OnEvent(Event::ArrowRight);

    component->OnEvent(Event::ArrowDown);
    component->OnEvent(Event::ArrowDown);
    component->OnEvent(Event::ArrowDown);

    component->OnEvent(Event::Return);
    
    component->OnEvent(Event::ArrowUp);
    component->OnEvent(Event::ArrowUp);
    
    component->OnEvent(Event::Character("1.0"));

    EXPECT_EQ(table->GetVal(), "1.0") << "Value is not equal to input";
    component->OnEvent(Event::Return);

    file.open(fileEx);
    ASSERT_TRUE(file.is_open()) << "Failed to open file: " << fileEx;
    float check = 1.0;
    file >> j;
    ASSERT_TRUE(j["Globus"]["hohm"]["ffsa"].is_number_float()) << "Is not float.";
    EXPECT_EQ(j["Globus"]["hohm"]["ffsa"], check);
    
    file.close();
    ASSERT_FALSE(file.is_open()) << "Failed to close file: " << fileEx;

}

TEST(TestFileCheck, changeValString){
    std::string fileEx = std::__fs::filesystem::path(__FILE__).parent_path().string() + "/../Example/Example.json";
    std::ifstream file(fileEx);
    ASSERT_TRUE(file.is_open()) << "Failed to open file: " << fileEx;

    json j;
    file >> j;

    ASSERT_FALSE(j.empty()) << "JSON file is empty or invalid.";

    file.close();

    ASSERT_FALSE(file.is_open()) << "Failed to close file: " << fileEx;

    TableJson* table = new TableJson();
    std::unique_ptr<JsonRenderer> renderer(table);

    auto screen = ftxui::ScreenInteractive::FitComponent();
    auto component = renderer->render(j, fileEx);

    component->OnEvent(Event::ArrowRight);
    component->OnEvent(Event::ArrowRight);
    component->OnEvent(Event::ArrowRight);

    component->OnEvent(Event::ArrowDown);
    component->OnEvent(Event::ArrowDown);
    component->OnEvent(Event::ArrowDown);
    component->OnEvent(Event::ArrowDown);

    component->OnEvent(Event::Return);

    component->OnEvent(Event::ArrowUp);
    component->OnEvent(Event::ArrowUp);
    component->OnEvent(Event::ArrowUp);
    component->OnEvent(Event::Character("sad"));

    EXPECT_EQ(table->GetVal(), "sad") << "Value is not equal to input";
    component->OnEvent(Event::Return);

    file.open(fileEx);
    ASSERT_TRUE(file.is_open()) << "Failed to open file: " << fileEx;
    std::string check = "sad";
    file >> j;
    ASSERT_TRUE(j["Globus"]["hohm"]["ffsa"].is_string()) << "Is not float.";
    EXPECT_EQ(j["Globus"]["hohm"]["ffsa"], check);
    
    file.close();
    ASSERT_FALSE(file.is_open()) << "Failed to close file: " << fileEx;

}

