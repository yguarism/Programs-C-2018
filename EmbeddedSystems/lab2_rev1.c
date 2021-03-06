#include "gpiolib_addr.h"
#include "gpiolib_reg.h"
#include "gpiolib_reg.c"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main(void)
{
	//Initialize the GPIO pins and check if initialize correctly
	GPIO_Handle gpio;
	gpio = gpiolib_init_gpio();
	if(gpio == NULL)
	{
		perror("Could not initialize GPIO");
	}
	

	//Set pin 17 as an output pin - Light 1
	uint32_t sel_reg = gpiolib_read_reg(gpio, GPFSEL(1));
	sel_reg |= 1  << 21;
	gpiolib_write_reg(gpio, GPFSEL(1), sel_reg);
	
	//Set pin 22 as an output pin - Light 2
	uint32_t sel_reg1 = gpiolib_read_reg(gpio, GPFSEL(2));
	sel_reg1 |= 1  << 6;
	gpiolib_write_reg(gpio, GPFSEL(2), sel_reg1);
	
	// initialize switch state array
	int sz = 10000;	
	int switchState [sz];
	
	while (1){
	// Turn -Light 1 On
	gpiolib_write_reg(gpio, GPSET(0), 1 << 17);
	
	
	//once pin_state is false (button is pressed) --> Turn off light 1
	int buttonp = 1;	
	
	while (buttonp){		
	/*Declare a variable called level_reg of 32-bit size	and assigned the value of the level register*/ 	
	uint32_t level_reg = gpiolib_read_reg(gpio, GPLEV(0));

	/* Declare a variable called pin_state and assign the value of bit 4 from the level_reg*/ 
				
	int pin_state = level_reg & (1 << 4);
	if(!pin_state) //pinstate is false when button is pushed
		{
			buttonp = 0;
		}

	}
	gpiolib_write_reg(gpio, GPCLR(0), 1 << 17);

	usleep(500000);
	int i = 0;
		
	while (i<sz){
		usleep (1000);//usleep for 1000 ms = 0.001s * 10000 = 10s total
		
		uint32_t level_reg = gpiolib_read_reg(gpio, GPLEV(0));

		int pin_state = level_reg & (1 << 4);
		
			if(!pin_state){			
				switchState[i] = 1;
				i++;
			}
			else {
				switchState[i] = 0;
				i++;
			}
		}

		/*for(int j = 0; j < sz; j++)
			printf("%d", switchState[j]);*/ //for testing purposes

		// I NEED LIGHT TO TURN ON for the number of 1's in ARRAY
		int j = 0;		
		while (j < sz-3){
			if (switchState[j] == 1 && switchState[j+1] ==1 && switchState[j+2] ==1 && switchState[j+3] ==0){
				
				gpiolib_write_reg(gpio, GPSET(0), 1 << 22);
				usleep(1000000);

				gpiolib_write_reg(gpio, GPCLR(0), 1 << 22);

				usleep(1000000);
				
			}

			j++;
		}
		
		usleep(5000000);
	
	}

}

