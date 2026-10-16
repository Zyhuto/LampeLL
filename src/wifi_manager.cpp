#include "wifi_manager.h"

#include <Arduino.h>
#include <WiFi.h>

#include "serial_interface.h"
#include "wifi_config.h"

namespace {

constexpr unsigned long CONNECT_TIMEOUT_MS = 15000;
constexpr unsigned long RETRY_INTERVAL_MS = 10000;
unsigned long nextRetryAt = 0;

const char *statusName(wl_status_t status) {
    switch (status) {
    case WL_NO_SSID_AVAIL:
        return "SSID nicht gefunden";
    case WL_CONNECT_FAILED:
        return "Authentifizierung/Verbindung fehlgeschlagen";
    case WL_CONNECTION_LOST:
        return "Verbindung verloren";
    case WL_DISCONNECTED:
        return "getrennt";
    default:
        return "unbekannter Status";
    }
}

void connect() {
    AppSerial::serial.printf("Verbinde mit WLAN '%s' ...\n", WifiConfig::SSID);
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    // WLAN und Bluetooth laufen gleichzeitig; dafür muss Modem-Sleep aktiviert bleiben.
    WiFi.setSleep(true);
    WiFi.disconnect(false, false);
    WiFi.begin(WifiConfig::SSID, WifiConfig::PASSWORD);

    const unsigned long startedAt = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startedAt < CONNECT_TIMEOUT_MS) {
        delay(250);
        AppSerial::serial.print(".");
    }
    AppSerial::serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        AppSerial::serial.printf("WLAN verbunden, IP: %s\n", WiFi.localIP().toString().c_str());
        nextRetryAt = 0;
    } else {
        const wl_status_t status = WiFi.status();
        AppSerial::serial.printf("WLAN-Verbindung fehlgeschlagen (Status %d: %s).\n",
                                 status,
                                 statusName(status));
        nextRetryAt = millis() + RETRY_INTERVAL_MS;
    }
}

}

namespace WifiManager {

void begin() {
    connect();
}

void maintainConnection() {
    if (WiFi.status() == WL_CONNECTED) {
        return;
    }

    if (nextRetryAt == 0 || static_cast<long>(millis() - nextRetryAt) >= 0) {
        connect();
    }
}

}
