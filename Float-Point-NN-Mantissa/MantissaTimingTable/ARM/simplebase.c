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

float     ip[128] = {
    (float)0/(float)128,
    (float)1/(float)128,
    (float)2/(float)128,
    (float)3/(float)128,
    (float)4/(float)128,
    (float)5/(float)128,
    (float)6/(float)128,
    (float)7/(float)128,
    (float)8/(float)128,
    (float)9/(float)128,
    (float)10/(float)128,
    (float)11/(float)128,
    (float)12/(float)128,
    (float)13/(float)128,
    (float)14/(float)128,
    (float)15/(float)128,
    (float)16/(float)128,
    (float)17/(float)128,
    (float)18/(float)128,
    (float)19/(float)128,
    (float)20/(float)128,
    (float)21/(float)128,
    (float)22/(float)128,
    (float)23/(float)128,
    (float)24/(float)128,
    (float)25/(float)128,
    (float)26/(float)128,
    (float)27/(float)128,
    (float)28/(float)128,
    (float)29/(float)128,
    (float)30/(float)128,
    (float)31/(float)128,
    (float)32/(float)128,
    (float)33/(float)128,
    (float)34/(float)128,
    (float)35/(float)128,
    (float)36/(float)128,
    (float)37/(float)128,
    (float)38/(float)128,
    (float)39/(float)128,
    (float)40/(float)128,
    (float)41/(float)128,
    (float)42/(float)128,
    (float)43/(float)128,
    (float)44/(float)128,
    (float)45/(float)128,
    (float)46/(float)128,
    (float)47/(float)128,
    (float)48/(float)128,
    (float)49/(float)128,
    (float)50/(float)128,
    (float)51/(float)128,
    (float)52/(float)128,
    (float)53/(float)128,
    (float)54/(float)128,
    (float)55/(float)128,
    (float)56/(float)128,
    (float)57/(float)128,
    (float)58/(float)128,
    (float)59/(float)128,
    (float)60/(float)128,
    (float)61/(float)128,
    (float)62/(float)128,
    (float)63/(float)128,
    (float)64/(float)128,
    (float)65/(float)128,
    (float)66/(float)128,
    (float)67/(float)128,
    (float)68/(float)128,
    (float)69/(float)128,
    (float)70/(float)128,
    (float)71/(float)128,
    (float)72/(float)128,
    (float)73/(float)128,
    (float)74/(float)128,
    (float)75/(float)128,
    (float)76/(float)128,
    (float)77/(float)128,
    (float)78/(float)128,
    (float)79/(float)128,
    (float)80/(float)128,
    (float)81/(float)128,
    (float)82/(float)128,
    (float)83/(float)128,
    (float)84/(float)128,
    (float)85/(float)128,
    (float)86/(float)128,
    (float)87/(float)128,
    (float)88/(float)128,
    (float)89/(float)128,
    (float)90/(float)128,
    (float)91/(float)128,
    (float)92/(float)128,
    (float)93/(float)128,
    (float)94/(float)128,
    (float)95/(float)128,
    (float)96/(float)128,
    (float)97/(float)128,
    (float)98/(float)128,
    (float)99/(float)128,
    (float)100/(float)128,
    (float)101/(float)128,
    (float)102/(float)128,
    (float)103/(float)128,
    (float)104/(float)128,
    (float)105/(float)128,
    (float)106/(float)128,
    (float)107/(float)128,
    (float)108/(float)128,
    (float)109/(float)128,
    (float)110/(float)128,
    (float)111/(float)128,
    (float)112/(float)128,
    (float)113/(float)128,
    (float)114/(float)128,
    (float)115/(float)128,
    (float)116/(float)128,
    (float)117/(float)128,
    (float)118/(float)128,
    (float)119/(float)128,
    (float)120/(float)128,
    (float)121/(float)128,
    (float)122/(float)128,
    (float)123/(float)128,
    (float)124/(float)128,
    (float)125/(float)128,
    (float)126/(float)128,
    (float)127/(float)128
};

float     wt[128] = {
    (float)0/(float)128,
    (float)1/(float)128,
    (float)2/(float)128,
    (float)3/(float)128,
    (float)4/(float)128,
    (float)5/(float)128,
    (float)6/(float)128,
    (float)7/(float)128,
    (float)8/(float)128,
    (float)9/(float)128,
    (float)10/(float)128,
    (float)11/(float)128,
    (float)12/(float)128,
    (float)13/(float)128,
    (float)14/(float)128,
    (float)15/(float)128,
    (float)16/(float)128,
    (float)17/(float)128,
    (float)18/(float)128,
    (float)19/(float)128,
    (float)20/(float)128,
    (float)21/(float)128,
    (float)22/(float)128,
    (float)23/(float)128,
    (float)24/(float)128,
    (float)25/(float)128,
    (float)26/(float)128,
    (float)27/(float)128,
    (float)28/(float)128,
    (float)29/(float)128,
    (float)30/(float)128,
    (float)31/(float)128,
    (float)32/(float)128,
    (float)33/(float)128,
    (float)34/(float)128,
    (float)35/(float)128,
    (float)36/(float)128,
    (float)37/(float)128,
    (float)38/(float)128,
    (float)39/(float)128,
    (float)40/(float)128,
    (float)41/(float)128,
    (float)42/(float)128,
    (float)43/(float)128,
    (float)44/(float)128,
    (float)45/(float)128,
    (float)46/(float)128,
    (float)47/(float)128,
    (float)48/(float)128,
    (float)49/(float)128,
    (float)50/(float)128,
    (float)51/(float)128,
    (float)52/(float)128,
    (float)53/(float)128,
    (float)54/(float)128,
    (float)55/(float)128,
    (float)56/(float)128,
    (float)57/(float)128,
    (float)58/(float)128,
    (float)59/(float)128,
    (float)60/(float)128,
    (float)61/(float)128,
    (float)62/(float)128,
    (float)63/(float)128,
    (float)64/(float)128,
    (float)65/(float)128,
    (float)66/(float)128,
    (float)67/(float)128,
    (float)68/(float)128,
    (float)69/(float)128,
    (float)70/(float)128,
    (float)71/(float)128,
    (float)72/(float)128,
    (float)73/(float)128,
    (float)74/(float)128,
    (float)75/(float)128,
    (float)76/(float)128,
    (float)77/(float)128,
    (float)78/(float)128,
    (float)79/(float)128,
    (float)80/(float)128,
    (float)81/(float)128,
    (float)82/(float)128,
    (float)83/(float)128,
    (float)84/(float)128,
    (float)85/(float)128,
    (float)86/(float)128,
    (float)87/(float)128,
    (float)88/(float)128,
    (float)89/(float)128,
    (float)90/(float)128,
    (float)91/(float)128,
    (float)92/(float)128,
    (float)93/(float)128,
    (float)94/(float)128,
    (float)95/(float)128,
    (float)96/(float)128,
    (float)97/(float)128,
    (float)98/(float)128,
    (float)99/(float)128,
    (float)100/(float)128,
    (float)101/(float)128,
    (float)102/(float)128,
    (float)103/(float)128,
    (float)104/(float)128,
    (float)105/(float)128,
    (float)106/(float)128,
    (float)107/(float)128,
    (float)108/(float)128,
    (float)109/(float)128,
    (float)110/(float)128,
    (float)111/(float)128,
    (float)112/(float)128,
    (float)113/(float)128,
    (float)114/(float)128,
    (float)115/(float)128,
    (float)116/(float)128,
    (float)117/(float)128,
    (float)118/(float)128,
    (float)119/(float)128,
    (float)120/(float)128,
    (float)121/(float)128,
    (float)122/(float)128,
    (float)123/(float)128,
    (float)124/(float)128,
    (float)125/(float)128,
    (float)126/(float)128,
    (float)127/(float)128
};

int      in   = 0;

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
    float   cal[128];
    int     i;
	/**********************************
	* Start user-specific code here. */
    trigger_high();
    for(i = 0 ; i < 128 ; i++) {
        cal[i] = wt[in] * ip[i];
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
    
    in += 1;
    
    pt[0]  = (char)cal[0] ^ (char)cal[16];
    pt[1]  = (char)cal[1] ^ (char)cal[17];
    pt[2]  = (char)cal[2] ^ (char)cal[18];
    pt[3]  = (char)cal[3] ^ (char)cal[19];
    pt[4]  = (char)cal[4] ^ (char)cal[20];
    pt[5]  = (char)cal[5] ^ (char)cal[21];
    pt[6]  = (char)cal[6] ^ (char)cal[22];
    pt[7]  = (char)cal[7] ^ (char)cal[23];
    pt[8]  = (char)cal[8] ^ (char)cal[24];
    pt[9]  = (char)cal[9] ^ (char)cal[25];
    pt[10] = (char)cal[10] ^ (char)cal[26];
    pt[11] = (char)cal[11] ^ (char)cal[27];
    pt[12] = (char)cal[12] ^ (char)cal[28];
    pt[13] = (char)cal[13] ^ (char)cal[29];
    pt[14] = (char)cal[14] ^ (char)cal[30];
    pt[15] = (char)cal[15] ^ (char)cal[31];
    
	simpleserial_put('r', 16, pt);
    
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
