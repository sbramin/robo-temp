#include <OneWire.h>
#include <DallasTemperature.h>
#include "U8glib.h"

U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);

#define ONE_WIRE_BUS 2
#define UP_BUTTON 3
#define DOWN_BUTTON 4
#define BUZZER 8

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

char aBuf[9];
char tBuf[9];
float actualTemp;
float newTemp;
int targetTemp = 25;

void setup(void) {
  pinMode (8, OUTPUT);

  u8g.setRot180();
  u8g.setColorIndex(1);
  u8g.setFont(u8g_font_unifont);

  sensors.begin();
  sensors.requestTemperatures();
  actualTemp = sensors.getTempCByIndex(0);
  tempChange();
}


void loop() {
  sensors.requestTemperatures();
  newTemp = sensors.getTempCByIndex(0);

  if (digitalRead(UP_BUTTON)) {
    targetTemp += 1;
    tempChange();
  } else if (digitalRead(DOWN_BUTTON)) {
    targetTemp -= 1;
    tempChange();
  }   else if ((newTemp > actualTemp + 0.5) || (newTemp < actualTemp - 0.5)) {
    tempChange();

  }

}

void buzz(int) {
  int l;
  int d;
  while (l < 500)
  {
    digitalWrite (BUZZER, HIGH) ;
    delay(1);
    digitalWrite (BUZZER, LOW) ;
    delay(d);
    l++;
  }
}

void tempChange() {
  actualTemp = newTemp;

  if ((int)actualTemp <= targetTemp) {
    buzz(5);
  }


  u8g.firstPage();
  do {
    draw();
  } while ( u8g.nextPage() );
}

void draw() {
  dtostrf(actualTemp, 2, 2, aBuf);
  u8g.drawStr(10, 10, "A Temp:");
  u8g.drawStr(70, 10, aBuf);
  u8g.drawStr(115, 10, "C");

  sprintf(tBuf, "%d", targetTemp);
  u8g.drawStr(10, 30, "T Temp:");
  u8g.drawStr(70, 30, tBuf);
  u8g.drawStr(85, 30, ".00");
  u8g.drawStr(115, 30, "C");
}
