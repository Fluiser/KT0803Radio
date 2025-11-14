#include <Arduino.h>
#include <KT0803.h>
#include <Wire.h>
#include <GyverOLED.h>


KT0803K fm;
GyverOLED<SSD1306_128x64> oled;

void setup() {
    pinMode(A2, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    delay(1000); // Delayed startup because kt0803 has not yet woken up for the first time.

    Wire.begin();
    fm.begin();
    fm.setFrequency(95);
    fm.setMute(false);

    fm.setPGA(0b110);
    fm.setMono();

    fm.setEurope();
    // fm.setBass(2);

    oled.init();
    oled.autoPrintln(true);
}

float lastMhz = 0.0;

void loop() {
    digitalWrite(LED_BUILTIN, 1);
    delay(10);
    digitalWrite(LED_BUILTIN, 0);
    delay(10);

    oled.clear();
    oled.home();

    float mhz = ((analogRead(A2)/1024.0f) * (108.0f-80.0f)) + 80.0f;

    if(abs((mhz - lastMhz)) > 0.3f) {

      lastMhz = mhz;
      fm.setFrequency(mhz);

      oled.setCursor(0,5);
      oled.print("Set new val...");
      oled.home();

    } else mhz = lastMhz;



    oled.print("MHz: ");
    oled.print(fm.getFrequency());
    oled.print("  Mute:");
    oled.println(fm.getMute());
    oled.print("ch: ");
    oled.println(fm.getChannel());
    oled.print("PHTCNST: ");
    oled.print(fm.getPHTCNST() ? 50 : 75);
    oled.println("us");
    oled.print("Bass: ");
    oled.print((int)fm.getBass());

    oled.update();
}
