#define ind_pin 15
#define btn1_pin 0
#define btn2_pin 2
#define lock_pin 4
#define buzzer_pin 23
#define no_of_cards 4
#define scanning_time_limit 5000
#define host "192.168.43.71"
#define port 80
#define url " /localhost/cle_ms/lems_web/api/update.php?"


byte rs = 13, en = 14, d4 = 27, d5 = 26, d6 = 25, d7 = 33;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
WiFiClient client;
boolean warning;

String previous_status[no_of_cards];
String card_status[no_of_cards];
String warning_card;

String card[no_of_cards] = {
  "147159141229235",
  "147159149229235",
  "147159145229235",
  "147159153229235"
};

String card_id[no_of_cards] = {
  "Laptop 11",
  "Projector",
  "PC Unit 3",
  "Printer"
};
