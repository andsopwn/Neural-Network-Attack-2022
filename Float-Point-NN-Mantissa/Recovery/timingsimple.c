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
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "simpleserial.h"
#include "StructMLP.h"

int count = 0;
ld      layer[3];

void weight_initialization(ld *Layer, float setting, int index) {
    for(int i = 0 ; i < Layer[index].num ; i++)    
        Layer[index].nd[i].weight = setting;
}

void layer_initialization(ld *Layer) {
    for(int i = 0 ; i < LM ; i++) {
        Layer[i].nd = (nd*)calloc(sizeof(nd), Layer[i].num);
    }
    weight_initialization(Layer, 1, 0);
}

void layer_free(ld *Layer) {
    for(int i = 0 ; i < LM ; i++)    
        free(Layer[i].nd);
}

void node_output(ld *layer) {
    for(int i = 1 ; i < LM ; i++) {
        for(int j = 0 ; j < layer[i].num ; j++) {
            for(int k = 0 ; k < layer[i - 1].num ; k++)
                layer[i].nd[j].output += layer[i].nd[j].weight * layer[i - 1].nd[k].output;
        }
        
        for(int j = 0 ; j < layer[i].num ; j++)
            layer[i].nd[j].output = hypertan(layer[i].nd[j].output);
    }
}

void node_set1(ld *layer) {
    // weight setting 1
    layer[1].nd[0].weight = 1+(float)50/128;
    layer[1].nd[1].weight = (float)10/128;
    layer[1].nd[2].weight = (float)80/128;
    layer[1].nd[3].weight = 1+(float)0/128;
    layer[1].nd[4].weight = 1+(float)60/128;
    layer[2].nd[0].weight = 1+(float)30/128;
    layer[2].nd[1].weight = 1+(float)10/128;
    layer[2].nd[2].weight = 1+(float)40/128;
}


uint8_t get_key(uint8_t* k, uint8_t len)
{
	// Load key here
	return 0x00;
}

uint8_t get_pt(uint8_t* pt, uint8_t len)
{
    float    f[3];
    unsigned a[3];
    
    a[0] = (pt[0] & 0xff) | ((pt[1] << 8) & 0x0000ff00) | ((pt[ 2] << 16) & 0x00ff0000) | ((pt[ 3] << 24) & 0xff000000);
    a[1] = (pt[4] & 0xff) | ((pt[5] << 8) & 0x0000ff00) | ((pt[ 6] << 16) & 0x00ff0000) | ((pt[ 7] << 24) & 0xff000000);
    a[2] = (pt[8] & 0xff) | ((pt[9] << 8) & 0x0000ff00) | ((pt[10] << 16) & 0x00ff0000) | ((pt[11] << 24) & 0xff000000);
    //a[0] = pt[0] | (pt[1] << 8) | (pt[ 2] << 16) | (pt[ 3] << 24);
    //a[1] = pt[4] | (pt[5] << 8) | (pt[ 6] << 16) | (pt[ 7] << 24);
    //a[2] = pt[8] | (pt[9] << 8) | (pt[10] << 16) | (pt[11] << 24);
    
    memcpy((unsigned char*)&f[0], (unsigned char*)&a[0], 4);
    memcpy((unsigned char*)&f[1], (unsigned char*)&a[1], 4);
    memcpy((unsigned char*)&f[2], (unsigned char*)&a[2], 4);
    //layer[0].nd[0].output = 1+(float)65/128;
    //layer[0].nd[1].output = 1+(float)15/128;
    //layer[0].nd[2].output = 1+(float)80/128;
    layer[0].nd[0].output = f[0];
    layer[0].nd[1].output = f[1];
    layer[0].nd[2].output = f[2];
    count += 3;
    
	/**********************************
	* Start user-specific code here. */
	trigger_high();
    
    node_output(layer);     // trigger
    
	//16 hex bytes held in 'pt' were sent
	//from the computer. Store your response
	//back into 'pt', which will send 16 bytes
	//back to computer. Can ignore of course if
	//not needed

	trigger_low();
    
    layer_free(layer);
    
    pt[0]  = ((unsigned int)layer[2].nd[0].output >> 24) & 0xff;
    pt[1]  = ((unsigned int)layer[2].nd[0].output >> 16) & 0xff;
    pt[2]  = ((unsigned int)layer[2].nd[0].output >>  8) & 0xff;
    pt[3]  =  (unsigned int)layer[2].nd[0].output & 0xff;
    pt[4]  = ((unsigned int)layer[2].nd[1].output >> 24) & 0xff;
    pt[5]  = ((unsigned int)layer[2].nd[1].output >> 16) & 0xff;
    pt[6]  = ((unsigned int)layer[2].nd[1].output >>  8) & 0xff;
    pt[7]  =  (unsigned int)layer[2].nd[1].output & 0xff;
    pt[8]  = ((unsigned int)layer[2].nd[2].output >> 24) & 0xff;
    pt[9]  = ((unsigned int)layer[2].nd[2].output >> 16) & 0xff;
    pt[10] = ((unsigned int)layer[2].nd[2].output >>  8) & 0xff;
    pt[11] =  (unsigned int)layer[2].nd[2].output & 0xff;
	/* End user-specific code here. *
	********************************/
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
    layer[0].num = 3;
    layer[1].num = 5;
    layer[2].num = 3;
    
    layer_initialization(layer);
    node_set1(layer);
    
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
