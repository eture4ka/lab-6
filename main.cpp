#include <iostream>
#include "TechManager.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    cout << "===== Лабораторна робота. Варіант 18 =====\n"
         << "Базовий клас: Бездротова технологія; похідні: WiFi, Bluetooth\n"
         << "Дружній клас: TechManager (введення, виведення, обробка)\n\n";

    TechManager manager;

    int mode = TechManager::readMenu("1 - ввести дані вручну, 2 - використати тестові дані: ", 1, 2);
    if (mode == 1) manager.fillManual();
    else manager.fillTestData();

    double sizeMB = TechManager::readFileSize();

    // ---- Виведення вхідних даних ----
    cout << "\n========== ВХІДНІ ДАНІ ==========\n";
    manager.printAll();
    manager.printSummary();

    // ---- Обчислення і результат ----
    Result r = manager.calculate();
    manager.printResult(r, sizeMB);

    return 0;  // тут спрацює деструктор TechManager, який видалить усі об'єкти
}
