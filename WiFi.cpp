#include "WiFi.h"
#include <iostream>

using namespace std;

bool WiFi::isValidStandard(const string& s) {
    return s == "802.11b" || s == "802.11g" || s == "802.11n" ||
           s == "802.11ac" || s == "802.11ax" || s == "802.11be";
}

WiFi::WiFi() : WirelessTechnology(), standard("802.11n"), channels(1), wpa3(false) {}

WiFi::WiFi(const string& name, const string& manufacturer, const TechSpec& spec,
           const string& st, int ch, bool w)
    : WirelessTechnology(name, manufacturer, spec), standard("802.11n"), channels(1), wpa3(w) {
    setStandard(st);
    setChannels(ch);
}

WiFi::~WiFi() {
    cout << "[Деструктор WiFi] " << getName() << "\n";
}

string WiFi::getStandard() const { return standard; }
int WiFi::getChannels() const { return channels; }
bool WiFi::hasWpa3() const { return wpa3; }

bool WiFi::setStandard(const string& value) {
    if (!isValidStandard(value)) {
        cout << "  ! Попередження: невідомий стандарт WiFi \"" << value << "\".\n";
        return false;
    }
    standard = value;
    return true;
}

bool WiFi::setChannels(int value) {
    if (value < 1 || value > 233) {
        cout << "  ! Попередження: кількість каналів має бути від 1 до 233.\n";
        return false;
    }
    channels = value;
    return true;
}

void WiFi::setWpa3(bool value) { wpa3 = value; }

void WiFi::input() {
    WirelessTechnology::input();
    while (true) {
        string st = readString("Стандарт (802.11b/g/n/ac/ax/be)");
        if (setStandard(st)) break;
    }
    setChannels(readInt("Кількість каналів", 1, 233));
    setWpa3(readYesNo("Підтримка WPA3"));
}

void WiFi::print() const {
    WirelessTechnology::print();
    cout << "  Стандарт:     " << standard << "\n"
         << "  Каналів:      " << channels << "\n"
         << "  WPA3:         " << (wpa3 ? "так" : "ні") << "\n";
}

string WiFi::getType() const { return "WiFi"; }

double WiFi::transferTime(double sizeMB) const {
    return WirelessTechnology::transferTime(sizeMB) / 0.6;
}
