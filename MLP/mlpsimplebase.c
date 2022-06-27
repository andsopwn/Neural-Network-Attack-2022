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
#include <math.h>

#include "simpleserial.h"
#include "StructMLP.h"

static int count = 0;

void weight_initialization(ld *Layer, float setting, int index) {
    for(int i = 0 ; i < Layer[index].num ; i++)    
        Layer[index].nd[i].weight = setting;
}

void layer_initialization(ld *Layer) {
    for(int i = 0 ; i < LAYERNUM ; i++) {
        layer[i].nd = (nd*)calloc(sizeof(nd), layer[i].num);
    }
    weight_initialization(Layer, 1, 0);
}

void layer_free(ld *Layer) {
    for(int i = 0 ; i < LAYERNUM ; i++)    
        free(Layer[i].nd);
}

void node_output(ld *layer) {
    for(int i = 1 ; i < LAYERNUM ; i++) {
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
    layer[1].nd[0].weight = -0.85;
    layer[1].nd[1].weight =  0.62;
    layer[1].nd[2].weight = -0.51;
    layer[1].nd[3].weight = -0.31;
    layer[1].nd[4].weight =  0.29;
    layer[1].nd[5].weight = -0.45;
    layer[2].nd[0].weight = -0.40;
    layer[2].nd[1].weight =  0.84;
}


uint8_t get_key(uint8_t* k, uint8_t len)
{
	// Load key here
	return 0x00;
}

uint8_t get_pt(uint8_t* pt, uint8_t len)
{
    
    layer_initialization(layer);
    node_set1(layer);
    
    layer[0].nd[0].output = pt[count];
    layer[0].nd[1].output = pt[count+1];
    layer[0].nd[2].output = pt[count+2];
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
	/* End user-specific code here. *
	********************************/
	simpleserial_put('r', 3, pt);
	return 0x00;
}

uint8_t reset(uint8_t* x, uint8_t len)
{
	// Reset key here if needed
	return 0x00;
}

int main(void)
{
    ld      layer[3];
    layer[0].num = 3;
    layer[1].num = 6;
    layer[2].num = 2;
    
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
