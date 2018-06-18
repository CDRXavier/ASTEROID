#include <Arduboy.h>
float shipX;
float shipY;
float moveX;
float moveY;
int8_t HDG;
bool released;
int8_t life;
uint8_t level = 0;
byte bulletCount;
byte asteroidCount;
byte rockCount;
byte pebbleCount;
unsigned short score;
signed short bullet[8][5];
float asteroids[8][4];
float rocks[16][4];
float pebbles[32][4];
uint8_t tick;
bool turnC = false;
static const PROGMEM float heading[24][2] = {
  { 0.00,-0.20},{ 0.05,-0.19},
  { 0.10,-0.17},{ 0.14,-0.14},
  { 0.17,-0.10},{ 0.19,-0.05},
  { 0.20, 0   },{ 0.19, 0.05},
  { 0.17, 0.10},{ 0.14, 0.14},
  { 0.10, 0.17},{ 0.05, 0.19},
  { 0   , 0.20},{-0.05, 0.19},
  {-0.10, 0.17},{-0.14, 0.14},
  {-0.17, 0.10},{-0.19, 0.05},
  {-0.20, 0   },{-0.19,-0.05},
  {-0.17,-0.10},{-0.14,-0.14},
  {-0.10,-0.17},{-0.05,-0.19}
};
static const PROGMEM signed short bulletST[24][2] = {
  { 0,-4},{ 2,-4},{ 3,-3},{ 3,-3},
  { 3,-3},{ 4,-2},{ 4, 0},{ 4, 2},
  { 3, 3},{ 3, 3},{ 3, 3},{ 2, 4},
  { 0, 4},{-2, 4},{-3, 3},{-3, 3},
  {-3, 3},{-4, 2},{-4, 0},{-4,-2},
  {-3,-3},{-3,-3},{-3,-3},{-2,-4}
};
Arduboy arduboy;
void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.initRandomSeed();
  arduboy.setFrameRate(30);
  arduboy.clear();
  shipX = 64;
  shipY = 48;
  moveX = moveY = bulletCount = asteroidCount = 0;
  life = 0;
  score = 0;
  tick = 240;
}

void loop() {

  // put your main code here, to run repeatedly:
  if (!arduboy.nextFrame())return;

  if (life > 0) {
    //begin ship
    if (arduboy.everyXFrames(2)) {
      if (arduboy.pressed(LEFT_BUTTON)) HDG--;
      if (arduboy.pressed(RIGHT_BUTTON)) HDG++;
      if (HDG > 23) HDG = HDG - 24;
      if (HDG < 0) HDG = HDG + 24;
      if (arduboy.pressed(UP_BUTTON)) {
        moveX = moveX + pgm_read_float(&heading[HDG][0]);
        moveY = moveY + pgm_read_float(&heading[HDG][1]);
      }
    }
    
    if (moveY > 3)moveY = 3;
    if (moveY < -3)moveY = -3;
    if (moveX > 3)moveX = 3;
    if (moveX < -3)moveX = -3;
    if (moveY > 0)moveY = moveY - 0.01;
    if (moveY < 0)moveY = moveY + 0.01;
    if (moveX > 0)moveX = moveX - 0.01;
    if (moveX < 0)moveX = moveX + 0.01;
    if (abs(moveX) < 0.03) moveX = 0;
    if (abs(moveY) < 0.03) moveY = 0;
    shipX = shipX + moveX;
    shipY = shipY + moveY;
    if (shipX < -5) shipX = shipX + 138;
    if (shipX > 133) shipX = shipX - 138;
    if (shipY < -5) shipY = shipY + 74;
    if (shipY > 69) shipY = shipY - 74;

    if (arduboy.pressed(DOWN_BUTTON) && released) {
      delay(300);
      released = false;
      moveX = moveY = 0;
      shipX = random(0, 128);
      shipY = random(0, 64);
    }
    //ship end


    //bullet begin
    if (arduboy.pressed(B_BUTTON) && released && (bulletCount < 8)) {
      released = false;
      bullet[bulletCount][1] = (short)(round)(pgm_read_float(&heading[HDG][0]) * 15);
      bullet[bulletCount][3] = (short)(round)(pgm_read_float(&heading[HDG][1]) * 15);
      bullet[bulletCount][0] = (short)shipX + (short)(round)pgm_read_float(&bulletST[HDG][0]);
      bullet[bulletCount][2] = (short)shipY + (short)(round)pgm_read_float(&bulletST[HDG][1]);
      bullet[bulletCount][4] = 100;
      bulletCount = bulletCount + 1;
    }
    arduboy.clear();
    for (uint8_t t = 0; t < bulletCount; t++) {
      bullet[t][0] = bullet[t][0] + bullet[t][1];
      bullet[t][2] = bullet[t][2] + bullet[t][3];
      if (bullet[t][0] < -5) bullet[t][0] = bullet[t][0] + 138;
      if (bullet[t][0] > 133) bullet[t][0] = bullet[t][0] - 138;
      if (bullet[t][2] < -5) bullet[t][2] = bullet[t][2] + 74;
      if (bullet[t][2] > 69) bullet[t][2] = bullet[t][2] - 74;
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
      arduboy.drawPixel(bullet[t][0], bullet[t][2], 1);
      bullet[t][4] = bullet[t][4] - 4;
    }
    //end bullet


    //begin Astroid
    for (uint8_t t = 0; t < asteroidCount; t++) {
      asteroids[t][0] = asteroids[t][0] + asteroids[t][1];
      asteroids[t][2] = asteroids[t][2] + asteroids[t][3];
      if (asteroids[t][0] < -5) asteroids[t][0] = asteroids[t][0] + 138;
      if (asteroids[t][0] > 133) asteroids[t][0] = asteroids[t][0] - 138;
      if (asteroids[t][2] < -5) asteroids[t][2] = asteroids[t][2] + 74;
      if (asteroids[t][2] > 69) asteroids[t][2] = asteroids[t][2] - 74;
      for (uint8_t b = 0; b < bulletCount; b++) {
        if (abs(bullet[b][0] - asteroids[t][0]) < 5 && abs(bullet[b][2] - asteroids[t][2]) < 5) {
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

          score = score + 5;

          rocks[rockCount][0] = asteroids[t][0];
          rocks[rockCount][2] = asteroids[t][2];
          rocks[rockCount][1] = random(-10, 10) * 0.05;
          rocks[rockCount][3] = random(-10, 10) * 0.05;
          rockCount = rockCount + 1;
          rocks[rockCount][0] = asteroids[t][0];
          rocks[rockCount][2] = asteroids[t][2];
          rocks[rockCount][1] = random(-10, 10) * 0.05;
          rocks[rockCount][3] = random(-10, 10) * 0.05;
          rockCount = rockCount + 1;

          for (uint8_t c = t; c < asteroidCount - 1; c++)
            for (uint8_t d = 0; d < 4; d++)
              asteroids[c][d] = asteroids[c + 1][d];
          asteroidCount = asteroidCount - 1;
        }
      }
      arduboy.drawCircle(asteroids[t][0], asteroids[t][2], 5, 5);
    }
    //end asteroids


    //begin rocks
    for (uint8_t r = 0; r < rockCount; r++) {
      rocks[r][0] = rocks[r][0] + rocks[r][1];
      rocks[r][2] = rocks[r][2] + rocks[r][3];
      if (rocks[r][0] < -5) rocks[r][0] = rocks[r][0] + 138;
      if (rocks[r][0] > 133) rocks[r][0] = rocks[r][0] - 138;
      if (rocks[r][2] < -5) rocks[r][2] = rocks[r][2] + 74;
      if (rocks[r][2] > 69) rocks[r][2] = rocks[r][2] - 74;
      for (uint8_t b = 0; b < bulletCount; b++) {
        if (abs(bullet[b][0] - rocks[r][0]) < 4 && abs(bullet[b][2] - rocks[r][2]) < 4) {
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

          score = score + 10;

          pebbles[pebbleCount][0] = rocks[r][0];
          pebbles[pebbleCount][2] = rocks[r][2];
          pebbles[pebbleCount][1] = random(-10, 10) * 0.05;
          pebbles[pebbleCount][3] = random(-10, 10) * 0.05;
          pebbleCount = pebbleCount + 1;
          pebbles[pebbleCount][0] = rocks[r][0];
          pebbles[pebbleCount][2] = rocks[r][2];
          pebbles[pebbleCount][1] = random(-10, 10) * 0.05;
          pebbles[pebbleCount][3] = random(-10, 10) * 0.05;
          pebbleCount = pebbleCount + 1;

          for (uint8_t c = r; c < rockCount - 1; c++)
            for (uint8_t d = 0; d < 4; d++)
              rocks[c][d] = rocks[c + 1][d];
          rockCount = rockCount - 1;
        }
      }
      arduboy.drawCircle(rocks[r][0], rocks[r][2], 4, 4);
    }
    //end rocks

    //begin pebbles
    for (uint8_t p = 0; p < pebbleCount; p++) {
      pebbles[p][0] = pebbles[p][0] + pebbles[p][1];
      pebbles[p][2] = pebbles[p][2] + pebbles[p][3];
      if (pebbles[p][0] < -5) pebbles[p][0] = pebbles[p][0] + 138;
      if (pebbles[p][0] > 133) pebbles[p][0] = pebbles[p][0] - 138;
      if (pebbles[p][2] < -5) pebbles[p][2] = pebbles[p][2] + 74;
      if (pebbles[p][2] > 69) pebbles[p][2] = pebbles[p][2] - 74;
      for (uint8_t b = 0; b < bulletCount; b++) {
        if (abs(bullet[b][0] - pebbles[p][0]) < 2 && abs(bullet[b][2] - pebbles[p][2]) < 2) {
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

          score = score + 15;

          for (uint8_t c = p; c < pebbleCount - 1; c++)
            for (uint8_t d = 0; d < 4; d++)
              pebbles[c][d] = pebbles[c + 1][d];
          pebbles[pebbleCount][0] = 0;
          pebbles[pebbleCount][1] = 0;
          pebbles[pebbleCount][2] = 0;
          pebbles[pebbleCount][3] = 0;
          pebbleCount = pebbleCount - 1;
        }
      }
      arduboy.drawCircle(pebbles[p][0], pebbles[p][2], 2, 2);
    }
    //end pebbles

    //begin crash
    for (uint8_t a = 0; a < asteroidCount; a ++) {
      if (abs(asteroids[a][0] - shipX) < 7 && abs(asteroids[a][2] - shipY) < 7) {
        delay(100);
        rocks[rockCount][0] = asteroids[a][0] + 10 * asteroids[a][1];
        rocks[rockCount][2] = asteroids[a][2] + 10 * asteroids[a][3];
        rocks[rockCount][1] = asteroids[a][1];
        rocks[rockCount][3] = asteroids[a][3];
        rockCount = rockCount + 1;
        for (uint8_t c = a; c < asteroidCount - 1; c++)
          for (uint8_t d = 0; d < 4; d++)
            asteroids[c][d] = asteroids[c + 1][d];
        asteroidCount = asteroidCount - 1;
        life = life - 1;
        shipX = 64;
        shipY = 32;
        moveX = 0;
        moveY = 0;
      }
    }
    for (uint8_t r = 0; r < rockCount; r ++) {
      if (abs(rocks[r][0] - shipX) < 5 && abs(rocks[r][2] - shipY) < 5) {
        pebbles[pebbleCount][0] = rocks[r][0] + 10 * rocks[r][1];
        pebbles[pebbleCount][2] = rocks[r][2] + 10 * rocks[r][3];
        pebbles[pebbleCount][1] = rocks[r][1];
        pebbles[pebbleCount][3] = rocks[r][3];
        pebbleCount = pebbleCount + 1;

        for (uint8_t c = r; c < rockCount - 1; c++)
          for (uint8_t d = 0; d < 4; d++)
            rocks[c][d] = rocks[c + 1][d];
        rockCount = rockCount - 1;
        life = life - 1;
        shipX = 64;
        shipY = 32;
        moveX = 0;
        moveY = 0;
      }
    }
    for (uint8_t p = 0; p < pebbleCount; p ++) {
      if (abs(pebbles[p][0] - shipX) < 3 && abs(pebbles[p][2] - shipY) < 3) {
        for (uint8_t c = p; c < pebbleCount - 1; c++)
          for (uint8_t d = 0; d < 4; d++)
            pebbles[c][d] = pebbles[c + 1][d];
        pebbleCount = pebbleCount - 1;
        life = life - 1;
        shipX = 64;
        shipY = 32;
        moveX = 0;
        moveY = 0;
      }
    }
    if (asteroidCount == 0 && rockCount == 0 && pebbleCount == 0) {
      tick = tick - 1;
      if (tick <= 0) {
        tick = 240;
        if (level < 8) level = level + 1;
        //THIS DOES NOT WORK!!!
        for (int8_t s = 0; s < level; s++) {
          asteroids[asteroidCount][0] = 96 + random(0,74);
          asteroids[asteroidCount][2] = 48 + random(0,42);
          asteroids[asteroidCount][1] = random(-10, 10) * 0.05;
          asteroids[asteroidCount][3] = random(-10, 10) * 0.05;
          asteroidCount = asteroidCount + 1;
        }
      }
    }
    if (life == 0) {
      shipY = 50;
      shipX = 64;
      level = 0;
      moveX = moveY = 0;
      tick = 240;
      asteroidCount = rockCount = pebbleCount = 0;
    }
    //end crash
  } else {
    //begin homescreen
    arduboy.clear();
    arduboy.setCursor(10, 10);
    arduboy.setTextSize(2);
    arduboy.print("ASTEROIDS");
    if (arduboy.pressed(B_BUTTON)) {
      life = 3;
      shipX = 64;
      shipY = 32;
      tick = 240;
      return;
    }
    if (arduboy.everyXFrames(15)) {
      turnC = random(0,2) == 1;
    }
    if (arduboy.everyXFrames(2)) {
      if (turnC) HDG = HDG + 1;
      else HDG = HDG - 1;
    }
    if (HDG > 23) HDG = HDG - 24;
    if (HDG < 0) HDG = HDG + 24;
    shipX = shipX + 1;
    if (shipX < -5) shipX = 133;
    if (shipX > 133) shipX = -5;
    if (arduboy.everyXFrames(10) && (bulletCount < 8)) {
      bullet[bulletCount][1] = (short)(round)(pgm_read_float(&heading[HDG][0]) * 15);
      bullet[bulletCount][3] = (short)(round)(pgm_read_float(&heading[HDG][1]) * 15);
      bullet[bulletCount][0] = (short)shipX + (short)(round)pgm_read_float(&bulletST[HDG][0]);
      bullet[bulletCount][2] = (short)shipY + (short)(round)pgm_read_float(&bulletST[HDG][1]);
      bullet[bulletCount][4] = 100;
      bulletCount = bulletCount + 1;
    }
    for (uint8_t t = 0; t < bulletCount; t++) {
      bullet[t][0] = bullet[t][0] + bullet[t][1];
      bullet[t][2] = bullet[t][2] + bullet[t][3];
      if (bullet[t][0] < -5) bullet[t][0] = bullet[t][0] + 138;
      if (bullet[t][0] > 133) bullet[t][0] = bullet[t][0] - 138;
      if (bullet[t][2] < -5) bullet[t][2] = bullet[t][2] + 74;
      if (bullet[t][2] > 69) bullet[t][2] = bullet[t][2] - 74;
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
      arduboy.drawPixel(bullet[t][0], bullet[t][2], 1);
      bullet[t][4] = bullet[t][4] - 4;
    }
  }
  //end homescreen
  
  //begin printObj
  //debug use
  arduboy.setTextSize(1);
  arduboy.setCursor(2, 10);
  arduboy.print(score);
  arduboy.setCursor(30, 10);
  arduboy.print(HDG);
  arduboy.setCursor(50, 10);
  arduboy.print(asteroidCount);
  arduboy.setCursor(70, 10);
  arduboy.print(rockCount);
  arduboy.setCursor(90, 10);
  arduboy.print(pebbleCount);
  arduboy.setCursor(110, 10);
  arduboy.print(bulletCount);
  arduboy.setCursor(120, 10);
  arduboy.print(life);
  arduboy.setCursor(100,25);
  arduboy.print(shipX);
  arduboy.setCursor(100,35);
  arduboy.print(shipY);
  arduboy.setCursor(100,45);
  arduboy.print(moveX);
  arduboy.setCursor(100,55);
  arduboy.print(moveY);
  arduboy.setCursor(10, 50);
  arduboy.print(tick);
  //debug use
  switch (HDG) {
    case  0: arduboy.drawTriangle(shipX    , shipY - 4, shipX + 3, shipY + 3, shipX - 3, shipY + 3, 1); break;

    case  1: arduboy.drawTriangle(shipX + 2, shipY - 4, shipX + 3, shipY + 3, shipX - 3, shipY + 2, 1); break;
    case  2: arduboy.drawTriangle(shipX + 3, shipY - 3, shipX + 2, shipY + 3, shipX - 3, shipY + 1, 1); break;
    case  3: arduboy.drawTriangle(shipX + 3, shipY - 3, shipX + 1, shipY + 3, shipX - 3, shipY - 1, 1); break;
    case  4: arduboy.drawTriangle(shipX + 3, shipY - 3, shipX - 1, shipY + 3, shipX - 3, shipY - 2, 1); break;
    case  5: arduboy.drawTriangle(shipX + 4, shipY - 2, shipX - 2, shipY + 3, shipX - 3, shipY - 3, 1); break;
    case  6: arduboy.drawTriangle(shipX + 4, shipY    , shipX - 3, shipY + 3, shipX - 3, shipY - 3, 1); break;

    case  7: arduboy.drawTriangle(shipX + 4, shipY + 2, shipX - 2, shipY - 3, shipX - 3, shipY + 3, 1); break;
    case  8: arduboy.drawTriangle(shipX + 3, shipY + 3, shipX - 1, shipY - 3, shipX - 3, shipY + 2, 1); break;
    case  9: arduboy.drawTriangle(shipX + 3, shipY + 3, shipX + 1, shipY - 3, shipX - 3, shipY + 1, 1); break;
    case 10: arduboy.drawTriangle(shipX + 3, shipY + 3, shipX + 2, shipY - 3, shipX - 3, shipY - 1, 1); break;
    case 11: arduboy.drawTriangle(shipX + 2, shipY + 4, shipX + 3, shipY - 3, shipX - 3, shipY - 2, 1); break;
    case 12: arduboy.drawTriangle(shipX    , shipY + 4, shipX + 3, shipY - 3, shipX - 3, shipY - 3, 1); break;

    case 13: arduboy.drawTriangle(shipX - 2, shipY + 4, shipX - 3, shipY - 3, shipX + 3, shipY - 2, 1); break;
    case 14: arduboy.drawTriangle(shipX - 3, shipY + 3, shipX - 2, shipY - 3, shipX + 3, shipY - 1, 1); break;
    case 15: arduboy.drawTriangle(shipX - 3, shipY + 3, shipX - 1, shipY - 3, shipX + 3, shipY + 1, 1); break;
    case 16: arduboy.drawTriangle(shipX - 3, shipY + 3, shipX + 1, shipY - 3, shipX + 3, shipY + 2, 1); break;
    case 17: arduboy.drawTriangle(shipX - 4, shipY + 2, shipX + 2, shipY - 3, shipX + 3, shipY + 3, 1); break;
    case 18: arduboy.drawTriangle(shipX - 4, shipY    , shipX + 3, shipY + 3, shipX + 3, shipY - 3, 1); break;

    case 19: arduboy.drawTriangle(shipX - 4, shipY - 2, shipX + 2, shipY + 3, shipX + 3, shipY - 3, 1); break;
    case 20: arduboy.drawTriangle(shipX - 3, shipY - 3, shipX + 1, shipY + 3, shipX + 3, shipY - 2, 1); break;
    case 21: arduboy.drawTriangle(shipX - 3, shipY - 3, shipX - 1, shipY + 3, shipX + 3, shipY - 1, 1); break;
    case 22: arduboy.drawTriangle(shipX - 3, shipY - 3, shipX - 2, shipY + 3, shipX + 3, shipY + 1, 1); break;
    case 23: arduboy.drawTriangle(shipX - 2, shipY - 4, shipX - 3, shipY + 3, shipX + 3, shipY + 2, 1); break;
    default: arduboy.drawCircle(shipX, shipY, 2, 2);
  }
  //end printObj



  //begin display
  arduboy.display();
  //end display

  //begin pollButtons
  if (arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON) && arduboy.notPressed(DOWN_BUTTON))
    released = true;
  //end pollButtons
}
