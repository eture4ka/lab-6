#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "WirelessTechnology.h"

// Похідний (дочірній) клас Bluetooth
class Bluetooth : public WirelessTechnology {
    friend class TechManager;  // дружність не успадковується, тому оголошуємо і тут
private:
    double version;     // версія Bluetooth (1.0 - 6.0)
    int maxDevices;     // макс. кількість одночасно підключених пристроїв
    bool lowEnergy;     // підтримка Bluetooth Low Energy

public:
    Bluetooth();
    Bluetooth(const std::string& name, const std::string& manufacturer, const TechSpec& spec,
              double version, int maxDevices, bool lowEnergy);
    ~Bluetooth() override;

    double getVersion() const;
    int getMaxDevices() const;
    bool isLowEnergy() const;

    bool setVersion(double value);
    bool setMaxDevices(int value);
    void setLowEnergy(bool value);

    void input() override;
    void print() const override;
    std::string getType() const override;
    // Реальна швидкість Bluetooth ~ 70% від максимальної
    double transferTime(double sizeMB) const override;
};

#endif
