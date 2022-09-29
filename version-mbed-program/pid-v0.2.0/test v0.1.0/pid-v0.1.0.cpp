#include "NOKIA_5110.h"
#include "PID.h"
#include "max6675.h"
#include "mbed.h"
#include <SPI.h>
#include <string>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// pins definition
AnalogIn pot1(p20); // pin in potentiometer
PwmOut pwm1(p21);   // pin out PWM AC
PwmOut pwm2(p22);   // pin out PWM paramter
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led4(LED4);
PwmOut led3(LED3);
InterruptIn pwm2_out(p23);       // pin in PWM paramter
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
const float pwm_period = 1/0.6;
float PID_value;
float PID_p;                          // proportional
float PID_i;                          // integral
float PID_d;                          // derivated
PID controller(40.0, 50.0, 0.4, pwm_period); // variables PID controller
float set_pid;
int change_info;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// characters to write lcd
const char arrow[5] = {0x41, 0x22, 0x14, 0x08, 0x00}; // simbole: >
void auto_control1(void);
void auto_control(void);

void extracted() { return; }

int read_temperature(int a) {
  char *test = (char *)malloc(20);
  char *msg_pc = (char *)calloc(13, 1);
  char *buff = (char *)malloc(3);
  snprintf(test, 20, "test 2");
  pc.set_blocking(false);

  // PID set paramters
  set_pid = 230;
  controller.setInputLimits(25.0, 235.0);
  controller.setOutputLimits(0.00, 1.00);
  controller.setMode(1);
  controller.setSetPoint(230.0);
  controller.setTunings(40.0, 50.0, 0.4);
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
  led2 = 0; 

  //pc.write("To view Temperature - Press key: 1", 36);

  // Interrupt to  pwm adjuste
  pwm2_out.fall(auto_control);

  // Draw a test pattern on the LCD and stall for 15 seconds
  while (1) {
    // Read mensage of pc serial command
    pc.read(buff, 1);

    // Print temperature
    snprintf(test, 20, "\nTemp.: %.2f\n", temp_n);

    if ((temp_d <= 5) && (temp_n != 0)) {
      snprintf(test, 20, "Temp.: %.2f", temp_n);
      myLcd.SetXY(0, 0);
      myLcd.DrawString(test);
      snprintf(test, 20, "PWM: %.2f", pwm_value);
      myLcd.SetXY(0, 3);
      myLcd.DrawString(test);
    }

    if (change_info == 1) {
      led2 =  1; 
      temp_n = max_spi.read_temp();
      temp_d = temp_n - temp_o;
      if (temp_d < 0) {
        temp_d = -temp_d;
      }
      if ((temp_d <= 5) && (temp_n != 0)) {
        controller.setProcessValue(temp_n);
        pwm_value = controller.compute();
        pwm1.write(pwm_value);
        if (buff[0] == '5') {
          snprintf(msg_pc, 13, "%03.2f;%01.2f", temp_n, pwm_value);
          wait_us(10000);
          pc.write(msg_pc, 13);

        }

        if (buff[0] == '1') {
            snprintf(msg_pc, 13, "\nTemp.: %03.2f", temp_n);
            pc.write(msg_pc, 13);
            wait_us(10000);
            snprintf(msg_pc, 12, "\nPWM: %01.2f", pwm_value);
            pc.write(msg_pc, 12);
            wait_us(10000);
      }
      led2 =  0; 
      }
      temp_o = temp_n;
      change_info = 0;
    }
  pc.sync();
  }
  // for(int j = 0; j < 6;j++){
  //     myLcd.SendDrawData(arrow[j]);
  // }
  // pc.write ("e\n", 3);
}

void auto_control(void) {
  change_info = 1;
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
  pwm1.period(pwm_period);
  pwm2.period(pwm_period);
  pwm2.write(0.75);
  led3.period(1.67f);
  led3.write(0.75);

  pwm_value = 0;
  temp_n = 0;
  temp_o = 0;
  change_info = 0;

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