#include "mbed.h"
#include <string>
#include <SPI.h> 
#include <Adafruit_GFX.h>  
#include <Adafruit_PCD8544.h> 
#include <SPI.h>
#include "NOKIA_5110.h"

// main() runs in its own thread in the OS
float variable; 
DigitalIn pb(p14); //push botton

const char arrow[5] = {0x41, 0x22, 0x14, 0x08, 0x00};

int main()
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
    
    // Start the LCD
    myLcd.InitLcd();
    variable = 0;
 
    // Draw a test pattern on the LCD and stall for 15 seconds
    myLcd.DrawString("my friend");
    for(int i = 0; i < 10; i++) {
    snprintf(test, 16, "test %d", i);
    myLcd.SetXY(0,0);
    myLcd.DrawString(test);
    for(int j = 0; j < 6;j++){
        myLcd.SendDrawData(arrow[j]);
    }
    wait_us( 2000000 );
    }
    
    // Turn off the LCD and enter an endless loop
    while (1){
    }
}
       