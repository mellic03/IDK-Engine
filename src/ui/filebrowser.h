#pragma once

#include <filesystem>

namespace fs = std::filesystem;

class FileBrowser {

  private:


  public:

    fs::path m_selected_path;

    FileBrowser(void) { };


    void drawDirectory(fs::path directory_path);


};