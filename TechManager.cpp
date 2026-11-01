#include "TechManager.h"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

TechManager::TechManager() {
    cout << "[Конструктор TechManager] створено порожній список пристроїв\n";
}

TechManager::~TechManager() {
    cout << "\n========== ЗВІЛЬНЕННЯ ПАМ'ЯТІ ==========\n";
    for (WirelessTechnology* t : list) delete t;  // віртуальний деструктор
    list.clear();
    cout << "[Деструктор TechManager] список очищено\n";
}

size_t TechManager::size() const { return list.size(); }

// ---------- Допоміжне введення ----------

int TechManager::readMenu(const string& prompt, int minV, int maxV) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v && v >= minV && v <= maxV) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return v;
        }
        cout << "  ! Помилка: введіть число від " << minV << " до " << maxV << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double TechManager::readFileSize() {
    double v;
    while (true) {
        cout << "Розмір файлу для передачі, МБ [0.1 - 100000]: ";
        if (cin >> v && v >= 0.1 && v <= 100000) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return v;
        }
        cout << "  ! Помилка: некоректний розмір файлу.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ---------- Введення даних ----------

void TechManager::fillTestData() {
    list.push_back(new WiFi("TP-Link Archer AX55", "TP-Link", {5.0, 2402, 50, 900}, "802.11ax", 25, true));
    list.push_back(new WiFi("ASUS RT-AC68U", "ASUS", {2.4, 600, 70, 1200}, "802.11ac", 13, false));
    list.push_back(new Bluetooth("JBL Tune 520BT", "JBL", {2.4, 2, 10, 10}, 5.3, 1, true));
    list.push_back(new Bluetooth("Intel AX210 BT", "Intel", {2.4, 3, 30, 25}, 5.2, 7, true));
    // Приклад некоректних даних: сетери видадуть попередження і залишать значення за замовчуванням
    cout << "\nСтворення об'єкта з некоректними даними:\n";
    list.push_back(new Bluetooth("Old Dongle", "NoName", {2.4, 0.7, -5, 50}, 9.0, 12, false));
}

void TechManager::fillManual() {
    int n = readMenu("Скільки пристроїв ввести? [1 - 20]: ", 1, 20);
    for (int i = 0; i < n; ++i) {
        cout << "\n--- Пристрій #" << i + 1 << " ---\n";
        int type = readMenu("Тип (1 - WiFi, 2 - Bluetooth): ", 1, 2);
        WirelessTechnology* t = (type == 1) ? static_cast<WirelessTechnology*>(new WiFi())
                                            : static_cast<WirelessTechnology*>(new Bluetooth());
        t->input();  // віртуальний виклик
        list.push_back(t);
    }
}

// ---------- Виведення даних ----------

void TechManager::printAll() const {
    if (list.empty()) {
        cout << "  (список порожній)\n";
        return;
    }
    for (size_t i = 0; i < list.size(); ++i) {
        cout << "\nПристрій #" << i + 1 << ":\n";
        list[i]->print();  // віртуальний виклик
    }
}

void TechManager::printSummary() const {
    cout << "\n---------- Короткий перелік ----------\n";
    for (size_t i = 0; i < list.size(); ++i) {
        const WirelessTechnology* t = list[i];
        // Прямий доступ до приватних полів бази (дозволено через friend)
        cout << "  " << i + 1 << ". " << t->name << " (" << t->manufacturer << "), "
             << t->spec.maxSpeedMbps << " Мбіт/с";
        // Прямий доступ до приватних полів похідних класів (friend оголошено і в них)
        if (const WiFi* w = dynamic_cast<const WiFi*>(t)) {
            cout << " | WiFi " << w->standard << ", каналів: " << w->channels
                 << ", WPA3: " << (w->wpa3 ? "так" : "ні");
        } else if (const Bluetooth* b = dynamic_cast<const Bluetooth*>(t)) {
            cout << " | Bluetooth " << b->version << ", пристроїв: " << b->maxDevices
                 << ", LE: " << (b->lowEnergy ? "так" : "ні");
        }
        cout << "\n";
    }
}

void TechManager::printResult(const Result& r, double sizeMB) const {
    cout << fixed << setprecision(2);
    cout << "\n========== РЕЗУЛЬТАТ ==========\n";
    if (!r.fastest) {
        cout << "Немає даних для обчислення.\n";
        return;
    }
    cout << "Усього пристроїв: " << WirelessTechnology::getCount()
         << " (WiFi: " << r.wifiCount << ", Bluetooth: " << r.btCount << ")\n";
    cout << "Найшвидший:              " << r.fastest->name << " ("
         << r.fastest->spec.maxSpeedMbps << " Мбіт/с)\n";
    cout << "Найбільший радіус дії:   " << r.longestRange->name << " ("
         << r.longestRange->spec.rangeM << " м)\n";
    cout << "Найенергоефективніший:   " << r.mostEfficient->name << " ("
         << r.mostEfficient->efficiency() << " Мбіт/с на мВт)\n";
    cout << "Середня швидкість:       " << r.avgSpeed << " Мбіт/с\n";
    cout << "Сумарна потужність:      " << r.totalPower << " мВт\n";

    cout << "\nЧас передачі файлу " << sizeMB << " МБ:\n";
    for (const WirelessTechnology* t : list) {
        cout << "  " << left << setw(22) << t->name << right
             << setw(12) << t->transferTime(sizeMB) << " с  (" << t->getType() << ")\n";
    }
}

// ---------- Обробка даних ----------

Result TechManager::calculate() const {
    Result r;
    double sumSpeed = 0;
    for (const WirelessTechnology* t : list) {
        const TechSpec& s = t->spec;  // приватне поле, доступ через friend
        sumSpeed += s.maxSpeedMbps;
        r.totalPower += s.powerMw;
        if (!r.fastest || s.maxSpeedMbps > r.fastest->spec.maxSpeedMbps) r.fastest = t;
        if (!r.longestRange || s.rangeM > r.longestRange->spec.rangeM) r.longestRange = t;
        if (!r.mostEfficient || t->efficiency() > r.mostEfficient->efficiency()) r.mostEfficient = t;
        if (dynamic_cast<const WiFi*>(t)) ++r.wifiCount;
        else if (dynamic_cast<const Bluetooth*>(t)) ++r.btCount;
    }
    if (!list.empty()) r.avgSpeed = sumSpeed / list.size();
    return r;
}
