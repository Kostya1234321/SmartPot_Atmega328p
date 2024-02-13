void init_mp3() {
  digitalWrite(Power_MP3, HIGH);
  mySoftwareSerial.begin(9600);  // Начало общения через TX(13) RX(14) с myDFPlayer
  digitalWrite(Power_Pomp, HIGH);
#if Debug >= 1
  //Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));  // Инициализация модуля
#endif
  while (!myDFPlayer.begin(mySoftwareSerial) && (millis() < 3000)) {  //Use softwareSerial to communicate with mp3.
#if Debug >= 1
  //  Serial.println(F("Unable to begin:"));
    //Serial.println(F("1.Please recheck the connection!"));  // Проверьте соединение
    //Serial.println(F("2.Please insert the SD card!"));      // Вставьте SD карту
#endif
    delay(500);
  }
#if Debug >= 1
 // Serial.println(F("DFPlayer Mini online."));  // Готов к работе
#endif
  myDFPlayer.volume(10);  // Установка громкости от 0 до 30
}

void MP3_doing() {

  static uint32_t static_Time[6];

  

  if (millis() - static_Time[0] > 5000) {

    static_Time[0] = millis();

    if (millis() - static_Time[1] > 8000 && Color < 8000) {
      static_Time[1] = millis();
      myDFPlayer.play(1 + millis() % 3);
    }

    if (millis() - static_Time[2] > 8000 && Color < 24000 && Color > 8000) {
      static_Time[2] = millis();
      myDFPlayer.play(4 + millis() % 3);
    }

    if (millis() - static_Time[3] > 8000 && Color > 24000) {
      static_Time[3] = millis();
      myDFPlayer.play(7 + millis() % 3);
    }
    Serial.print(analogRead(A3));
    if (millis() - static_Time[4] > 8000 && !status_bat) {
      static_Time[4] = millis();
      myDFPlayer.play(10 + millis() % 3);
    }

    if (millis() - static_Time[5] > 8000 && status_water) {
      static_Time[5] = millis();
      myDFPlayer.play(13 + millis() % 3);
    }
  }
}