#pragma once

#include <string>
#include <vector>

#include "imgui.h"



namespace ImGui {
  void Dropdown(std::string label, std::vector<std::string> items, std::string *selected, int *index);
}




