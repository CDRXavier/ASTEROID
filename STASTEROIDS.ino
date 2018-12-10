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
signed char bulletCount;
signed char asteroidCount;
signed char rockCount;
signed char pebbleCount;
signed char life;
signed char level = 0;
unsigned short score;
signed short tick;
//bool turnC;
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
  //ship.var
  //  0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
  {  +3, +3, +2, +1, -1, -2, -3, -2, -1, +1, +2, +3, +3, -3, -2, -1, +1, +2, +3, +2, +1, -1, -2, -3,},
  {  +3, +3, +3, +3, +3, +3, +3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, +3, +3, +3, +3, +3, +3,},
  {  +0, +2, +3, +3, +3, +4, +4, +4, +3, +3, +3, +2, +0, -2, -3, -3, -3, -4, -4, -4, -3, -3, -3, -2,},
  {  -4, -4, -3, -3, -3, -2, +0, +2, +3, +3, +3, +4, +4, +4, +3, +3, +3, +2, +0, -2, -3, -3, -3, -4,},
  {  -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, +3, +3, +3, +3, +3, +3, +3, +3, +3, +3, +3,},
  {  +3, +2, +1, -1, -2, -3, -3, +3, +2, +1, -1, -2, -3, -2, -1, +1, +2, +3, -3, -3, -2, -1, +1, +2,},
};
struct Object {
  signed short x;
  signed short y;
  signed short vx;
  signed short vy;
  char var;
};
Object ship;
Object bullet[8];
Object asteroids[8];
Object rocks[16];
Object pebbles[32];
Object UFO;
Object UFOB;
//UFO:
//UFO includes UFO's x,y,vx & vy as well as heading(for bullets)
//UFOB && bullets use var as storage for bullet life
//rocks, pebbles and asteroids use it as different sprite choice
#define shipX0  ship.x + (int8_t)(pgm_read_byte(&shipTbl[0][ship.var]))
#define shipY0  ship.y + (int8_t)(pgm_read_byte(&shipTbl[1][ship.var]))
#define shipX1  ship.x + (int8_t)(pgm_read_byte(&shipTbl[2][ship.var]))
#define shipY1  ship.y + (int8_t)(pgm_read_byte(&shipTbl[3][ship.var]))
#define shipX2  ship.x + (int8_t)(pgm_read_byte(&shipTbl[4][ship.var]))
#define shipY2  ship.y + (int8_t)(pgm_read_byte(&shipTbl[5][ship.var]))
#define ADC_TEMP (_BV(REFS0) | _BV(REFS1) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0))
#define ADC_VOLTAGE (_BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1))
ProgState simState = ProgState::Main;
unsigned char invasion = 0;
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
void guidance() {
  if (ship.vy > 0)ship.vy = ship.vy - 1;
  if (ship.vy < 0)ship.vy = ship.vy + 1;
  if (ship.vy > 0)ship.vx = ship.vx - 1;
  if (ship.vx < 0)ship.vx = ship.vx + 1;
  //if (abs(ship.vx) < 5) ship.vx = 0;
  //if (abs(ship.vy) < 5) ship.vy = 0;
  if (ship.vy > 300)ship.vy = 300;
  if (ship.vy < -300)ship.vy = -300;
  if (ship.vx > 300)ship.vx = 300;
  if (ship.vx < -300)ship.vx = -300;
  ship.x = ship.x + ship.vx;
  ship.y = ship.y + ship.vy;
  if (ship.x < -500) ship.x = ship.x + 13800;
  if (ship.x > 13300) ship.x = ship.x - 13800;
  if (ship.y < -500) ship.y = ship.y + 7400;
  if (ship.y > 6900) ship.y = ship.y - 7400;
  if (tick > 0)
    arduboy.drawTriangle(ship.x / 100 + (int8_t)(pgm_read_byte(&shipTbl[0][ship.var])), ship.y / 100 + (int8_t)(pgm_read_byte(&shipTbl[1][ship.var])),
                         ship.x / 100 + (int8_t)(pgm_read_byte(&shipTbl[2][ship.var])), ship.y / 100 + (int8_t)(pgm_read_byte(&shipTbl[3][ship.var])),
                         ship.x / 100 + (int8_t)(pgm_read_byte(&shipTbl[4][ship.var])), ship.y / 100 + (int8_t)(pgm_read_byte(&shipTbl[5][ship.var])), WHITE);
  else arduboy.fillTriangle(ship.x / 100 + (int8_t)(pgm_read_byte(&shipTbl[0][ship.var])), ship.y / 100 + (int8_t)(pgm_read_byte(&shipTbl[1][ship.var])),
                              ship.x / 100 + (int8_t)(pgm_read_byte(&shipTbl[2][ship.var])), ship.y / 100 + (int8_t)(pgm_read_byte(&shipTbl[3][ship.var])),
                              ship.x / 100 + (int8_t)(pgm_read_byte(&shipTbl[4][ship.var])), ship.y / 100 + (int8_t)(pgm_read_byte(&shipTbl[5][ship.var])), WHITE);
}
void navigation() {
  if (arduboy.everyXFrames(2)) {
    if (arduboy.pressed(LEFT_BUTTON)) ship.var = ship.var - 1;
    if (arduboy.pressed(RIGHT_BUTTON)) ship.var = ship.var + 1;
    if (ship.var > 23) ship.var = ship.var - 24;
    if (ship.var < 0) ship.var = ship.var + 24;
    if (arduboy.pressed(UP_BUTTON)) {
      ship.vx = ship.vx + (int8_t)pgm_read_byte(&heading[ship.var][0]);
      ship.vy = ship.vy + (int8_t)pgm_read_byte(&heading[ship.var][1]);
    }
  }
  if (arduboy.justPressed(DOWN_BUTTON)) {
    delay(300);
    if (random(0, 3) == 2) {
      delay(200);
      collision();
    } else {
      ship.vx = ship.vy = 0;
      ship.x = random(0, 12800);
      ship.y = random(0, 6400);
    }
  }
}

void firing() {
  bullet[bulletCount].vx = (int8_t)(pgm_read_byte(&heading[ship.var][0])) * 20;
  bullet[bulletCount].vy = (int8_t)(pgm_read_byte(&heading[ship.var][1])) * 20;
  bullet[bulletCount].x = shipX1;
  bullet[bulletCount].y = shipY1;
  bullet[bulletCount].var = 100;
  bulletCount = bulletCount + 1;
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
}
void trajectory() {
  for (uint8_t b = 0; b < bulletCount; b++) {
    bullet[b].x = bullet[b].x + bullet[b].vx;
    bullet[b].y = bullet[b].y + bullet[b].vy;
    if (bullet[b].x < -500) bullet[b].x = bullet[b].x + 13800;
    if (bullet[b].x > 13300) bullet[b].x = bullet[b].x - 13800;
    if (bullet[b].y < -500) bullet[b].y = bullet[b].y + 7400;
    if (bullet[b].y > 6900) bullet[b].y = bullet[b].y - 7400;
    if (bullet[b].var < 0) {
      bullet[b].var = 0;
      for (uint8_t c = b; c < bulletCount - 1; c++) {
        bullet[c].x = bullet[c + 1].x;
        bullet[c].vx = bullet[c + 1].vx;
        bullet[c].y = bullet[c + 1].y;
        bullet[c].vy = bullet[c + 1].vy;
        bullet[c].var = bullet[c + 1].var;
      }
      bullet[bulletCount].x = 0;
      bullet[bulletCount].vx = 0;
      bullet[bulletCount].y = 0;
      bullet[bulletCount].vy = 0;
      bullet[bulletCount].var = 0;
      bulletCount = bulletCount - 1;
    }
    arduboy.drawPixel(bullet[b].x / 100, bullet[b].y / 100, WHITE);
    bullet[b].var = bullet[b].var - 4;
  }
}

//done!
void asteroid() {
  for (uint8_t a = 0; a < asteroidCount; a++) {
    asteroids[a].x = asteroids[a].x + asteroids[a].vx;
    asteroids[a].y = asteroids[a].y + asteroids[a].vy;
    if (asteroids[a].x < -500) asteroids[a].x = asteroids[a].x + 13800;
    if (asteroids[a].x > 13300) asteroids[a].x = asteroids[a].x - 13800;
    if (asteroids[a].y < -500) asteroids[a].y = asteroids[a].y + 7400;
    if (asteroids[a].y > 6900) asteroids[a].y = asteroids[a].y - 7400;
    for (uint8_t c = 0; c < bulletCount; c++) {
      if ((abs(bullet[c].x - asteroids[a].x) <= 500 && abs(bullet[c].y - asteroids[a].y) <= 500) ||
          (abs(bullet[c].x - asteroids[a].x) + abs(bullet[c].y - asteroids[a].y) < 700)) {
        for (uint8_t e = c; e < bulletCount - 1; e++) {
          bullet[e] = bullet[e + 1];
        }
        bullet[bulletCount].x = 0;
        bullet[bulletCount].vx = 0;
        bullet[bulletCount].y = 0;
        bullet[bulletCount].vy = 0;
        bullet[bulletCount].var = 0;
        bulletCount = bulletCount - 1;

        sound.tone(150, 75);
        score = score + 2;

        rocks[rockCount].x = asteroids[a].x;
        rocks[rockCount].x = asteroids[a].y;
        rocks[rockCount].vx = random(-50, 50);
        rocks[rockCount].vy = random(-50, 50);
        rocks[rockCount].var = random(0, 4);
        rockCount = rockCount + 1;
        rocks[rockCount].x = asteroids[a].x;
        rocks[rockCount].y = asteroids[a].y;
        rocks[rockCount].vx = random(-50, 50);
        rocks[rockCount].vy = random(-50, 50);
        rocks[rockCount].var = random(0, 4);
        rockCount = rockCount + 1;

        for (uint8_t d = a; d < asteroidCount - 1; d++) {
          asteroids[d] = asteroids[d + 1];
        }
        asteroidCount = asteroidCount - 1;
      }
    }
    switch (asteroids[a].var) {
      case 0: arduboy.drawBitmap(asteroids[a].x / 100 - 8, asteroids[a].y / 100 - 8, asteroidS0, 16, 16, WHITE); break;
      case 1: arduboy.drawBitmap(asteroids[a].x / 100 - 8, asteroids[a].y / 100 - 8, asteroidS1, 16, 16, WHITE); break;
      case 2: arduboy.drawBitmap(asteroids[a].x / 100 - 8, asteroids[a].y / 100 - 8, asteroidS2, 16, 16, WHITE); break;
      case 3: arduboy.drawBitmap(asteroids[a].x / 100 - 8, asteroids[a].y / 100 - 8, asteroidS3, 16, 16, WHITE); break;
      default: arduboy.drawCircle(asteroids[a].x / 100, asteroids[a].y / 100, 5, 5);
    }
  }
}
void rock() {
  for (uint8_t r = 0; r < rockCount; r++) {
    rocks[r].x = rocks[r].x + rocks[r].vx;
    rocks[r].y = rocks[r].y + rocks[r].vy;
    if (rocks[r].x < -500) rocks[r].x = rocks[r].x + 13800;
    if (rocks[r].x > 13300) rocks[r].x = rocks[r].x - 13800;
    if (rocks[r].y < -500) rocks[r].y = rocks[r].y + 7400;
    if (rocks[r].y > 6900) rocks[r].y = rocks[r].y - 7400;
    for (uint8_t c = 0; c < bulletCount; c++) {
      if ((abs(bullet[c].x - rocks[r].x) <= 400 && abs(bullet[c].y - rocks[r].y) <= 400) ||
          (abs(bullet[c].x - rocks[r].x) + abs(bullet[c].y - rocks[r].y) < 600)) {
        for (uint8_t e = c; e < bulletCount - 1; e++) {
          bullet[e] = bullet[e + 1];
        }
        bullet[bulletCount].x = 0;
        bullet[bulletCount].vx = 0;
        bullet[bulletCount].y = 0;
        bullet[bulletCount].vy = 0;
        bullet[bulletCount].var = 0;
        bulletCount = bulletCount - 1;

        sound.tone(150, 75);
        score = score + 4;

        pebbles[pebbleCount].x = rocks[r].x;
        pebbles[pebbleCount].y = rocks[r].y;
        pebbles[pebbleCount].vx = random(-50, 50);
        pebbles[pebbleCount].vy = random(-50, 50);
        pebbles[pebbleCount].var = random(0, 4);
        pebbleCount = pebbleCount + 1;
        pebbles[pebbleCount].x = rocks[r].x;
        pebbles[pebbleCount].y = rocks[r].y;
        pebbles[pebbleCount].vx = random(-50, 50);
        pebbles[pebbleCount].vy = random(-50, 50);
        pebbles[pebbleCount].var = random(0, 4);
        pebbleCount = pebbleCount + 1;

        for (uint8_t d = r; d < rockCount - 1; d++) {
          rocks[d] = rocks [d + 1];
        }
        rockCount = rockCount - 1;
      }
    }
    switch (rocks[r].var) {
      case 0: arduboy.drawBitmap(rocks[r].x / 100 - 4, rocks[r].y / 100 - 4, rockS0, 8, 8, WHITE); break;
      case 1: arduboy.drawBitmap(rocks[r].x / 100 - 4, rocks[r].y / 100 - 4, rockS1, 8, 8, WHITE); break;
      case 2: arduboy.drawBitmap(rocks[r].x / 100 - 4, rocks[r].y / 100 - 4, rockS2, 8, 8, WHITE); break;
      case 3: arduboy.drawBitmap(rocks[r].x / 100 - 4, rocks[r].y / 100 - 4, rockS3, 8, 8, WHITE); break;
      default: arduboy.drawCircle(rocks[r].x / 100 , rocks[r].y / 100, 4, 4);
    }
  }
}
void pebble() {
  for (uint8_t p = 0; p < pebbleCount; p++) {
    pebbles[p].x = pebbles[p].x + pebbles[p].vx;
    pebbles[p].y = pebbles[p].y + pebbles[p].vy;
    if (pebbles[p].x < -500) pebbles[p].x = pebbles[p].x + 13800;
    if (pebbles[p].x > 13300) pebbles[p].x = pebbles[p].x - 13800;
    if (pebbles[p].y < -500) pebbles[p].y = pebbles[p].y + 7400;
    if (pebbles[p].y > 6900) pebbles[p].y = pebbles[p].y - 7400;
    for (uint8_t c = 0; c < bulletCount; c++) {
      if (abs(bullet[c].x - pebbles[p].x) <= 200 && abs(bullet[c].y - pebbles[p].y) <= 200) {
        for (uint8_t e = c; e < bulletCount - 1; e++) {
          bullet[e] = bullet[e + 1];
        }
        bulletCount = bulletCount - 1;

        sound.tone(150, 75);
        score = score + 8;

        for (uint8_t d = p; d < pebbleCount - 1; d++) {
          pebbles[d] = pebbles[d + 1];
        }
        pebbleCount = pebbleCount - 1;
      }
    }
    switch (pebbles[p].var) {
      case 0: arduboy.drawBitmap(pebbles[p].x / 100 - 2, pebbles[p].y / 100 - 2, pebbleS0, 4, 4, WHITE); break;
      case 1: arduboy.drawBitmap(pebbles[p].x / 100 - 2, pebbles[p].y / 100 - 2, pebbleS1, 4, 4, WHITE); break;
      case 2: arduboy.drawBitmap(pebbles[p].x / 100 - 2, pebbles[p].y / 100 - 2, pebbleS2, 4, 4, WHITE); break;
      case 3: arduboy.drawBitmap(pebbles[p].x / 100 - 2, pebbles[p].y / 100 - 2, pebbleS3, 4, 4, WHITE); break;
      default: arduboy.drawCircle(pebbles[p].x / 100 , pebbles[p].y / 100 , 2, 2);
    }
  }
}
void radar() {
  if (tick >= 0) {
    for (uint8_t a = 0; a < asteroidCount; a ++) {
      if ((abs(asteroids[a].x - shipX0) < 500 && abs(asteroids[a].y - shipY0) < 500) ||
          ((abs(asteroids[a].x - shipX0) + abs(asteroids[a].y - shipY0)) < 700) ||
          (abs(asteroids[a].x - shipX1) < 500 && abs(asteroids[a].y - shipY1) < 500) ||
          ((abs(asteroids[a].x - shipX1) + abs(asteroids[a].y - shipY1)) < 700) ||
          (abs(asteroids[a].x - shipX2) < 500 && abs(asteroids[a].y - shipY2) < 500) ||
          ((abs(asteroids[a].x - shipX2) + abs(asteroids[a].y - shipY2)) < 700)) {
        sound.tone(150, 50);
        collision();
        rocks[rockCount].x = asteroids[a].x + asteroids[a].vx;
        rocks[rockCount].y = asteroids[a].y + asteroids[a].vy;
        rocks[rockCount].vx = random(-50, 50);
        rocks[rockCount].vy = random(-50, 50);
        rocks[rockCount].var = random(0, 4);
        rockCount = rockCount + 1;
        rocks[rockCount].x = asteroids[a].x + asteroids[a].vx;
        rocks[rockCount].y = asteroids[a].y + asteroids[a].vy;
        rocks[rockCount].vx = random(-50, 50);
        rocks[rockCount].vy = random(-50, 50);
        rocks[rockCount].var = random(0, 4);
        rockCount = rockCount + 1;
        for (uint8_t d = a; d < asteroidCount - 1; d++) {
          asteroids[d] = asteroids[d + 1];
        }
        asteroidCount = asteroidCount - 1;
      }
    }
    for (uint8_t r = 0; r < rockCount; r ++) {
      if ((abs(rocks[r].x - shipX0) < 400 && abs(rocks[r].y - shipY0) < 400) ||
          (abs(rocks[r].x - shipX0) + abs(rocks[r].y - shipY0) < 600) ||
          (abs(rocks[r].x - shipX1) < 400 && abs(rocks[r].y - shipY1) < 400) ||
          (abs(rocks[r].x - shipX1) + abs(rocks[r].y - shipY1) < 600) ||
          (abs(rocks[r].x - shipX2) < 400 && abs(rocks[r].y - shipY2) < 400) ||
          (abs(rocks[r].x - shipX2) + abs(rocks[r].y - shipY2) < 600)) {
        sound.tone(150, 50);
        collision();
        pebbles[pebbleCount].x = rocks[r].x + rocks[r].vx;
        pebbles[pebbleCount].y = rocks[r].y + rocks[r].vy;
        pebbles[pebbleCount].vx = random(-50, 50);
        pebbles[pebbleCount].vy = random(-50, 50);
        pebbles[pebbleCount].var = random(0, 4);
        pebbleCount = pebbleCount + 1;
        pebbles[pebbleCount].x = rocks[r].x + rocks[r].vx;
        pebbles[pebbleCount].y = rocks[r].y + rocks[r].vy;
        pebbles[pebbleCount].vx = random(-50, 50);
        pebbles[pebbleCount].vy = random(-50, 50);
        pebbles[pebbleCount].var = random(0, 4);
        pebbleCount = pebbleCount + 1;
        for (uint8_t d = r; d < rockCount - 1; d++)
          rocks[d] = rocks[d + 1];
        rockCount = rockCount - 1;
      }
    }
    for (uint8_t p = 0; p < pebbleCount; p ++) {
      if ((abs(pebbles[p].x - shipX0) < 400 && abs(pebbles[p].y - shipY0) < 400) ||
          (abs(pebbles[p].x - shipX1) < 400 && abs(pebbles[p].y - shipY1) < 400) ||
          (abs(pebbles[p].x - shipX2) < 400 && abs(pebbles[p].y - shipY2) < 400)) {
        delay(200);
        collision();
        for (uint8_t d = p; d < pebbleCount - 1; d++) {
          pebbles[d] = pebbles[d + 1];
        }
        pebbleCount = pebbleCount - 1;
      }
    }
  } else {
    tick = tick + 1;
    if (tick >= 0) tick = 120;
  }
  if (asteroidCount == 0 && rockCount == 0 && pebbleCount == 0 && tick >= 0 && invasion <= 0) {
    tick = tick - 1;
    if (tick <= 0) {
      tick = 120;
      if (level < 8) level = level + 1;
      for (int8_t s = 0; s < level; s++) {
        asteroids[asteroidCount].x = 9600 + random(0, 7400);
        asteroids[asteroidCount].y = 4800 + random(0, 4200);
        asteroids[asteroidCount].vx = random(-50, 50);
        asteroids[asteroidCount].vy = random(-50, 50);
        asteroids[asteroidCount].var = random(0, 4);
        asteroidCount = asteroidCount + 1;
      }
    }
  }
}
void collision() {
  tick = -240;
  sound.tone(150, 50);
  life = life - 1;
  ship.x = 6400;
  ship.y = 3200;
  ship.vx = 0;
  ship.vy = 0;
}
void resetSim() {
  //ship.y = 5000;
  //shipX = 6400;
  UFO.x = 6400;
  UFO.y = 5000;
  resetField();
  simState = ProgState::Main;
  score = 0;
}
void resetField() {
  level = 0;
  ship.vx = ship.vy = 0;
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
void Alien() {
  if (invasion > 0) {
    invasion = invasion - 1;
    if (UFO.x < 12000 && UFO.x > 800 && UFO.y > 400 && UFO.y < 6000 && invasion < 1)
      invasion = 1;
    UFO.x = UFO.x + UFO.vx;
    UFO.y = UFO.y + UFO.vy;
    if (UFO.x > 13000) UFO.x = UFO.x - 13800;
    if (UFO.x < -500) UFO.x = UFO.x + 13800;
    if (UFO.y > 6900) UFO.y = UFO.y - 7400;
    if (UFO.y < -500) UFO.y = UFO.y + 7400;
    if (UFOB.var > 0) {
      UFOB.x = UFOB.x + UFOB.vx;
      UFOB.y = UFOB.y + UFOB.vy;
      UFOB.var = UFOB.var - 4;
    }
    if (arduboy.everyXFrames(30)) {
      UFO.vx = random (-50, 50);
      UFO.vy = random (-50, 50);
      UFO.var = random(0, 24);
    }
    if (arduboy.everyXFrames(105)) {
      UFOB.x = UFO.x;
      UFOB.y = UFO.y;
      UFOB.vx = (int8_t)(pgm_read_byte(&heading[UFO.var][0])) * 20;
      UFOB.vy = (int8_t)(pgm_read_byte(&heading[UFO.var][1])) * 20;
      UFOB.var = 100;
    }
    if (UFOB.var <= 0) {
      UFOB.x = UFOB.y = UFOB.vx = UFOB.vy = UFOB.var = 0;
    } else {
      arduboy.drawPixel(UFOB.x / 100, UFOB.y / 100, WHITE);
      UFOB.var = UFOB.var - 4;
      /*if ((abs(UFOV[0] - ship.x) < 700 && abs(UFOV[1] - ship.y) < 700) ||
          (abs(UFOV[0] - ship.x) + abs(UFOV[1] - ship.y) < 1000)) {
        sound.tone(150, 50);
        collision();
        UFOB.x = UFOB.y = UFOB.vx = UFOB.vy = UFOB.var = 0;
        }*/
    }
    arduboy.drawBitmap(UFO.x / 100 - 4, UFO.y / 100 - 4, UFOSprite, 8, 8, WHITE);
  } else {
    invasion = 0;
    if (arduboy.everyXFrames(30)) {
      if (tick == 120 && (random(0, 10) == 8)) {
        invasion = 240;
        UFO.x = 9600 + random(0, 7400);
        UFO.y = 4800 + random(0, 4200);
        UFO.vx = random (-50, 50);
        UFO.vy = random (-50, 50);
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
        UFO.x = UFO.x + UFO.vx;
        UFO.y = UFO.y + UFO.vy;
        if (UFO.x > 13000) UFO.x = UFO.x - 13800;
        if (UFO.x < -500) UFO.x = UFO.x + 13800;
        if (UFO.y > 6900) UFO.y = UFO.y - 7400;
        if (UFO.y < -500) UFO.y = UFO.y + 7400;
        if (arduboy.everyXFrames(30)) {
          UFO.vx = random (-50, 50);
          UFO.vy = random (-50, 50);
        }
        arduboy.drawBitmap(UFO.x / 100 - 4, UFO.y / 100 - 4, UFOSprite, 8, 8, WHITE);
        /*if (arduboy.everyXFrames(10)) {
          turnC = random(0, 2) == 1;
          }
          if (arduboy.everyXFrames(3)) {
          if (turnC) ship.var = ship.var + 1;
          else ship.var = ship.var - 1;
          }
          if (ship.var > 23) ship.var = ship.var - 24;
          if (ship.var < 0) ship.var = ship.var + 24;
          ship.vx = 100;
          ship();
          if (arduboy.everyXFrames(5) && (bulletCount < 8)) {
          bullet[bulletCount][1] = (int8_t)(pgm_read_byte(&heading[ship.var][0])) * 20;
          bullet[bulletCount][3] = (int8_t)(pgm_read_byte(&heading[ship.var][1])) * 20;
          bullet[bulletCount][0] = shipX + (int8_t)(pgm_read_byte(&shipTbl[2][ship.var]));
          bullet[bulletCount][2] = ship.y + (int8_t)(pgm_read_byte(&shipTbl[3][ship.var]));
          bullet[bulletCount][4] = 100;
          bulletCount = bulletCount + 1;
          }
          trajectory();
        */
        if (arduboy.justPressed(B_BUTTON)) {
          life = 3;
          ship.x = 6400;
          ship.y = 3200;
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
            if (arduboy.justPressed(B_BUTTON) && (bulletCount < 8)) firing();
            trajectory();
          }
          asteroid();
          rock();
          pebble();
          radar();
          Alien();
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
  /*//*/
  arduboy.setTextSize(1);
  arduboy.setCursor(2, 8);
  arduboy.print(score);
  arduboy.setCursor(32, 8);
  arduboy.print(level);
  arduboy.setCursor(120, 8);
  arduboy.print(life);
  /*//*/
    arduboy.setCursor(44, 8);
    arduboy.print(ship.var);
    arduboy.setCursor(60, 8);
    arduboy.print(asteroidCount);
    arduboy.setCursor(78, 8);
    arduboy.print(rockCount);
    arduboy.setCursor(96, 8);
    arduboy.print(pebbleCount);
    arduboy.setCursor(110, 8);
    arduboy.print(bulletCount);
    /*//*/
  arduboy.setCursor(100, 24);
  arduboy.print(ship.x);
  arduboy.setCursor(100, 34);
  arduboy.print(ship.y);
  arduboy.setCursor(100, 44);
  arduboy.print(ship.vx);
  arduboy.setCursor(100, 54);
  arduboy.print(ship.vy);
  /*//*/
  arduboy.setCursor(0, 24);
  arduboy.print(UFO.x);
  arduboy.setCursor(0, 34);
  arduboy.print(UFO.y);
  arduboy.setCursor(0, 44);
  arduboy.print(UFO.vx);
  arduboy.setCursor(0, 54);
  arduboy.print(UFO.vy);

  arduboy.setCursor(30, 55);
  arduboy.print(arduboy.cpuLoad());
  arduboy.setCursor(54, 55);
  arduboy.print(tick);
  arduboy.setCursor(80, 55);
  arduboy.print(invasion);

  /*//*/

  /**///splash screen
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
