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
#include <filesystem>

char inputW[128] = "/путь/до/";
char nameOfFile[128] = "название_файла";

ImGuiFileDialog instance_a;
static bool defaultPath = true;
static int e = 0;

namespace fs = std::filesystem;

struct FileInfo {
    std::string name;
    std::string newName;
    bool toDelete;
};

std::vector<FileInfo> fileInfos;

void GetFiles() {
    fileInfos.clear();

    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        if (entry.path().extension() == ".txt") {
            FileInfo fileInfo;
            fileInfo.name = entry.path().filename().string();
            fileInfo.newName = fileInfo.name;
            fileInfo.toDelete = false;
            fileInfos.push_back(fileInfo);
        }
    }
}

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
void createProcess() {
    system("cp ./config/hello.service /lib/systemd/system/hello.service");
    system("systemctl stop hello.service");
    system("systemctl enable hello.service");
    system("systemctl start hello.service");
}

void startScript(int dur_s) {
    std::ofstream dir_w("./config/dir_w.txt", std::ios::in | std::ios::trunc);
    std::ofstream dur("./config/dur.txt", std::ios::in | std::ios::trunc);
    std::ofstream last("./config/last.txt", std::ios::in | std::ios::trunc);
    if(defaultPath == true) { dir_w << "./saves"; }
    else { dir_w << inputW; }
    dur << dur_s;
    last << time(NULL);
}

void drawTable() {
    if (ImGui::Button("Обновить")) {
            GetFiles();
    }

        ImGui::Columns(3, "Колонны", true);
        ImGui::Text("Файл");
        ImGui::NextColumn();
        ImGui::Text("Новое имя");
        ImGui::NextColumn();
        ImGui::Text("Удалить");
        ImGui::NextColumn();

        for (auto& fileInfo : fileInfos) {
            ImGui::Text("%s", fileInfo.name.c_str());
            ImGui::NextColumn();

            ImGui::InputText(fileInfo.name.c_str(), &fileInfo.newName[0], fileInfo.newName.size());
            ImGui::NextColumn();

            ImGui::Checkbox(("##" + fileInfo.name).c_str(), &fileInfo.toDelete);
            ImGui::NextColumn();
        }

        ImGui::Columns();

        if (ImGui::Button("Сохранить изменения")) {
            for (auto& fileInfo : fileInfos) {
                if (!fileInfo.toDelete) {
                    if (fileInfo.newName != fileInfo.name) {
                        fs::rename(fileInfo.name, fileInfo.newName);
                    }
                } else {
                    fs::remove(fileInfo.name);
                }
            }

            GetFiles();
        }
}

void processScript() {
    std::ifstream dur("./config/dur.txt", std::ios::out);
    std::ifstream last("./config/last.txt", std::ios::out);
    std::ifstream dir_w("./config/dir_w.txt", std::ios::out);
    std::ifstream name("./config/name.txt", std::ios::out);
   

    std::string w, d, l, n;
    
    std::getline(dir_w, w);
    std::string ww = "-w " + w + " ";

    std::getline(dur, d);
    std::getline(last, l);
    
    std::getline(name, n);
    std::string nn = "-n " + n + " ";


    while (true) {
        if(stoi(d) + stoi(l) <= time(NULL)) {
            system((std::string("./script.sh ") + ww + nn).c_str());
            std::ofstream last_re("./config/last.txt", std::ios::in | std::ios::trunc);
            if(!last_re) {std::cout << "Error\n";}
            last_re << time(NULL);
            last_re.close();
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
            if(defaultPath == true) ImGui::BeginDisabled(true);
            ImGui::InputText("Сохранить в...", inputW, IM_ARRAYSIZE(inputW));
            drawGui("Выбрать директорию", inputW, instance_a);
            if(defaultPath == true) ImGui::EndDisabled();
            ImGui::SameLine();
            ImGui::Checkbox("Использовать дефолтный путь", &defaultPath);

            ImGui::SetCursorPos(ImVec2(600, 450));
            if (ImGui::Button("Дальше")) currentTab++;
        }
        ImGui::PopFont();
        
        if(currentTab == 3) {
            ImGui::Text("Сделать бекап через");
            
            static int e = 0;
            ImGui::RadioButton("5 минут", &e, 25); ImGui::SameLine();
            ImGui::RadioButton("1 час", &e, 3600); ImGui::SameLine();
            ImGui::RadioButton("1 неделя", &e, 604800); ImGui::SameLine(); 
            ImGui::RadioButton("Сейчас", &e, 0); 
            
            ImGui::SetCursorPos(ImVec2(600, 450));
            if (ImGui::Button("Далее")) currentTab++;

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
        if (currentTab == 4) {
            drawTable();
            ImGui::SetCursorPos(ImVec2(25, 450));
            if (ImGui::Button("Назад")) currentTab--;

            ImGui::SetCursorPos(ImVec2(600, 450));
            if (ImGui::Button("Готово")) {
                if(e != 0) {
                    startScript(e);
                    createProcess();
                } else {
                    startScript(e);
                    processScript();
                }
            }

        }
        ImGui::End();
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
