#ifndef i2cLCD_h
#define i2cLCD_h

#include <Wire.h>

#define LCD_ADRS 0x3E

/**
 * @brief LCD display class controled with I2C communication
 */
class i2cLCD {
private:
  unsigned char voltage_;

public:
  i2cLCD() { voltage_ = 5; };
  i2cLCD(unsigned char voltage) { voltage_ = voltage; };
  /**
   * @brief show a character in a LCD
   * @param t_data displayed data
   */
  void writeData(unsigned char t_data) {
    Wire.beginTransmission(LCD_ADRS);
    Wire.write(0x40);
    Wire.write(t_data);
    Wire.endTransmission();
    delay(1);
  }

  /**
   * @brief send command id to a microchip on LCD
   * @param t_command sent command
   */
  void writeCommand(unsigned char t_command) {
    Wire.beginTransmission(LCD_ADRS);
    Wire.write(0x00);
    Wire.write(t_command);
    Wire.endTransmission();
    delay(10);
  }

  /**
   * @brief initialize display
   */
  void init() {
    Wire.begin();
    delay(100);
    writeCommand(0x38);
    delay(20);
    writeCommand(0x39);
    delay(20);
    writeCommand(0x14);
    delay(20);
    writeCommand(0x73);
    delay(20);
    if (voltage_ == 5) {
      // 0x50 + [Icon,Bon,C5,C4]
      writeCommand(0x52); // 5V動作用
    } else {
      writeCommand(0x56); // 3V動作用
    }
    delay(20);
    writeCommand(0x6C);
    delay(20);
    writeCommand(0x38);
    delay(20);
    clear();
    writeCommand(0x0C);
    delay(20);
  }

  /**
   * @brief clear display
   */
  void clear() {
    writeCommand(0x01);
    delay(20);
  }

  /**
   * @brief move a cursor
   * @param row 行
   * @param col 列
   */
  void move(unsigned char row, unsigned char col) {
    if (row == 0) {
      writeCommand(col + 0x80);
    } else if (row == 1) {
      writeCommand(col + 0x40 + 0x80);
    }
    delay(20);
  }
};

#endif
