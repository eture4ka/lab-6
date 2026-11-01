#include "Bluetooth.h"
#include <iostream>

using namespace std;

Bluetooth::Bluetooth() : WirelessTechnology(), version(5.0), maxDevices(1), lowEnergy(false) {}

Bluetooth::Bluetooth(const string& name, const string& manufacturer, const TechSpec& spec,
                     double v, int md, bool le)
    : WirelessTechnology(name, manufacturer, spec), version(5.0), maxDevices(1), lowEnergy(le) {
    setVersion(v);
    setMaxDevices(md);
}

Bluetooth::~Bluetooth() {
    cout << "[Деструктор Bluetooth] " << getName() << "\n";
}

double Bluetooth::getVersion() const { return version; }
int Bluetooth::getMaxDevices() const { return maxDevices; }
bool Bluetooth::isLowEnergy() const { return lowEnergy; }

bool Bluetooth::setVersion(double value) {
    if (value < 1.0 || value > 6.0) {
        cout << "  ! Попередження: версія Bluetooth має бути від 1.0 до 6.0.\n";
        return false;
    }
    version = value;
    return true;
}

bool Bluetooth::setMaxDevices(int value) {
    if (value < 1 || value > 7) {
        cout << "  ! Попередження: Bluetooth підтримує від 1 до 7 активних пристроїв.\n";
        return false;
    }
    maxDevices = value;
    return true;
}

void Bluetooth::setLowEnergy(bool value) { lowEnergy = value; }

void Bluetooth::input() {
    WirelessTechnology::input();
    setVersion(readDouble("Версія Bluetooth", 1.0, 6.0));
    setMaxDevices(readInt("Макс. кількість пристроїв", 1, 7));
    setLowEnergy(readYesNo("Підтримка Low Energy"));
}

void Bluetooth::print() const {
    WirelessTechnology::print();
    cout << "  Версія:       " << version << "\n"
         << "  Пристроїв:    " << maxDevices << "\n"
         << "  Low Energy:   " << (lowEnergy ? "так" : "ні") << "\n";
}

string Bluetooth::getType() const { return "Bluetooth"; }

double Bluetooth::transferTime(double sizeMB) const {
    return WirelessTechnology::transferTime(sizeMB) / 0.7;
}
