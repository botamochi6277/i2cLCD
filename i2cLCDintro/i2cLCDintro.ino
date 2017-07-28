/**
* @file i2cLCD.ino
*/
// http://s2jp.com/2015/03/mini-char-lcd/
#include "i2cLCD.h"
//SCL=A5, SDA=A4
char moji[]="AQM1602XA-RN-GBW";

i2cLCD mylcd;

void setup()
{
  mylcd.init();
}

void loop()
{
  for(int i=0;i<16;i++)
  {
    mylcd.writeData(moji[i]);
  }
  mylcd.move(1,0);
  for(int i=0;i<16;i++)
  {
    mylcd.writeData(i+0xB1);
  }
  delay(3000); // wait for 3 seconds

  mylcd.clear();
  for(int j = 0;j<2;++j){
    for(int i=0;i<16;++i){
      mylcd.clear();
      mylcd.move(j,i);
      mylcd.writeData('*');
      delay(200);
    }
  }
  mylcd.clear();

}
