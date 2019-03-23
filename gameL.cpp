#include <SPI.h>
#include <EEPROM.h>
#include "gameL.h"

#define cs   10
#define sclp 11
#define sdap 12
#define dc   9
#define rst  8


volatile uint8_t *dataport, *clkport, *csport, *rsport;
uint8_t datapinmask, clkpinmask, cspinmask, rspinmask, colstart, rowstart;

SPISettings spisettings;

int16_t _width, _height;

uint8_t tabcolor;

#define INITR_REDTAB   0x1

#define ST7735_SWRESET 0x01
#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5
#define ST7735_INVOFF  0x20
#define ST7735_MADCTL  0x36
#define ST7735_COLMOD  0x3A
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1
#define ST7735_NORON   0x13
#define ST7735_DISPON  0x29

#define ST7735_RAMWR   0x2C

#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160

#define DELAY 0x80
PROGMEM const static unsigned char
Rcmd1[] = {                 // Init for 7735R, part 1 (red or green tab)
  15,                       // 15 commands in list:
  ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
  150,                    //     150 ms delay
  ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
  255,                    //     500 ms delay
  ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
  0x01, 0x1C, 0x00,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
  ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
  0x01, 0x2C, 0x2D,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
  ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
  0x01, 0x2C, 0x2D,       //     Dot inversion mode
  0x01, 0x2C, 0x2D,       //     Line inversion mode
  ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
  0x07,                   //     No inversion
  ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
  0xA2,
  0x02,                   //     -4.6V
  0x84,                   //     AUTO mode
  ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
  0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
  ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
  0x0A,                   //     Opamp current small
  0x00,                   //     Boost frequency
  ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
  0x8A,                   //     BCLK/2, Opamp current small & Medium low
  0x2A,
  ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
  0x8A, 0xEE,
  ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
  0x0E,
  ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
  ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
  0xC8,                   //     row addr/col addr, bottom to top refresh
  ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
  0x05
},                 //     16-bit color
Rcmd2red[] = {              // Init for 7735R, part 2 (red tab only)
  2,                        //  2 commands in list:
  ST7735_CASET  , 4      ,  //  1: Column addr set, 4 args, no delay:
  0x00, 0x00,             //     XSTART = 0
  0x00, 0x7F,             //     XEND = 127
  ST7735_RASET  , 4      ,  //  2: Row addr set, 4 args, no delay:
  0x00, 0x00,             //     XSTART = 0
  0x00, 0x9F
},           //     XEND = 159

Rcmd3[] = {                 // Init for 7735R, part 3 (red or green tab)
  4,                        //  4 commands in list:
  ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
  0x02, 0x1c, 0x07, 0x12,
  0x37, 0x32, 0x29, 0x2d,
  0x29, 0x25, 0x2B, 0x39,
  0x00, 0x01, 0x03, 0x10,
  ST7735_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
  0x03, 0x1d, 0x07, 0x06,
  0x2E, 0x2C, 0x29, 0x2D,
  0x2E, 0x2E, 0x37, 0x3F,
  0x00, 0x00, 0x02, 0x10,
  ST7735_NORON  ,    DELAY, //  3: Normal display on, no args, w/delay
  10,                     //     10 ms delay
  ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
  100
};


void writecommand(uint8_t c) {
  SPI.beginTransaction(spisettings);


  *rsport &= ~rspinmask;
  *csport &= ~cspinmask;

  SPI.transfer(c);

  *csport |= cspinmask;
  SPI.endTransaction();
}


void writedata(uint8_t c) {
  SPI.beginTransaction(spisettings);


  *rsport |=  rspinmask;
  *csport &= ~cspinmask;

  SPI.transfer(c);

  *csport |= cspinmask;
  SPI.endTransaction();
}

void commandList(const uint8_t *addr) {

  uint8_t  numCommands, numArgs;
  uint16_t ms;

  numCommands = pgm_read_byte(addr++);   // Number of commands to follow
  while (numCommands--) {                // For each command...
    writecommand(pgm_read_byte(addr++)); //   Read, issue command
    numArgs  = pgm_read_byte(addr++);    //   Number of args to follow
    ms       = numArgs & DELAY;          //   If hibit set, delay follows args
    numArgs &= ~DELAY;                   //   Mask out delay bit
    while (numArgs--) {                  //   For each argument...
      writedata(pgm_read_byte(addr++));  //     Read, issue argument
    }

    if (ms) {
      ms = pgm_read_byte(addr++); // Read post-command delay time (ms)
      if (ms == 255) ms = 500;    // If 255, delay for 500 ms
      delay(ms);
    }
  }
}

void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {

  writecommand(ST7735_CASET); // Column addr set
  writedata(0x00);
  writedata(x0 + colstart);   // XSTART
  writedata(0x00);
  writedata(x1 + colstart);   // XEND

  writecommand(ST7735_RASET); // Row addr set
  writedata(0x00);
  writedata(y0 + rowstart);   // YSTART
  writedata(0x00);
  writedata(y1 + rowstart);   // YEND

  writecommand(ST7735_RAMWR); // write to RAM
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {

  if ((x >= _width) || (y >= _height)) return;
  if ((x + w - 1) >= _width)  w = _width  - x;
  if ((y + h - 1) >= _height) h = _height - y;

  setAddrWindow(x, y, x + w - 1, y + h - 1);

  uint8_t hi = color >> 8, lo = color;
  SPI.beginTransaction(spisettings);


  *rsport |=  rspinmask;
  *csport &= ~cspinmask;

  for (y = h; y > 0; y--) {
    for (x = w; x > 0; x--) {
      SPI.transfer(hi);
      SPI.transfer(lo);
    }
  }

  *csport |= cspinmask;
  SPI.endTransaction();
}

#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x08
#define MADCTL_MH  0x04

void setRotation(uint8_t m) {

  writecommand(ST7735_MADCTL);
  switch (m) {
    case 0:
      writedata(MADCTL_MX | MADCTL_MY | MADCTL_RGB);
      _width  = ST7735_TFTWIDTH;
      _height = ST7735_TFTHEIGHT;
      break;
    case 1:
      writedata(MADCTL_MY | MADCTL_MV | MADCTL_RGB);
      _width  = ST7735_TFTHEIGHT;
      _height = ST7735_TFTWIDTH;
      break;
    case 2:
      writedata(MADCTL_RGB);
      _width  = ST7735_TFTWIDTH;
      _height = ST7735_TFTHEIGHT;
      break;
    case 3:
      writedata(MADCTL_MX | MADCTL_MV | MADCTL_RGB);
      _width  = ST7735_TFTHEIGHT;
      _height = ST7735_TFTWIDTH;
      break;
  }
}

uint16_t newColor(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

#define RESOLUTION 65536    // Timer1 is 16 bit
unsigned char clockSelectBits;
char oldSREG;         // To hold Status Register while ints disabled

void initializeInterrupt(long microseconds)
{
  TCCR1A = 0;                 // clear control register A
  TCCR1B = _BV(WGM13);        // set mode 8: phase and frequency correct pwm, stop the timer

  long cycles = (F_CPU / 2000000) * microseconds;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  if (cycles < RESOLUTION)              clockSelectBits = _BV(CS10);             // no prescale, full xtal
  else if ((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11);             // prescale by /8
  else if ((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11) | _BV(CS10); // prescale by /64
  else if ((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12);             // prescale by /256
  else if ((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12) | _BV(CS10); // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum

  oldSREG = SREG;
  cli();              // Disable interrupts for 16 bit register access
  ICR1 = cycles;                                          // ICR1 is TOP in p & f correct pwm mode
  SREG = oldSREG;

  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
  TCCR1B |= clockSelectBits;                                          // reset clock select register, and starts the clock

  //isrCallback = isr;                                       // register the user's callback with the real ISR
  TIMSK1 = _BV(TOIE1);                                     // sets the timer overflow interrupt enable bit
  // might be running with interrupts disabled (eg inside an ISR), so don't touch the global state
  //  sei();
  TCCR1B |= clockSelectBits; // resume
}

void getFullRect(int16_t *x1, int16_t *y1, byte *w1, byte *h1, int16_t *x2, int16_t *y2, byte *w2, byte *h2)
{
  int16_t hx = max(*x1 + *w1, *x2 + *w2);
  int16_t hy = max(*y1 + *h1, *y2 + *h2);
  int16_t lx = min(*x1, *x2);
  int16_t ly = min(*y1, *y2);
  int16_t w = hx - lx;
  int16_t h = hy - ly;
  *x1 = lx;
  *y1 = ly;
  *w1 = w;
  *h1 = h;
  *x2 = *x2 - lx;
  *y2 = *y2 - ly;
}



void drawBit(int16_t x, int16_t y, byte w, byte h, int16_t xo, int16_t yo, byte wo, byte ho, uint8_t arr[])
{

  getFullRect(&xo, &yo, &wo, &ho, &x, &y, &w, &h);

  if ((xo + wo - 1 < 0) || (yo + ho - 1 < 0)) return;
  if ((xo - 1 > _width) || (yo - 1 > _height)) return;

  byte mx = 0, my = 0, mAx = 0, mAy = 0;

  if (xo < 0)
  {
    mx = -xo;
    xo = 0;
  }

  else if (xo + wo > _width)
  {
    mAx = xo + wo - _width;
  }

  if (yo < 0)
  {
    my = -yo;
    yo = 0;
  }

  else if (yo + ho > _height)
  {
    mAy = yo + ho - _height;
  }

  setAddrWindow(xo, yo, xo + wo - 1 - mx - mAx, yo + ho - 1 - my - mAy);


  SPI.beginTransaction(spisettings);


  *rsport |=  rspinmask;
  *csport &= ~cspinmask;

  uint8_t pos = 0, count = 0;
  uint8_t color1 = 255;
  for (byte lyo = my; lyo < ho - mAy; lyo++) {
    for (byte lxo = mx; lxo < wo - mAx; lxo++) {
      if (lxo < w + x && lyo < h + y && 0 <= lxo - x && 0 <= lyo - y)
      {
        if ((arr[((lxo - x) + w * (lyo - y)) / 8] >> (7 - ((lxo - x) + w * (lyo - y)) % 8)) & 1 == 1)
        {
          SPI.transfer(0);
          SPI.transfer(0);
        }
        else
        {
          SPI.transfer(255);
          SPI.transfer(255);
        }
      }
      else
      {
        SPI.transfer(255);
        SPI.transfer(255);
      }
    }
  }

  *csport |= cspinmask;
  SPI.endTransaction();
}

void drawBitL(int16_t x, int16_t y, byte w, byte h, int16_t xo, int16_t yo, byte wo, byte ho, uint8_t arr[])
{

  getFullRect(&xo, &yo, &wo, &ho, &x, &y, &w, &h);

  if ((xo + wo - 1 < 0) || (yo + ho - 1 < 0)) return;
  if ((xo - 1 > _width) || (yo - 1 > _height)) return;

  byte mx = 0, my = 0, mAx = 0, mAy = 0;

  if (xo < 0)
  {
    mx = -xo;
    xo = 0;
  }

  else if (xo + wo > _width)
  {
    mAx = xo + wo - _width;
  }

  if (yo < 0)
  {
    my = -yo;
    yo = 0;
  }

  else if (yo + ho > _height)
  {
    mAy = yo + ho - _height;
  }

  setAddrWindow(xo, yo, xo + wo - 1 - mx - mAx, yo + ho - 1 - my - mAy);


  SPI.beginTransaction(spisettings);


  *rsport |=  rspinmask;
  *csport &= ~cspinmask;

  uint8_t pos = 0, count = 0;
  uint8_t color1 = 255;
  for (byte lyo = my; lyo < ho - mAy; lyo++) {
    for (byte lxo = mx; lxo < wo - mAx; lxo++) {
      if (lxo < w + x && lyo < h + y && 0 <= lxo - x && 0 <= lyo - y)
      {
        int16_t prec = (lxo - x) + w * (lyo - y);
        if (arr[prec] > 0)
        {
          SPI.transfer(0);
          SPI.transfer(0);
        }
        else
        {
          SPI.transfer(255);
          SPI.transfer(255);
        }
      }
      else
      {
        SPI.transfer(255);
        SPI.transfer(255);
      }
    }
  }

  *csport |= cspinmask;
  SPI.endTransaction();
}

void clscr()
{
	fillRect(0, 0, 160, 128, newColor(255, 255, 255));
}

uint8_t _rand(uint8_t n)
{
	return random(0, n);
}

//x1,y1 mat drawing start point
//w1,h1 mat size
//x2,y2 offset for new mat
//w2,h2 new mat size


//extern "C" void asfunc();

//extern "C" void otherfunc() {

//}

void setup() {
  //asfunc();

  colstart  = rowstart = 0; // May be overridden in init func

  pinMode(dc, OUTPUT);
  pinMode(cs, OUTPUT);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  csport    = portOutputRegister(digitalPinToPort(cs));
  cspinmask = digitalPinToBitMask(cs);
  rsport    = portOutputRegister(digitalPinToPort(dc));
  rspinmask = digitalPinToBitMask(dc);
  //Serial.begin(115200);
  SPI.begin();
  
  _pre_start();

  spisettings = SPISettings(20000000L, MSBFIRST, SPI_MODE0);

  *csport &= ~cspinmask;
  if (rst) {
    pinMode(rst, OUTPUT);
    digitalWrite(rst, HIGH);
    delay(500);
    digitalWrite(rst, LOW);
    delay(500);
    digitalWrite(rst, HIGH);
    delay(500);
  }

  commandList(Rcmd1);

  commandList(Rcmd2red);

  commandList(Rcmd3);

  tabcolor = INITR_REDTAB;

  setRotation(1);
  fillRect(0, 0, 160, 128, newColor(255, 255, 255));
  
  initializeInterrupt(24550);
}

ISR(TIMER1_OVF_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  _game_loop();
}

void loop() {
}