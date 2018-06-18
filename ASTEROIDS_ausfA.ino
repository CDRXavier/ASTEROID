#include <Arduboy.h>
float tankX;
float tankY;
float moveX;
float moveY;
float bullet[8][5];
float Squares[16][4];
float Triangles[16][4];
float Pentagons[16][4];
byte HDG;
bool released;
byte life;
byte bulletCount;
byte SquareCount;
byte TriangleCount;
byte PentagonCount;
unsigned short score;
Arduboy arduboy;
void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.clear();
  tankX = 64;
  tankY = 48;
  moveX = moveY = 0;
  bulletCount = 0;
  SquareCount = 0;
  life = 0;
  score = 0;
}

void loop() {

  // put your main code here, to run repeatedly:
  if (!arduboy.nextFrame())return;
  
  
  //begin guidance
  if (HDG > 23) HDG = 0;
  if (HDG < 0) HDG = 23;
  //end guidance
  
  if (life > 0) {
    //begin ship
    if (arduboy.pressed(LEFT_BUTTON)) HDG--;
    if (arduboy.pressed(RIGHT_BUTTON)) HDG++;
    if (arduboy.pressed(UP_BUTTON))
    switch (HDG) {
    case 0: moveY = moveY - 2.0; break;
    case 1: moveY = moveY - 1.9;
            moveX = moveX + 0.5; break;
    case 13:moveY = moveY + 1.9;
            moveX = moveX - 0.5; break;
    case 2: moveY = moveY - 1.7;
            moveX = moveX + 1.0; break;
    case 14:moveY = moveY + 1.7;
            moveX = moveX - 1.0; break;
    case 3: moveY = moveY - 1.4;
            moveX = moveX + 1.4; break;
    case 15:moveY = moveY + 1.4;
            moveX = moveX - 1.4; break;
    case 4: moveY = moveY - 1.0;
            moveX = moveX + 1.7; break;
    case 16:moveY = moveY + 1.0;
            moveX = moveX - 1.7; break;
    case 5: moveY = moveY - 0.5;
            moveX = moveX + 1.9; break;
    case 17:moveY = moveY + 0.5;
            moveX = moveX - 1.9; break;
    case 6: moveX = moveX + 2.0; break;
    case 18:moveX = moveX - 2.0; break;
    case 7: moveY = moveY + 0.5;
            moveX = moveX + 1.9; break;
    case 19:moveY = moveY - 0.5;
            moveX = moveX - 1.9; break;
    case 8: moveY = moveY + 1.0;
            moveX = moveX + 1.7; break;
    case 20:moveY = moveY - 1.0;
            moveX = moveX - 1.7; break;
    case 9: moveY = moveY + 1.4;
            moveX = moveX + 1.4; break;
    case 21:moveY = moveY - 1.4;
            moveX = moveX - 1.4; break;
    case 10:moveY = moveY + 1.7;
            moveX = moveX + 1.0; break;
    case 22:moveY = moveY - 1.7;
            moveX = moveX - 1.0; break;
    case 11:moveY = moveY + 1.9;
            moveX = moveX + 0.5; break;
    case 23:moveY = moveY - 1.9;
            moveX = moveX - 0.5; break;
    case 12:moveY = moveY + 2.0; break;
    }
    if (life < 100) life = life + 1;
    if (moveY > 2)moveY = 2;
    if (moveY <-2)moveY = -2;
    if (moveX > 2)moveX = 2;
    if (moveX <-2)moveX = -2;
    if (moveY > 0)moveY = moveY - 0.1;
    if (moveY < 0)moveY = moveY + 0.1;
    if (moveX > 0)moveX = moveX - 0.1;
    if (moveX < 0)moveX = moveX + 0.1;
    if (abs(moveX) < 0.1) moveX = 0;
    if (abs(moveY) < 0.1) moveY = 0;
    tankX = tankX + moveX;
    tankY = tankY + moveY;
    if (tankX < 0) {
      tankX = 0;
      moveX = 0;
    }
    if (tankX > 128) {
      moveX = 0;
      tankX = 128;
    }
    if (tankY < 0){
      moveY = 0;
      tankY = 0;
    }
    if (tankY > 64) {
      moveY = 0;
      tankY = 64;
    }
    if (arduboy.pressed(A_BUTTON) && released) {
      released = false;
      Squares[SquareCount][0] = random(0,128);
      Squares[SquareCount][2] = random(0, 64);
      Squares[SquareCount][1] = random(0,10) * 0.1;
      Squares[SquareCount][3] = random(0,10) * 0.1;
      SquareCount = SquareCount + 1;
    }
    //ship end
  
  
    //bullet begin
    if (arduboy.pressed(B_BUTTON) && released && (bulletCount < 8)) {
      released = false;
      switch (HDG) {
        case  0:bullet[bulletCount][0] =   tankX  ;
                bullet[bulletCount][1] =    0     ;
                bullet[bulletCount][2] = tankY - 4;
                bullet[bulletCount][3] =   -4     ; break;
        case  1:bullet[bulletCount][0] = tankX + 2;
                bullet[bulletCount][1] =    1     ;
                bullet[bulletCount][2] = tankY - 4;
                bullet[bulletCount][3] =   -3.8   ; break;
        case  2:bullet[bulletCount][0] = tankX + 3;
                bullet[bulletCount][1] =    1     ;
                bullet[bulletCount][2] = tankY - 3;
                bullet[bulletCount][3] =   -3.4   ; break;
        case  3:bullet[bulletCount][0] = tankX + 3;
                bullet[bulletCount][1] =    2.8   ;
                bullet[bulletCount][2] = tankY - 3;
                bullet[bulletCount][3] =   -2.8   ; break;
        case  4:bullet[bulletCount][0] = tankX + 3;
                bullet[bulletCount][1] =    3.4   ;
                bullet[bulletCount][2] = tankY - 3;
                bullet[bulletCount][3] =   -2     ; break;
        case  5:bullet[bulletCount][0] = tankX + 4;
                bullet[bulletCount][1] =    3.8   ;
                bullet[bulletCount][2] = tankY - 2;
                bullet[bulletCount][3] =   -1     ; break;
        case  6:bullet[bulletCount][0] = tankX + 4;
                bullet[bulletCount][1] =    4     ;
                bullet[bulletCount][2] =  tankY   ;
                bullet[bulletCount][3] =    0     ; break;
        case  7:bullet[bulletCount][0] = tankX + 4;
                bullet[bulletCount][1] =    3.8   ;
                bullet[bulletCount][2] = tankY + 2;
                bullet[bulletCount][3] =    1     ; break;
        case  8:bullet[bulletCount][0] = tankX + 3;
                bullet[bulletCount][1] =    3.4   ;
                bullet[bulletCount][2] = tankY + 3;
                bullet[bulletCount][3] =    2     ; break;
        case  9:bullet[bulletCount][0] = tankX + 3;
                bullet[bulletCount][1] =    2.8   ;
                bullet[bulletCount][2] = tankY + 3;
                bullet[bulletCount][3] =    2.8   ; break;
        case 10:bullet[bulletCount][0] = tankX + 3;
                bullet[bulletCount][1] =     2    ;
                bullet[bulletCount][2] = tankY + 3;
                bullet[bulletCount][3] =    3.4   ; break;
        case 11:bullet[bulletCount][0] = tankX + 2;
                bullet[bulletCount][1] =    1     ;
                bullet[bulletCount][2] = tankY + 4;
                bullet[bulletCount][3] =    3.4   ; break;
        case 12:bullet[bulletCount][0] =   tankX  ;
                bullet[bulletCount][1] =    0     ;
                bullet[bulletCount][2] = tankY + 4;
                bullet[bulletCount][3] =    4     ; break;
        case 13:bullet[bulletCount][0] = tankX - 2;
                bullet[bulletCount][1] =   -1     ;
                bullet[bulletCount][2] = tankY + 4;
                bullet[bulletCount][3] =    3.8   ; break;
        case 14:bullet[bulletCount][0] = tankX - 3;
                bullet[bulletCount][1] =   -2     ;
                bullet[bulletCount][2] = tankY + 3;
                bullet[bulletCount][3] =    3.4   ; break;
        case 15:bullet[bulletCount][0] = tankX - 3;
                bullet[bulletCount][1] =   -2.8   ;
                bullet[bulletCount][2] = tankY + 3;
                bullet[bulletCount][3] =    2.8   ; break;
        case 16:bullet[bulletCount][0] = tankX - 3;
                bullet[bulletCount][1] =   -3.4   ;
                bullet[bulletCount][2] = tankY + 3;
                bullet[bulletCount][3] =    2     ; break;
        case 17:bullet[bulletCount][0] = tankX - 2;
                bullet[bulletCount][1] =   -3.8   ;
                bullet[bulletCount][2] = tankY + 4;
                bullet[bulletCount][3] =    1     ; break;
        case 18:bullet[bulletCount][0] = tankX - 4;
                bullet[bulletCount][1] =   -4     ;
                bullet[bulletCount][2] =  tankY   ;
                bullet[bulletCount][3] =    0     ; break;
        case 19:bullet[bulletCount][0] = tankX - 4;
                bullet[bulletCount][1] =   -3.8   ;
                bullet[bulletCount][2] = tankY - 2;
                bullet[bulletCount][3] =   -1     ; break;
        case 20:bullet[bulletCount][0] = tankX - 3;
                bullet[bulletCount][1] =   -3.4   ;
                bullet[bulletCount][2] = tankY - 3;
                bullet[bulletCount][3] =   -2     ; break;
        case 21:bullet[bulletCount][0] = tankX - 3;
                bullet[bulletCount][1] =   -2.8   ;
                bullet[bulletCount][2] = tankY - 3;
                bullet[bulletCount][3] =   -2.8   ; break;
        case 22:bullet[bulletCount][0] = tankX - 3;
                bullet[bulletCount][1] =    -2    ;
                bullet[bulletCount][2] = tankY - 3;
                bullet[bulletCount][3] =   -3.4   ; break;
        case 23:bullet[bulletCount][0] = tankX - 2;
                bullet[bulletCount][1] =   -1     ;
                bullet[bulletCount][2] = tankY - 4;
                bullet[bulletCount][3] =   -3.8   ; break;
        default:bullet[bulletCount][0] =  tankX   ;
                bullet[bulletCount][1] =    0     ;
                bullet[bulletCount][2] =  tankY   ;
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


    //begin Squares
    for (uint8_t t = 0; t < SquareCount; t++) {
      Squares[t][0] = Squares[t][0] + Squares[t][1];
      Squares[t][2] = Squares[t][2] + Squares[t][3];
      if (Squares[t][0] < -5) Squares[t][0] = 133;
      if (Squares[t][0] > 133) Squares[t][0] = -5;
      if (Squares[t][2] < -5) Squares[t][2] = 69;
      if (Squares[t][2] > 69) Squares[t][2] = -5;
      for (uint8_t b = 0; b < bulletCount; b++) {
        if (abs(bullet[b][0] - Squares[t][0]) < 5 && abs(bullet[b][2] - Squares[t][2]) < 5) {
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
          
          score = score + 2;
          for (uint8_t c = t; c < SquareCount - 1; c++)
            for (uint8_t d = 0; d < 4; d++)
              Squares[c][d] = Squares[c + 1][d];
          SquareCount = SquareCount - 1;
        }
      }
      arduboy.drawRect(Squares[t][0] - 2, Squares[t][2] + 2,4,4,1);
    }
    //end Squares


    //begin Triangles
    for (uint8_t r = 0; r < TriangleCount; r++) {
      Triangles[r][0] = Triangles[r][0] + Triangles[r][1];
      Triangles[r][2] = Triangles[r][2] + Triangles[r][3];
      if (Triangles[r][0] < -5) Triangles[r][0] = 133;
      if (Triangles[r][0] > 133) Triangles[r][0] = -5;
      if (Triangles[r][2] < -5) Triangles[r][2] = 69;
      if (Triangles[r][2] > 69) Triangles[r][2] = -5;
      for (uint8_t b = 0; b < bulletCount; b++) {
        if (abs(bullet[b][0] - Triangles[r][0]) < 4 && abs(bullet[b][2] - Triangles[r][2]) < 4) {
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

          score = score + 1;
          
          for (uint8_t c = r; c < TriangleCount - 1; c++)
            for (uint8_t d = 0; d < 4; d++)
              Triangles[c][d] = Triangles[c + 1][d];
          TriangleCount = TriangleCount - 1;
        }
      }
      arduboy.drawTriangle(Triangles[r][0] - 2, Triangles[r][2] + 1,Triangles[r][0], Triangles[r][2] - 3,Triangles[r][0] + 2, Triangles[r][2] + 1,1);
    }
    //end Triangles

    //begin Pentagons
    for (uint8_t p = 0; p < PentagonCount; p++) {
      Pentagons[p][0] = Pentagons[p][0] + Pentagons[p][1];
      Pentagons[p][2] = Pentagons[p][2] + Pentagons[p][3];
      if (Pentagons[p][0] < -5) Pentagons[p][0] = 133;
      if (Pentagons[p][0] > 133) Pentagons[p][0] = -5;
      if (Pentagons[p][2] < -5) Pentagons[p][2] = 69;
      if (Pentagons[p][2] > 69) Pentagons[p][2] = -5;
      for (uint8_t b = 0; b < bulletCount; b++) {
        if (abs(bullet[b][0] - Pentagons[p][0]) < 2 && abs(bullet[b][2] - Pentagons[p][2]) < 2) {
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

          for (uint8_t c = p; c < PentagonCount - 1; c++)
            for (uint8_t d = 0; d < 4; d++)
              Pentagons[c][d] = Pentagons[c + 1][d];
          Pentagons[PentagonCount][0] = 0;
          Pentagons[PentagonCount][1] = 0;
          Pentagons[PentagonCount][2] = 0;
          Pentagons[PentagonCount][3] = 0;
          PentagonCount = PentagonCount - 1;
        }
      }
      arduboy.drawCircle(Pentagons[p][0], Pentagons[p][2],3,3);
    }
    //end Pentagons
  
    //begin crash
    for (uint8_t a = 0; a < SquareCount; a ++) {
      if (abs(Squares[a][0] - tankX) < 3 && abs(Squares[a][2] - tankY) < 3) {
        life = life - 1;
        moveX = -moveX * 0.3;
        moveY = -moveY * 0.3;
        Squares[a][1] = -Squares[a][1] * 0.5;
        Squares[a][3] = -Squares[a][3] * 0.5;
      }
    }
    for (uint8_t a = 0; a < TriangleCount; a ++) {
      if (abs(Triangles[a][0] - tankX) < 3 && abs(Triangles[a][2] - tankY) < 3) {
        life = life - 10;
        moveX = -moveX * 0.5;
        moveY = -moveY * 0.5;
        Triangles[a][1] = -Triangles[a][1] * 0.7;
        Triangles[a][3] = -Triangles[a][3] * 0.7;
      }
    }
    for (uint8_t a = 0; a < PentagonCount; a ++) {
      if (abs(Pentagons[a][0] - tankX) < 3 && abs(Pentagons[a][2] - tankY) < 3) {
        life = life - 5;
        moveX = -moveX * 0.8;
        moveY = -moveY * 0.8;
        Pentagons[a][1] = -Pentagons[a][1] * 0.5;
        Pentagons[a][3] = -Pentagons[a][3] * 0.5;
      }
    }
    //end crash
  } else {
    arduboy.clear();
    arduboy.setCursor(10,10);
    arduboy.setTextSize(2);
    arduboy.print("Diep.io");
    if (arduboy.pressed(B_BUTTON)) {
      life = 100;
      tankX = 64;
      tankY = 32;
    }
  }
  //begin printObj
  //arduboy.setCursor(10,10);
  //arduboy.print(HDG);
  //arduboy.setCursor(30,10);
  //arduboy.print(SquareCount);
  //arduboy.setCursor(50,10);
  //arduboy.print(TriangleCount);
  //arduboy.setCursor(70,10);
  //arduboy.print(PentagonCount);
  //arduboy.setCursor(100,10);
  //arduboy.print(bulletCount);
  //arduboy.drawPixel(tankX,tankY,1);
  //end printObj
  arduboy.fillCircle(tankX,tankY,3,3);
  switch (HDG) {
    case  0: arduboy.drawTriangle(tankX    , tankY - 4, tankX + 3, tankY + 3, tankX - 3, tankY + 3, 1); break;
    
    case  1: arduboy.drawTriangle(tankX + 2, tankY - 4, tankX + 3, tankY + 3, tankX - 3, tankY + 2, 1); break;
    case  2: arduboy.drawTriangle(tankX + 3, tankY - 3, tankX + 2, tankY + 3, tankX - 3, tankY + 1, 1); break;
    case  3: arduboy.drawTriangle(tankX + 3, tankY - 3, tankX + 1, tankY + 3, tankX - 3, tankY - 1, 1); break;
    case  4: arduboy.drawTriangle(tankX + 3, tankY - 3, tankX - 1, tankY + 3, tankX - 3, tankY - 2, 1); break;
    case  5: arduboy.drawTriangle(tankX + 4, tankY - 2, tankX - 2, tankY + 3, tankX - 3, tankY - 3, 1); break;
    case  6: arduboy.drawTriangle(tankX + 4, tankY    , tankX - 3, tankY + 3, tankX - 3, tankY - 3, 1); break;
    
    case  7: arduboy.drawTriangle(tankX + 4, tankY + 2, tankX - 2, tankY - 3, tankX - 3, tankY + 3, 1); break;
    case  8: arduboy.drawTriangle(tankX + 3, tankY + 3, tankX - 1, tankY - 3, tankX - 3, tankY + 2, 1); break;
    case  9: arduboy.drawTriangle(tankX + 3, tankY + 3, tankX + 1, tankY - 3, tankX - 3, tankY + 1, 1); break;
    case 10: arduboy.drawTriangle(tankX + 3, tankY + 3, tankX + 2, tankY - 3, tankX - 3, tankY - 1, 1); break;
    case 11: arduboy.drawTriangle(tankX + 2, tankY + 4, tankX + 3, tankY - 3, tankX - 3, tankY - 2, 1); break;
    case 12: arduboy.drawTriangle(tankX    , tankY + 4, tankX + 3, tankY - 3, tankX - 3, tankY - 3, 1); break;
    
    case 13: arduboy.drawTriangle(tankX - 2, tankY + 4, tankX - 3, tankY - 3, tankX + 3, tankY - 2, 1); break;
    case 14: arduboy.drawTriangle(tankX - 3, tankY + 3, tankX - 2, tankY - 3, tankX + 3, tankY - 1, 1); break;
    case 15: arduboy.drawTriangle(tankX - 3, tankY + 3, tankX - 1, tankY - 3, tankX + 3, tankY + 1, 1); break;
    case 16: arduboy.drawTriangle(tankX - 3, tankY + 3, tankX + 1, tankY - 3, tankX + 3, tankY + 2, 1); break;
    case 17: arduboy.drawTriangle(tankX - 4, tankY + 2, tankX + 2, tankY - 3, tankX + 3, tankY + 3, 1); break;
    case 18: arduboy.drawTriangle(tankX - 4, tankY    , tankX + 3, tankY + 3, tankX + 3, tankY - 3, 1); break;

    case 19: arduboy.drawTriangle(tankX - 4, tankY - 2, tankX + 2, tankY + 3, tankX + 3, tankY - 3, 1); break;
    case 20: arduboy.drawTriangle(tankX - 3, tankY - 3, tankX + 1, tankY + 3, tankX + 3, tankY - 2, 1); break;
    case 21: arduboy.drawTriangle(tankX - 3, tankY - 3, tankX - 1, tankY + 3, tankX + 3, tankY - 1, 1); break;
    case 22: arduboy.drawTriangle(tankX - 3, tankY - 3, tankX - 2, tankY + 3, tankX + 3, tankY + 1, 1); break;
    case 23: arduboy.drawTriangle(tankX - 2, tankY - 4, tankX - 3, tankY + 3, tankX + 3, tankY + 2, 1); break;
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
