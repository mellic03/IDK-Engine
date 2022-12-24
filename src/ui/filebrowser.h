#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class FileBrowser {

  private:


  public:

    fs::path selected_path;

    FileBrowser(void) { };


    void drawDirectory(fs::path directory_path);

};


namespace EngineUI {
  void draw_directory_recursive(fs::path pth, fs::path *selected_filepath, bool *changed);
};

