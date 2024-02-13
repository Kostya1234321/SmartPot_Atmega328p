void Water_level_sensor() {
  digitalWrite(Power_Water_sensor, HIGH);
  delay(50);
 
  Water = analogRead(Logical_Signal_Water_sensor);
  //Serial.print("Water");
  //Serial.println(Water);

  IF_OFF digitalWrite(Power_Water_sensor, LOW);
}

void Emcas_sensor_doing() {
  digitalWrite(Power_Emcas_sensor, HIGH);
  delay(50);

  Emcas = analogRead(Logical_Signal_Emcas_sensor);
  //Serial.print("Emcas");
 // Serial.println(Emcas);
  IF_OFF digitalWrite(Power_Emcas_sensor, LOW);
}

void Calibration()
{
  static uint32_t static_Time = millis();
  if(!((digitalRead(Button_calibration_moisture))^(digitalRead(Button_calibration_dryness)))){static_Time=millis();}
  if(digitalRead(Button_calibration_moisture) && millis()-static_Time>500)  {static_Time=millis();EEPROM.put(0, Emcas); Serial.println(Emcas); }
  if(digitalRead(Button_calibration_dryness) && millis()-static_Time>500)  {static_Time=millis();EEPROM.put(2, Emcas); Serial.println(Emcas); }

  init_EEPROM_settings();

 // Serial.println(moist_limit);
 // Serial.println(dry_limit);
}

void init_EEPROM_settings() {

  EEPROM.get(0, moist_limit);
  EEPROM.get(2, dry_limit);

  //Serial.println(moist_limit);
  //Serial.println(dry_limit);

  Checking_Variables_EEPROM();
}

void Checking_Variables_EEPROM() {
  if (dry_limit <= moist_limit) {dry_limit = 460; moist_limit = 280;}
  if (dry_limit < 0 || dry_limit > 1023) {dry_limit = 460;moist_limit = 280;}
  if (moist_limit < 0 || moist_limit > 1023) {moist_limit = 280;dry_limit = 460;}
}