#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <xmc_gpio.h>


int volatile hwCounter = 0;
bool volatile notsend = true;

//GPIO Pin für LED konfigurieren
const XMC_GPIO_CONFIG_t configLED = \
        {.mode=XMC_GPIO_MODE_OUTPUT_PUSH_PULL,\
         .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,\
         .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};


void MCUInit(){ //Configure & initialize MCU
  SystemInit();
	SysTick_Config(SystemCoreClock/100); //SysTick 10ms
	// weitere Initialisierungen durchführen
}

void SysTick_Handler(void){hwCounter++;}

void dot(void){
	//toggle LED on
	XMC_GPIO_ToggleOutput(XMC_GPIO_PORT1, 0);
	//wait 100ms
 	while(notsend){
		if (counter == 10){
			//toggle LED off
 			XMC_GPIO_ToggleOutput(XMC_GPIO_PORT1, 0);
 			counter = 0;
			notsend = false;
		}
	}
	notsend = true;
}

void wait(void){
 	while(notsend){
		if (counter == 20){
 			counter = 0;
			notsend = false;
		}
	}
	notsend = true;

}

void stripe(void){
 XMC_GPIO_ToggleOutput(XMC_GPIO_PORT1, 0);
	while(notsend){
		if (counter == 80){
			XMC_GPIO_ToggleOutput(XMC_GPIO_PORT1, 0);
 			counter = 0;
			notsend = false;
		}
	}
	notsend = true;

}

void wait5(void){
 	while(notsend){
		if (counter == 300){
 			counter = 0;
			notsend = false;
		}
	}
	notsend = true;
}




int main(void)
{
  XMC_GPIO_Init(XMC_GPIO_PORT1, 0, &LED_config); //LED-config übergeben
	MCUInit();

  char morseString[] = "I CAN MORSE"; //definition of the string to send out

	while(1){

    //h
    dot();
    wait();
    dot();
    wait();
    dot();
    wait();
    dot();

    //pause
    wait();
    wait();
    wait();

    //e
    dot();

    wait();
    wait();
    wait();

    //y
    stripe();
    wait();
    dot();
    wait();
    stripe();
    wait();
    stripe();

    //wait 3 sec
    wait5();


  }
 		//send "hey" with LED

	return 0;
}
