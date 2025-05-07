#include "Table.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

using namespace ftxui;

Component TableJson::render(const nlohmann::json& json, std::string& filePath) {
  collectKeys(json, selections);

  FillingTabs();

  auto container =
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
          changeVal(value, selections, filePath);
        }
          return false;
      }
      return true;
    });

    return container;
}