#ifndef WIRELESS_TECHNOLOGY_H
#define WIRELESS_TECHNOLOGY_H

#include <string>

// Структура з технічними характеристиками бездротової технології
struct TechSpec {
    double frequencyGHz;   // робоча частота, ГГц
    double maxSpeedMbps;   // максимальна швидкість передачі, Мбіт/с
    double rangeM;         // радіус дії, м
    double powerMw;        // споживана потужність, мВт
};

// Базовий (батьківський) клас
class WirelessTechnology {
    friend class TechManager;  // дружній клас: доступ до private/protected членів
private:
    std::string name;          // назва пристрою / модуля
    std::string manufacturer;  // виробник
    TechSpec spec;             // характеристики

    static int count;          // кількість створених об'єктів

protected:
    // Допоміжні функції введення з перевіркою валідності
    static double readDouble(const std::string& prompt, double minV, double maxV);
    static int readInt(const std::string& prompt, int minV, int maxV);
    static std::string readString(const std::string& prompt);
    static bool readYesNo(const std::string& prompt);

public:
    WirelessTechnology();
    WirelessTechnology(const std::string& name, const std::string& manufacturer,
                       const TechSpec& spec);
    virtual ~WirelessTechnology();

    // Гетери
    std::string getName() const;
    std::string getManufacturer() const;
    TechSpec getSpec() const;
    static int getCount();

    // Сетери з перевіркою (повертають false, якщо значення некоректне)
    bool setName(const std::string& value);
    bool setManufacturer(const std::string& value);
    bool setFrequency(double value);
    bool setMaxSpeed(double value);
    bool setRange(double value);
    bool setPower(double value);

    // Віртуальні методи
    virtual void input();
    virtual void print() const;
    virtual std::string getType() const;

    // Час передачі файлу заданого розміру (МБ), с
    virtual double transferTime(double sizeMB) const;
    // Енергоефективність: Мбіт/с на 1 мВт
    double efficiency() const;
};

#endif
