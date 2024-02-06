#include "imgui.h"
#include "imgui-SFML.h"
#include "Table.hpp"
#include "FileDialog.hpp"
#include "Scripts.hpp"
#include "TaskBar.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

static bool createCopyNow = true;
static int currentTab = 1;
static int duration = 0;

static ImGuiFileDialog instance_a, instance_b;

static char nameOfFile[64] = "Имя_%Y-%m-%d_%H-%M-%S";
static char editedName[64];
void ShowDateTimeWidget() {
    std::time_t now = std::time(0);
    std::tm* timeNow = std::localtime(&now);

    std::strftime(editedName, sizeof(editedName), nameOfFile, timeNow);

    ImGui::Text("Имя будет отображено как: %s", editedName);

    ImGui::InputText("Имя шаблона", nameOfFile, IM_ARRAYSIZE(nameOfFile));
}

int main(int argc, char *argv[]) {
    // Если запускается программа с аргументом то вызывается функция выше
    if(argc > 1) {
        startWithDur();
    }

    // Создание окна
    sf::RenderWindow window(sf::VideoMode(900, 500), "ImGui + SFML = <3", sf::Style::Titlebar | sf::Style::Close);
    ImGui::SFML::Init(window, false);
    
    sf::Clock deltaClock;
    ImGuiIO& io = ImGui::GetIO();
     
    // Загрузка шрифта
    io.Fonts->Clear();
    ImFont* font = io.Fonts->AddFontFromFileTTF("/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf", 18.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ImGui::SFML::UpdateFontTexture();
    IM_ASSERT(font != NULL);
    
    // Основной цикл
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
       
        ImGui::SetNextWindowSize(ImVec2(900, 500));
        ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
        
        ImGui::Begin("Бекап", 0, ImGuiWindowFlags_NoMove);
        
        // Отрисовка сверху закладок
        DrawTaskBar(currentTab);
        
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

        if(currentTab == 1) {
            // Выбор пути для сохранения
            ImGui::NewLine();
            if(defaultPath == true) ImGui::BeginDisabled(true);
                ImGui::InputText("Сохранить в...", inputW, IM_ARRAYSIZE(inputW));
                drawDirGui("Выбрать директорию", inputW, instance_a);
            if(defaultPath == true) ImGui::EndDisabled(); 
            ImGui::SameLine();
            ImGui::Checkbox("Использовать дефолтный путь", &defaultPath);
            ImGui::NewLine();
            
            std::ofstream dir_w("/usr/share/backup_app/config/dir_w.txt", std::ios::in | std::ios::trunc);
            dir_w << inputW;
            dir_w.close();


            // Выбор имени для копии
            //ImGui::InputText("Имя копии", nameOfFile, IM_ARRAYSIZE(nameOfFile));
            ShowDateTimeWidget();
            std::ofstream name("/usr/share/backup_app/config/name.txt", std::ios::in | std::ios::trunc);
            name << editedName;
            name.close();
            ImGui::NewLine();

            // Создание копии
            ImGui::Text("Сделать бекап через");
            ImGui::Checkbox("Сделать копию сейчас", &createCopyNow);
            if(createCopyNow == true) ImGui::BeginDisabled(true);
            ImGui::RadioButton("Каждые 5 минут", &duration, 25); ImGui::SameLine();
            ImGui::RadioButton("Каждый час", &duration, 3600); ImGui::SameLine();
            ImGui::RadioButton("Каждую неделя", &duration, 604800);
            if(createCopyNow == true) ImGui::EndDisabled();
            
            ImGui::SetCursorPos(ImVec2(750, 450));
            if (ImGui::Button("Сделать копию")) {
                if(createCopyNow == false) {
                    writeToConfig(duration);
                    createProcess();
                } else {
                    writeToConfig(0);
                    startNow();
                }
            }
        }
        ImGui::PopFont();
        
        if(currentTab == 2) {
            ImGui::NewLine();
            drawTable();
            ImGui::NewLine();
            ImGui::InputText("Копия для восстановления", inputRecovery, IM_ARRAYSIZE(inputRecovery));
            drawFileGui("Выбрать копию", instance_b);
            ImGui::SetCursorPos(ImVec2(750, 450));
            std::string inputRecoveryStr = inputRecovery;
            if(ImGui::Button("Начать восстановление")) {
                system((std::string("/usr/share/backup_app/recovery.sh -n ") + inputRecoveryStr).c_str());
            }
        }

        if(currentTab == 3) {
            ImGui::Text("Справка\nСправка\nПокупка наркотиков");
        }

        ImGui::End();
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}
