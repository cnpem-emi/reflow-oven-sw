#include "mbed.h"

// main() runs in its own thread in the OS
AnalogIn anlg1(p20);
PwmOut pwm1(p21);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

int main() { 
    pwm1.period(1.67f);
    while (1) {
        pwm1.pulsewidth(anlg1);
    
        led1 = (anlg1 > 0.2f) ? 1 : 0;
        led2 = (anlg1 > 0.4f) ? 1 : 0;
        led3 = (anlg1 > 0.6f) ? 1 : 0;
        led4 = (anlg1 > 0.8f) ? 1 : 0;    
        }
}