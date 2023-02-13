#include <WiFi.h>
#include <LiquidCrystal.h>
#include "Decralation.h"
#include "WiFi_credentials.h"
#include "Functions.h"

void setup() {
  pin_conf();
  Serial.begin(9600);
  Serial2.begin(9600);
  lcd.begin(16, 2);

  WiFi.begin(ssid, password);
  WiFi_connection();
  lcd_print("Wi-Fi", "CONNECTED"); delay(1e3);
  lcd_print("UHF RFID", "Security System");
}

void loop() {
  WiFi_connection();
  for (byte i = 0; i < no_of_cards; i++) {
    Serial.println("Scanning card");
    card_status[i] = scan_card(card[i]);
    lcd_print(card_id[i], card_status[i]);

    if (card_id[i] == warning_card && card_status[i] == "Present") warning = false;
    if (card_status[i] != previous_status[i]) {
      Serial.println("Send a data request");
      send_request("device_id=" + card_id[i] + "&status=" + card_status[i]);
      previous_status[i] = card_status[i];
      if (card_status[i] == "Absent") {
        warning_card = card_id[i];
        warning = true;
      }
    }
  }
  
  if (digitalRead(btn1_pin) == LOW) {
    while(digitalRead(btn1_pin) == LOW)
        lcd_print("Show missing IDs", "DONE");
    digitalWrite(ind_pin, HIGH); delay(2e3);
    while (true) {
      for (byte i = 0; i < no_of_cards; i++) {
        if (card_status[i] == "Absent") {
          lcd_print("Missing ID", card_id[i]);
          delay(1e3);
        }
      }
      if (digitalRead(btn1_pin) == LOW) {
        lcd_print("QUIT", ""); delay(1e3);
        lcd_print("UHF RFID", "Security System");
        break;
      }
    }
    digitalWrite(ind_pin, LOW);
  }
  if (digitalRead(btn2_pin) == HIGH) {
    while(digitalRead(btn2_pin) == HIGH)
       lcd_print("Cancel alert", "DONE");
    warning = false;
    lcd_print("UHF RFID", "Security System");
  }

  if (warning) {
    for (byte i = 0; i < 3; i++)
      warning_indicator();
  } else {
    digitalWrite(lock_pin, HIGH);
  }
}
