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
#include <fstream>

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
void startScript(int dur_s) {
    std::ofstream dir_c("/home/yuichi/backup_app/config/dir_c.txt", std::ios::in | std::ios::trunc);
    std::ofstream dir_w("/home/yuichi/backup_app/config/dir_w.txt", std::ios::in | std::ios::trunc);
    std::ofstream dur("/home/yuichi/backup_app/config/dur.txt", std::ios::in | std::ios::trunc);
    std::ofstream last("/home/yuichi/backup_app/config/last.txt", std::ios::in | std::ios::trunc);
    dir_c << inputC;
    dir_w << inputW;
    dur << dur_s;
    last << time(NULL);
}

void processScript() {
    std::ifstream dur("/home/yuichi/backup_app/config/dur.txt", std::ios::out);
    std::ifstream last("/home/yuichi/backup_app/config/last.txt", std::ios::out);
    std::ifstream dir_c("/home/yuichi/backup_app/config/dir_c.txt", std::ios::out);
    std::ifstream dir_w("/home/yuichi/backup_app/config/dir_w.txt", std::ios::out);
    std::ifstream name("/home/yuichi/backup_app/config/name.txt", std::ios::out);
   

    std::string c, w, d, l, n;
    
    std::getline(dir_c, c);
    std::getline(dir_w, w);
    std::string cc = "-c " + c + " ";
    std::string ww = "-w " + w + " ";

    std::getline(dur, d);
    std::getline(last, l);
    
    std::getline(name, n);
    std::string nn = "-n " + n + " ";


    while (true) {
        if(stoi(d) + stoi(l) <= time(NULL)) {
            system((std::string("./myscript.sh ") + cc + ww + nn).c_str());
            exit(3);
        }
    }
}
int main(int argc, char *argv[]) {
    if(argc > 1) {
        processScript();
    }

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
            ImGui::RadioButton("5 минут", &e, 15); ImGui::SameLine();
            ImGui::RadioButton("1 час", &e, 3600); ImGui::SameLine();
            ImGui::RadioButton("1 неделя", &e, 604800); ImGui::SameLine(); 
            ImGui::RadioButton("Сейчас", &e, 0); 
            
            ImGui::SetCursorPos(ImVec2(600, 450));
            if (ImGui::Button("Готово")) {
                    startScript(e);
                    processScript();
                }

            ImGui::SetCursorPos(ImVec2(25, 450));
            if (ImGui::Button("Назад")) currentTab--;
        }
        if(currentTab == 2) {
            ImGui::InputText("Имя копии", nameOfFile, IM_ARRAYSIZE(nameOfFile));
            
            std::ostringstream os;
            os << nameOfFile;
            std::time_t t = std::time(0);   // get time now
            std::tm* now = std::localtime(&t);
            os << (now->tm_year + 1900) << '-' 
         << (now->tm_mon + 1) << '-'
         <<  now->tm_mday
         << "\n";
            ImGui::Text((std::string("Имя будет отображено как: ") + os.str()).c_str());
            
            ImGui::SetCursorPos(ImVec2(600, 450));
            if (ImGui::Button("Дальше")) {
                currentTab++;
                std::ofstream name("/home/yuichi/backup_app/config/name.txt", std::ios::in | std::ios::trunc);
                name << os.str();
            }

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
