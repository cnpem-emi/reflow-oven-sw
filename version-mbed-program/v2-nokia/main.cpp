#include "mbed.h"
#include <string>
#include <SPI.h> 
#include <Adafruit_GFX.h>  
#include "NOKIA_5110.h"
#include "max6675.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// pin definition 
DigitalIn pb1(p14); //push botton
DigitalIn pb2(p29);
DigitalIn pb3(p30);
SPI spi(p5,p6,p7);
max6675 max_spi(spi,p18);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float temp;
const char arrow[5] = {0x41, 0x22, 0x14, 0x08, 0x00};
int variable = 1;
int menu;

int temp_function;

int temperature (int a)
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
    
    // Start the LCD
    NokiaLcd myLcd( myPins );
    myLcd.InitLcd();
    myLcd.ClearLcdMem();
    variable = 0;
 
    // Draw a test pattern on the LCD and stall for 15 seconds
    while (1) {
    float temp = max_spi.read_temp();
    snprintf(test, 16, "test %.2f", temp);
    myLcd.SetXY(0,0);
    myLcd.DrawString(test);
    //for(int j = 0; j < 6;j++){
    //    myLcd.SendDrawData(arrow[j]);
    //}
    wait_us( 2000000 );
    }   
}

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
       