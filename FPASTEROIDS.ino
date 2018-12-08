/*
  ASTEROIDS
  Copyright (C) 2018 Xavier

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "Sprites.h"
signed short shipX;
signed short shipY;
signed short velocityX;
signed short velocityY;
signed char HDG;
signed short bullet[8][5];
signed short asteroids[8][4];
signed char asteroidDraw[8];
signed short rocks[16][4];
signed char rockDraw[16];
signed short pebbles[32][4];
signed char pebbleDraw[32];
signed char bulletCount;
signed char asteroidCount;
signed char rockCount;
signed char pebbleCount;
signed char life;
signed char level = 0;
unsigned short score;
signed short tick;
bool turnC;
enum class ProgState : uint8_t {Main, Simulation,  DataEntry, DataDisplay, Pause, DataErasure};
char initials[3];
unsigned short battery;
static const PROGMEM signed char heading[24][2] = {
  { +00, -20}, { +05, -19},
  { +10, -17}, { +14, -14},
  { +17, -10}, { +19, -05},
  { +20, +00}, { +19, +05},
  { +17, +10}, { +14, +14},
  { +10, +17}, { +05, +19},
  { +00, +20}, { -05, +19},
  { -10, +17}, { -14, +14},
  { -17, +10}, { -19, +05},
  { -20, +00}, { -19, -05},
  { -17, -10}, { -14, -14},
  { -10, -17}, { -05, -19}
};

static const PROGMEM signed char shipTbl[6][24] PROGMEM = {
  //HDG
  //  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
  {  +3, +3, +2, +1, -1, -2, -3, -2, -1, +1, +2, +3, +3, -3, -2, -1, +1, +2, +3, +2, +1, -1, -2, -3,},
  {  +3, +3, +3, +3, +3, +3, +3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, +3, +3, +3, +3, +3, +3,},
  {  +0, +2, +3, +3, +3, +4, +4, +4, +3, +3, +3, +2, +0, -2, -3, -3, -3, -4, -4, -4, -3, -3, -3, -2,},
  {  -4, -4, -3, -3, -3, -2, +0, +2, +3, +3, +3, +4, +4, +4, +3, +3, +3, +2, +0, -2, -3, -3, -3, -4,},
  {  -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, +3, +3, +3, +3, +3, +3, +3, +3, +3, +3, +3,},
  {  +3, +2, +1, -1, -2, -3, -3, +3, +2, +1, -1, -2, -3, -2, -1, +1, +2, +3, -3, -3, -2, -1, +1, +2,},
};
#define shipX0  shipX + (int8_t)(pgm_read_byte(&shipTbl[0][HDG]))
#define shipY0  shipY + (int8_t)(pgm_read_byte(&shipTbl[1][HDG]))
#define shipX1  shipX + (int8_t)(pgm_read_byte(&shipTbl[2][HDG]))
#define shipY1  shipY + (int8_t)(pgm_read_byte(&shipTbl[3][HDG]))
#define shipX2  shipX + (int8_t)(pgm_read_byte(&shipTbl[4][HDG]))
#define shipY2  shipY + (int8_t)(pgm_read_byte(&shipTbl[5][HDG]))
#define ADC_TEMP (_BV(REFS0) | _BV(REFS1) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0))
#define ADC_VOLTAGE (_BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1))
ProgState simState = ProgState::Main;
signed char UFOV [9];
bool invasion = false;
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
void guidance() {
  if (velocityY > 0)velocityY = velocityY - 1;
  if (velocityY < 0)velocityY = velocityY + 1;
  if (velocityX > 0)velocityX = velocityX - 1;
  if (velocityX < 0)velocityX = velocityX + 1;
  if (abs(velocityX) < 5) velocityX = 0;
  if (abs(velocityY) < 5) velocityY = 0;
  if (velocityY > 300)velocityY = 300;
  if (velocityY < -300)velocityY = -300;
  if (velocityX > 300)velocityX = 300;
  if (velocityX < -300)velocityX = -300;
}
void ship() {
  shipX = shipX + velocityX;
  shipY = shipY + velocityY;
  if (shipX < -500) shipX = shipX + 13800;
  if (shipX > 13300) shipX = shipX - 13800;
  if (shipY < -500) shipY = shipY + 7400;
  if (shipY > 6900) shipY = shipY - 7400;
  if (tick > 0)
    arduboy.drawTriangle(shipX / 100 + (int8_t)(pgm_read_byte(&shipTbl[0][HDG])), shipY / 100 + (int8_t)(pgm_read_byte(&shipTbl[1][HDG])),
                         shipY / 100 + (int8_t)(pgm_read_byte(&shipTbl[2][HDG])), shipY / 100 + (int8_t)(pgm_read_byte(&shipTbl[3][HDG])),
                         shipY / 100 + (int8_t)(pgm_read_byte(&shipTbl[4][HDG])), shipY / 100 + (int8_t)(pgm_read_byte(&shipTbl[5][HDG])), WHITE);
  else arduboy.fillTriangle(shipX / 100 + (int8_t)(pgm_read_byte(&shipTbl[0][HDG])), shipY / 100 + (int8_t)(pgm_read_byte(&shipTbl[1][HDG])),
                              shipY / 100 + (int8_t)(pgm_read_byte(&shipTbl[2][HDG])), shipY / 100 + (int8_t)(pgm_read_byte(&shipTbl[3][HDG])),
                              shipY / 100 + (int8_t)(pgm_read_byte(&shipTbl[4][HDG])), shipY / 100 + (int8_t)(pgm_read_byte(&shipTbl[5][HDG])), WHITE);
}
void navigation() {
  if (arduboy.everyXFrames(2)) {
    if (arduboy.pressed(LEFT_BUTTON)) HDG--;
    if (arduboy.pressed(RIGHT_BUTTON)) HDG++;
    if (HDG > 23) HDG = HDG - 24;
    if (HDG < 0) HDG = HDG + 24;
    if (arduboy.pressed(UP_BUTTON)) {
      velocityX = velocityX + (int8_t)pgm_read_byte(&heading[HDG][0]);
      velocityY = velocityY + (int8_t)pgm_read_byte(&heading[HDG][1]);
    }
  }
  if (arduboy.justPressed(DOWN_BUTTON)) {
    delay(300);
    if (random(0, 3) == 2) {
      delay(200);
      collision();
    } else {
      velocityX = velocityY = 0;
      shipX = random(0, 12800);
      shipY = random(0, 6400);
    }
  }
}

void firing() {
  bullet[bulletCount][1] = (int8_t)(pgm_read_byte(&heading[HDG][0])) * 20;
  bullet[bulletCount][3] = (int8_t)(pgm_read_byte(&heading[HDG][1])) * 20;
  bullet[bulletCount][0] = shipX1;
  bullet[bulletCount][2] = shipY1;
  bullet[bulletCount][4] = 100;
  bulletCount = bulletCount + 1;
  /*
    sound.tone(1100, 4);
    delay(4);
    sound.tone(1000, 4);
    delay(4);
    sound.tone(900, 4);
    delay(4);
    sound.tone(800, 4);
    delay(4);
    sound.tone(700, 4);
    delay(4);
    sound.tone(600, 4);
    delay(4);
    sound.tone(500, 4);
    delay(4);
    sound.tone(400, 4);
  */
}
void trajectory() {
  for (uint8_t t = 0; t < bulletCount; t++) {
    bullet[t][0] = bullet[t][0] + bullet[t][1];
    bullet[t][2] = bullet[t][2] + bullet[t][3];
    if (bullet[t][0] < -500) bullet[t][0] = bullet[t][0] + 13800;
    if (bullet[t][0] > 13300) bullet[t][0] = bullet[t][0] - 13800;
    if (bullet[t][2] < -500) bullet[t][2] = bullet[t][2] + 7400;
    if (bullet[t][2] > 6900) bullet[t][2] = bullet[t][2] - 7400;
    if (bullet[t][4] < 0) {
      bullet[t][4] = 0;
      for (uint8_t a = 0; a < bulletCount - 1; a++)
        for (uint8_t b = 0; b < 5; b++)
          bullet[a][b] = bullet[a + 1][b];
      bullet[bulletCount][0] = 0;
      bullet[bulletCount][1] = 0;
      bullet[bulletCount][2] = 0;
      bullet[bulletCount][3] = 0;
      bullet[bulletCount][4] = 0;
      bulletCount = bulletCount - 1;
    }
    arduboy.drawPixel(bullet[t][0] / 100, bullet[t][2] / 100, WHITE);
    bullet[t][4] = bullet[t][4] - 4;
  }
}
void asteroid() {
  for (uint8_t t = 0; t < asteroidCount; t++) {
    asteroids[t][0] = asteroids[t][0] + asteroids[t][1];
    asteroids[t][2] = asteroids[t][2] + asteroids[t][3];
    if (asteroids[t][0] < -500) asteroids[t][0] = asteroids[t][0] + 13800;
    if (asteroids[t][0] > 13300) asteroids[t][0] = asteroids[t][0] - 13800;
    if (asteroids[t][2] < -500) asteroids[t][2] = asteroids[t][2] + 7400;
    if (asteroids[t][2] > 6900) asteroids[t][2] = asteroids[t][2] - 7400;
    for (uint8_t b = 0; b < bulletCount; b++) {
      if ((abs(bullet[b][0] - asteroids[t][0]) <= 500 && abs(bullet[b][2] - asteroids[t][2]) <= 500) ||
          (abs(bullet[b][0] - asteroids[t][0]) + abs(bullet[b][2] - asteroids[t][2]) < 700)) {
        for (uint8_t a = b; a < bulletCount - 1; a++) {
          bullet[a][0] = bullet[a + 1][0];
          bullet[a][1] = bullet[a + 1][1];
          bullet[a][2] = bullet[a + 1][2];
          bullet[a][3] = bullet[a + 1][3];
          bullet[a][4] = bullet[a + 1][4];
        }
        bullet[bulletCount][0] = 0;
        bullet[bulletCount][1] = 0;
        bullet[bulletCount][2] = 0;
        bullet[bulletCount][3] = 0;
        bullet[bulletCount][4] = 0;
        bulletCount = bulletCount - 1;

        sound.tone(150, 75);
        score = score + 2;

        rocks[rockCount][0] = asteroids[t][0];
        rocks[rockCount][2] = asteroids[t][2];
        rocks[rockCount][1] = random(-50, 50);
        rocks[rockCount][3] = random(-50, 50);
        rockDraw[rockCount] = random(0, 4);
        rockCount = rockCount + 1;
        rocks[rockCount][0] = asteroids[t][0];
        rocks[rockCount][2] = asteroids[t][2];
        rocks[rockCount][1] = random(-50, 50);
        rocks[rockCount][3] = random(-50, 50);
        rockDraw[rockCount] = random(0, 4);
        rockCount = rockCount + 1;

        for (uint8_t c = t; c < asteroidCount - 1; c++) {
          asteroidDraw[c] = asteroidDraw[c + 1];
          for (uint8_t d = 0; d < 4; d++)
            asteroids[c][d] = asteroids[c + 1][d];
        }
        asteroidCount = asteroidCount - 1;
      }
    }
    switch (asteroidDraw[t]) {
      case 0: arduboy.drawBitmap(asteroids[t][0] * 0.01 - 8, asteroids[t][2] * 0.01 - 8, asteroidS0, 16, 16, WHITE); break;
      case 1: arduboy.drawBitmap(asteroids[t][0] * 0.01 - 8, asteroids[t][2] * 0.01 - 8, asteroidS1, 16, 16, WHITE); break;
      case 2: arduboy.drawBitmap(asteroids[t][0] * 0.01 - 8, asteroids[t][2] * 0.01 - 8, asteroidS2, 16, 16, WHITE); break;
      case 3: arduboy.drawBitmap(asteroids[t][0] * 0.01 - 8, asteroids[t][2] * 0.01 - 8, asteroidS3, 16, 16, WHITE); break;
      default: arduboy.drawCircle(asteroids[t][0] * 0.01, asteroids[t][2] * 0.01, 5, 5);
    }
  }
}
void rock() {
  for (uint8_t r = 0; r < rockCount; r++) {
    rocks[r][0] = rocks[r][0] + rocks[r][1];
    rocks[r][2] = rocks[r][2] + rocks[r][3];
    if (rocks[r][0] < -500) rocks[r][0] = rocks[r][0] + 13800;
    if (rocks[r][0] > 13300) rocks[r][0] = rocks[r][0] - 13800;
    if (rocks[r][2] < -500) rocks[r][2] = rocks[r][2] + 7400;
    if (rocks[r][2] > 6900) rocks[r][2] = rocks[r][2] - 7400;
    for (uint8_t b = 0; b < bulletCount; b++) {
      if ((abs(bullet[b][0] - rocks[r][0]) <= 400 && abs(bullet[b][2] - rocks[r][2]) <= 400) ||
          (abs(bullet[b][0] - rocks[r][0]) + abs(bullet[b][2] - rocks[r][2]) < 600)) {
        for (uint8_t a = b; a < bulletCount - 1; a++) {
          bullet[a][0] = bullet[a + 1][0];
          bullet[a][1] = bullet[a + 1][1];
          bullet[a][2] = bullet[a + 1][2];
          bullet[a][3] = bullet[a + 1][3];
          bullet[a][4] = bullet[a + 1][4];
        }
        bullet[bulletCount][0] = 0;
        bullet[bulletCount][1] = 0;
        bullet[bulletCount][2] = 0;
        bullet[bulletCount][3] = 0;
        bullet[bulletCount][4] = 0;
        bulletCount = bulletCount - 1;

        sound.tone(150, 75);
        score = score + 4;

        pebbles[pebbleCount][0] = rocks[r][0];
        pebbles[pebbleCount][2] = rocks[r][2];
        pebbles[pebbleCount][1] = random(-50, 50);
        pebbles[pebbleCount][3] = random(-50, 50);
        pebbleDraw[pebbleCount] = random(0, 4);
        pebbleCount = pebbleCount + 1;
        pebbles[pebbleCount][0] = rocks[r][0];
        pebbles[pebbleCount][2] = rocks[r][2];
        pebbles[pebbleCount][1] = random(-50, 50);
        pebbles[pebbleCount][3] = random(-50, 50);
        pebbleDraw[pebbleCount] = random(0, 4);
        pebbleCount = pebbleCount + 1;

        for (uint8_t c = r; c < rockCount - 1; c++) {
          rockDraw[c] = rockDraw[c + 1];
          for (uint8_t d = 0; d < 4; d++)
            rocks[c][d] = rocks[c + 1][d];
        }
        rockCount = rockCount - 1;
      }
    }
    switch (rockDraw[r]) {
      case 0: arduboy.drawBitmap(rocks[r][0] * 0.01 - 4, rocks[r][2] * 0.01 - 4, rockS0, 8, 8, WHITE); break;
      case 1: arduboy.drawBitmap(rocks[r][0] * 0.01 - 4, rocks[r][2] * 0.01 - 4, rockS1, 8, 8, WHITE); break;
      case 2: arduboy.drawBitmap(rocks[r][0] * 0.01 - 4, rocks[r][2] * 0.01 - 4, rockS2, 8, 8, WHITE); break;
      case 3: arduboy.drawBitmap(rocks[r][0] * 0.01 - 4, rocks[r][2] * 0.01 - 4, rockS3, 8, 8, WHITE); break;
      default: arduboy.drawCircle(rocks[r][0] * 0.01 , rocks[r][2] * 0.01, 4, 4);
    }
  }
}
void pebble() {
  for (uint8_t p = 0; p < pebbleCount; p++) {
    pebbles[p][0] = pebbles[p][0] + pebbles[p][1];
    pebbles[p][2] = pebbles[p][2] + pebbles[p][3];
    if (pebbles[p][0] < -500) pebbles[p][0] = pebbles[p][0] + 13800;
    if (pebbles[p][0] > 13300) pebbles[p][0] = pebbles[p][0] - 13800;
    if (pebbles[p][2] < -500) pebbles[p][2] = pebbles[p][2] + 7400;
    if (pebbles[p][2] > 6900) pebbles[p][2] = pebbles[p][2] - 7400;
    for (uint8_t b = 0; b < bulletCount; b++) {
      if (abs(bullet[b][0] - pebbles[p][0]) <= 200 && abs(bullet[b][2] - pebbles[p][2]) <= 200) {
        for (uint8_t a = b; a < bulletCount - 1; a++) {
          bullet[a][0] = bullet[a + 1][0];
          bullet[a][1] = bullet[a + 1][1];
          bullet[a][2] = bullet[a + 1][2];
          bullet[a][3] = bullet[a + 1][3];
          bullet[a][4] = bullet[a + 1][4];
        }
        bulletCount = bulletCount - 1;

        sound.tone(150, 75);
        score = score + 8;

        for (uint8_t c = p; c < pebbleCount - 1; c++) {
          pebbleDraw[c] = pebbleDraw[c + 1];
          for (uint8_t d = 0; d < 4; d++)
            pebbles[c][d] = pebbles[c + 1][d];
        }
        pebbleCount = pebbleCount - 1;
      }
    }
    switch (pebbleDraw[p]) {
      case 0: arduboy.drawBitmap(pebbles[p][0] * 0.01 - 2, pebbles[p][2] * 0.01 - 2, pebbleS0, 4, 4, WHITE); break;
      case 1: arduboy.drawBitmap(pebbles[p][0] * 0.01 - 2, pebbles[p][2] * 0.01 - 2, pebbleS1, 4, 4, WHITE); break;
      case 2: arduboy.drawBitmap(pebbles[p][0] * 0.01 - 2, pebbles[p][2] * 0.01 - 2, pebbleS2, 4, 4, WHITE); break;
      case 3: arduboy.drawBitmap(pebbles[p][0] * 0.01 - 2, pebbles[p][2] * 0.01 - 2, pebbleS3, 4, 4, WHITE); break;
      default: arduboy.drawCircle(pebbles[p][0] * 0.01 , pebbles[p][2] * 0.01 , 2, 2);
    }
  }
}
void radar() {
  if (tick < 0) {
    for (uint8_t a = 0; a < asteroidCount; a ++) {
      if ((abs(asteroids[a][0] - shipX0) < 500 && abs(asteroids[a][2] - shipY0) < 500) ||
          (abs(asteroids[a][0] - shipX0) + abs(asteroids[a][1] - shipY0) < 700) ||
          (abs(asteroids[a][0] - shipX1) < 500 && abs(asteroids[a][2] - shipY1) < 500) ||
          (abs(asteroids[a][0] - shipX1) + abs(asteroids[a][1] - shipY1) < 700) ||
          (abs(asteroids[a][0] - shipX2) < 500 && abs(asteroids[a][2] - shipY2) < 500) ||
          (abs(asteroids[a][0] - shipX2) + abs(asteroids[a][1] - shipY2) < 700)) {
        sound.tone(150, 50);
        collision();
        rocks[rockCount][0] = asteroids[a][0] + asteroids[a][1];
        rocks[rockCount][2] = asteroids[a][2] + asteroids[a][3];
        rocks[rockCount][1] = random(-50, 50);
        rocks[rockCount][3] = random(-50, 50);
        rockDraw[rockCount] = random(0, 4);
        rockCount = rockCount + 1;
        rocks[rockCount][0] = asteroids[a][0] + asteroids[a][1];
        rocks[rockCount][2] = asteroids[a][2] + asteroids[a][3];
        rocks[rockCount][1] = random(-50, 50);
        rocks[rockCount][3] = random(-50, 50);
        rockDraw[rockCount] = random(0, 4);
        rockCount = rockCount + 1;
        for (uint8_t c = a; c < asteroidCount - 1; c++)
          for (uint8_t d = 0; d < 4; d++)
            asteroids[c][d] = asteroids[c + 1][d];
        asteroidCount = asteroidCount - 1;
      }
    }
    for (uint8_t r = 0; r < rockCount; r ++) {
      if ((abs(rocks[r][0] - shipX0) < 400 && abs(rocks[r][2] - shipY0) < 400) ||
          (abs(pebbles[r][0] - shipX0) + abs(pebbles[r][1] - shipY0) < 600) ||
          (abs(rocks[r][0] - shipX1) < 400 && abs(rocks[r][2] - shipY1) < 400) ||
          (abs(pebbles[r][0] - shipX1) + abs(pebbles[r][1] - shipY1) < 600) ||
          (abs(rocks[r][0] - shipX2) < 400 && abs(rocks[r][2] - shipY2) < 400) ||
          (abs(pebbles[r][0] - shipX2) + abs(pebbles[r][1] - shipY2) < 600)) {
        sound.tone(150, 50);
        collision();
        pebbles[pebbleCount][0] = rocks[r][0] + rocks[r][1];
        pebbles[pebbleCount][2] = rocks[r][2] + rocks[r][3];
        pebbles[pebbleCount][1] = random(-50, 50);
        pebbles[pebbleCount][3] = random(-50, 50);
        pebbleDraw[pebbleCount] = random(0, 4);
        pebbleCount = pebbleCount + 1;
        pebbles[pebbleCount][0] = rocks[r][0] + rocks[r][1];
        pebbles[pebbleCount][2] = rocks[r][2] + rocks[r][3];
        pebbles[pebbleCount][1] = random(-50, 50);
        pebbles[pebbleCount][3] = random(-50, 50);
        pebbleDraw[pebbleCount] = random(0, 4);
        pebbleCount = pebbleCount + 1;
        for (uint8_t c = r; c < rockCount - 1; c++)
          for (uint8_t d = 0; d < 4; d++)
            rocks[c][d] = rocks[c + 1][d];
        rockCount = rockCount - 1;
      }
    }
    for (uint8_t p = 0; p < pebbleCount; p ++) {
      if ((abs(pebbles[p][0] - shipX0) < 400 && abs(pebbles[p][2] - shipY0) < 400) ||
          (abs(pebbles[p][0] - shipX1) < 400 && abs(pebbles[p][2] - shipY1) < 400) ||
          (abs(pebbles[p][0] - shipX2) < 400 && abs(pebbles[p][2] - shipY2) < 400)) {
        delay(200);
        collision();
        for (uint8_t c = p; c < pebbleCount - 1; c++)
          for (uint8_t d = 0; d < 4; d++)
            pebbles[c][d] = pebbles[c + 1][d];
        pebbleCount = pebbleCount - 1;
      }
    }
    tick = tick + 1;
  } else if (asteroidCount == 0 && rockCount == 0 && pebbleCount == 0) {
    tick = tick - 1;
    if (tick <= 0) {
      tick = 120;
      if (level < 8) level = level + 1;
      for (int8_t s = 0; s < level; s++) {
        asteroids[asteroidCount][0] = 9600 + random(0, 7400);
        asteroids[asteroidCount][2] = 4800 + random(0, 4200);
        asteroids[asteroidCount][1] = random(-50, 50);
        asteroids[asteroidCount][3] = random(-50, 50);
        asteroidDraw[asteroidCount] = random(0, 4);
        asteroidCount = asteroidCount + 1;
      }
    }
  }
}
void collision() {
  tick = -240;
  sound.tone(150, 50);
  life = life - 1;
  shipX = 6400;
  shipY = 3200;
  velocityX = 0;
  velocityY = 0;
}
void resetSim() {
  shipY = 5000;
  shipX = 6400;
  resetField();
  simState = ProgState::Main;
  score = 0;
}
void resetField() {
  level = 0;
  velocityX = velocityY = 0;
  tick = 120;
  bulletCount = 0;
  asteroidCount = rockCount = pebbleCount = 0;
}

void enterInitials() {
  int8_t index = 0;
  initials[0] = ' ';
  initials[1] = ' ';
  initials[2] = ' ';
  while (index < 3) {
    if (arduboy.nextFrame()) {
      arduboy.clear();
      arduboy.setCursor(16, 0);
      arduboy.print(F("HIGH SCORE"));
      arduboy.setCursor(88, 0);
      arduboy.print(score);

      arduboy.setCursor(56, 20);
      arduboy.print(initials[0]);
      arduboy.setCursor(64, 20);
      arduboy.print(initials[1]);
      arduboy.setCursor(72, 20);
      arduboy.print(initials[2]);
      arduboy.drawLine(56, 27, 56 + 6, 27, 1);
      arduboy.drawLine(64, 27, 64 + 6, 27, 1);
      arduboy.drawLine(72, 27, 72 + 6, 27, 1);
      arduboy.drawLine(56 + (index * 8), 28, 56 + (index * 8) + 6, 28, 1);
      delay(150);
      if (arduboy.justPressed(LEFT_BUTTON)) {
        sound.tone(1046, 200);
        index--;
        if (index < 0) index = 0;
      }

      if (arduboy.justPressed(RIGHT_BUTTON)) {
        index++;
        if (index > 2) index = 2;
        sound.tone(1046, 200);
      }

      if (arduboy.pressed(DOWN_BUTTON)) {
        initials[index]++;
        sound.tone(523, 150);
        // A-Z 0-9 :-? !-/ ' '
        if (initials[index] == '0') initials[index] = ' ';
        if (initials[index] == '!') initials[index] = 'A';
        if (initials[index] == '[') initials[index] = '0';
        if (initials[index] == '@') initials[index] = '!';
      }
      if (arduboy.pressed(UP_BUTTON)) {
        initials[index]--;
        sound.tone(523, 150);
        if (initials[index] == ' ') initials[index] = '?';
        if (initials[index] == '/') initials[index] = 'Z';
        if (initials[index] == 31)  initials[index] = '/';
        if (initials[index] == '@') initials[index] = ' ';
      }
      if (arduboy.pressed(A_BUTTON)) {
        if (index >= 2) {
          index = index + 1;
          sound.tone(1046, 200);
        }
      }
      arduboy.pollButtons();
      arduboy.display();
    }
  }
}
void UFO() {
  if (tick == 120 && !invasion) {
    if (random(0, 10) >= 10) {
      invasion = true;
      UFOV[0] = 9600 + random(0, 7400);
      UFOV[2] = 4800 + random(0, 4200);
      UFOV[1] = random (-50, 50);
      UFOV[3] = random (-50, 50);
    }
  }
  if (invasion) {
    UFOV[0] = UFOV[0] + UFOV[1];
    UFOV[2] = UFOV[2] + UFOV[3];
    UFOV[4] = UFOV[4] + UFOV[6];
    UFOV[5] = UFOV[5] + UFOV[7];
    UFOV[8] = UFOV[8] - 4;
    if (arduboy.everyXFrames(30)) {
      UFOV[1] = random (-50, 50);
      UFOV[3] = random (-50, 50);
    }
    if (arduboy.everyXFrames(150)) {
      UFOV[4] = UFOV[0];
      UFOV[5] = UFOV[2];
      UFOV[6] = random(-4, 4) * 10;
      UFOV[7] = random(-4, 4) * 10;
      UFOV[8] = 100;
    }
    if (UFOV[8] < 0) {
      UFOV[4] = UFOV[5] = UFOV[6] = UFOV[7] = 0;
    } else {
      if ((abs(UFOV[4] - shipX) < 700 && abs(UFOV[5] - shipY) < 700) ||
          (abs(UFOV[4] - shipX) + abs(UFOV[5] - shipY) < 1000)) {
        sound.tone(150, 50);
        collision();
        UFOV[4] = UFOV[5] = UFOV[6] = UFOV[7] = UFOV[8] = 0;
      }
    }
  }
}
uint16_t rawADC(uint8_t adc_bits) {
  uint16_t value;
  power_adc_enable(); // ADC on
  ADMUX = adc_bits;
  // we also need MUX5 for temperature check
  if (adc_bits == ADC_TEMP) {
    ADCSRB = _BV(MUX5);
  }
  delay(2); // Wait for ADMUX setting to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // measuring
  value = ADC;
  power_adc_disable();
  return value;
}
void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.initRandomSeed();
  arduboy.setFrameRate(30);
  arduboy.clear();
  resetSim();
  if (arduboy.audio.enabled()) arduboy.audio.on();
  else arduboy.audio.off();
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:
  if (simState == ProgState::Simulation) arduboy.setFrameRate(30);
  else if (simState == ProgState::Main) arduboy.setFrameRate(20);
  else arduboy.setFrameRate(15);
  if (!arduboy.nextFrame()) return;
  arduboy.clear();
  switch (simState) {
    case ProgState::Main: {
        //begin homescreen
        arduboy.setCursor(10, 10);
        arduboy.setTextSize(2);
        arduboy.print(F("ASTEROIDS"));
        if (arduboy.everyXFrames(10)) {
          turnC = random(0, 2) == 1;
        }
        if (arduboy.everyXFrames(3)) {
          if (turnC) HDG = HDG + 1;
          else HDG = HDG - 1;
        }
        if (HDG > 23) HDG = HDG - 24;
        if (HDG < 0) HDG = HDG + 24;
        velocityX = 100;
        ship();
        if (arduboy.everyXFrames(5) && (bulletCount < 8)) {
          bullet[bulletCount][1] = (int8_t)(pgm_read_byte(&heading[HDG][0])) * 20;
          bullet[bulletCount][3] = (int8_t)(pgm_read_byte(&heading[HDG][1])) * 20;
          bullet[bulletCount][0] = shipX + (int8_t)(pgm_read_byte(&shipTbl[2][HDG]));
          bullet[bulletCount][2] = shipY + (int8_t)(pgm_read_byte(&shipTbl[3][HDG]));
          bullet[bulletCount][4] = 100;
          bulletCount = bulletCount + 1;
        }
        trajectory();
        if (arduboy.justPressed(B_BUTTON)) {
          life = 3;
          shipX = 6400;
          shipY = 3200;
          resetField();
          simState = ProgState::Simulation;
        }
        if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON)) {
          if (arduboy.audio.enabled()) arduboy.audio.off();
          else arduboy.audio.on();
        }
        if (arduboy.justPressed(A_BUTTON)) simState = ProgState::DataDisplay;
      }
      break;
    case ProgState::Simulation: {
        if (life > 0) {
          if (tick > -120) {
            navigation();
            guidance();
            ship();
            if (arduboy.justPressed(B_BUTTON) && (bulletCount < 8)) firing();
            trajectory();
          }
          asteroid();
          rock();
          pebble();
          radar();

          arduboy.setTextSize(1);
          arduboy.setCursor(2, 10);
          arduboy.print(score);
          arduboy.setCursor(32, 10);
          arduboy.print(level);
          arduboy.setCursor(120, 10);
          arduboy.print(life);
          if (life == 0) {
            resetField();
          }
          if (arduboy.justPressed(A_BUTTON)) {
            simState = ProgState::Pause;
          }
        } else {
          delay(3000);
          resetField();
          simState = ProgState::DataEntry;
        }
      }
      break;
    case ProgState::DataEntry: {
        char tmpInitials[3];
        unsigned short tmpScore;
        // Each block of EEPROM has 7 high scores, and each high score entry
        // is 5 byte long:  3 char for initials and a short for score.
        // High score processing
        for (int8_t i = 0; i < 7; i++) {
          EEPROM.get(100 + (5 * i), tmpScore);
          if (tmpScore == 0xFFFF) tmpScore = 0;
          if (score > tmpScore) {
            enterInitials();
            for (byte j = i; j < 7; j++) {
              EEPROM.get(100 + (5 * j), tmpScore);
              if (tmpScore == 0xFFFF) tmpScore = 0;

              tmpInitials[0] = (char)EEPROM.read(100 + (5 * j) + 2);
              tmpInitials[1] = (char)EEPROM.read(100 + (5 * j) + 3);
              tmpInitials[2] = (char)EEPROM.read(100 + (5 * j) + 4);

              // write score and initials to current slot
              EEPROM.put(100 + (5 * j), score);
              EEPROM.write(100 + (5 * j) + 2, initials[0]);
              EEPROM.write(100 + (5 * j) + 3, initials[1]);
              EEPROM.write(100 + (5 * j) + 4, initials[2]);

              // tmpScore and tmpInitials now hold what we want to
              //write in the next slot.
              score = tmpScore;
              initials[0] = tmpInitials[0];
              initials[1] = tmpInitials[1];
              initials[2] = tmpInitials[2];
            }
            //reset the initials
            initials[0] = ' ';
            initials[1] = ' ';
            initials[2] = ' ';
            break;
          }
        }
      }
      resetSim();
      break;
    case ProgState::DataDisplay:
      // Each block of EEPROM has 7 high scores, and each high score entry
      // is 5 int8_ts long:  3 int8_ts for initials and one int16_t for score
      arduboy.setCursor(32, 0);
      arduboy.setTextSize(1);
      arduboy.print("HIGH SCORES");

      for (int i = 0; i < 7; i++) {
        EEPROM.get(100 + (5 * i), score);
        if (score == 0xFFFF) score = 0;
        initials[0] = (char)EEPROM.read(100 + (5 * i) + 2);
        initials[1] = (char)EEPROM.read(100 + (5 * i) + 3);
        initials[2] = (char)EEPROM.read(100 + (5 * i) + 4);

        arduboy.setCursor(30, 9 + (i * 8));
        arduboy.print(i + 1);
        arduboy.print(F(" "));
        arduboy.print(initials[0]);
        arduboy.print(initials[1]);
        arduboy.print(initials[2]);
        arduboy.print(F(" "));
        arduboy.print(score);
        score = 0;
      }
      if (arduboy.justPressed(A_BUTTON)) simState = ProgState::Main;
      if (arduboy.justPressed(LEFT_BUTTON)) simState = ProgState::DataErasure;
      if (arduboy.everyXFrames(15)) battery = 1126400L/*1.1x1024x1000*/ / rawADC(ADC_VOLTAGE);
      arduboy.setCursor(100, 50);
      arduboy.print(battery);
      break;
    case ProgState::Pause:
      arduboy.setCursor(32, 30);
      arduboy.setTextSize(2);
      arduboy.print(F("PAUSE"));
      if (arduboy.justPressed(B_BUTTON)) simState = ProgState::Simulation;
      if (arduboy.justPressed(A_BUTTON)) resetSim();
      if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON)) {
        if (arduboy.audio.enabled()) arduboy.audio.off();
        else arduboy.audio.on();
        arduboy.audio.saveOnOff();
      }
      if (arduboy.everyXFrames(20)) battery = 1126400L/*1.1x1024x1000*/ / rawADC(ADC_VOLTAGE);
      arduboy.setCursor(100, 50);
      arduboy.setTextSize(1);
      arduboy.print(battery);
      break;
    case ProgState::DataErasure:
      arduboy.setCursor(14, 32);
      arduboy.setTextSize(1);
      arduboy.print(F("EREASE HIGHSCORE?"));
      arduboy.setCursor(14, 40);
      arduboy.print(F("START = RIGHT + B"));
      if (arduboy.pressed(RIGHT_BUTTON | B_BUTTON)) {
        for (int i = 0; i < 35; i ++) {
          arduboy.clear();
          arduboy.setCursor(36, HEIGHT / 2);
          arduboy.print(F("WORKING..."));
          arduboy.display();
          EEPROM.update(i + 100, 0xff);
        }
        delay(500);
        arduboy.setCursor(32, HEIGHT / 2);
        arduboy.clear();
        arduboy.print(F("EREASE DONE"));
        arduboy.display();
        delay(1000);
        simState = ProgState::Main;
      }
      if (arduboy.justPressed(A_BUTTON)) simState = ProgState::Main;
      break;



  }

  //begin printObj

  //spash screen
  arduboy.setTextSize(1);
  arduboy.setCursor(2, 10);
  arduboy.print(score);
  arduboy.setCursor(32, 10);
  arduboy.print(level);
  arduboy.setCursor(120, 10);
  arduboy.print(life);

  arduboy.setCursor(44, 10);
  arduboy.print(HDG);
  arduboy.setCursor(60, 10);
  arduboy.print(asteroidCount);
  arduboy.setCursor(78, 10);
  arduboy.print(rockCount);
  arduboy.setCursor(96, 10);
  arduboy.print(pebbleCount);
  arduboy.setCursor(110, 10);
  arduboy.print(bulletCount);

  arduboy.setCursor(100, 25);
  arduboy.print(shipX);
  arduboy.setCursor(100, 35);
  arduboy.print(shipY);
  arduboy.setCursor(100, 45);
  arduboy.print(velocityX);
  arduboy.setCursor(100, 55);
  arduboy.print(velocityY);
  arduboy.setCursor(10, 50);
  arduboy.print(tick);

  arduboy.setCursor(10, 30);
  arduboy.print(arduboy.cpuLoad());
  //splash screen
  //end printObj

  //begin display
  if (arduboy.audio.enabled()) {
    arduboy.drawRect(WIDTH - 3, HEIGHT - 2, 2, 2, 1);
    arduboy.drawLine(WIDTH - 2, HEIGHT - 5, WIDTH - 2, HEIGHT - 1, 1);
    arduboy.drawPixel(WIDTH - 1, HEIGHT - 4, 1);
  }
  Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
  arduboy.display();

  //end display

  //begin pollButtons
  arduboy.pollButtons();
  //end pollButtons
}
