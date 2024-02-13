void init_led() {
  strip.begin();  // Инициализируем объект NeoPixel
  strip.show();   // Отключаем все пиксели на ленте
}

void LED_doing(uint8_t bright_leds) {
  static uint32_t static_Time;
  
  Emcas = constrain(Emcas, moist_limit, dry_limit);
  Color = map(Emcas, moist_limit, dry_limit, 32768, 0);

  strip.fill(strip.ColorHSV(Color, saturation_led, bright_leds), 0, LED_COUNT);
  strip.show();
  
}


void LED_doing_exceptional(uint8_t bright_leds) {
  strip.fill(strip.ColorHSV(0, saturation_led, bright_leds), 0, 1);
  strip.show();
}

void init_sleep() {
  power.autoCalibrate();
  power.setSleepMode(POWERDOWN_SLEEP);
}

void Sleep() {
  if ( status_bat && !status_water &&  !status_pomp &&(!digitalRead(Button_OFF_ON))) {
    myDFPlayer.pause();  //pause the mp3
    delay(10);
    digitalWrite(RX, LOW);
    digitalWrite(TX, LOW);
    digitalWrite(Power_MP3, LOW);
    LED_doing(0);  
    Serial.println("SLEEP");
    delay(100);
    power.sleepDelay(Time);
    IF_ON init_led();
    IF_ON myDFPlayer.start();
  }
}