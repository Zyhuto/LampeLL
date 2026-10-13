#include "rfid.h"

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

namespace {

constexpr byte SS_PIN = 5;
constexpr byte RST_PIN = 22;
constexpr byte SCK_PIN = 18;
constexpr byte MISO_PIN = 19;
constexpr byte MOSI_PIN = 23;

MFRC522 reader(SS_PIN, RST_PIN);

}

namespace Rfid {

void begin() {
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
    reader.PCD_Init();

    Serial.println("RFID-Modul bereit.");
    Serial.println("Bitte RFID-Karte auflegen...");
}

void poll() {
    // Prüfen, ob eine neue Karte vorhanden ist
    if (!reader.PICC_IsNewCardPresent()) {
        return;
    }

    // Kartendaten lesen
    if (!reader.PICC_ReadCardSerial()) {
        return;
    }

    Serial.print("RFID erkannt! UID: ");

    for (byte i = 0; i < reader.uid.size; i++) {
        if (reader.uid.uidByte[i] < 0x10) {
            Serial.print("0");
        }

        Serial.print(reader.uid.uidByte[i], HEX);
        Serial.print(" ");
    }

    Serial.println();

    reader.PICC_HaltA();
    reader.PCD_StopCrypto1();

    delay(500);
}

}
