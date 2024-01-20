#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <ImGuiFileDialog.h>
#include <ctime>
#include <sstream>
#include <iostream>

char inputC[128] = "/путь/до";
char inputW[128] = "/путь/до";
char nameOfFile[128] = "название_файла";

ImGuiFileDialog instance_a;
ImGuiFileDialog instance_b;

std::string currentTime;
void drawGui(std::string text, char* path, ImGuiFileDialog &instance) { 
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

int main() {

    sf::RenderWindow window(sf::VideoMode(700, 500), "ImGui + SFML = <3", sf::Style::Titlebar | sf::Style::Close);
    ImGui::SFML::Init(window, false);
    
    int currentTab = 1;
    sf::Clock deltaClock;
    ImGuiIO& io = ImGui::GetIO();
    
    io.Fonts->Clear();
    ImFont* font = io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf", 20.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ImGui::SFML::UpdateFontTexture();
    IM_ASSERT(font != NULL);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
       
        ImGui::SetNextWindowSize(ImVec2(700, 500));
        ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
       
        ImGui::Begin("Бекап", 0, ImGuiWindowFlags_NoMove);
        
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        if(currentTab == 1) {
            ImGui::InputText("Директория для бекапа", inputC, IM_ARRAYSIZE(inputC));
            drawGui("Выбрать директорию##1", inputC, instance_a);
            
            ImGui::InputText("Сохранить в...", inputW, IM_ARRAYSIZE(inputW));
            drawGui("Выбрать директорию##2", inputW, instance_b);
            
            ImGui::SetCursorPos(ImVec2(600, 450));
            if (ImGui::Button("Дальше")) currentTab++;
        }
        ImGui::PopFont();
        
        if(currentTab == 3) {
            ImGui::Text(u8"Сделать бекап через");
            
            static int e = 0;
            ImGui::RadioButton("5 минут", &e, 1); ImGui::SameLine();
            ImGui::RadioButton("1 час", &e, 2); ImGui::SameLine();
            ImGui::RadioButton("1 неделя", &e, 3); ImGui::SameLine(); 
            ImGui::RadioButton("Сейчас", &e, 0); 
            
            ImGui::SetCursorPos(ImVec2(600, 450));
            if (ImGui::Button("Готово")) {
                if(e == 0) {
                    system("./myscript.sh");
                }
            }

            ImGui::SetCursorPos(ImVec2(25, 450));
            if (ImGui::Button("Назад")) currentTab--;
        }
        if(currentTab == 2) {
            ImGui::InputText("Имя копии", nameOfFile, IM_ARRAYSIZE(nameOfFile));
            
            std::ostringstream os;
            os << "Имя будет отображено как: ";
            os << nameOfFile;
            std::time_t t = std::time(0);   // get time now
            std::tm* now = std::localtime(&t);
            os << (now->tm_year + 1900) << '-' 
         << (now->tm_mon + 1) << '-'
         <<  now->tm_mday
         << "\n";
            ImGui::Text(os.str().c_str());
            
            ImGui::SetCursorPos(ImVec2(600, 450));
            if (ImGui::Button("Дальше")) currentTab++;

            ImGui::SetCursorPos(ImVec2(25, 450));
            if (ImGui::Button("Назад")) currentTab--;

        }
        ImGui::End();
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
