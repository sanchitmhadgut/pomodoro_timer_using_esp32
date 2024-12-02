/*
 * button_scan.h
 *
 *  Created on: Oct 2, 2024
 *      Author: Admin
 */

#ifndef INC_BUTTON_SCAN_H_
#define INC_BUTTON_SCAN_H_
#define BUTTON_ENABLED

#define LONG_PRESS_TIME  6

/*  button PINS on NE76
----BUTTON PINS ------
K1 = P0.0
K2 = P0.1
K3 = P0.2
K4 = P1.0
----- PRESS NUMBER LOGIC --------
bts_sts is of 8 bits we are using 8 bit as
     0               0             0               0                0           0           0         0
K4_LONGPRESS    K3_LONGPRESS    K2_LONGPRESS    K1_LONGPRESS    K4_PRESS    K3_PRESS   K2_PRESS   K1_PRESS

So in Button.c Code we just using OR command so that that we can easily get diffrent number for diffrent things.
For Example :-  K2_PRESS and K4_PRESS         K1_LONGPRESS & K3_LONGPRESS
     K2_PRESS    00000010 =2             K1_LONGPRESS   00010000 =16
     K4_PRESS  ||00001000 =8             K3_LONGPRESS   01000000 =64
                 00001010 =12							              01010000 =80
*/
static const int buttonUp = 19;
static const int buttonDwn = 18;
static const int buttonBck = 5;
static const int buttonEntr = 27;

//@atish
#define NOKEY_PRESS 0
#define K1_PRESS  1
#define K2_PRESS  2
#define K3_PRESS  4
#define K4_PRESS  8
#define K1_K2_PRESS  3
#define K1_K3_PRESS  5
#define K2_K3_PRESS  6
#define K1_K4_PRESS  9
#define K2_K4_PRESS  10
#define K3_K4_PRESS  12
#define K1_LONGPRESS 16
#define K2_LONGPRESS 32
#define K3_LONGPRESS 64
#define K4_LONGPRESS 128
#define K1_K2_LONGPRESS 48
#define K1_K3_LONGPRESS 80
#define K2_K3_LONGPRESS 96
#define K1_K4_LONGPRESS 144
#define K2_K4_LONGPRESS 160
#define K3_K4_LONGPRESS 192

#define K1   0
#define K2   1
#define K3   2
#define K4   3

union F{
	unsigned char rawkey:1;
	unsigned char dbnckeypress:1;
	unsigned char keypress:1;
	unsigned char keychange:1;
	unsigned char dbnctime:4;

  unsigned char bitvalue;
};

typedef struct
{
	  union F Flag;
    unsigned char clicktime;
    unsigned char PORT_pin;
}BTN_struct;

unsigned char  Button_Scan(void);



#endif /* INC_BUTTON_SCAN_H_ */
