#include <Arduboy.h>
float shipX;
float shipY;
float moveX;
float moveY;
int8_t HDG;
bool released;
uint8_t level;
uint8_t bulletCount;
int8_t astroidCount;
uint8_t rockCount;
uint8_t pebbleCount;
Arduboy arduboy;
void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(15);
  arduboy.clear();
  shipX = 64;
  shipY = 48;
  moveX = moveY = 0;
  bulletCount = 0;
  astroidCount = 0;
  level = 0;
}

void loop() {

  // put your main code here, to run repeatedly:
  if (!arduboy.nextFrame())return;
  
  
  //begin guidance
  if (HDG > 23) HDG = 0;
  if (HDG < 0) HDG = 23;
  //end guidance
  if (level > 0) {
    //begin var
    float bullet[8][5];
    float astroids[8][4];
    float rocks[16][4];
    float pebbles[32][4];
    
    //end var
    //begin ship
    if (arduboy.pressed(LEFT_BUTTON)) HDG--;
    if (arduboy.pressed(RIGHT_BUTTON)) HDG++;
    if (arduboy.pressed(UP_BUTTON))
    switch (HDG) {
    case 0: moveY = moveY - 0.20; break;
    case 1: moveY = moveY - 0.19;
            moveX = moveX + 0.05; break;
    case 13:moveY = moveY + 0.19;
            moveX = moveX - 0.05; break;
    case 2: moveY = moveY - 0.17;
            moveX = moveX + 0.10; break;
    case 14:moveY = moveY + 0.17;
            moveX = moveX - 0.10; break;
    case 3: moveY = moveY - 0.14;
            moveX = moveX + 0.14; break;
    case 15:moveY = moveY + 0.14;
            moveX = moveX - 0.14; break;
    case 4: moveY = moveY - 0.10;
            moveX = moveX + 0.17; break;
    case 16:moveY = moveY + 0.10;
            moveX = moveX - 0.17; break;
    case 5: moveY = moveY - 0.05;
            moveX = moveX + 0.19; break;
    case 17:moveY = moveY + 0.05;
            moveX = moveX - 0.19; break;
    case 6: moveX = moveX + 0.20; break;
    case 18:moveX = moveX - 0.20; break;
    case 7: moveY = moveY + 0.05;
            moveX = moveX + 0.19; break;
    case 19:moveY = moveY - 0.05;
            moveX = moveX - 0.19; break;
    case 8: moveY = moveY + 0.10;
            moveX = moveX + 0.17; break;
    case 20:moveY = moveY - 0.10;
            moveX = moveX - 0.17; break;
    case 9: moveY = moveY + 0.14;
            moveX = moveX + 0.14; break;
    case 21:moveY = moveY - 0.14;
            moveX = moveX - 0.14; break;
    case 10:moveY = moveY + 0.17;
            moveX = moveX + 0.10; break;
    case 22:moveY = moveY - 0.17;
            moveX = moveX - 0.10; break;
    case 11:moveY = moveY + 0.19;
            moveX = moveX + 0.05; break;
    case 23:moveY = moveY - 0.19;
            moveX = moveX - 0.05; break;
    case 12:moveY = moveY + 0.20; break;
    }
  
    if (moveY > 3)moveY = 3;
    if (moveY <-3)moveY = -3;
    if (moveX > 3)moveX = 3;
    if (moveX <-3)moveX = -3;
    if (moveY > 0)moveY = moveY - 0.01;
    if (moveY < 0)moveY = moveY + 0.01;
    if (moveX > 0)moveX = moveX - 0.01;
    if (moveX < 0)moveX = moveX + 0.01;
    if (abs(moveX) < 0.025) moveX = 0;
    if (abs(moveY) < 0.025) moveY = 0;
    shipX = shipX + moveX;
    shipY = shipY + moveY;
    if (shipX < -5) shipX = 133;
    if (shipX > 133) shipX = -5;
    if (shipY < -5) shipY = 69;
    if (shipY > 69) shipY = -5;
    
    if (arduboy.pressed(DOWN_BUTTON) && released) {
      delay(300);
      released = false;
      moveX = moveY = 0;
      shipX = random(0,128);
      shipY = random(0,64);
    }
    if (arduboy.pressed(A_BUTTON) && released) {
      released = false;
      astroids[astroidCount][0] = random(0,128);
      astroids[astroidCount][2] = random(0, 64);
      astroids[astroidCount][1] = random(0,10) * 0.1;
      astroids[astroidCount][3] = random(0,10) * 0.1;
      astroidCount = astroidCount + 1;
    }
    //ship end
  
  
    //bullet begin
    if (arduboy.pressed(B_BUTTON) && released && (bulletCount < 8)) {
      released = false;
      switch (HDG) {
        case  0: bullet[bulletCount][0] =   shipX  ;
                 bullet[bulletCount][1] =    0     ;
                 bullet[bulletCount][2] = shipY - 4;
                 bullet[bulletCount][3] =   -4     ; break;
        case  1: bullet[bulletCount][0] = shipX + 2;
                 bullet[bulletCount][1] =    1     ;
                 bullet[bulletCount][2] = shipY - 4;
                 bullet[bulletCount][3] =   -3.8   ; break;
        case  2: bullet[bulletCount][0] = shipX + 3;
                 bullet[bulletCount][1] =    1     ;
                 bullet[bulletCount][2] = shipY - 3;
                 bullet[bulletCount][3] =   -3.4   ; break;
        case  3: bullet[bulletCount][0] = shipX + 3;
                 bullet[bulletCount][1] =    2.8   ;
                 bullet[bulletCount][2] = shipY - 3;
                 bullet[bulletCount][3] =   -2.8   ; break;
        case  4: bullet[bulletCount][0] = shipX + 3;
                 bullet[bulletCount][1] =    3.4   ;
                 bullet[bulletCount][2] = shipY - 3;
                 bullet[bulletCount][3] =   -2     ; break;
        case  5: bullet[bulletCount][0] = shipX + 4;
                 bullet[bulletCount][1] =    3.8   ;
                 bullet[bulletCount][2] = shipY - 2;
                 bullet[bulletCount][3] =   -1     ; break;
        case  6: bullet[bulletCount][0] = shipX + 4;
                 bullet[bulletCount][1] =    4     ;
                 bullet[bulletCount][2] =  shipY   ;
                 bullet[bulletCount][3] =    0     ; break;
        case  7: bullet[bulletCount][0] = shipX + 4;
                 bullet[bulletCount][1] =    3.8   ;
                 bullet[bulletCount][2] = shipY + 2;
                 bullet[bulletCount][3] =    1     ; break;
        case  8: bullet[bulletCount][0] = shipX + 3;
                 bullet[bulletCount][1] =    3.4   ;
                 bullet[bulletCount][2] = shipY + 3;
                 bullet[bulletCount][3] =    2     ; break;
        case  9: bullet[bulletCount][0] = shipX + 3;
                 bullet[bulletCount][1] =    2.8   ;
                 bullet[bulletCount][2] = shipY + 3;
                 bullet[bulletCount][3] =    2.8   ; break;
        case 10: bullet[bulletCount][0] = shipX + 3;
                 bullet[bulletCount][1] =     2    ;
                 bullet[bulletCount][2] = shipY + 3;
                 bullet[bulletCount][3] =    3.4   ; break;
        case 11: bullet[bulletCount][0] = shipX + 2;
                 bullet[bulletCount][1] =    1     ;
                 bullet[bulletCount][2] = shipY + 4;
                 bullet[bulletCount][3] =    3.4   ; break;
        case 12: bullet[bulletCount][0] =   shipX  ;
                 bullet[bulletCount][1] =    0     ;
               bullet[bulletCount][2] = shipY + 4;
               bullet[bulletCount][3] =    4     ; break;
      case 13: bullet[bulletCount][0] = shipX - 2;
               bullet[bulletCount][1] =   -1     ;
               bullet[bulletCount][2] = shipY + 4;
               bullet[bulletCount][3] =    3.8   ; break;
      case 14: bullet[bulletCount][0] = shipX - 3;
               bullet[bulletCount][1] =   -2     ;
               bullet[bulletCount][2] = shipY + 3;
               bullet[bulletCount][3] =    3.4   ; break;
      case 15: bullet[bulletCount][0] = shipX - 3;
               bullet[bulletCount][1] =   -2.8   ;
               bullet[bulletCount][2] = shipY + 3;
               bullet[bulletCount][3] =    2.8   ; break;
      case 16: bullet[bulletCount][0] = shipX - 3;
               bullet[bulletCount][1] =   -3.4   ;
               bullet[bulletCount][2] = shipY + 3;
               bullet[bulletCount][3] =    2     ; break;
      case 17: bullet[bulletCount][0] = shipX - 2;
               bullet[bulletCount][1] =   -3.8   ;
               bullet[bulletCount][2] = shipY + 4;
               bullet[bulletCount][3] =    1     ; break;
      case 18: bullet[bulletCount][0] = shipX - 4;
               bullet[bulletCount][1] =   -4     ;
               bullet[bulletCount][2] =  shipY   ;
               bullet[bulletCount][3] =    0     ; break;
     case  19: bullet[bulletCount][0] = shipX - 4;
               bullet[bulletCount][1] =   -3.8   ;
               bullet[bulletCount][2] = shipY - 2;
               bullet[bulletCount][3] =   -1     ; break;
     case  20: bullet[bulletCount][0] = shipX - 3;
               bullet[bulletCount][1] =   -3.4   ;
               bullet[bulletCount][2] = shipY - 3;
               bullet[bulletCount][3] =   -2     ; break;
      case 21: bullet[bulletCount][0] = shipX - 3;
               bullet[bulletCount][1] =   -2.8   ;
               bullet[bulletCount][2] = shipY - 3;
               bullet[bulletCount][3] =   -2.8   ; break;
      case 22: bullet[bulletCount][0] = shipX - 3;
               bullet[bulletCount][1] =    -2    ;
               bullet[bulletCount][2] = shipY - 3;
               bullet[bulletCount][3] =   -3.4   ; break;
      case 23: bullet[bulletCount][0] = shipX - 2;
               bullet[bulletCount][1] =   -1     ;
               bullet[bulletCount][2] = shipY - 4;
               bullet[bulletCount][3] =   -3.8   ; break;
      default: bullet[bulletCount][0] =  shipX   ;
               bullet[bulletCount][1] =    0     ;
               bullet[bulletCount][2] =  shipY   ;
               bullet[bulletCount][3] =    0     ; break;
    }
    bullet[bulletCount][4] = 1;
    bulletCount = bulletCount + 1;
  }
  arduboy.clear();
  for(uint8_t t = 0; t < bulletCount; t++) {
    bullet[t][0] = bullet[t][0] + bullet[t][1];
    bullet[t][2] = bullet[t][2] + bullet[t][3];
    if (bullet[t][0] < -5) bullet[t][0] = 133;
    if (bullet[t][0] > 133) bullet[t][0] = -5;
    if (bullet[t][2] < -5) bullet[t][2] = 69;
    if (bullet[t][2] > 69) bullet[t][2] = -5;
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
    arduboy.drawPixel(bullet[t][0],bullet[t][2],1);
    bullet[t][4] = bullet[t][4] - 0.04; 
  }
  //end bullet


  //begin Astroid
  for (uint8_t t = 0; t < astroidCount; t++) {
    astroids[t][0] = astroids[t][0] + astroids[t][1];
    astroids[t][2] = astroids[t][2] + astroids[t][3];
    if (astroids[t][0] < -5) astroids[t][0] = 133;
    if (astroids[t][0] > 133) astroids[t][0] = -5;
    if (astroids[t][2] < -5) astroids[t][2] = 69;
    if (astroids[t][2] > 69) astroids[t][2] = -5;
    for (uint8_t b = 0; b < bulletCount; b++) {
      if (abs(bullet[b][0] - astroids[t][0]) < 5 && abs(bullet[b][2] - astroids[t][2]) < 5) {
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
      
      rocks[rockCount][0] = astroids[t][0];
      rocks[rockCount][2] = astroids[t][2];
      rocks[rockCount][1] = random(-10,10) * 0.1;
      rocks[rockCount][3] = random(-10,10) * 0.1;
      rockCount = rockCount + 1;
      rocks[rockCount][0] = astroids[t][0];
      rocks[rockCount][2] = astroids[t][2];
      rocks[rockCount][1] = random(-10,10) * 0.1;
      rocks[rockCount][3] = random(-10,10) * 0.1;
      rockCount = rockCount + 1;
      
      for (uint8_t c = t; c < astroidCount - 1; c++)
        for (uint8_t d = 0; d < 4; d++)
          astroids[c][d] = astroids[c + 1][d];
      astroidCount = astroidCount - 1;
      }
    }
    arduboy.drawCircle(astroids[t][0], astroids[t][2],5,5);
    
    
  }
  //end Astroids


  //begin rocks
  for (uint8_t r = 0; r < rockCount; r++) {
    rocks[r][0] = rocks[r][0] + rocks[r][1];
    rocks[r][2] = rocks[r][2] + rocks[r][3];
    if (rocks[r][0] < -5) rocks[r][0] = 133;
    if (rocks[r][0] > 133) rocks[r][0] = -5;
    if (rocks[r][2] < -5) rocks[r][2] = 69;
    if (rocks[r][2] > 69) rocks[r][2] = -5;
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
    
        pebbles[pebbleCount][0] = rocks[r][0];
        pebbles[pebbleCount][2] = rocks[r][2];
        pebbles[pebbleCount][1] = random(-10,10) * 0.1;
        pebbles[pebbleCount][3] = random(-10,10) * 0.1;
        pebbleCount = pebbleCount + 1;
        pebbles[pebbleCount][0] = rocks[r][0];
        pebbles[pebbleCount][2] = rocks[r][2];
        pebbles[pebbleCount][1] = random(-10,10) * 0.1;
        pebbles[pebbleCount][3] = random(-10,10) * 0.1;
        pebbleCount = pebbleCount + 1;
        
        for (uint8_t c = r; c < rockCount - 1; c++)
          for (uint8_t d = 0; d < 4; d++)
            rocks[c][d] = rocks[c + 1][d];
        rockCount = rockCount - 1;
      }
    }
    arduboy.drawCircle(rocks[r][0], rocks[r][2],4,4);
  }
  //end rocks

  //begin pebbles
  for (uint8_t p = 0; p < pebbleCount; p++) {
    pebbles[p][0] = pebbles[p][0] + pebbles[p][1];
    pebbles[p][2] = pebbles[p][2] + pebbles[p][3];
    if (pebbles[p][0] < -5) pebbles[p][0] = 133;
    if (pebbles[p][0] > 133) pebbles[p][0] = -5;
    if (pebbles[p][2] < -5) pebbles[p][2] = 69;
    if (pebbles[p][2] > 69) pebbles[p][2] = -5;
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
      arduboy.drawCircle(pebbles[p][0], pebbles[p][2],2,2);
    }
    //end pebbles
  
    //begin crash
    
    for (uint8_t a = 0; a < astroidCount; a ++) {
      if (abs(astroids[a][0] - shipX) < 3 && abs(astroids[a][2] - shipY) < 3) {
        level = 0;
        shipX = 64;
        shipY = 32;
        moveX = 0;
        moveY = 0;
        bulletCount = pebbleCount = rockCount = astroidCount = 0;
        arduboy.setFrameRate(15);
      }
    }
    for (uint8_t a = 0; a < rockCount; a ++) {
      if (abs(rocks[a][0] - shipX) < 3 && abs(rocks[a][2] - shipY) < 3) {
        level = 0;
        shipX = 64;
        shipY = 32;
        moveX = 0;
        moveY = 0;
        bulletCount = pebbleCount = rockCount = astroidCount = 0;
        arduboy.setFrameRate(15);
      }
    }
    for (uint8_t a = 0; a < pebbleCount; a ++) {
      if (abs(pebbles[a][0] - shipX) < 3 && abs(pebbles[a][2] - shipY) < 3) {
        level = 0;
        shipX = 64;
        shipY = 48;
        moveX = 0;
        moveY = 0;
        bulletCount = pebbleCount = rockCount = astroidCount = 0;
        arduboy.setFrameRate(15);
      }
    }
    //end crash
  }
  else {
    arduboy.clear();
    arduboy.setCursor(20,10);
    arduboy.setTextSize(2);
    arduboy.print("ASTROID");
    if (arduboy.pressed(B_BUTTON)) {
      level = 1;
      arduboy.setFrameRate(30);
      shipX = 64;
      shipY = 32;
      return;
    }
    HDG = HDG + 1;
    shipX = shipX + 1;
    if (shipX < -5) shipX = 133;
    if (shipX > 133) shipX = -5;
  }
  //begin printObj
  //arduboy.setCursor(10,10);
  //arduboy.print(HDG);
  //arduboy.setCursor(30,10);
  //arduboy.print(astroidCount);
  //arduboy.setCursor(50,10);
  //arduboy.print(rockCount);
  //arduboy.setCursor(70,10);
  //arduboy.print(pebbleCount);
  //arduboy.setCursor(100,10);
  //arduboy.print(bulletCount);
  //arduboy.drawPixel(shipX,shipY,1);
  //end printObj
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
    default:arduboy.drawCircle(shipX,shipY,2,2);
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
