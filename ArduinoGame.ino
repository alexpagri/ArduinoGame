#include <SPI.h>
#include <EEPROM.h>
#include "gameL.h"

extern "C" void processInteractive();

extern "C" void speedReset();

extern "C" void displayScore();

extern "C" void displayHScore();

extern "C" void _game_loop_();

extern "C" void _pre_start_();

extern int16_t xoo = 0;
extern const uint8_t a0[] = {6, 7, 49, 108, 243, 205, 163, 0};
extern const uint8_t a1[] = {6, 7, 49, 195, 12, 48, 207, 192};
extern const uint8_t a2[] = {6, 7, 123, 49, 222, 227, 15, 192};
extern const uint8_t a3[] = {6, 7, 124, 99, 14, 15, 55, 128};
extern const uint8_t a4[] = {6, 7, 24, 230, 146, 124, 32, 128};
extern const uint8_t a5[] = {6, 7, 251, 15, 131, 15, 55, 128};
extern const uint8_t a6[] = {6, 7, 57, 140, 62, 207, 55, 128};
extern const uint8_t a7[] = {6, 7, 125, 16, 134, 48, 195, 0};
extern const uint8_t a8[] = {6, 7, 115, 46, 156, 190, 55, 128};
extern const uint8_t a9[] = {6, 7, 123, 60, 223, 12, 39, 128};
extern const uint8_t hi[] = {12, 7, 141, 248, 204, 140, 207, 204, 140, 200, 204, 141, 240};
extern const uint8_t superCompDino1[] = {20, 21, 0, 31, 224, 3, 127, 0, 63, 240, 3, 255, 0, 63, 240, 3, 224, 0, 127, 200, 15, 192, 129, 252, 12, 63, 240, 231, 253, 15, 255, 192, 255, 252, 7, 255, 192, 63, 248, 1, 255, 0, 15, 224, 0, 115, 128, 6, 0, 0, 64, 0, 6, 0, 0};
extern const uint8_t superCompDino2[] = {20, 21, 0, 31, 224, 3, 127, 0, 63, 240, 3, 255, 0, 63, 240, 3, 224, 0, 127, 200, 15, 192, 129, 252, 12, 63, 240, 231, 253, 15, 255, 192, 255, 252, 7, 255, 192, 63, 248, 1, 255, 0, 15, 224, 0, 102, 0, 3, 32, 0, 2, 0, 0, 48, 0};
extern const uint8_t superCompBg[] = {160, 7, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 56, 0, 0, 24, 0, 1, 224, 12, 0, 128, 1, 224, 0, 0, 0, 0, 112, 0, 0, 0, 0, 0, 70, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 8, 0, 3, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 128, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0};
extern const uint8_t decompBg[] = {160, 7, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 82, 82, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 82, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 82, 82, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 82, 82, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 82, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 0, 0, 0, 82, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 82, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 82, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#define GROUND 91
extern int16_t gli = GROUND - superCompDino1[1];
#define MAXCACT 5
#define CACT_T 6
extern int16_t cvec[MAXCACT] = {0};
extern uint8_t cvecT[MAXCACT] = {0};
extern uint8_t cvecI = 0;
extern const uint8_t minDist = 80; //must be larger than XPOS + superCompDino1[0];
extern int16_t cDist = minDist;
extern const uint8_t cact1[] = {7, 17, 16, 112, 233, 219, 183, 110, 221, 191, 115, 225, 195, 135, 14, 28, 56};
extern const uint8_t cact2[] = {16, 17, 24, 8, 24, 28, 24, 93, 27, 125, 27, 125, 219, 125, 219, 125, 219, 125, 222, 29, 216, 31, 120, 28, 24, 28, 24, 28, 24, 28, 24, 28, 24, 28, 24, 28};
extern const uint8_t cact3[] = {25, 17, 24, 8, 12, 12, 14, 6, 6, 23, 27, 3, 107, 141, 177, 181, 214, 222, 218, 235, 111, 109, 117, 183, 182, 186, 219, 222, 93, 61, 236, 46, 135, 158, 31, 67, 3, 7, 225, 129, 129, 224, 192, 192, 224, 96, 96, 112, 48, 48, 56, 24, 24, 28, 12, 0};
extern const uint8_t cact4[] = {12, 23, 6, 0, 240, 15, 0, 240, 15, 0, 242, 79, 126, 247, 239, 126, 247, 239, 126, 247, 239, 127, 254, 127, 195, 240, 15, 0, 240, 15, 0, 240, 15, 0, 240, 15, 64};
extern const uint8_t cact5[] = {24, 23, 14, 0, 96, 14, 0, 240, 14, 0, 240, 14, 14, 240, 14, 14, 240, 14, 78, 242, 206, 110, 247, 206, 110, 247, 206, 110, 247, 206, 110, 247, 206, 111, 247, 206, 103, 247, 206, 99, 247, 255, 192, 254, 255, 128, 252, 126, 0, 240, 14, 0, 240, 14, 0, 240, 14, 0, 240, 14, 0, 240, 14, 0, 240, 14, 0, 240, 14, 0, 244};
extern const uint8_t cact6[] = {36, 24, 14, 0, 0, 7, 0, 224, 24, 0, 112, 14, 1, 128, 7, 0, 224, 24, 6, 112, 14, 1, 144, 103, 0, 224, 25, 6, 112, 206, 97, 144, 103, 60, 230, 25, 6, 115, 206, 105, 144, 103, 60, 230, 223, 6, 115, 206, 109, 134, 127, 60, 230, 216, 103, 243, 206, 109, 150, 191, 60, 230, 217, 104, 115, 255, 205, 150, 135, 231, 224, 249, 104, 112, 14, 7, 150, 135, 0, 224, 25, 248, 112, 14, 1, 142, 7, 0, 224, 24, 96, 112, 14, 1, 134, 7, 0, 224, 24, 96, 112, 14, 1, 134, 7, 1, 225, 24, 96, 240};
extern uint8_t SPEED = 1;
#define XPOS 10
extern const uint8_t jumpHeights[] = {38, 13, 9, 7};
extern uint8_t jump_height = jumpHeights[SPEED - 1];
extern const uint8_t bgSpeeds[] = {1, 4, 6, 8};
extern uint8_t bgs = bgSpeeds[SPEED - 1];
extern const uint8_t ibgSpeeds[] = {5, 2, 1, 0};//{32, 4, 2, 1}
extern uint8_t ibgs = ibgSpeeds[SPEED - 1];
extern const uint8_t animSpeeds[] = {8, 5, 3, 2};
extern uint8_t cyclesanim = animSpeeds[SPEED - 1];
extern uint8_t cycleanim = 0;
extern uint8_t *anim = superCompDino1;
#define COL_DX 3
#define COL_DY 5
extern int8_t cColHeight = -1;
extern uint8_t pause = 0;
extern uint8_t pauseSpawn = 0;
#define SPEEDUP_TIMER 200
extern uint16_t speedupTimer = 0;
#define SPEEDUP_CYCLES 100
extern uint16_t speedupCycles = 0;
#define COOLDOWN 30
extern uint8_t cooldown = COOLDOWN;
#define SCORE_DISPX 120
#define SCORE_DISPY 10
#define SCORE_SPACING 7
extern uint16_t cScore = 0;
extern uint16_t hScore = 0;
extern int8_t dx = -jump_height;
extern uint8_t doJump = 1;
extern int16_t bgposx = 0;

extern uint8_t *mapCactus[] = {cact1, cact2, cact3, cact4, cact5, cact6};

extern uint8_t *getNumber[] = {a0, a1, a2, a3, a4, a5, a6, a7, a8, a9};

void processInteractive() {
  //processInteractiveA();
  if (cDist >= minDist && cvecI < MAXCACT && pauseSpawn == 0) {
    if (_rand(20) == 0) {
      uint8_t *cact = mapCactus[cvecT[cvecI] = _rand(CACT_T)];
      cvec[cvecI] = _width + cact[0];
      cvecI++;
    }
  }
  for (int i = 0; i < cvecI; i++) {
    uint8_t *cact = mapCactus[cvecT[i]];
    cvec[i] -= bgs;
    drawBit(cvec[i] - cact[0], GROUND - cact[1], cact[0], cact[1], cvec[i] - cact[0] + bgs, GROUND - cact[1], cact[0], cact[1], cact + 2);
  }
  if (cvecI > 0) { //only last cactus collides
    uint8_t *cact = mapCactus[cvecT[0]];
    if (cvec[0] - cact[0] < XPOS + superCompDino1[1] - COL_DX && cvec[0] > XPOS + COL_DX) {
      cColHeight = cact[1] - COL_DY;
    } else cColHeight = -1;
  }
  if (cvec[0] < 0) {
    for (int i = 0; i < cvecI - 1; i++) {
      cvec[i] = cvec[i + 1];
      cvecT[i] = cvecT[i + 1];
    }
    cvecI--;
    cvec[cvecI] = 0;
    cColHeight = -1;
  }
  cDist = _width - cvec[cvecI - 1];
  }

void speedReset() {
  jump_height = jumpHeights[SPEED - 1];
  bgs = bgSpeeds[SPEED - 1];
  ibgs = ibgSpeeds[SPEED - 1];
  cyclesanim = animSpeeds[SPEED - 1];
  dx = -jump_height;
  }

void displayScore() {
  uint16_t csc = cScore;
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t *nb = getNumber[csc % 10];
    drawBit(SCORE_DISPX + SCORE_SPACING * (3 - i), SCORE_DISPY, nb[0], nb[1], SCORE_DISPX + SCORE_SPACING * (3 - i), SCORE_DISPY, nb[0], nb[1], nb + 2);
    csc /= 10;
  }
  }

void displayHScore() {
  uint16_t csc = hScore;
  drawBit(SCORE_DISPX + SCORE_SPACING - 75, SCORE_DISPY, hi[0], hi[1], SCORE_DISPX + SCORE_SPACING - 75, SCORE_DISPY, hi[0], hi[1], hi + 2);
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t *nb = getNumber[csc % 10];
    drawBit(SCORE_DISPX + SCORE_SPACING * (3 - i) - 50, SCORE_DISPY, nb[0], nb[1], SCORE_DISPX + SCORE_SPACING * (3 - i) - 50, SCORE_DISPY, nb[0], nb[1], nb + 2);
    csc /= 10;
  }
  }

void _pre_start() {
  if (~PIND >> 6 & 1) {
    EEPROM.write(0, 0);
    EEPROM.write(1, 0);
  }
  _delay_ms(1000);
  hScore = EEPROM.read(0);
  *((uint8_t *)&hScore + 1) = EEPROM.read(1);
  _delay_ms(1000);
  }

void _game_loop()
  {
  uint8_t btn = ~PIND >> 6 & 1;
  if (pause == 0) {
    processInteractive();
    if (gli + superCompDino1[1] > GROUND - cColHeight) {
      pause = 1;
    }
    drawBitL(160 + bgposx, 91, decompBg[0], decompBg[1], 160 + bgposx, 91, 0, 0, decompBg + 2);
    drawBitL(bgposx, 91, decompBg[0], decompBg[1], bgposx, 91, 0, 0, decompBg + 2);
    if (bgposx <= -160)bgposx += 160;
    bgposx -= bgs;
    drawBit(XPOS, gli, anim[0], anim[1], XPOS, xoo, anim[0], anim[1], anim + 2);
    xoo = gli;
    if (btn && cvecI > 0) {
      doJump = 1;
    }
    cycleanim++;
    if (cycleanim >= cyclesanim)
    {
      cycleanim = 0;
      anim = (anim == superCompDino1) ? superCompDino2 : superCompDino1;
    }
    if (doJump)anim = superCompDino1;
    if (dx >= jump_height - 1)
    {
      dx = -jump_height;
      doJump = 0;
    }
    else if (doJump) dx++;
    if (doJump)gli += dx * 4 / ibgs;
    if (pauseSpawn == 0) {
      if (SPEED < 4) {
        speedupTimer++;
      }
      cScore++;
    }
    if (cScore > hScore)hScore = cScore;
    displayScore();
    displayHScore();
    if (speedupTimer >= SPEEDUP_TIMER) {
      speedupTimer = 0;
      pauseSpawn = 1;
      speedupCycles = 0;
    }
    if (pauseSpawn && cvecI == 0) {
      speedupCycles++;
      if (speedupCycles >= SPEEDUP_CYCLES) {
        pauseSpawn = 0;
        SPEED++;
        speedReset();
      }
    }
  }
  else {
    if (btn && cooldown <= 0) {
      pause = 0;
      for (int i = 0; i < cvecI; i++) {
        cvec[i] = 0;
      }
      cvecI = 0;
      doJump = 1;
      clscr();
      bgposx = xoo = 0;
      gli = GROUND - superCompDino1[1];
      dx = -jump_height;
      cColHeight = -1;
      SPEED = 1;
      speedReset();
      speedupTimer = 0;
      pauseSpawn = 0;
      speedupCycles = 0;
      cScore = 0;
      cooldown = COOLDOWN;
    }
    if (cooldown == COOLDOWN - 1) {
      if (cScore == hScore) {
        EEPROM.write(0, hScore);
        EEPROM.write(1, *((uint8_t *)&hScore + 1));
        _delay_ms(1000);
      }
    }
    if (cooldown > 0)cooldown--;
  }
  }
