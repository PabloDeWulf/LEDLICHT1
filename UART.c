/**
  Section: Included Files
 */

#include <xc.h>
#include "mcc_generated_files/mcc.h"
#include "PI.h"
#include <stdlib.h> //for atoi and atof functions
#include <ctype.h> //for toupper command
#include "led.h"

/**
  Section: UART Module APIs
 */

static uint8_t sensorHeight;
static uint8_t setpoint; //240 = top, 0 = bottom
static float ki;
static float kp;
static int ledh;

static char command;
static int index;
static char data[8]; //"S20" of "p1.25"
static char value[7];
static int printCycle = 0;

void UART(void) {
    //Write to JAVA
//    if (printCycle > 10) {
//        sensorHeight = PI_GetSensorHeight();
//        printf("%d", sensorHeight); // data naar com poort printen
//        printf(">"); // stop teken
//        printCycle = 0;
//    }
//    printCycle++;

    //Read from PC
    if (EUSART_DataReady) {
        index = 0;
        __delay_ms(5); // wacht tot alle data ontvangen is
        while (EUSART_DataReady) {
            data[index] = EUSART_Read(); // ontvangen data lezen     
            if(data[index]=="\n"){
                break;
            }
                
            index++;
        }
        data[index] = '\0'; // \0 toevoegen voor atoi en atof functies
        command = data[0]; // eerste char = command
        command = (char) toupper(command); //converteer naar hoofdletter (voor de zekerheid)
        for (int i = 0; i <8; i++){ //value = data vanaf het 2e karakter
            value[i] = data[i+1];
        }

        switch (command) {
            case 'S': //Setpoint                            
                setpoint = (uint8_t) atoi(value); //atoi = ASCII to integer
                PI_SetSetpoint(setpoint);
                break;
            case 'P': //Proportional                           
                kp = (float) atof(value); //atof = ASCII to float
                PI_SetKp(kp);
                break;
            case 'I': //Integrate                                           
                ki = (float) atof(value);
                PI_SetKi(ki);
                break;
                
            case 'L':
                ledh= atoi(value);
                updateLedStrip(ledh);
                /*for(i=0;i<lenArray;i++){
                    EUSART_Write(array[i]);
                }*/
                EUSART_Write(ledh);
                break;
        };
    }
}

/**
 End of File
 */
