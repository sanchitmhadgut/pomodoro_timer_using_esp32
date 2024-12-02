/*
 * button_scan.c
 *
 *  Created on: Oct 2, 2024
 *      Author: Admin
 */
#include "button_scan.h"
#include <stdint.h>


BTN_struct  BTN_Key[4];

uint8_t  Button_Scan(void)
{
	uint8_t  btnStatus;
    uint8_t  i;
    //This holds the array to the PORTB address and pin
    for(i=0;i<4;i++)
    {
        BTN_Key[i].Flag.keychange=0;
        BTN_Key[i].Flag.keypress=BTN_Key[i].Flag.dbnckeypress;

		switch (i) {
		case 0:
			if (digitalRead(buttonUp)){ //K// S7// up..

				BTN_Key[i].Flag.rawkey = 0; //Button is not pressed

			}
			else {

				BTN_Key[i].Flag.rawkey = 1; //Button is pressed

			}


			break;
		case 1:
			if (digitalRead(buttonDwn)) {//K// S7// down.. 
				BTN_Key[i].Flag.rawkey = 0; //Button is not pressed

			}
			else{
				BTN_Key[i].Flag.rawkey = 1; //Button is pressed

			}

			break;

		case 2:
			if (digitalRead(buttonBck)){ //K// S7// bck..
				BTN_Key[i].Flag.rawkey = 0; //Button is not pressed

			}
			else
			{
				BTN_Key[i].Flag.rawkey = 1; //Button is pressed

			}
			break;

		case 3:
			if (digitalRead(buttonEntr)) {//K// S7// entr.. 
				BTN_Key[i].Flag.rawkey = 0; //Button is not pressed

			}
			else{
				BTN_Key[i].Flag.rawkey = 1; //Button is pressed


			}
			break;
		}



        if(BTN_Key[i].Flag.rawkey == BTN_Key[i].Flag.dbnckeypress) //check for the previous state
        {
            if(!BTN_Key[i].Flag.dbnckeypress )
            {
                BTN_Key[i].Flag.dbnctime =8;//Debounce at 100msec for key press
                BTN_Key[i].clicktime=0;//Reset the button press time
            }
            else
            {
                BTN_Key[i].clicktime++;
                BTN_Key[i].Flag.dbnctime=15;//Debounce at 150msec for key release
            }
        }
        else
        {
            if(--BTN_Key[i].Flag.dbnctime==0)
            {
                BTN_Key[i].Flag.dbnckeypress=BTN_Key[i].Flag.rawkey;
                BTN_Key[i].Flag.keychange=1;
                BTN_Key[i].Flag.keypress=BTN_Key[i].Flag.dbnckeypress;
                if(!BTN_Key[i].Flag.dbnckeypress )
                      BTN_Key[i].Flag.dbnctime=2;//Debounce at 100msec for key press
                else
                     BTN_Key[i].Flag.dbnctime=3;//Debounce at 150msec for key release
            }
        }
    }
		btnStatus = NOKEY_PRESS;
		//printf("hii1 = %bd\n",btn_sts);
		if(BTN_Key[K1].Flag.dbnckeypress && BTN_Key[K1].Flag.keychange)
		{
			if(BTN_Key[K1].clicktime > LONG_PRESS_TIME)
				btnStatus |=K1_LONGPRESS;
			else
				btnStatus |=K1_PRESS;
		}
		if(BTN_Key[K2].Flag.dbnckeypress && BTN_Key[K2].Flag.keychange)
		{
			if(BTN_Key[K2].clicktime > LONG_PRESS_TIME)
				btnStatus |=K2_LONGPRESS;
			else
				btnStatus |=K2_PRESS;
		}
		if(BTN_Key[K3].Flag.dbnckeypress && BTN_Key[K3].Flag.keychange)
		{
			if(BTN_Key[K3].clicktime > LONG_PRESS_TIME)
				btnStatus |=K3_LONGPRESS;
			else
				btnStatus |=K3_PRESS;
		}
		if(BTN_Key[K4].Flag.dbnckeypress && BTN_Key[K4].Flag.keychange)
		{
			if(BTN_Key[K4].clicktime > LONG_PRESS_TIME)
				btnStatus |=K4_LONGPRESS;
			else
				btnStatus |=K4_PRESS;
		}
	  
		return btnStatus;
 }

