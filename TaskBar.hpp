#include "imgui.h"

static void DrawTaskBar(int &currentTab) {
    if(ImGui::Button("Создание копии")) currentTab = 1;ImGui::SameLine();
    if(ImGui::Button("Таблица")) currentTab = 2;ImGui::SameLine();
    if(ImGui::Button("Справка")) currentTab = 3;
  
}