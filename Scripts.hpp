#include <fstream>
#include <iostream>

static bool defaultPath = true;
static char inputW[64] = "/путь/до/";

// Создание процесса для автоматического бекапа
static void createProcess() {
    system("cp /usr/share/backup_app/config/hello.service /lib/systemd/system/hello.service");
    system("systemctl stop hello.service");
    system("systemctl enable hello.service");
    system("systemctl start hello.service");
}

// Записывание переменных в конфиге
static void writeToConfig(int dur_s) {
    std::ofstream dir_w("/usr/share/backup_app/config/dir_w.txt", std::ios::in | std::ios::trunc);
    std::ofstream dur("/usr/share/backup_app/config/dur.txt", std::ios::in | std::ios::trunc);
    std::ofstream last("/usr/share/backup_app/config/last.txt", std::ios::in | std::ios::trunc);
    if(defaultPath == true) { dir_w << "/usr/share/backup_app"; }
    else { dir_w << inputW; }
    dur << dur_s;
    last << time(NULL);
}

void startNow() {
    std::ifstream dir_w("/usr/share/backup_app/config/dir_w.txt", std::ios::out);
    std::ifstream name("/usr/share/backup_app/config/name.txt", std::ios::out);
    
    std::string n, w;

    std::getline(dir_w, w);
    std::string ww = "-w " + w + " ";
    std::getline(name, n);
    std::string nn = "-n " + n + " ";

    std::cout << w  << "/" << n << ".tar.zst" << std::endl;

    system((std::string("/usr/share/backup_app/copy.sh ") + ww + nn).c_str());
    std::ofstream last_re("/usr/share/backup_app/config/last.txt", std::ios::in | std::ios::trunc);
    if(!last_re) {std::cout << "Error\n";}
    last_re << time(NULL);
    last_re.close();
    std::cout << w << n << std::endl;
    std::ifstream file(w  + "/" + n + ".tar.zst",std::ios::binary | std::ios::ate);
    if (!file) {
        std::cout << "Ошибка открытия файла!" << std::endl;
    }

    std::streampos filesize = file.tellg();
    file.close();
    filesize = filesize / (1024.0);
    std::cout << "Размер файла " << " : " << filesize << " Кбайт" << std::endl;
    
    char notification[120];
    sprintf(notification, "notify-send Успешно 'Размер копии %d кбайт'", filesize);

    system(notification);
}

void startWithDur() {
    while (true) {
    std::ifstream dur("/usr/share/backup_app/config/dur.txt", std::ios::out);
    std::ifstream last("/usr/share/backup_app/config/last.txt", std::ios::out);
    std::ifstream dir_w("/usr/share/backup_app/config/dir_w.txt", std::ios::out);
    std::ifstream name("/usr/share/backup_app/config/name.txt", std::ios::out);
   

    std::string w, d, l, n;
    
    std::getline(dir_w, w);
    std::string ww = "-w " + w + " ";

    std::getline(dur, d);
    std::getline(last, l);

    std::getline(name, n);
    std::string nn = "-n " + n + " ";

    // Настало ли время для бекап
        if(stoi(d) + stoi(l) <= time(NULL)) {
            system((std::string("/usr/share/backup_app/copy.sh ") + ww + nn).c_str());
            std::ofstream last_re("/usr/share/backup_app/config/last.txt", std::ios::in | std::ios::trunc);
            if(!last_re) {std::cout << "Error\n";}
            last_re << time(NULL);
            last_re.close();
            system("notify-send hello '123'");
        }
    }
}
