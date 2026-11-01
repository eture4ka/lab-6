#ifndef WIFI_H
#define WIFI_H

#include "WirelessTechnology.h"

// Похідний (дочірній) клас WiFi
class WiFi : public WirelessTechnology {
    friend class TechManager;  // дружність не успадковується, тому оголошуємо і тут
private:
    std::string standard;  // стандарт IEEE (802.11n, 802.11ac, 802.11ax, 802.11be)
    int channels;          // кількість каналів
    bool wpa3;             // підтримка шифрування WPA3

    static bool isValidStandard(const std::string& s);

public:
    WiFi();
    WiFi(const std::string& name, const std::string& manufacturer, const TechSpec& spec,
         const std::string& standard, int channels, bool wpa3);
    ~WiFi() override;

    std::string getStandard() const;
    int getChannels() const;
    bool hasWpa3() const;

    bool setStandard(const std::string& value);
    bool setChannels(int value);
    void setWpa3(bool value);

    void input() override;
    void print() const override;
    std::string getType() const override;
    // Реальна швидкість WiFi ~ 60% від максимальної (накладні витрати протоколу)
    double transferTime(double sizeMB) const override;
};

#endif
