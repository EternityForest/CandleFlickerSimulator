/*INSTRUCTIONS:Connect an LED and resistor betewwn any GPIO pin except GP3 and ground.*/


#include <xc.h>

// CONFIG
#pragma config WDTE = OFF       // Watchdog Timer (WDT disabled)
#pragma config CP = OFF         // Code Protect (Code protection off)
#pragma config MCLRE = OFF      // Master Clear Enable (GP3/MCLR pin fuction is digital I/O, MCLR internally tied to VDD)

#define WIND_VARIABILITY 5
#define FLAME_AGILITY 2
#define WIND_CALMNESS_CONSTANT 2
#define WIND_BASELINE 30

unsigned char flame, flameprime, i, wind, x, a, b, c;

unsigned char random() {
    x++; //x is incremented every round and is not affected by any other variable
    a = (a ^ c ^ x); //note the mix of addition and XOR
    b = (b + a); //And the use of very few instructions
    c = ((c + (b >> 1) ^ a)); // the AES S-Box Operation ensures an even distributon of entropy
    return (c);
}

void main() {
    TRISGPIO = 0; //All pins are outputs

    //The PWM loop
    while (1) {
        for (i = 0; i < 255; i++) {
            if (i < flameprime) {
                GPIO = 255;
            } else {
                GPIO = 0;
            }
        }



        //We simulate a gust of wind by setting the wind var to a random value
        if (random() < WIND_VARIABILITY) {
            //Make a gust of wind less likely with two random teata because 255 is not enough resolution
            if(random() > 220)
            {
                 wind = random();
            }
        }

        //The wind constantly settles towards its baseline value
        if (wind>WIND_BASELINE) {
            wind--;
        }

        //The flame constantly gets brighter till the wind knocks it down
        if (flame<255) {
            flame++;
        }

        //Depending on the wind strength and the calmnes modifer we calcuate the odds
        //of the wind knocking down the flame by setting it to random values
        if (random() < (wind>>WIND_CALMNESS_CONSTANT)) {
            flame = random();
        }

        //Real flames ook like they have inertia so we use this constant-aproach-rate filter
        //To lowpass the flame height
        if (flame > flameprime) {
            if (flameprime < (255 - FLAME_AGILITY)) {
                flameprime += FLAME_AGILITY;
            }
        } else {
            if (flameprime > (FLAME_AGILITY)) {
                flameprime -= FLAME_AGILITY;
            }
        }
        //How do we prevent jittering when the two are equal?
        //We don't. It adds to the realism.
    }

}
