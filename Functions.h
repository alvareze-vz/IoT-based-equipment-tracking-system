void pin_conf(){
  pinMode(ind_pin, OUTPUT);
  pinMode(btn1_pin, INPUT);
  pinMode(btn2_pin, INPUT);
  pinMode(lock_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
}

String scan_card(String card_to_scan){
  byte byte_data;
  String card_no = "";
  unsigned long initial_time = millis();
  boolean is_present = false, in_time = true;
  
  while(!is_present){
    while (Serial2.available()) {
     byte_data = Serial2.read();
     card_no = card_no + String(byte_data);
    }
    
    if (card_no.indexOf(card_to_scan) != -1)
      return "Present";
    if(millis() - initial_time >= scanning_time_limit)
      return "Absent";
  }
}


String cut_string(String typing, byte _length) {
  typing = typing.substring(0, _length);
  return typing;
}

void lcd_print(String str1, String str2) {
  lcd.clear();
  const byte column = 16;
  if (str1.length() > column) str1 = cut_string(str1, column);
  if (str2.length() > column) str2 = cut_string(str2, column);

  byte pre_space1 = (column - str1.length()) / 2;
  byte pre_space2 = (column - str2.length()) / 2;

  lcd.setCursor(pre_space1, 0); lcd.print(str1);
  lcd.setCursor(pre_space2, 1); lcd.print(str2);
  delay(500);
}

void send_request(String _data){
  if (!client.connect(host, port)) {
    Serial.println("Client to server connection fail");
    lcd_print("Client to Server", "connection fail");
    return;
  }

  Serial.println(String("GET ")+ url + _data +" HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
  client.print(String("GET ")+ url + _data +" HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
  delay(100);
  while (client.available()) {
    String payload = client.readStringUntil('\r');
    Serial.println(payload);
  }
}


void WiFi_connection(){
  while(WiFi.status() != WL_CONNECTED){
    lcd_print("Wi-Fi", "Connecting.");
    lcd_print("Wi-Fi", "Connecting..");
  }
}

void warning_indicator(){
  Serial.println("Warning Detected");
  digitalWrite(lock_pin, LOW);
  digitalWrite(ind_pin, HIGH);
  digitalWrite(buzzer_pin, HIGH);
  delay(1e3);
  digitalWrite(ind_pin, LOW);
  digitalWrite(buzzer_pin, LOW);
  delay(1e3);
}
