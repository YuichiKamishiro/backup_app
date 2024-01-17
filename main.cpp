#include <imgui.h>
#include <ImGuiFileDialog.h>

std::string pathToDir = "";

void drawFileChooser()
{ 
  // open Dialog Simple
  if (ImGui::Button("Open File Dialog"))
  {
    IGFD::FileDialogConfig config;
    config.path = ".";
    ImGuiFileDialog::Instance()->OpenDialog("ChooseDirDlgKey", "Choose a Directory", nullptr, config);
  }
  // display
  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) 
  {
    // action if OK
    if (ImGuiFileDialog::Instance()->IsOk())
    {
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      pathToDir = filePath;
    }
    
    // close
    ImGuiFileDialog::Instance()->Close();
  }
}

void NextStep(int &ct) {
    if(ct < 3) {
        ct++;
    }
}

void PrevStep(int &ct) {
    if(ct > 1) {
        ct--;
    }
}

int main()
{
    int currentTab = 1;
    ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
    if(currentTab == 1){
        drawFileChooser();
        if (ImGui::Button("Next Step")){
            NextStep(&currentTab);
        }
        if (ImGui::Button("Next Step")){
            PrevStep(&currentTab)
        }
    }
    ImGui::End();
}