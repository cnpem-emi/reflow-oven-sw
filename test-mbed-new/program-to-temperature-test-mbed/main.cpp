#include "mbed.h"
#include "max6675.h"
 
SPI spi(p5,p6,p7);
max6675 max(spi,p18);
 
Serial pc(USBTX,USBRX);
 
int main() {
    pc.baud(9600);
    pc.printf("Starting...\n\r"); 
    while (1) {
        float temp = max.read_temp();
        char c = pc.getc();
        if (c == '1') {  
            pc.printf("%0.2f", temp);
        }
        wait_us(250000);
    }
}