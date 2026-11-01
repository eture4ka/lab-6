#ifndef TECH_MANAGER_H
#define TECH_MANAGER_H

#include <string>
#include <vector>
#include "WirelessTechnology.h"
#include "WiFi.h"
#include "Bluetooth.h"

// Структура для збереження результатів обчислень
struct Result {
    const WirelessTechnology* fastest = nullptr;       // найшвидший
    const WirelessTechnology* longestRange = nullptr;  // найбільший радіус
    const WirelessTechnology* mostEfficient = nullptr; // найенергоефективніший
    double avgSpeed = 0;     // середня швидкість, Мбіт/с
    double totalPower = 0;   // сумарна потужність, мВт
    int wifiCount = 0;
    int btCount = 0;
};

// Дружній клас: оголошений як friend у WirelessTechnology, WiFi та Bluetooth,
// тому напряму працює з їхніми приватними полями.
// Відповідає за введення, виведення та обробку даних батьківського й дочірніх класів.
class TechManager {
private:
    std::vector<WirelessTechnology*> list;  // колекція (власник об'єктів)

public:
    TechManager();
    ~TechManager();                                    // видаляє всі об'єкти зі списку
    TechManager(const TechManager&) = delete;          // копіювати власника вказівників не можна
    TechManager& operator=(const TechManager&) = delete;

    // ---- Допоміжне введення ----
    static int readMenu(const std::string& prompt, int minV, int maxV);
    static double readFileSize();

    // ---- Введення даних ----
    void fillTestData();   // тестові дані (разом з некоректним прикладом)
    void fillManual();     // введення з клавіатури

    // ---- Виведення даних ----
    void printAll() const;                              // повний вивід кожного пристрою
    void printSummary() const;                          // короткий перелік (через friend-доступ)
    void printResult(const Result& r, double sizeMB) const;

    // ---- Обробка даних ----
    Result calculate() const;

    std::size_t size() const;
};

#endif
