// Authors: Harsh Patil     - UCID: 30125049
//          Sankalp Bartwal - UCID: 30132025

#include <unistd.h>
#include <stdio.h>
#include "initGPIO.h"
#include <wiringPi.h>

unsigned int *gpio_ptr;

#define LATCH_PIN 9
#define CLOCK_PIN 11
#define DATA_PIN 10
#define GPSET0 7  // 28 / 4
#define GPCLR0 10 // 40/4
#define GPLEV0 13 // 52/4
#define INP_GPIO(g) *(gpio_ptr + ((g) / 10)) &= ~(7 << (((g) % 10) * 3))
#define OUT_GPIO(g) *(gpio_ptr + ((g) / 10)) |= (1 << (((g) % 10) * 3))

void readSNES(int *buttons);
void init_GPIO();
void writeLatch(int to_write);
void writeClock(int to_write);
int readData();
int pressedButtonId(int *buttons);

int button_pressed;

int input()
{
    // Get gpio pointer
    gpio_ptr = getGPIOPtr();
    button_pressed = -1;
    int buttons[16];

    init_GPIO();
    for(int i=0; i<16; i++) {
        buttons[i] = 0;
    }
   
    while (1)
    {
        button_pressed = -1;
        // Read the serialized buttons into 
        // the buttons array
        readSNES(buttons);
       
        button_pressed = pressedButtonId(buttons);

        // Activates only when some button is pressed
        if(button_pressed != -1){
            delayMicroseconds(200000);
        }

        
    }

    return 0;
}

// Get the ID of the pressed button
int pressedButtonId(int *buttons){
    for(int i = 0; i < 16; i++){
        if(buttons[i] == 0){
            return(i+1);
        }
    }
    return(-1);
}

// Initialize the GPIO
void init_GPIO()
{
    INP_GPIO(CLOCK_PIN);
    OUT_GPIO(CLOCK_PIN);
   
    INP_GPIO(LATCH_PIN);
    OUT_GPIO(LATCH_PIN);
   
    INP_GPIO(DATA_PIN);
}

// Write to the latch pin
void writeLatch(int to_write)
{
    if (to_write == 1)
    {
        gpio_ptr[GPSET0] = (1 << LATCH_PIN);
    }
    else if (to_write == 0)
    {
        gpio_ptr[GPCLR0] = (1 << LATCH_PIN);
    }
}

// Write to the clock pin
void writeClock(int to_write)
{
    if (to_write == 1)
    {
        gpio_ptr[GPSET0] = 1 << (CLOCK_PIN);
    }
    else if (to_write == 0)
    {
        gpio_ptr[GPCLR0] = 1 << (CLOCK_PIN);
    }
}

// Read the data from the SNES Controller
int readData()
{
    int v = (gpio_ptr[GPLEV0] >> DATA_PIN) & 1;
    return (v);
}

// Initiate the serialization by turning the 
// latch on for 12 microseconds and recieving the
// input stream from the SNES controller
void readSNES(int *buttons)
{
    writeClock(1);
    writeLatch(1);
    delayMicroseconds(12);
    writeLatch(0);
       
    int i = 1;    
    while(i<=16)
    {
        delayMicroseconds(6);
        writeClock(0);
        buttons[i-1] = readData();
        delayMicroseconds(6);
        writeClock(1);
        i++;
    }
}
