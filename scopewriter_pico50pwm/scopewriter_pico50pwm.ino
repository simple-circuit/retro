//    Pico Processor Scope Writer using PWM
//    Copyright (C) 2024  Simple-Circuit
//    This program displays a line of characters on an oscilloscope using
//    One I/O pin.
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

volatile word bc[128][8];    //ascii character pixel matrix
volatile word a[100];        //ascii character line

//Your oscilloscope horizontal scan time is us/div times total divisions
//My scope is 50us/div * 12 div = 6ms
//Character time is MAXCHAR * 7 * SYNCBIT * 2 = 5.6ms
//Character time must be less than the horizontal scan time
//Retrace time is Character time + SYNC1 + 2 * SYNC2 = 7.08ms
//Retrace time should be greater than the horizontal scan time

#define SYNC1 1000          //sync pulse delay counts us
#define SYNC2 240           //pre and post line delay us
#define MAXCHAR 50          //the size of the character display
#define SYNCBIT 8           //us clock pixel synchronization bit use 8,16,32

uint slice_num ;            //Global variable for PWM slice number

void setup() {
  //the PWM is set to 16 levels at 8MHz. Pin-14 is the output
    gpio_set_function(14, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(14);
    pwm_set_wrap(slice_num, 15);
    pwm_set_clkdiv(slice_num,1);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 8);
    pwm_set_enabled(slice_num, true);
 
  Serial.begin(115200); 
     
  int i;
  char message[] = "50 Character O'Scope Display #{67]*835?!14$%^&()=+/.,ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz?";
  for (i = 0; i < MAXCHAR; i++) a[i] = message[i];

//128 ascii character matrix values for a 5x7 pixel display
//A@
bc[ 0x40] [ 6 ] = B01110;
bc[ 0x40] [5 ] =  B10001;
bc[ 0x40] [4 ] =  B10111;
bc[ 0x40] [3 ] =  B11001;
bc[ 0x40] [ 2 ] = B10110;
bc[ 0x40] [ 1 ] = B10000;
bc[ 0x40] [ 0 ] = B01100;  
//A
bc[ 0x41] [ 6 ] = B00100;
bc[ 0x41] [5 ] =  B01010;
bc[ 0x41] [4 ] =  B10001;
bc[ 0x41] [3 ] =  B11111;
bc[ 0x41] [ 2 ] = B10001;
bc[ 0x41] [ 1 ] = B10001;
bc[ 0x41] [ 0 ] = B10001;
//B
bc[ 0x42] [ 6 ] = B11110;
bc[ 0x42] [ 5 ] = B10001;
bc[ 0x42] [ 4 ] = B10001;
bc[ 0x42] [ 3 ] = B11110;
bc[ 0x42] [ 2 ] = B10001;
bc[ 0x42] [ 1 ] = B10001;
bc[ 0x42] [ 0 ] = B11110;
//C
bc[ 0x43] [ 6 ] = B01111;
bc[ 0x43] [ 5 ] = B10000;
bc[ 0x43] [ 4 ] = B10000;
bc[ 0x43] [ 3 ] = B10000;
bc[ 0x43] [ 2 ] = B10000;
bc[ 0x43] [ 1 ] = B10000;
bc[ 0x43] [ 0 ] = B01111;
//D
bc[ 0x44] [ 6 ] = B11110;
bc[ 0x44] [ 5 ] = B10001;
bc[ 0x44] [ 4 ] = B10001;
bc[ 0x44] [ 3 ] = B10001;
bc[ 0x44] [ 2 ] = B10001;
bc[ 0x44] [ 1 ] = B10001;
bc[ 0x44] [ 0 ] = B11110;
//E
bc[ 0x45] [ 6 ] = B11111;
bc[ 0x45] [ 5 ] = B10000;
bc[ 0x45] [ 4 ] = B10000;
bc[ 0x45] [ 3 ] = B11110;
bc[ 0x45] [ 2 ] = B10000;
bc[ 0x45] [ 1 ] = B10000;
bc[ 0x45] [ 0 ] = B11111;
//F
bc[ 0x46] [ 6 ] = B11110;
bc[ 0x46] [ 5 ] = B10000;
bc[ 0x46] [ 4 ] = B10000;
bc[ 0x46] [ 3 ] = B11110;
bc[ 0x46] [ 2 ] = B10000;
bc[ 0x46] [ 1 ] = B10000;
bc[ 0x46] [ 0 ] = B10000;
//G
bc[ 0x47] [ 6 ] = B11111;
bc[ 0x47] [ 5 ] = B10000;
bc[ 0x47] [ 4 ] = B10000;
bc[ 0x47] [ 3 ] = B10011;
bc[ 0x47] [ 2 ] = B10001;
bc[ 0x47] [ 1 ] = B10001;
bc[ 0x47] [ 0 ] = B11111;
//H
bc[ 0x48] [ 6 ] = B10001;
bc[ 0x48] [ 5 ] = B10001;
bc[ 0x48] [ 4 ] = B10001;
bc[ 0x48] [ 3 ] = B11111;
bc[ 0x48] [ 2 ] = B10001;
bc[ 0x48] [ 1 ] = B10001;
bc[ 0x48] [ 0 ] = B10001;
//I
bc[ 0x49] [ 6 ] = B01110;
bc[ 0x49] [ 5 ] = B00100;
bc[ 0x49] [ 4 ] = B00100;
bc[ 0x49] [ 3 ] = B00100;
bc[ 0x49] [ 2 ] = B00100;
bc[ 0x49] [ 1 ] = B00100;
bc[ 0x49] [ 0 ] = B11111;
//J
bc[ 0x4A] [ 6 ] = B11111;
bc[ 0x4A] [ 5 ] = B00010;
bc[ 0x4A] [ 4 ] = B00010;
bc[ 0x4A] [ 3 ] = B00010;
bc[ 0x4A] [ 2 ] = B00010;
bc[ 0x4A] [ 1 ] = B10010;
bc[ 0x4A] [ 0 ] = B01100;
//K
bc[ 0x4B] [ 6 ] = B10001;
bc[ 0x4B] [ 5 ] = B10010;
bc[ 0x4B] [ 4 ] = B10100;
bc[ 0x4B] [ 3 ] = B11000;
bc[ 0x4B] [ 2 ] = B10100;
bc[ 0x4B] [ 1 ] = B10010;
bc[ 0x4B] [ 0 ] = B10001;
//L
bc[ 0x4C] [ 6 ] = B10000;
bc[ 0x4C] [ 5 ] = B10000;
bc[ 0x4C] [ 4 ] = B10000;
bc[ 0x4C] [ 3 ] = B10000;
bc[ 0x4C] [ 2 ] = B10000;
bc[ 0x4C] [ 1 ] = B10000;
bc[ 0x4C] [ 0 ] = B11111;
//M
bc[ 0x4D] [ 6 ] = B11011;
bc[ 0x4D] [ 5 ] = B10101;
bc[ 0x4D] [ 4 ] = B10101;
bc[ 0x4D] [ 3 ] = B10101;
bc[ 0x4D] [ 2 ] = B10001;
bc[ 0x4D] [ 1 ] = B10001;
bc[ 0x4D] [ 0 ] = B10001;
//N
bc[ 0x4E] [ 6 ] = B11001;
bc[ 0x4E] [ 5 ] = B11001;
bc[ 0x4E] [ 4 ] = B10101;
bc[ 0x4E] [ 3 ] = B10101;
bc[ 0x4E] [ 2 ] = B10011;
bc[ 0x4E] [ 1 ] = B10011;
bc[ 0x4E] [ 0 ] = B10001;
//O
bc[ 0x4F] [ 6 ] = B01110;
bc[ 0x4F] [ 5 ] = B10001;
bc[ 0x4F] [ 4 ] = B10001;
bc[ 0x4F] [ 3 ] = B10001;
bc[ 0x4F] [ 2 ] = B10001;
bc[ 0x4F] [ 1 ] = B10001;
bc[ 0x4F] [ 0 ] = B01110;
//P
bc[ 0x50] [ 6 ] = B11110;
bc[ 0x50] [ 5 ] = B10001;
bc[ 0x50] [ 4 ] = B10001;
bc[ 0x50] [ 3 ] = B11110;
bc[ 0x50] [ 2 ] = B10000;
bc[ 0x50] [ 1 ] = B10000;
bc[ 0x50] [ 0 ] = B10000;
//Q
bc[ 0x51] [ 6 ] = B01110;
bc[ 0x51] [ 5 ] = B10001;
bc[ 0x51] [ 4 ] = B10001;
bc[ 0x51] [ 3 ] = B10001;
bc[ 0x51] [ 2 ] = B10101;
bc[ 0x51] [ 1 ] = B10011;
bc[ 0x51] [ 0 ] = B01111;
//R
bc[ 0x52] [ 6 ] = B11110;
bc[ 0x52] [ 5 ] = B10001;
bc[ 0x52] [ 4 ] = B10001;
bc[ 0x52] [ 3 ] = B11110;
bc[ 0x52] [ 2 ] = B10100;
bc[ 0x52] [ 1 ] = B10010;
bc[ 0x52] [ 0 ] = B10001;
//S
bc[ 0x53] [ 6 ] = B01111;
bc[ 0x53] [ 5 ] = B10000;
bc[ 0x53] [ 4 ] = B10000;
bc[ 0x53] [ 3 ] = B01110;
bc[ 0x53] [ 2 ] = B00001;
bc[ 0x53] [ 1 ] = B00001;
bc[ 0x53] [ 0 ] = B11110;
//T
bc[ 0x54] [ 6 ] = B11111;
bc[ 0x54] [ 5 ] = B00100;
bc[ 0x54] [ 4 ] = B00100;
bc[ 0x54] [ 3 ] = B00100;
bc[ 0x54] [ 2 ] = B00100;
bc[ 0x54] [ 1 ] = B00100;
bc[ 0x54] [ 0 ] = B00100;
//U
bc[ 0x55] [ 6 ] = B10001;
bc[ 0x55] [ 5 ] = B10001;
bc[ 0x55] [ 4 ] = B10001;
bc[ 0x55] [ 3 ] = B10001;
bc[ 0x55] [ 2 ] = B10001;
bc[ 0x55] [ 1 ] = B10001;
bc[ 0x55] [ 0 ] = B01110;
//V
bc[ 0x56] [ 6 ] = B10001;
bc[ 0x56] [ 5 ] = B10001;
bc[ 0x56] [ 4 ] = B10001;
bc[ 0x56] [ 3 ] = B10001;
bc[ 0x56] [ 2 ] = B01010;
bc[ 0x56] [ 1 ] = B01010;
bc[ 0x56] [ 0 ] = B00100;
//W
bc[ 0x57] [ 6 ] = B10001;
bc[ 0x57] [ 5 ] = B10001;
bc[ 0x57] [ 4 ] = B10101;
bc[ 0x57] [ 3 ] = B10101;
bc[ 0x57] [ 2 ] = B10101;
bc[ 0x57] [ 1 ] = B11011;
bc[ 0x57] [ 0 ] = B10001;
//X
bc[ 0x58] [ 6 ] = B10001;
bc[ 0x58] [ 5 ] = B10001;
bc[ 0x58] [ 4 ] = B01010;
bc[ 0x58] [ 3 ] = B00100;
bc[ 0x58] [ 2 ] = B01010;
bc[ 0x58] [ 1 ] = B10001;
bc[ 0x58] [ 0 ] = B10001;
//Y
bc[ 0x59] [ 6 ] = B10001;
bc[ 0x59] [ 5 ] = B10001;
bc[ 0x59] [ 4 ] = B10001;
bc[ 0x59] [ 3 ] = B01110;
bc[ 0x59] [ 2 ] = B00100;
bc[ 0x59] [ 1 ] = B00100;
bc[ 0x59] [ 0 ] = B00100;
//Z
bc[ 0x5A] [ 6 ] = B11111;
bc[ 0x5A] [ 5 ] = B00001;
bc[ 0x5A] [ 4 ] = B00010;
bc[ 0x5A] [ 3 ] = B00100;
bc[ 0x5A] [ 2 ] = B01000;
bc[ 0x5A] [ 1 ] = B10000;
bc[ 0x5A] [ 0 ] = B11111;
//[
bc[ 0x5B] [ 6 ] = B11100;
bc[ 0x5B] [ 5 ] = B10000;
bc[ 0x5B] [ 4 ] = B10000;
bc[ 0x5B] [ 3 ] = B10000;
bc[ 0x5B] [ 2 ] = B10000;
bc[ 0x5B] [ 1 ] = B10000;
bc[ 0x5B] [ 0 ] = B11100;
//\
bc[ 0x5C] [ 6 ] = B00000;
bc[ 0x5C] [ 5 ] = B10000;
bc[ 0x5C] [ 4 ] = B01000;
bc[ 0x5C] [ 3 ] = B00100;
bc[ 0x5C] [ 2 ] = B00010;
bc[ 0x5C] [ 1 ] = B00001;
bc[ 0x5C] [ 0 ] = B00000;
//]
bc[ 0x5D] [ 6 ] = B00111;
bc[ 0x5D] [ 5 ] = B00001;
bc[ 0x5D] [ 4 ] = B00001;
bc[ 0x5D] [ 3 ] = B00001;
bc[ 0x5D] [ 2 ] = B00001;
bc[ 0x5D] [ 1 ] = B00001;
bc[ 0x5D] [ 0 ] = B00111;
//^
bc[ 0x5E] [ 6 ] = B00000;
bc[ 0x5E] [ 5 ] = B00100;
bc[ 0x5E] [ 4 ] = B01010;
bc[ 0x5E] [ 3 ] = B10001;
bc[ 0x5E] [ 2 ] = B00000;
bc[ 0x5E] [ 1 ] = B00000;
bc[ 0x5E] [ 0 ] = B00000;
//<--
bc[ 0x5F] [ 6 ] = B00000;
bc[ 0x5F] [ 5 ] = B00100;
bc[ 0x5F] [ 4 ] = B01000;
bc[ 0x5F] [ 3 ] = B11111;
bc[ 0x5F] [ 2 ] = B01000;
bc[ 0x5F] [ 1 ] = B00100;
bc[ 0x5F] [ 0 ] = B00000;
// SPACE
bc[ 0x20] [ 6 ] = B00000;
bc[ 0x20] [ 5 ] = B00000;
bc[ 0x20] [ 4 ] = B00000;
bc[ 0x20] [ 3 ] = B00000;
bc[ 0x20] [ 2 ] = B00000;
bc[ 0x20] [ 1 ] = B00000;
bc[ 0x20] [ 0 ] = B00000;
// !
bc[ 0x21] [ 6 ] = B01100;
bc[ 0x21] [ 5 ] = B01100;
bc[ 0x21] [ 4 ] = B01100;
bc[ 0x21] [ 3 ] = B01100;
bc[ 0x21] [ 2 ] = B01100;
bc[ 0x21] [ 1 ] = B00000;
bc[ 0x21] [ 0 ] = B01100;
// "
bc[ 0x22] [ 6 ] = B01010;
bc[ 0x22] [ 5 ] = B01010;
bc[ 0x22] [ 4 ] = B00000;
bc[ 0x22] [ 3 ] = B00000;
bc[ 0x22] [ 2 ] = B00000;
bc[ 0x22] [ 1 ] = B00000;
bc[ 0x22] [ 0 ] = B00000;
// #
bc[ 0x23] [ 6 ] = B00000;
bc[ 0x23] [ 5 ] = B01010;
bc[ 0x23] [ 4 ] = B11111;
bc[ 0x23] [ 3 ] = B01010;
bc[ 0x23] [ 2 ] = B11111;
bc[ 0x23] [ 1 ] = B01010;
bc[ 0x23] [ 0 ] = B00000;
// $
bc[ 0x24] [ 6 ] = B00100;
bc[ 0x24] [ 5 ] = B00111;
bc[ 0x24] [ 4 ] = B10100;
bc[ 0x24] [ 3 ] = B11110;
bc[ 0x24] [ 2 ] = B00101;
bc[ 0x24] [ 1 ] = B11100;
bc[ 0x24] [ 0 ] = B00100;
// %
bc[ 0x25] [ 6 ] = B11000;
bc[ 0x25] [ 5 ] = B11001;
bc[ 0x25] [ 4 ] = B00010;
bc[ 0x25] [ 3 ] = B00100;
bc[ 0x25] [ 2 ] = B01000;
bc[ 0x25] [ 1 ] = B10011;
bc[ 0x25] [ 0 ] = B00011;
// &
bc[ 0x26] [ 6 ] = B01110;
bc[ 0x26] [ 5 ] = B10010;
bc[ 0x26] [ 4 ] = B01000;
bc[ 0x26] [ 3 ] = B01100;
bc[ 0x26] [ 2 ] = B10010;
bc[ 0x26] [ 1 ] = B10001;
bc[ 0x26] [ 0 ] = B01110;
// '
bc[ 0x27] [ 6 ] = B00110;
bc[ 0x27] [ 5 ] = B00010;
bc[ 0x27] [ 4 ] = B00000;
bc[ 0x27] [ 3 ] = B00000;
bc[ 0x27] [ 2 ] = B00000;
bc[ 0x27] [ 1 ] = B00000;
bc[ 0x27] [ 0 ] = B00000;
// (
bc[ 0x28] [ 6 ] = B00010;
bc[ 0x28] [ 5 ] = B00100;
bc[ 0x28] [ 4 ] = B01000;
bc[ 0x28] [ 3 ] = B01000;
bc[ 0x28] [ 2 ] = B01000;
bc[ 0x28] [ 1 ] = B00100;
bc[ 0x28] [ 0 ] = B00010;
// )
bc[ 0x29] [ 6 ] = B01000;
bc[ 0x29] [ 5 ] = B00100;
bc[ 0x29] [ 4 ] = B00010;
bc[ 0x29] [ 3 ] = B00010;
bc[ 0x29] [ 2 ] = B00010;
bc[ 0x29] [ 1 ] = B00100;
bc[ 0x29] [ 0 ] = B01000;
// *
bc[ 0x2A] [ 6 ] = B00000;
bc[ 0x2A] [ 5 ] = B10101;
bc[ 0x2A] [ 4 ] = B01110;
bc[ 0x2A] [ 3 ] = B11111;
bc[ 0x2A] [ 2 ] = B01110;
bc[ 0x2A] [ 1 ] = B10101;
bc[ 0x2A] [ 0 ] = B00000;
//+
bc[ 0x2B] [ 6 ] = B00000;
bc[ 0x2B] [ 5 ] = B00100;
bc[ 0x2B] [ 4 ] = B00100;
bc[ 0x2B] [ 3 ] = B11111;
bc[ 0x2B] [ 2 ] = B00100;
bc[ 0x2B] [ 1 ] = B00100;
bc[ 0x2B] [ 0 ] = B00000;
// ,
bc[ 0x2C] [ 6 ] = B00000;
bc[ 0x2C] [ 5 ] = B00000;
bc[ 0x2C] [ 4 ] = B00000;
bc[ 0x2C] [ 3 ] = B00000;
bc[ 0x2C] [ 2 ] = B00000;
bc[ 0x2C] [ 1 ] = B01100;
bc[ 0x2C] [ 0 ] = B00100;
//-
bc[ 0x2D] [ 6 ] = B00000;
bc[ 0x2D] [ 5 ] = B00000;
bc[ 0x2D] [ 4 ] = B00000;
bc[ 0x2D] [ 3 ] = B11111;
bc[ 0x2D] [ 2 ] = B00000;
bc[ 0x2D] [ 1 ] = B00000;
bc[ 0x2D] [ 0 ] = B00000;
// .
bc[ 0x2E] [ 6 ] = B00000;
bc[ 0x2E] [ 5 ] = B00000;
bc[ 0x2E] [ 4 ] = B00000;
bc[ 0x2E] [ 3 ] = B00000;
bc[ 0x2E] [ 2 ] = B00000;
bc[ 0x2E] [ 1 ] = B01100;
bc[ 0x2E] [ 0 ] = B01100;
// /
bc[ 0x2F] [ 6 ] = B00000;
bc[ 0x2F] [ 5 ] = B00001;
bc[ 0x2F] [ 4 ] = B00010;
bc[ 0x2F] [ 3 ] = B00100;
bc[ 0x2F] [ 2 ] = B01000;
bc[ 0x2F] [ 1 ] = B10000;
bc[ 0x2F] [ 0 ] = B00000;
//0
bc[ 0x30] [ 6 ] = B01110;
bc[ 0x30] [ 5 ] = B10001;
bc[ 0x30] [ 4 ] = B11001;
bc[ 0x30] [ 3 ] = B10101;
bc[ 0x30] [ 2 ] = B10011;
bc[ 0x30] [ 1 ] = B10001;
bc[ 0x30] [ 0 ] = B01110;
//1
bc[ 0x31] [ 6 ] = B00100;
bc[ 0x31] [ 5 ] = B01100;
bc[ 0x31] [ 4 ] = B00100;
bc[ 0x31] [ 3 ] = B00100;
bc[ 0x31] [ 2 ] = B00100;
bc[ 0x31] [ 1 ] = B00100;
bc[ 0x31] [ 0 ] = B01110;
//2
bc[ 0x32] [ 6 ] = B11110;
bc[ 0x32] [ 5 ] = B00001;
bc[ 0x32] [ 4 ] = B00001;
bc[ 0x32] [ 3 ] = B01110;
bc[ 0x32] [ 2 ] = B10000;
bc[ 0x32] [ 1 ] = B10000;
bc[ 0x32] [ 0 ] = B11110;
//3
bc[ 0x33] [ 6 ] = B11110;
bc[ 0x33] [ 5 ] = B00001;
bc[ 0x33] [ 4 ] = B00001;
bc[ 0x33] [ 3 ] = B11110;
bc[ 0x33] [ 2 ] = B00001;
bc[ 0x33] [ 1 ] = B00001;
bc[ 0x33] [ 0 ] = B11110;
//4
bc[ 0x34] [ 6 ] = B10010;
bc[ 0x34] [ 5 ] = B10010;
bc[ 0x34] [ 4 ] = B10010;
bc[ 0x34] [ 3 ] = B11111;
bc[ 0x34] [ 2 ] = B00010;
bc[ 0x34] [ 1 ] = B00010;
bc[ 0x34] [ 0 ] = B00010;
//5
bc[ 0x35] [ 6 ] = B11111;
bc[ 0x35] [ 5 ] = B10000;
bc[ 0x35] [ 4 ] = B10000;
bc[ 0x35] [ 3 ] = B01110;
bc[ 0x35] [ 2 ] = B00001;
bc[ 0x35] [ 1 ] = B00001;
bc[ 0x35] [ 0 ] = B11110;
//6
bc[ 0x36] [ 6 ] = B01110;
bc[ 0x36] [ 5 ] = B10000;
bc[ 0x36] [ 4 ] = B10000;
bc[ 0x36] [ 3 ] = B11110;
bc[ 0x36] [ 2 ] = B10001;
bc[ 0x36] [ 1 ] = B10001;
bc[ 0x36] [ 0 ] = B01110;
//7
bc[ 0x37] [ 6 ] = B11111;
bc[ 0x37] [ 5 ] = B10001;
bc[ 0x37] [ 4 ] = B10010;
bc[ 0x37] [ 3 ] = B00100;
bc[ 0x37] [ 2 ] = B01000;
bc[ 0x37] [ 1 ] = B01000;
bc[ 0x37] [ 0 ] = B01000;
//8
bc[ 0x38] [ 6 ] = B01110;
bc[ 0x38] [ 5 ] = B10001;
bc[ 0x38] [ 4 ] = B10001;
bc[ 0x38] [ 3 ] = B01110;
bc[ 0x38] [ 2 ] = B10001;
bc[ 0x38] [ 1 ] = B10001;
bc[ 0x38] [ 0 ] = B01110;
//9
bc[ 0x39] [ 6 ] = B01110;
bc[ 0x39] [ 5 ] = B10001;
bc[ 0x39] [ 4 ] = B10001;
bc[ 0x39] [ 3 ] = B01111;
bc[ 0x39] [ 2 ] = B00001;
bc[ 0x39] [ 1 ] = B10001;
bc[ 0x39] [ 0 ] = B01110;
//:
bc[ 0x3A] [ 6 ] = B00000;
bc[ 0x3A] [ 5 ] = B01100;
bc[ 0x3A] [ 4 ] = B01100;
bc[ 0x3A] [ 3 ] = B00000;
bc[ 0x3A] [ 2 ] = B01100;
bc[ 0x3A] [ 1 ] = B01100;
bc[ 0x3A] [ 0 ] = B00000;
//;
bc[ 0x3B] [ 6 ] = B00000;
bc[ 0x3B] [ 5 ] = B01100;
bc[ 0x3B] [ 4 ] = B01100;
bc[ 0x3B] [ 3 ] = B00000;
bc[ 0x3B] [ 2 ] = B01100;
bc[ 0x3B] [ 1 ] = B00100;
bc[ 0x3B] [ 0 ] = B00000;
//<
bc[ 0x3C] [ 6 ] = B00001;
bc[ 0x3C] [ 5 ] = B00010;
bc[ 0x3C] [ 4 ] = B00100;
bc[ 0x3C] [ 3 ] = B01000;
bc[ 0x3C] [ 2 ] = B00100;
bc[ 0x3C] [ 1 ] = B00010;
bc[ 0x3C] [ 0 ] = B00001;
//=
bc[ 0x3D] [ 6 ] = B00000;
bc[ 0x3D] [ 5 ] = B00000;
bc[ 0x3D] [ 4 ] = B11111;
bc[ 0x3D] [ 3 ] = B00000;
bc[ 0x3D] [ 2 ] = B00000;
bc[ 0x3D] [ 1 ] = B11111;
bc[ 0x3D] [ 0 ] = B00000;
//>
bc[ 0x3E] [ 6 ] = B01000;
bc[ 0x3E] [ 5 ] = B00100;
bc[ 0x3E] [ 4 ] = B00010;
bc[ 0x3E] [ 3 ] = B00001;
bc[ 0x3E] [ 2 ] = B00010;
bc[ 0x3E] [ 1 ] = B00100;
bc[ 0x3E] [ 0 ] = B01000;
//?
bc[ 0x3F] [ 6 ] = B01110;
bc[ 0x3F] [ 5 ] = B10001;
bc[ 0x3F] [ 4 ] = B00010;
bc[ 0x3F] [ 3 ] = B00100;
bc[ 0x3F] [ 2 ] = B00100;
bc[ 0x3F] [ 1 ] = B00000;
bc[ 0x3F] [ 0 ] = B00100;
// \
bc[ 0x60] [ 6 ] = B01110;
bc[ 0x60] [5 ] =  B10001;
bc[ 0x60] [4 ] =  B10111;
bc[ 0x60] [3 ] =  B11001;
bc[ 0x60] [ 2 ] = B10110;
bc[ 0x60] [ 1 ] = B10000;
bc[ 0x60] [ 0 ] = B01100;  
// a
bc[ 0x61] [ 6 ] = B00000;
bc[ 0x61] [5 ] =  B00000;
bc[ 0x61] [4 ] =  B01110;
bc[ 0x61] [3 ] =  B00001;
bc[ 0x61] [ 2 ] = B01111;
bc[ 0x61] [ 1 ] = B10001;
bc[ 0x61] [ 0 ] = B01111;
// b
bc[ 0x62] [ 6 ] = B00000;
bc[ 0x62] [ 5 ] = B10000;
bc[ 0x62] [ 4 ] = B10000;
bc[ 0x62] [ 3 ] = B11110;
bc[ 0x62] [ 2 ] = B10001;
bc[ 0x62] [ 1 ] = B10001;
bc[ 0x62] [ 0 ] = B11110;
// c
bc[ 0x63] [ 6 ] = B00000;
bc[ 0x63] [ 5 ] = B00000;
bc[ 0x63] [ 4 ] = B00000;
bc[ 0x63] [ 3 ] = B01111;
bc[ 0x63] [ 2 ] = B10000;
bc[ 0x63] [ 1 ] = B10000;
bc[ 0x63] [ 0 ] = B01111;
// d
bc[ 0x64] [ 6 ] = B00000;
bc[ 0x64] [ 5 ] = B00001;
bc[ 0x64] [ 4 ] = B00001;
bc[ 0x64] [ 3 ] = B00001;
bc[ 0x64] [ 2 ] = B01111;
bc[ 0x64] [ 1 ] = B10001;
bc[ 0x64] [ 0 ] = B11111;
// e
bc[ 0x65] [ 6 ] = B00000;
bc[ 0x65] [ 5 ] = B00000;
bc[ 0x65] [ 4 ] = B01110;
bc[ 0x65] [ 3 ] = B10001;
bc[ 0x65] [ 2 ] = B11110;
bc[ 0x65] [ 1 ] = B10000;
bc[ 0x65] [ 0 ] = B01110;
// f
bc[ 0x66] [ 6 ] = B00000;
bc[ 0x66] [ 5 ] = B01110;
bc[ 0x66] [ 4 ] = B10001;
bc[ 0x66] [ 3 ] = B10000;
bc[ 0x66] [ 2 ] = B11110;
bc[ 0x66] [ 1 ] = B10000;
bc[ 0x66] [ 0 ] = B10000;
// g
bc[ 0x67] [ 6 ] = B00000;
bc[ 0x67] [ 5 ] = B01110;
bc[ 0x67] [ 4 ] = B10001;
bc[ 0x67] [ 3 ] = B01111;
bc[ 0x67] [ 2 ] = B00001;
bc[ 0x67] [ 1 ] = B00001;
bc[ 0x67] [ 0 ] = B01110;
// h
bc[ 0x68] [ 6 ] = B00000;
bc[ 0x68] [ 5 ] = B10000;
bc[ 0x68] [ 4 ] = B10000;
bc[ 0x68] [ 3 ] = B10000;
bc[ 0x68] [ 2 ] = B11110;
bc[ 0x68] [ 1 ] = B10001;
bc[ 0x68] [ 0 ] = B10001;
// i
bc[ 0x69] [ 6 ] = B00000;
bc[ 0x69] [ 5 ] = B00100;
bc[ 0x69] [ 4 ] = B00000;
bc[ 0x69] [ 3 ] = B00100;
bc[ 0x69] [ 2 ] = B00100;
bc[ 0x69] [ 1 ] = B00100;
bc[ 0x69] [ 0 ] = B01110;
// j
bc[ 0x6A] [ 6 ] = B00000;
bc[ 0x6A] [ 5 ] = B00010;
bc[ 0x6A] [ 4 ] = B00000;
bc[ 0x6A] [ 3 ] = B00010;
bc[ 0x6A] [ 2 ] = B00010;
bc[ 0x6A] [ 1 ] = B10010;
bc[ 0x6A] [ 0 ] = B01100;
// k
bc[ 0x6B] [ 6 ] = B00000;
bc[ 0x6B] [ 5 ] = B10000;
bc[ 0x6B] [ 4 ] = B10010;
bc[ 0x6B] [ 3 ] = B10100;
bc[ 0x6B] [ 2 ] = B11000;
bc[ 0x6B] [ 1 ] = B10100;
bc[ 0x6B] [ 0 ] = B10010;
// l
bc[ 0x6C] [ 6 ] = B00000;
bc[ 0x6C] [ 5 ] = B01100;
bc[ 0x6C] [ 4 ] = B00100;
bc[ 0x6C] [ 3 ] = B00100;
bc[ 0x6C] [ 2 ] = B00100;
bc[ 0x6C] [ 1 ] = B00100;
bc[ 0x6C] [ 0 ] = B01110;
// m
bc[ 0x6D] [ 6 ] = B00000;
bc[ 0x6D] [ 5 ] = B00000;
bc[ 0x6D] [ 4 ] = B00000;
bc[ 0x6D] [ 3 ] = B11011;
bc[ 0x6D] [ 2 ] = B10101;
bc[ 0x6D] [ 1 ] = B10101;
bc[ 0x6D] [ 0 ] = B10101;
// n
bc[ 0x6E] [ 6 ] = B00000;
bc[ 0x6E] [ 5 ] = B00000;
bc[ 0x6E] [ 4 ] = B00000;
bc[ 0x6E] [ 3 ] = B11110;
bc[ 0x6E] [ 2 ] = B10001;
bc[ 0x6E] [ 1 ] = B10001;
bc[ 0x6E] [ 0 ] = B10001;
// o
bc[ 0x6F] [ 6 ] = B00000;
bc[ 0x6F] [ 5 ] = B00000;
bc[ 0x6F] [ 4 ] = B00000;
bc[ 0x6F] [ 3 ] = B01110;
bc[ 0x6F] [ 2 ] = B10001;
bc[ 0x6F] [ 1 ] = B10001;
bc[ 0x6F] [ 0 ] = B01110;
// p
bc[ 0x70] [ 6 ] = B00000;
bc[ 0x70] [ 5 ] = B00000;
bc[ 0x70] [ 4 ] = B11110;
bc[ 0x70] [ 3 ] = B10001;
bc[ 0x70] [ 2 ] = B11110;
bc[ 0x70] [ 1 ] = B10000;
bc[ 0x70] [ 0 ] = B10000;
// q
bc[ 0x71] [ 6 ] = B00000;
bc[ 0x71] [ 5 ] = B00000;
bc[ 0x71] [ 4 ] = B01111;
bc[ 0x71] [ 3 ] = B10001;
bc[ 0x71] [ 2 ] = B01111;
bc[ 0x71] [ 1 ] = B00001;
bc[ 0x71] [ 0 ] = B00001;
// r
bc[ 0x72] [ 6 ] = B00000;
bc[ 0x72] [ 5 ] = B00000;
bc[ 0x72] [ 4 ] = B00000;
bc[ 0x72] [ 3 ] = B11110;
bc[ 0x72] [ 2 ] = B10001;
bc[ 0x72] [ 1 ] = B10000;
bc[ 0x72] [ 0 ] = B10000;
// s
bc[ 0x73] [ 6 ] = B00000;
bc[ 0x73] [ 5 ] = B00000;
bc[ 0x73] [ 4 ] = B01110;
bc[ 0x73] [ 3 ] = B10000;
bc[ 0x73] [ 2 ] = B01110;
bc[ 0x73] [ 1 ] = B00001;
bc[ 0x73] [ 0 ] = B11110;
// t
bc[ 0x74] [ 6 ] = B00000;
bc[ 0x74] [ 5 ] = B00000;
bc[ 0x74] [ 4 ] = B00100;
bc[ 0x74] [ 3 ] = B01111;
bc[ 0x74] [ 2 ] = B00100;
bc[ 0x74] [ 1 ] = B00100;
bc[ 0x74] [ 0 ] = B00100;
// u
bc[ 0x75] [ 6 ] = B00000;
bc[ 0x75] [ 5 ] = B00000;
bc[ 0x75] [ 4 ] = B00000;
bc[ 0x75] [ 3 ] = B10001;
bc[ 0x75] [ 2 ] = B10001;
bc[ 0x75] [ 1 ] = B10001;
bc[ 0x75] [ 0 ] = B01110;
// v
bc[ 0x76] [ 6 ] = B00000;
bc[ 0x76] [ 5 ] = B00000;
bc[ 0x76] [ 4 ] = B00000;
bc[ 0x76] [ 3 ] = B10001;
bc[ 0x76] [ 2 ] = B01010;
bc[ 0x76] [ 1 ] = B01010;
bc[ 0x76] [ 0 ] = B00100;
// w
bc[ 0x77] [ 6 ] = B00000;
bc[ 0x77] [ 5 ] = B00000;
bc[ 0x77] [ 4 ] = B00000;
bc[ 0x77] [ 3 ] = B10001;
bc[ 0x77] [ 2 ] = B10101;
bc[ 0x77] [ 1 ] = B11011;
bc[ 0x77] [ 0 ] = B10001;
// x
bc[ 0x78] [ 6 ] = B00000;
bc[ 0x78] [ 5 ] = B00000;
bc[ 0x78] [ 4 ] = B00000;
bc[ 0x78] [ 3 ] = B10001;
bc[ 0x78] [ 2 ] = B00100;
bc[ 0x78] [ 1 ] = B01010;
bc[ 0x78] [ 0 ] = B10001;
// y
bc[ 0x79] [ 6 ] = B00000;
bc[ 0x79] [ 5 ] = B00000;
bc[ 0x79] [ 4 ] = B00000;
bc[ 0x79] [ 3 ] = B01001;
bc[ 0x79] [ 2 ] = B00101;
bc[ 0x79] [ 1 ] = B00010;
bc[ 0x79] [ 0 ] = B00010;
// z
bc[ 0x7A] [ 6 ] = B00000;
bc[ 0x7A] [ 5 ] = B00000;
bc[ 0x7A] [ 4 ] = B00000;
bc[ 0x7A] [ 3 ] = B11110;
bc[ 0x7A] [ 2 ] = B00100;
bc[ 0x7A] [ 1 ] = B01000;
bc[ 0x7A] [ 0 ] = B11110;
// {
bc[ 0x7B] [ 6 ] = B00110;
bc[ 0x7B] [ 5 ] = B01000;
bc[ 0x7B] [ 4 ] = B01000;
bc[ 0x7B] [ 3 ] = B11000;
bc[ 0x7B] [ 2 ] = B01000;
bc[ 0x7B] [ 1 ] = B01000;
bc[ 0x7B] [ 0 ] = B00110;
// |
bc[ 0x7C] [ 6 ] = B00100;
bc[ 0x7C] [ 5 ] = B00100;
bc[ 0x7C] [ 4 ] = B00100;
bc[ 0x7C] [ 3 ] = B00000;
bc[ 0x7C] [ 2 ] = B00100;
bc[ 0x7C] [ 1 ] = B00100;
bc[ 0x7C] [ 0 ] = B00100;
// }
bc[ 0x7D] [ 6 ] = B01100;
bc[ 0x7D] [ 5 ] = B00010;
bc[ 0x7D] [ 4 ] = B00010;
bc[ 0x7D] [ 3 ] = B00011;
bc[ 0x7D] [ 2 ] = B00010;
bc[ 0x7D] [ 1 ] = B00010;
bc[ 0x7D] [ 0 ] = B01100;
// ~
bc[ 0x7E] [ 6 ] = B00000;
bc[ 0x7E] [ 5 ] = B00000;
bc[ 0x7E] [ 4 ] = B00000;
bc[ 0x7E] [ 3 ] = B01001;
bc[ 0x7E] [ 2 ] = B10110;
bc[ 0x7E] [ 1 ] = B00000;
bc[ 0x7E] [ 0 ] = B00000;
//< _
bc[ 0x7F] [ 6 ] = B00000;
bc[ 0x7F] [ 5 ] = B00000;
bc[ 0x7F] [ 4 ] = B00000;
bc[ 0x7F] [ 3 ] = B00000;
bc[ 0x7F] [ 2 ] = B00000;
bc[ 0x7F] [ 1 ] = B00000;
bc[ 0x7F] [ 0 ] = B11111;
}

//core-1 receives serial data and pushes it on to the end of the line buffer
void loop(){
  byte c;
  volatile unsigned int k;

  while ( Serial.available() == 0 );
  c = Serial.read();
  Serial.write(c);
  for (k = 0; k < (MAXCHAR-1); k++) a[k] = a[k+1]; 
    a[MAXCHAR-1] = c & 0xff;
}

void setup1(){

} 

//Core=2 modulates pin-14 with the raster scan values
void loop1() {
  volatile unsigned int i,j,k,d,b,n,hi;
  while(true){
  for (j = 0; j < 7; j++) {                        //raster has 7-levels
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);  //pwm zero level
    delayMicroseconds(SYNC1);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 6);  //front porch and horizontal sync
    delayMicroseconds(SYNC2);
    hi = j + 8;                                    //on level is scan number plus 8
    for (i = 0; i< MAXCHAR; i++) {                 //all characters shown each line scan
    n = a[i];                                      //read the text buffer
    d = bc[n][j];                                  //look up the matrix value       
    b = 32;                                        //set the pixel shift bit to left most 
    for (k = 0; k <= 6; k++) {                     //do 7 shifts
     if ((d & b) != 0)   pwm_set_chan_level(slice_num, PWM_CHAN_A, hi); //set pixel On or Off
     else  pwm_set_chan_level(slice_num, PWM_CHAN_A, 6);
     b = b >> 1;                                   //shift to next pixel
     while ((time_us_32() & SYNCBIT)!= 0);         //synchronize on micro second clock bits
     while ((time_us_32() & SYNCBIT) == 0);
      }
    }
    delayMicroseconds(SYNC2);                      //back porch
   }
  } 
}
