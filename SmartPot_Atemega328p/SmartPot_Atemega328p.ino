#include <GyverPower.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define Button_OFF_ON 3                 // пин кнопки включения
#define Button_calibration_dryness 12   // пин кнопки калибровки показующая максимальную сухость
#define Button_calibration_moisture 11  // пин кнопки калибровки показующая максимальную влажность
#define Power_Water_sensor 0     //6       // пин питания сенсора уровня воды//
#define Power_Emcas_sensor 1   //5         // пин питания сенсора влажности почвы//
#define Logical_Signal_Water_sensor A5  // аналоговый сигна с сенсора уровня воды
#define Logical_Signal_Emcas_sensor A4  // аналоговый сигна с сенсора влажности почвы
#define Battery_charge A3               // напряжение батареи для сообщения о малом заряде человеку
#define Power_Pomp 2    //7              // пин управляющий питанием помпы//
#define Power_MP3 4                     // пин управляющий питанием MP3 player
#define LED_COUNT 20
#define LED_PIN 5 //2//
#define TX 10               // связь с MP3 плеером
#define RX 9                // связь с MP3 плеером
#define saturation_led 255  // Задаем начальние настройки светодиодам
#define bright_led 25       // Задаем начальние настройки светодиодам
#define IF_ON if (digitalRead(Button_OFF_ON))
#define IF_OFF if (!digitalRead(Button_OFF_ON))
#define Time 4 * 1e3  // Время проверки состояния горшка во время выключеного прибора в миллисекундах
#define Debug 3

DFRobotDFPlayerMini myDFPlayer;                                     //объект MP3 плеера
SoftwareSerial mySoftwareSerial(RX, TX);                            // RX, TX для mp3 плеера
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);  // объэкт светодиодов

int Emcas, Water;
bool status_pomp = false, status_water = false, status_bat = false;
int64_t Color;
uint16_t moist_limit, dry_limit;

void All_pinMode() {
  pinMode(Button_OFF_ON, INPUT);
  pinMode(Power_Emcas_sensor, OUTPUT);
  pinMode(Power_Water_sensor, OUTPUT);
  pinMode(Power_Pomp, OUTPUT);
  pinMode(Power_MP3 , OUTPUT);
  pinMode(Button_calibration_moisture, INPUT);
  pinMode(Button_calibration_dryness , INPUT);
}

void wakeup() {
  power.wakeUp();
}

void setup() {
  Serial.begin(9600);
  All_pinMode();
  init_led();
  init_mp3();
  init_sleep();
  init_EEPROM_settings();
  attachInterrupt(1, wakeup, RISING);
}

void loop() {

  Emcas_sensor_doing();
  Water_level_sensor();
  Exceptional_cases();
  Pomp();
  IF_ON Calibration();
  IF_ON MP3_doing();

  IF_ON LED_doing(bright_led);
  IF_OFF LED_doing(0);

  IF_OFF Sleep();
}

void Exceptional_cases() {
  if (Water < 350) {
    status_water = true;
    LED_exceptional();
  } else status_water = false;

  if (analogRead(A3) < 736) status_bat = false;
  else status_bat = true;

  if (!status_bat) LED_exceptional();
}

void LED_exceptional() {
  static uint32_t static_Time = millis();
  static uint16_t bright_leds = bright_led;

  if (millis() - static_Time > 500) {
    static_Time = millis();
    LED_doing_exceptional(bright_led);
    delay(100);
  }
}
