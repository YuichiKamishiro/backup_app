#include <filesystem>
#include <string>
#include "imgui.h"
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

// Структура для Таблицы
struct FileInfo {
    std::string name;
    std::string newName;
    bool toDelete;
};

// Файлы которые будут отображаться в таблице
static std::vector<FileInfo> fileInfos;
static fs::path directoryPath = "/home/yuichi/backup_app/saves/";
// Получение всех файлов для таблицы
void GetFiles() {
    fileInfos.clear();

    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".zst") {
            FileInfo fileInfo;
            fileInfo.name = entry.path().filename().string();
            fileInfo.newName = fileInfo.name;
            fileInfo.toDelete = false;
            fileInfos.push_back(fileInfo);
       }
    }
}

// Отрисовка таблицы
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
                        fs::rename(directoryPath.string()+ fileInfo.name, directoryPath.string()+ fileInfo.newName);
                    }
                } else {
                    fs::remove(directoryPath.string() + fileInfo.name);
                }
            }

            GetFiles();
        }
}
