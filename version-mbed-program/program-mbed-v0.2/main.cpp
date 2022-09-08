#include "NOKIA_5110.h"
#include "max6675.h"
#include "mbed.h"
#include <SPI.h>
#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// pins definition
AnalogIn pot1(p20);              // pin in potentiometer
PwmOut pwm1(p21);                // pin out PWM
DigitalIn pb1_left(p26);         // push botton left
DigitalIn pb2_mid(p29);          // push botton
DigitalIn pb3_right(p30);        // push botton
SPI spi(p5, p6, p7);             // spi communication to temperature sensor
max6675 max_spi(spi, p18);       // max6675 pin definitions
BufferedSerial pc(USBTX, USBRX); // Serial communication

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// variables definition
float temp;
int variable = 1;
int menu;
int temp_function;
float pwm_value;
int command_pc;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// characters to write lcd
const char arrow[5] = {0x41, 0x22, 0x14, 0x08, 0x00}; // simbole: >

int read_temperature(int a) {
  char *test = (char *)malloc(20);
  char *buff = (char *)malloc(3);
  snprintf(test, 20, "test 2");
  pc.set_blocking(false);
  pwm_value = 0;

  // Init the data structures and NokiaLcd class
  LcdPins myPins;
  myPins.sce = p8;
  myPins.rst = p9;
  myPins.dc = p10;
  myPins.mosi = p11;
  myPins.miso = NC;
  myPins.sclk = p13;

  // Start the LCD
  NokiaLcd myLcd(myPins);
  myLcd.InitLcd();
  myLcd.ClearLcdMem();
  variable = 0;

  // Draw a test pattern on the LCD and stall for 15 seconds
  while (1) {
    // pwm start in 0%
    pwm1.write(pwm_value);
    
    //read temperature
    temp = max_spi.read_temp();
    snprintf(test, 20, "Temperature: %.2f\n", temp);

    //pc serial command 
    pc.read(buff, 1);

    if (buff[0] == '1') {
      pc.write(test, 20);
    } else {
      pc.write("no commands!\n", 14);
    }

    if (pb2_mid.read() == 1 && pwm_value < 1) {
      pwm_value = pwm_value + 0.25;
    }
    if (pb3_right.read() == 1 && pwm_value > 0) {
      pwm_value = pwm_value - 0.25;
    }
    myLcd.SetXY(0, 0);
    myLcd.DrawString(test);

    snprintf(test, 20, "pwm: %.2f", pwm_value);
    myLcd.SetXY(0, 3);
    myLcd.DrawString(test);
    // for(int j = 0; j < 6;j++){
    //     myLcd.SendDrawData(arrow[j]);
    // }
    pc.sync();
    wait_us(1000000);
  }
}

int main() {
  char *test = (char *)malloc(16);
  snprintf(test, 16, "test 2");
  pwm1.period(1.67f);
  // Init the data structures and NokiaLcd class
  LcdPins myPins;
  myPins.sce = p8;
  myPins.rst = p9;
  myPins.dc = p10;
  myPins.mosi = p11;
  myPins.miso = NC;
  myPins.sclk = p13;

  NokiaLcd myLcd(myPins);
  myLcd.InitLcd();
  myLcd.ClearLcdMem();

  while (1) {
    snprintf(test, 16, "Waiting press button");
    myLcd.ClearLcdMem();
    myLcd.SetXY(0, 0);
    myLcd.DrawString(test);
    while (!pb1_left) {
    }
    temp_function = read_temperature(1);
  }
}
