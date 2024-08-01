#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <Windows.h>
namespace fs = std::filesystem;


std::string find_directory(const fs::path& root, const std::string& dirName, fs::perm_options opts = fs::perm_options::replace) {
    try {
        std::string s;
        for (const auto& entry : fs::recursive_directory_iterator(root)) {
            if (entry.is_directory() && entry.path().filename() == dirName) {
                std::cout << entry.path();
                s = entry.path().u8string();
                return s;
            }
        }
        std::cout << "Директория с именем \"" << dirName << "\" не найдена в " << root << "\n";
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Ошибка файловой системы: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
}


int main() {
    bool check = true;
    setlocale(LC_ALL, "rus");
    fs::path exePath = fs::current_path();
    fs::path exeDir = fs::absolute(exePath).parent_path();
    try {
        if (fs::exists(exePath)) {  
            std::cout << "Программа находится в директории: " << exeDir << "\n";
        }
        else {
            std::cerr << "Не удалось определить директорию исполняемого файла.\n";
            check = false;
            return 1;
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Ошибка файловой системы: " << e.what() << "\n";
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }
    std::string dirName = "Counter-Strike Global Offensive";
    std::string rootPath;

    std::cout << "Введите диск на котором у вас лежит CS2: ";
    std::getline(std::cin, rootPath);
    fs::path destinationDirectory;
    fs::path root(rootPath);
    if (fs::exists(root) && fs::is_directory(root)) {
        destinationDirectory = find_directory(root, dirName);
    }
    else {
        std::cerr << "Корневая директория " << rootPath << " не существует или не является директорией.\n";
        check = false;
    }
    fs::path sourceFilePath = exeDir / "autoexec.cfg";
    
    
    fs::path destinationFilePath = destinationDirectory / "game" / "csgo" / "cfg" / "autoexec.cfg";
    try {
        std::ofstream outFile(sourceFilePath);
        if (!outFile || check == false) {
            throw std::runtime_error("Не удалось создать файл.");
            return 1;
        } else { outFile << "alias\"checkfwd\" \"\"\n"
                "alias \"checkback\" \"\"\n"
                "alias \"checkleft\" \"\"\n"
                "alias \"checkright\" \"\"\n"
                "alias \"goForward\" \"-forward; +forward\"\n"
                "alias \"goBack\" \"-back; +back\"\n"
                "alias \"goLeft\" \"-left; +left\"\n"
                "alias \"goRight\" \"-right; +right\"\n"
                "alias \"+mfwd\" \"-back; +forward; alias checkfwd goForward\"\n"
                "alias \"+mback\" \"-forward; +back; alias checkback goBack\"\n"
                "alias \"+mleft\" \"-right; +left; alias checkleft goLeft\"\n"
                "alias \"+mright\" \"-left; +right; alias checkright goRight\"\n"
                "alias \"-mfwd\" \"-forward; checkback; alias checkfwd\"\n"
                "alias \"-mback\" \"-back; checkfwd; alias checkback\"\n"
                "alias \"-mleft\" \"-left; checkright; alias checkleft\"\n"
                "alias \"-mright\" \"-right; checkleft; alias checkright\"\n"
                "bind \"w\" \"+mfwd\"\n"
                "bind \"s\" \"+mback\"\n"
                "bind \"a\" \"+mleft\"\n"
                "bind \"d\" \"+mright\"\n";
        }
        outFile.close();
        std::cout << "Файл успешно создан.\n";

        if (!fs::exists(destinationDirectory)) {
            fs::create_directories(destinationDirectory);
        }
        fs::rename(sourceFilePath, destinationFilePath);
        std::cout << "Файл успешно перемещен в " << destinationFilePath << "\n";
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Ошибка файловой системы: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }

    return 0;
}