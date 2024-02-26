#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <xmc_gpio.h>

#define DOT_LENGTH  100
#define DASH_LENGTH  300
#define SPACE_BETW_TWO_SIG_OF_LETTER  100
#define SPACE_BETW_TWO_LETTERS  300
#define SPACE_BETW_TWO_WORDS  700
#define PAUSE_FIVE_SECONDS 5000


int volatile hwCounter = 0;
int volatile sendetGerade = 1;
char *morseStringFinal;

//GPIO Pin für LED konfigurieren
const XMC_GPIO_CONFIG_t configLED = \
        {.mode=XMC_GPIO_MODE_OUTPUT_PUSH_PULL,\
         .output_level=XMC_GPIO_OUTPUT_LEVEL_LOW,\
         .output_strength=XMC_GPIO_OUTPUT_STRENGTH_STRONG_SHARP_EDGE};


void MCUInit(){ //Configure & initialize MCU
  SystemInit();
	SysTick_Config(SystemCoreClock/1000); //SysTick 1ms konfigurieren
}

void SysTick_Handler(void){
  hwCounter++; //inkrementiert den Hardware hwCounter
}

//__________________DEFINING WAITING FUNCTIONS______________________________

void dot(void){
	//toggle LED on
	XMC_GPIO_ToggleOutput(XMC_GPIO_PORT1, 0);
	//wait 100ms
 	while(sendetGerade){
		if (hwCounter == DOT_LENGTH){
			//toggle LED off
 			XMC_GPIO_ToggleOutput(XMC_GPIO_PORT1, 0);
 			hwCounter = 0; //counter zurücksetzen
			sendetGerade = 0; //stop while loop
		}
	}
	sendetGerade = 1; //zurücksetzen
}

void dash(void){
//toggle LED on
 XMC_GPIO_ToggleOutput(XMC_GPIO_PORT1, 0);
 //wait 300ms
	while(sendetGerade){
		if (hwCounter == DASH_LENGTH){
			XMC_GPIO_ToggleOutput(XMC_GPIO_PORT1, 0);
 			hwCounter = 0;
			sendetGerade = 0;
		}
	}
	sendetGerade = 1; //zurücksetzen
}

void betwSigOfLetterWait(void){
  //waiting between the signals forming the same letter (equal to 1 dot)
 	while(sendetGerade){
		if (hwCounter == SPACE_BETW_TWO_SIG_OF_LETTER){
 			hwCounter = 0;
			sendetGerade = 0;
		}
	}
	sendetGerade = 1; //zurücksetzen
}

void betwTwoLetters(void){
 	while(sendetGerade){
		if (hwCounter == SPACE_BETW_TWO_LETTERS){
 			hwCounter = 0;
			sendetGerade = 0;
		}
	}
	sendetGerade = 1; //zurücksetzen
}

void betweenWord(void){
 	while(sendetGerade){
		if (hwCounter == SPACE_BETW_TWO_WORDS){
 			hwCounter = 0;
			sendetGerade = 0;
		}
	}
	sendetGerade = 1; //zurücksetzen
}

void pauseFiveSeconds(void){
 	while(sendetGerade){
		if (hwCounter == PAUSE_FIVE_SECONDS){
 			hwCounter = 0;
			sendetGerade = 0;
		}
	}
	sendetGerade = 1; //zurücksetzen
}

//___________________________________________________________________________


//__________________________DEFINING LIBRARY__________________________________

char lowerCase(char upperCaseChar){
  //transforms upper-case into lowerCase
  char lowerCaseChar = upperCaseChar;

  switch(upperCaseChar){
    case 'A': lowerCaseChar = 'a' ; break;
    case 'B': lowerCaseChar = 'b' ; break;
    case 'C': lowerCaseChar = 'c' ; break;
    case 'D': lowerCaseChar = 'd' ; break;
    case 'E': lowerCaseChar = 'e' ; break;
    case 'F': lowerCaseChar = 'f' ; break;
    case 'G': lowerCaseChar = 'g' ; break;
    case 'H': lowerCaseChar = 'h' ; break;
    case 'I': lowerCaseChar = 'i' ; break;
    case 'J': lowerCaseChar = 'j' ; break;
    case 'K': lowerCaseChar = 'k' ; break;
    case 'L': lowerCaseChar = 'l' ; break;
    case 'M': lowerCaseChar = 'm' ; break;
    case 'N': lowerCaseChar = 'n' ; break;
    case 'O': lowerCaseChar = 'o' ; break;
    case 'P': lowerCaseChar = 'p' ; break;
    case 'Q': lowerCaseChar = 'q' ; break;
    case 'R': lowerCaseChar = 'r' ; break;
    case 'S': lowerCaseChar = 's' ; break;
    case 'T': lowerCaseChar = 't' ; break;
    case 'U': lowerCaseChar = 'u' ; break;
    case 'V': lowerCaseChar = 'v' ; break;
    case 'W': lowerCaseChar = 'w' ; break;
    case 'X': lowerCaseChar = 'x' ; break;
    case 'Y': lowerCaseChar = 'y' ; break;
    case 'Z': lowerCaseChar = 'z' ; break;
    default: break;

  }

  return lowerCaseChar;
}


char* identifyLetter(char charToBeTranslated){
  //This method translates the given char into morse-string
  //Upper-case will be transformed into lower-case

  char charToBeTranslated_LowerCase = lowerCase(charToBeTranslated);

  char *result;

  switch(charToBeTranslated_LowerCase){
    case 'a': result = ".-N"; break;
    case 'b': result = "-...N"; break;
    case 'c': result = "-.-.N"; break;
    case 'd': result = "-..N"; break;
    case 'e': result = ".N"; break;
    case 'f': result = "..-.N"; break;
    case 'g': result = "--.N"; break;
    case 'h': result = "....N"; break;
    case 'i': result = "..N"; break;
    case 'j': result = ".---N"; break;
    case 'k': result = "-.-N"; break;
    case 'l': result = ".-..N"; break;
    case 'm': result = "--N"; break;
    case 'n': result = "-.N"; break;
    case 'o': result = "---N"; break;
    case 'p': result = ".--.N"; break;
    case 'q': result = "--.-N"; break;
    case 'r': result = ".-.N"; break;
    case 's': result = "...N"; break;
    case 't': result = "-N"; break;
    case 'u': result = "..-N"; break;
    case 'v': result = "...-N"; break;
    case 'w': result = ".--N"; break;
    case 'x': result = "-..-N"; break;
    case 'y': result = "-.--N"; break;
    case 'z': result = "--..N"; break;
    case ' ': result = "_"; break;
    default: break;

  }

  return result;
}

void executeMorse(char *stringWithMorsecode){ //Pointer übergeben vom Morsecode-String ("1221231121")

  int k;

  //loop through stringWithMorsecode
  //for each char (digit) perform the LED-representation

  for(k = 0; k < strlen(stringWithMorsecode)-1; k++){

    if(stringWithMorsecode[k] == '.'){
      dot();
      if( stringWithMorsecode[k+1] != 'N' ) {
        betwSigOfLetterWait();
      }


    } else if(stringWithMorsecode[k] == 'N'){
        if(stringWithMorsecode[k+1] != ' '){
          betwTwoLetters();
        }


    } else if(stringWithMorsecode[k] == '-'){
      dash();
      if(stringWithMorsecode[k+1] != 'N'){
        betwSigOfLetterWait();
      }

    } else if (stringWithMorsecode[k] == '_'){
      betweenWord();

    } else{ //if '\0' occurs (end of string)
      return; //end the execution of this function (because string is over)
    }

  }
}


/*
* This function takes a pointer of the word ("I CAN MORSE" - string) as input
* It returns a String (Char-Array) which contains the word in Morsecode (".-...- .-..")
* whereas: . = dot; - = dash; _ = wait between word
*
*/
char* convertStringIntoMorsecode(char *stringToConvert){ //String-Pointer übergeben vom Wort-String

  //init String which will be returned
  char resultString[200];
  int i;
  char actualChar;
  char *thisStringReplacesTheActualCharWithMorseChars;
  int copyIndex = 0;

  //Loop through String (char-Array)
  for(i = 0; i < strlen(stringToConvert)-1; i++){

    //aux var
    int j = 0;

    //For Each char give a translation string with dots and dashes
    actualChar = stringToConvert[i];

    //char-wise translation
    thisStringReplacesTheActualCharWithMorseChars = identifyLetter(actualChar); //address / pointer ?

    //now loop through thisStringReplacesTheActualChar string and put elements of it into stringToConvert

    for(j = 0; j < strlen(thisStringReplacesTheActualCharWithMorseChars)-1; j++){
      resultString[copyIndex+j] = thisStringReplacesTheActualCharWithMorseChars[j]; //put morse-chars into the result string
    }

    copyIndex = copyIndex + j;


  }

  morseStringFinal = resultString;

  return resultString; //gibt man so Strings zurück?
}

//___________________________________________________________________________


int main(void)
{
  XMC_GPIO_Init(XMC_GPIO_PORT1, 0, &configLED); //LED-config übergeben
	MCUInit();

  char toBeMorsed[] = "I CAN MORSE"; //definition of the string to send out

	while(1){

    //just call functions
    convertStringIntoMorsecode(toBeMorsed);
    executeMorse(morseStringFinal);
    pauseFiveSeconds();

  }

	return 0;
}
