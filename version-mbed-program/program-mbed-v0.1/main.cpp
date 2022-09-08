#include "mbed.h"
#include <string>
#include <SPI.h>  
#include "NOKIA_5110.h"
#include "max6675.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// pins definition
AnalogIn anlg1(p20); // pin in potentiometer 
PwmOut pwm1(p21); // pin out PWM 
DigitalIn pb1(p26); // push botton
DigitalIn pb2(p29); // push botton
DigitalIn pb3(p30); // push botton
SPI spi(p5,p6,p7); // spi communication to temperature sensor
max6675 max_spi(spi,p18); // max6675 pin definitions
BufferedSerial pc(USBTX,USBRX); // Serial communication

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// variables definition
float temp;
int variable = 1;
int menu;
int temp_function;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// characters to write lcd 
const char arrow[5] = {0x41, 0x22, 0x14, 0x08, 0x00}; // simbole: >

int temperature (int a)
{
    char *test = (char *)malloc(20);
    char *buff = (char *)malloc(3);
    snprintf(test, 20, "test 2");
    pc.set_blocking (false);
    // Init the data structures and NokiaLcd class
    LcdPins myPins;
    myPins.sce  = p8;
    myPins.rst  = p9;
    myPins.dc   = p10;
    myPins.mosi = p11;
    myPins.miso = NC;
    myPins.sclk = p13;
    
    // Start the LCD
    NokiaLcd myLcd( myPins );
    myLcd.InitLcd();
    myLcd.ClearLcdMem();
    variable = 0;
 
    // Draw a test pattern on the LCD and stall for 15 seconds
    while (1) {
    float temp = max_spi.read_temp();
    snprintf(test, 20, "Temperature: %.2f\n", temp);
    int command_pc = pc.read(buff, 1);
    pc.write(buff, sizeof(buff));
    if (buff[0] == '1'){
        pc.write(test, 20);
    }
    else {
        pc.write("no commands!\n", 14);
    }
    myLcd.SetXY(0,0);
    myLcd.DrawString(test);
    //for(int j = 0; j < 6;j++){
    //    myLcd.SendDrawData(arrow[j]);
    //}
    pc.sync();
    wait_us( 2000000 );
    }   
}

void read_serial (float temp); 

int main ()
{
    char *test = (char *)malloc(16);
    snprintf(test, 16, "test 2");
    // Init the data structures and NokiaLcd class
    LcdPins myPins;
    myPins.sce  = p8;
    myPins.rst  = p9;
    myPins.dc   = p10;
    myPins.mosi = p11;
    myPins.miso = NC;
    myPins.sclk = p13;
    
    NokiaLcd myLcd( myPins );
    myLcd.InitLcd();
    myLcd.ClearLcdMem();
    
    while (1){
    snprintf(test, 16, "Waiting press button");
    myLcd.ClearLcdMem();
    myLcd.SetXY(0,0);
    myLcd.DrawString(test);
    while (!pb1)
        {
    }
    temp_function = temperature(1);
      
    } 
}
       