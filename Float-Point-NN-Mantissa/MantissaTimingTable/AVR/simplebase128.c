/*
    This file is part of the ChipWhisperer Example Targets
    Copyright (C) 2012-2017 NewAE Technology Inc.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hal.h"
#include <stdint.h>
#include <stdlib.h>

#include "simpleserial.h"

#define num 128

static float    wt   = 1.0;
static int      wn   = 0;
static float    ip   = 1.0;
static int      in   = 0;
float mul(float weight, float input) {
    return  weight * input;
}


uint8_t get_key(uint8_t* k, uint8_t len)
{
	// Load key here
	return 0x00;
}

uint8_t get_pt(uint8_t* pt, uint8_t len)
{
    float   dummy = 0;
    float   dummycal = 0;
    float   unit  = 1/(float)128;
    int     i;
    
    // unit 추가

    /*      Init        */
    if(in / 4 == 1) ip += 32/(float)128;
    else if(in / 4 == 2) ip += 64/(float)128;
    else if(in / 4 == 3) ip += 96/(float)128;
    
    
	/**********************************
	* Start user-specific code here. */
    trigger_high();
    for(i = 0 ; i < 32 ; i++, ip += unit) {
        
        dummy = wt * ip;
        //printf("wt[%.2f] x ip[%.2f] = %.2f\n", wt, ip, result[i]);
    }
    trigger_low();
	//16 hex bytes held in 'pt' were sent
	//from the computer. Store your response
	//back into 'pt', which will send 16 bytes
	//back to computer. Can ignore of course if
	//not needed

	/* End user-specific code here. *
	********************************/
    dummycal = dummy * dummy;
    dummy += dummycal;
    
    
	simpleserial_put('r', 16, pt);
    
    if(in == 128) wt += 1.0/(float)num;
    
	return 0x00;
}

uint8_t reset(uint8_t* x, uint8_t len)
{
	// Reset key here if needed
	return 0x00;
}

int main(void)
{
    platform_init();
	init_uart();
	trigger_setup();

 	/* Uncomment this to get a HELLO message for debug */
	/*
	putch('h');
	putch('e');
	putch('l');
	putch('l');
	putch('o');
	putch('\n');
	*/

	simpleserial_init();
    simpleserial_addcmd('p', 16, get_pt);
#if SS_VER != SS_VER_2_1
	simpleserial_addcmd('k', 16, get_key);
	simpleserial_addcmd('x', 0, reset);
#else
    simpleserial_addcmd(0x01, 16, aes);

#endif
	while(1)
		simpleserial_get();
}
