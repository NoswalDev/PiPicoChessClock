/*
  Operation:
    Buttons: left mid right
    Pause during game: mid once, displays invert
      Reset: hold mid, hold duration countdown/countup display until reset
    Turn indicator: line at bottom of timer
  UI:
    Start with mode selection:
      Bullet: 1m|0, 1m|1s, 2m|1s, 30s|0, 20s|1s
      Blitz: 3m|0, 3m|2s, 5m|0, 5m|5s, 5m|2s
      Rapid: 10m|0, 15m|10s, 30m|0, 10m|5s, 20m|0, 60m|0
      Custom: 
    Selected mode sets params:
      Time
      Delay: time given at start of a move, but remainder not added to total
      Increment: time added at the end of a move and remainder added to total
      Time at move X
      Time at move X
  Time types:
    Simple Time Delay: time added at start of move, but unused time is not kept.
    Bronstein Time Delay: time added after move up to time spent, but less than the delay set. Mathematically equivalent to Simple Time Delay.
    Fischer Increment: time added at start of move and unused time is kept.
  Program flags:
    Main time:
    Delay time:
    Added at start or end:
    Remainder kept:

  Code:
    write in single core and then allocate graphics processing to secondary core.

*/

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <pico/multicore.h>

#define GPIO_MIN 0
#define GPIO_PLU 1
#define GPIO_O 2


//screens
Adafruit_SSD1306 dsp0(128, 32, &Wire, -1);
Adafruit_SSD1306 dsp1(128, 32, &Wire, -1);

void core1_setup(){
  // dsp0.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // dsp1.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  // dsp0.clearDisplay();
  // dsp0.display();
  // dsp1.display();
}

void setup() {
  // multicore_launch_core1(core1_setup);
  dsp0.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  dsp1.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  dsp0.clearDisplay();
  dsp0.display();
  dsp1.display();
  pinMode(GPIO_MIN, INPUT_PULLUP);
  pinMode(GPIO_PLU, INPUT_PULLUP);
  pinMode(GPIO_O, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
}
