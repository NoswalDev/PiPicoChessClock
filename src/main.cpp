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
  VFD 256 x 50 GP1287BI
  Display and PCB produces heat, need temperature compensation
  IP5310 Recommended 5V 2.3A supply (based on other pcb design) for continuous full brightness all pixels lit
  Do menu setup until display arrives
*/

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <pico/stdlib.h>
#include <hardware/rtc.h>
#include <pico/util/datetime.h>
#include <hardware/clocks.h>
#include <hardware/pll.h>
#include <hardware/xosc.h>

// #include <stdio.h> //uart serial out


/* Constructor */
// U8G2_GP1287AI_256X50_F_4W_HW_SPI vfd(U8G2_R0, cs, dc [, reset]);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C dsp(U8G2_R0,U8X8_PIN_NONE);
//VCOMH0

#define SDA_PIN 4
#define SCL_PIN 5
#define MIN_PIN 0
#define PLS_PIN 1
#define MENU_PIN 2
#define RLY_PIN 8

//display stuff
#define dspwd 128
#define dspht 64
#define anifps 10

uint8_t scale = min(dspwd,dspht)>>4;
uint8_t ratio = 1; //dspwd/dspht;

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

//rtc
// datetime_t t = {
//         .year  = 2020,
//         .month = 06,
//         .day   = 05,
//         .dotw  = 5, // 0 is Sunday, so 5 is Friday
//         .hour  = 15,
//         .min   = 45,
//         .sec   = 00
// };
datetime_t t = {0,1,1,0,0,0,0};
// absolute_time_t at = 0;
char datetime_buf[256];
char *datetime_str = &datetime_buf[0];

bool flag = false;

void runBootAnimation(){
  //need to add framepacing
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
  etile = tilecntx>>1;
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
  dsp.clearBuffer();
  dsp.updateDisplayArea(0,0,1,tilecnty);
  dsp.updateDisplayArea(tilecntx-1,0,1,tilecnty);
}



void irq01(uint gpio, uint32_t events) //the parameters just need to be there. let's you figure out where the call is coming from.
{
  
  if(flag){
    xosc_disable();
    flag = false;
  }
  else{
    xosc_init();
    flag = true;
  }
  
}

void start_clocks(){
  clocks_init();
  // xosc_init();
  // clock_configure(clk_rtc,
  //                   0, // No GLMUX
  //                   CLOCKS_CLK_RTC_CTRL_AUXSRC_VALUE_XOSC_CLKSRC,
  //                   XOSC_MHZ * MHZ,
  //                   46875);
  rtc_init();
  rtc_set_datetime(&t);
  sleep_us(64);
  rtc_get_datetime(&t);
}


void setup() {
  //button setups
  gpio_init(MIN_PIN); //also sets function to SIO and disables output (set to input)
  // gpio_set_dir(MIN_PIN, false); //not needed with gpio_init()
  //gpio_set_dir_masked(); //set all gpios at once
  gpio_pull_up(MIN_PIN);
  /*irq handler notes
    gpio_add_raw_irq_handler() //adds additional irq handlers independent of default.
    gpio_acknowledge_irq() //needed for the callback functions
  */
  gpio_set_irq_enabled_with_callback(0,GPIO_IRQ_EDGE_FALL,true, irq01);

  //rtc
  start_clocks();

  //menu setup
  dsp.begin();
  dsp.setDrawColor(1);
  dsp.setFont(u8g2_font_5x7_tr);
  runBootAnimation();
}

void loop() {
  // put your main code here, to run repeatedly:
  dsp.clearBuffer();
  if(rtc_running()){
    rtc_get_datetime(&t);
    datetime_to_str(datetime_str, sizeof(datetime_buf), &t);
    dsp.drawStr(0,32,datetime_str);
  }
  dsp.sendBuffer();
  delay(100);
}
