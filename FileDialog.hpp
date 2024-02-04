#include "imgui.h"
#include <ImGuiFileDialog.h>
#include <ctime>
#include <sstream>
#include <iostream>
#include <fstream>

extern char inputRecovery[64] = "/путь/до/";

// Отрисовка FileDialog
void drawDirGui(std::string text, char* path, ImGuiFileDialog &instance) { 
    if (ImGui::Button(text.c_str())){
        IGFD::FileDialogConfig config;
        config.path = ".";
        instance.OpenDialog("ChooseDirDlgKey", "Choose a Directory", nullptr, config);
    }

    if (instance.Display("ChooseDirDlgKey")) {
        if (instance.IsOk()) {
            std::string filePath = instance.GetCurrentPath();
            for(int x = 0; x < filePath.size(); x++) {
                path[x] = filePath[x];
        }}
        instance.Close();
    }
}
static void drawFileGui(std::string text, ImGuiFileDialog &instance) { 
  // open Dialog Simple
  if (ImGui::Button(text.c_str())) {
    IGFD::FileDialogConfig config;config.path = ".";
    instance.OpenDialog("ChooseFileDlgKey", "Choose File", ".tar.zst,.h,.hpp", config);
  }
  // display
  if (instance.Display("ChooseFileDlgKey")) {
    if (instance.IsOk()) { // action if OK
      
      std::string filePath;
      filePath += instance.GetFilePathName();
      for(int x = 0; x < filePath.size(); x++) {
        inputRecovery[x] = filePath[x];
      }
    }
    
    // close
    instance.Close();
  }
}