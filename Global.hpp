#pragma once
#include "FileDialog.hpp"
#include "imgui.h"

extern bool createCopyNow = true;
extern int currentTab = 1;
extern int duration = 0;

extern char inputW[64] = "/путь/до/";
extern char inputRecovery[64] = "/путь/до/";
extern char nameOfFile[64] = "Name_%Y-%m-%d_%H-%M-%S";

extern ImGuiFileDialog instance_a, instance_b;

// Использовать дефолтный путь при выборе директории для бекапа
c
