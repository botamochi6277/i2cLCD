
#include "i2cLCD.h"

int const MAX_OBS = 10; // the Max. number of obstacles
int const NUM_COLS = 16;
int const BUTTON_PIN = 2;

int playerPos[2];// player position, x-y
int obstaclePos[10][2];// obstacle position, first index: obstacleId,second index: positon
char pixels[16][2];

int numObs = 0;
unsigned long cnt = 0;
bool isCrush = false;
int margin = 0;
int maxX = 0;
i2cLCD mylcd;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  mylcd.init();
  for(int i=0;i<numObs;++i){
    if(obstaclePos[i][0] < 0){
      obstaclePos[i][0] = 0;
      obstaclePos[i][1] = 0;
    }
  }
}

void loop() {
  // gaming loop
  for(;;){
    // break gaming loop
    if(isCrush){
      break;
    }

    // increace the number of obstacle
    if(cnt%10 == 1 && numObs < MAX_OBS){
      numObs++;
    }

    // moving obstacle
    for(int i=0;i < numObs;++i){
      --obstaclePos[i][0];
    }
    margin++;

    // rebirth obstacles
    for(int i=0;i < numObs;++i){
      if(obstaclePos[i][0] < 0 && margin > 2){
        obstaclePos[i][0] = NUM_COLS-1;
        obstaclePos[i][1] = random(2); // ramdom number: 0 or 1s
        margin = 0;
      }
    }

    // input, player positon
    playerPos[0] = 1; // x-positon
    playerPos[1] = digitalRead(BUTTON_PIN);

    //  crush check
    for(int i=0;i < numObs;++i){
      // x-position check
      if(playerPos[0] == obstaclePos[i][0]){
        // y-position check
        if(playerPos[1] == obstaclePos[i][1]){
         // Crush!!
         isCrush = true;
        }
      }
    }

    //display

    // fill pixels with ` `(space)
    for(int x = 0;x<16;++x){
      for(int y = 0; y<2 ;++y){
        pixels[x][y] = ' ';
      }
    }

    for(int i=0;i < numObs;++i){
      if(obstaclePos[i][0] > -1){
        pixels[obstaclePos[i][0]][obstaclePos[i][1]] = '*';
      }
    }

    if(!isCrush){
      pixels[playerPos[0]][playerPos[1]] = '>';
    }else{
      pixels[playerPos[0]][playerPos[1]] = 'X';
    }

    //    write display

    for(int y = 0; y<2 ;++y){
      mylcd.move(y,0);
      for(int x = 0;x<16;++x){
        mylcd.writeData(pixels[x][y]);
      }
    }

    cnt++;

    Serial.print(cnt);
    Serial.print(", ");
    Serial.print(cnt%10);
    Serial.print(", ");
    Serial.println(numObs);
    delay(500);
  }

  // Game over
  delay(3000);
  mylcd.fill(' ');
  mylcd.move(0,0);
  char msg[] = "** GAME  OVER **";
  for(int i=0;i<16;i++){
    mylcd.writeData(msg[i]);
  }
  sprintf(msg, "** score %04d **", cnt);
  mylcd.move(1,0);
  for(int i=0;i<16;i++){
    mylcd.writeData(msg[i]);
  }

  // reset
  isCrush = false;
  cnt = 0;
  numObs = 0;

  for(int i=0;i < numObs;++i){
    obstaclePos[i][0] = 0;
    obstaclePos[i][1] = 0;
  }
  while(digitalRead(BUTTON_PIN)){
    delay(30);
  }

}
