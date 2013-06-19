
// PIC10F202 Configuration Bit Settings

#include <xc.h>

// CONFIG
#pragma config WDTE = OFF       // Watchdog Timer (WDT disabled)
#pragma config CP = OFF         // Code Protect (Code protection off)
#pragma config MCLRE = OFF      // Master Clear Enable (GP3/MCLR pin fuction is digital I/O, MCLR internally tied to VDD)


unsigned char brightness, i,j,blue,green,red,mode;
unsigned char s[8] = "hrryptt";
const unsigned char redtable[6] = {255,0,0,0,255,255}; //a table of rgb values to be
const unsigned char bluetable[6] = {0,0,255,255,255,0};//interpolate for a rainbow
const unsigned char greentable[6] = {255,255,255,0,0,0};
const unsigned char imax = 5; //actually denotes to proportion of PWM frames to logical frames
const unsigned char upby = 4; //cv filter up velocity
const unsigned char downby = 2;//cv filter down velocity
#define movingtowards ((i>>4)&7) //because lower  i is a frame counter, get a few bits for the


//destiation color
//#define GPIO GPIO
//#define hue TMR0
/* */
#define temp j
#define MIDPOINT(x,y) (((x)>>1)+((y)>>1))//the mean of two values

void randomize() //this just works.
{

	s[( s[0] ) & 7] ^= s[1];             // s[s[0] & 7]
	s[0] += 1;                         //s[0]
	s[1] += s[( s[0] + 1 ) & 7];
/*
	s[( s[0] ) & 7] = s[s[0] & 7] ^ s[1];
	s[0] = s[0] + 1;
	s[1] = s[1] + s[( s[0] + 1 ) & 7];
*/

}


void altpwm()
{

	for ( j = 0; !(j == 255); j++ ) //a pwm routine. j also used elsewhere
	//We basically just increment a counter and compare each value to it.
	//if a value if higher, the corresponding channel is on
	//otherwise we turn it off. s the higher the value, the more time it takes on
	//and the brighter it looks.
	{


		if ( j >=(red)  )
		{
			GPIO &= 0b11111110;
		}
		else
		{
			GPIO |= 0b00000001;
		}

		if ( j >= (green))
		{
			GPIO &= 0b11111101;
		}
		else
		{
			GPIO |= 0b00000010;
		}

		if ( j >=blue)
		{
			GPIO &= 0b11111011;
		}
		else
		{
			GPIO |= 0b00000100;
		}
	}

}


/* */
void main()
{
	TRISGPIO = 0b00011000;		         //that one pin is for mode select.

	while ( 1 )
	{
		for ( i &=0b11110000; !((i&15)==imax); i++ )         //ignore most of the bits so we cat use them elsewhere
		{
			altpwm();
			/*
			for ( j = 0; j < 255; j++ ) {
								if ( j > brightness ) {
			GPIO &= (!255) ;
			}
			else {
			GPIO |= 255;
			}
			}
			*/

			         //take a filered version of s[0](the output from the randomizer)

			j = s[0];// + 4
			if ( brightness < j )
			{
				         //this filter just works by moving towads a goal
				brightness +=   upby;
			}
			else
			{

				if ( !( brightness < downby ) )//avoid arith overflow
				{
					brightness -=  downby;
				}
			}//////BEGINING OF COLOR UPDATE CODE///
		if (!(GPIO&0b00011000)) //if we are in candle that is not rainbow mode
		//all we need to do is set our three component colors to the filter brightness output.
		{

			red = brightness;
			green = brightness;
			blue = brightness;


		}
		else //if we are in rainbow flicker
		//the idea of the rainbow algorithm is to figure out the next color from the current color //
		//while not using an intermediate variable to hold the location along a spectrum/
		//to do this we rely on the fact that only one color changes at atime so we get which ofrgb//
		//is not equal to the destination color from a table and move it towards by 1//
		//if all colors are the right color, increment the destination register by 1 modulo 6.//
		//so instead of using a variable to hold location along a spectrum, we use 3 bits//

		{
			j = redtable[movingtowards];   //using j as a temp to only acess the array once
			if ((!(red==j)))        //if red is not the same as the destination, red must be changing
			{                       //therefore we dont need to check the other colors.
				if (red>j)          //small errors in other colors will be dealt with once redarrives
				{                   // so it does not contain lots of obvious unrecoverable states.
					red-=1;//red
				}
				else
				{
					red +=1;//red
				}

			}

				else //if rednot there yet
				{
						j = bluetable[movingtowards];
						if (!(blue==j))
					{
						if (blue>j)//bwds
						{
							blue-=1;//blue
						}
						else
						{
							blue +=1;//blue
						}

					} //else
						else
						{
								j = greentable[movingtowards];
						if (!(green==j))
							{
							if (green>j)
							{
								green-=1;//green
							}
							else
							{
								green +=1;//green
							}

							}

						else{

					i +=16;//increment movingtowards, leav loop counter alonei
					if ((((i>>4)&7)==6)){
					i&=0b00001111; //reset color registeri
					}


						}
}
				}


	}


		}
randomize(); //go to the randomizer
	}
}
