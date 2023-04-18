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
  Battery indicator
  VFD 256 x 50
  Display and PCB produces heat
  Recommended 5V 2.3A supply (based on other pcb design) for continuous full brightness all pixels lit
  Do menu setup until display arrives
*/

#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>


/* Constructor */
// U8G2_GP1287AI_256X50_F_4W_HW_SPI vfd(U8G2_R0, cs, dc [, reset]);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C dsp(U8G2_R0, U8X8_PIN_NONE);

// #define SDA 0
// #define SCL 1
#define GPIO_MIN 0
#define GPIO_PLU 1
#define GPIO_O 2

//display stuff
#define dspwd 128
#define dspht 64
#define anifps 10

uint8_t scale = min(dspwd,dspht)>>4;
uint8_t ratio = dspwd/dspht;

uint8_t dspctrx = (dspwd>>1)-1;
uint8_t dspctry = (dspht>>1)-1;
uint8_t tilecntx = (dspwd>>3);
uint8_t tilecnty = (dspht>>3);

uint8_t drawn = 0;
uint8_t drawn2 = 0;

uint8_t vtile = 0;
uint8_t htile = 0;
uint8_t etile = 0;
uint8_t ftile = 0;

void runBootAnimation(){
  //disc
  htile = (dspctrx - scale)>>3;
  vtile = (dspctry - scale)>>3;
  etile = (dspctrx + 1 + scale)>>3;
  ftile = (dspctry + 1 + scale)>>3;
  for(int i = 0; i <= anifps; i++){
    dsp.clearBuffer();
    drawn = max(sq(i/(float)anifps)*scale,1);
    dsp.drawDisc(dspctrx, dspctry, drawn, U8G2_DRAW_UPPER_LEFT);
    dsp.drawDisc(dspctrx, dspctry + 1, drawn, U8G2_DRAW_LOWER_LEFT);
    dsp.drawDisc(dspctrx + 1, dspctry, drawn, U8G2_DRAW_UPPER_RIGHT);
    dsp.drawDisc(dspctrx + 1, dspctry + 1, drawn, U8G2_DRAW_LOWER_RIGHT);
    dsp.updateDisplayArea(htile, vtile, etile - htile + 1, ftile - vtile + 1);
  }
  //vert
  for(int i = 0; i <= anifps; i++){
    dsp.clearBuffer();
    //draw rect
    drawn = scale*(1 - i/(float)anifps);
    drawn2 = (dspctry+1)*sq(i/(float)anifps);
    dsp.drawBox(dspctrx - max(0,drawn - 1), dspctry - drawn2 + 1, max(1,drawn) , drawn2);
    dsp.drawBox(dspctrx - max(0,drawn - 1), dspctry + 1, max(1,drawn) , drawn2);
    dsp.drawBox(dspctrx + 1, dspctry - drawn2 + 1, max(1,drawn) , drawn2);
    dsp.drawBox(dspctrx + 1, dspctry + 1, max(1,drawn) , drawn2);
    //draw disc
    dsp.drawDisc(dspctrx, dspctry, drawn, U8G2_DRAW_UPPER_LEFT);
    dsp.drawDisc(dspctrx, dspctry + 1, drawn, U8G2_DRAW_LOWER_LEFT);
    dsp.drawDisc(dspctrx + 1, dspctry, drawn, U8G2_DRAW_UPPER_RIGHT);
    dsp.drawDisc(dspctrx + 1, dspctry + 1, drawn, U8G2_DRAW_LOWER_RIGHT);
    dsp.updateDisplayArea(htile, 0, etile-htile+1, tilecnty);
  }
  //scan
  etile = 0;
  for(int i = 0; i <=anifps*ratio; i++){
    dsp.clearBuffer();
    drawn = (sq(i/(float)anifps/ratio)*dspctrx);
    dsp.drawVLine(dspctrx - drawn,0,dspht);
    dsp.drawVLine(dspctrx + 1 + drawn,0,dspht);
    htile = (dspctrx - drawn)>>3;
    dsp.updateDisplayArea(htile, 0, 1, tilecnty);
    dsp.updateDisplayArea(tilecntx - htile - 1, 0, 1, tilecnty);
    if (etile!=htile){
      dsp.updateDisplayArea(etile, 0, 1, tilecnty);
      dsp.updateDisplayArea(tilecntx - etile - 1, 0, 1, tilecnty);
      etile = htile;
    }
  }
}

void setup() {
  //button setups
  pinMode(GPIO_MIN, INPUT_PULLUP);
  pinMode(GPIO_PLU, INPUT_PULLUP);
  pinMode(GPIO_O, INPUT_PULLUP);
  //menu setup


}

void loop() {
  // put your main code here, to run repeatedly:
}
void setup1() {
  // vfd.begin();
  dsp.begin();
  dsp.setDrawColor(1);
}

void loop1() {
  dsp.clearBuffer();
  dsp.sendBuffer();
  delay(1000);
  // startAnimation();
  runBootAnimation();
  delay(1000);
}
