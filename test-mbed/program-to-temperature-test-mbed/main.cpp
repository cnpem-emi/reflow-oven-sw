#include "mbed.h"
#include "max6675.h"
 
SPI spi(p5,p6,p7);
max6675 max(spi,p18);
 
Serial pc(USBTX,USBRX);
float temp_n;
float temp_o; 
 
int main() {
    pc.baud(9600);
    pc.printf("Starting...\n\r"); 
    while (1) {
        temp_n = max.read_temp();
        char c = pc.getc();
        float temp_d = temp_n - temp_o;
        if (temp_d < 0) {
            temp_d = -temp_d;
        }  
        if ((c == '1') && (temp_d <= 5) && (temp_n != 0)) {  
            pc.printf("%0.2f", temp_n);
        }
        temp_o = temp_n;
        wait_us(250000);
    }
}