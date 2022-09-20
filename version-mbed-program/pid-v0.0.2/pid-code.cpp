#include "NOKIA_5110.h"
#include "PID.h"
#include "max6675.h"
#include "mbed.h"
#include <SPI.h>
#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// pins definition
AnalogIn pot1(p20);              // pin in potentiometer
PwmOut pwm1(p21);        // pin out PWM AC
PwmOut pwm2(p22);        // pin out PWM paramter
DigitalIn pwm2_out(p23);    // pin in PWM paramter
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
float temp_n;     // new acquisition temperature
float temp_o;     // old aquisition temperature
float temp_d;     // temperature variation
float temp_error; // variable to PID
float temp_set;   // set temp variable to PID

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// controller variables
float PID_value;
float PID_p;                          // proportional
float PID_i;                          // integral
float PID_d;                          // derivated
PID controller(0.4, 550.0, 8.0, 0.8); // variables PID controller
float set_pid;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// characters to write lcd
const char arrow[5] = {0x41, 0x22, 0x14, 0x08, 0x00}; // simbole: >

// Chose set point paramters
void Set_pid() {
  if (temp_n >= 150) {
    set_pid = 190;
    controller.setSetPoint(set_pid);
  }
  if (temp_n >= 190) {
    set_pid = 230;
    controller.setSetPoint(set_pid);
  } else
    return;
}

int read_temperature(int a) {
  char *test = (char *)malloc(20);
  char *msg_pc = (char *)calloc(12, 1);
  char *buff = (char *)malloc(3);
  snprintf(test, 20, "test 2");
  pc.set_blocking(false);
  pwm_value = 0;

  // PID set paramters
  set_pid = 230;
  controller.setInputLimits(0.0, 360.0);
  controller.setOutputLimits(0.0, 1.0);
  controller.setMode(1);
  controller.setSetPoint(set_pid);
  controller.setTunings(15, 0, 0);

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
    // read temperature
    temp_n = max_spi.read_temp();
    temp_d = temp_n - temp_o;
    controller.setProcessValue(temp_n);

    if (temp_d < 0) {
      temp_d = -temp_d;
    }
    snprintf(msg_pc, 12, "%03.2f;%01.2f", temp_n, pwm_value);

    // pc serial command
    pc.read(buff, 1);

    if ((temp_d <= 5) && (temp_n != 0)) {
      if (buff[0] == '1') {
        pc.write(msg_pc, 20);
      }
    } else {
      pc.write("no commands!\n", 14);
    }

    pwm_value = controller.compute();
    pwm1.write(pwm_value);

    snprintf(test, 20, "Temp.: %.2f\n", temp_n);

    myLcd.SetXY(0, 0);
    myLcd.DrawString(test);

    snprintf(test, 20, "pwm: %.2f", pwm_value);
    myLcd.SetXY(0, 3);
    myLcd.DrawString(test);
    // for(int j = 0; j < 6;j++){
    //     myLcd.SendDrawData(arrow[j]);
    // }
    temp_o = temp_n;
    pc.sync();
    wait_us(500000);
  }
}

int main() {
  char *test = (char *)malloc(16);
  snprintf(test, 16, "test 2");
  
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

  // pwm start in 0%
  pwm1.write(0);
  pwm1.period(1.67f);
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
