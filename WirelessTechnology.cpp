#include "WirelessTechnology.h"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

int WirelessTechnology::count = 0;

// ---------- Допоміжні функції введення ----------

double WirelessTechnology::readDouble(const string& prompt, double minV, double maxV) {
    double value;
    while (true) {
        cout << prompt << " [" << minV << " - " << maxV << "]: ";
        if (cin >> value && value >= minV && value <= maxV) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "  ! Помилка: введіть число від " << minV << " до " << maxV << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int WirelessTechnology::readInt(const string& prompt, int minV, int maxV) {
    int value;
    while (true) {
        cout << prompt << " [" << minV << " - " << maxV << "]: ";
        if (cin >> value && value >= minV && value <= maxV) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "  ! Помилка: введіть ціле число від " << minV << " до " << maxV << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string WirelessTechnology::readString(const string& prompt) {
    string value;
    while (true) {
        cout << prompt << ": ";
        getline(cin, value);
        if (!value.empty() && value.find_first_not_of(' ') != string::npos)
            return value;
        cout << "  ! Помилка: рядок не може бути порожнім.\n";
    }
}

bool WirelessTechnology::readYesNo(const string& prompt) {
    while (true) {
        cout << prompt << " (y/n): ";
        string s;
        getline(cin, s);
        if (s == "y" || s == "Y") return true;
        if (s == "n" || s == "N") return false;
        cout << "  ! Помилка: введіть y або n.\n";
    }
}

// ---------- Конструктори / деструктор ----------

WirelessTechnology::WirelessTechnology()
    : name("Невідомо"), manufacturer("Невідомо"), spec{2.4, 1.0, 1.0, 1.0} {
    ++count;
}

WirelessTechnology::WirelessTechnology(const string& n, const string& m, const TechSpec& s)
    : WirelessTechnology() {  // делегуючий конструктор (лічильник збільшується там)
    setName(n);
    setManufacturer(m);
    setFrequency(s.frequencyGHz);
    setMaxSpeed(s.maxSpeedMbps);
    setRange(s.rangeM);
    setPower(s.powerMw);
}

WirelessTechnology::~WirelessTechnology() {
    --count;
    cout << "[Деструктор WirelessTechnology] " << name << " знищено.\n";
}

// ---------- Гетери ----------

string WirelessTechnology::getName() const { return name; }
string WirelessTechnology::getManufacturer() const { return manufacturer; }
TechSpec WirelessTechnology::getSpec() const { return spec; }
int WirelessTechnology::getCount() { return count; }

// ---------- Сетери з перевіркою ----------

bool WirelessTechnology::setName(const string& value) {
    if (value.empty()) {
        cout << "  ! Попередження: назва не може бути порожньою.\n";
        return false;
    }
    name = value;
    return true;
}

bool WirelessTechnology::setManufacturer(const string& value) {
    if (value.empty()) {
        cout << "  ! Попередження: виробник не може бути порожнім.\n";
        return false;
    }
    manufacturer = value;
    return true;
}

bool WirelessTechnology::setFrequency(double value) {
    if (value <= 0 || value > 100) {
        cout << "  ! Попередження: некоректна частота " << value << " ГГц.\n";
        return false;
    }
    spec.frequencyGHz = value;
    return true;
}

bool WirelessTechnology::setMaxSpeed(double value) {
    if (value <= 0 || value > 50000) {
        cout << "  ! Попередження: некоректна швидкість " << value << " Мбіт/с.\n";
        return false;
    }
    spec.maxSpeedMbps = value;
    return true;
}

bool WirelessTechnology::setRange(double value) {
    if (value <= 0 || value > 1000) {
        cout << "  ! Попередження: некоректний радіус дії " << value << " м.\n";
        return false;
    }
    spec.rangeM = value;
    return true;
}

bool WirelessTechnology::setPower(double value) {
    if (value <= 0 || value > 10000) {
        cout << "  ! Попередження: некоректна потужність " << value << " мВт.\n";
        return false;
    }
    spec.powerMw = value;
    return true;
}

// ---------- Віртуальні методи ----------

void WirelessTechnology::input() {
    setName(readString("Назва пристрою"));
    setManufacturer(readString("Виробник"));
    setFrequency(readDouble("Частота, ГГц", 0.1, 100));
    setMaxSpeed(readDouble("Макс. швидкість, Мбіт/с", 0.1, 50000));
    setRange(readDouble("Радіус дії, м", 0.1, 1000));
    setPower(readDouble("Потужність, мВт", 0.1, 10000));
}

void WirelessTechnology::print() const {
    cout << fixed << setprecision(2);
    cout << "  Тип:          " << getType() << "\n"
         << "  Назва:        " << name << "\n"
         << "  Виробник:     " << manufacturer << "\n"
         << "  Частота:      " << spec.frequencyGHz << " ГГц\n"
         << "  Швидкість:    " << spec.maxSpeedMbps << " Мбіт/с\n"
         << "  Радіус дії:   " << spec.rangeM << " м\n"
         << "  Потужність:   " << spec.powerMw << " мВт\n";
}

string WirelessTechnology::getType() const { return "Бездротова технологія"; }

double WirelessTechnology::transferTime(double sizeMB) const {
    // 1 МБ = 8 Мбіт
    return sizeMB * 8.0 / spec.maxSpeedMbps;
}

double WirelessTechnology::efficiency() const {
    return spec.maxSpeedMbps / spec.powerMw;
}
