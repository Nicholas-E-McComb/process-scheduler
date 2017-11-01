#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "vmm.h"
#include "commands.h"
#include "replacement.h"

void read(uint address){
	int i, assigned;
	uint frame_index;
	printf("START_READ\n");
	if(address < 0 || address >= num_pages){
		printf("\tSegmentation fault: illegal_page = %i\n",address);
		return;
	}
	assigned = 0;
	r_count++;
	printf("\tvirt_address = %x\n", address);
	// If the page is valid
	if(page_table[address] & VCHECK){
		assigned = 1;
		// do translation and such
		printf("\tphys_address = %x\n", page_table[address] << (uint) (32-3-log2(block_size)));
		page_table[address] |= READIT;

	}
	else{
		printf("\tPage fault: page = %u\n",address);
		page_faults++;
		// Check frame table to look for free frames
		for(i=0;i<num_frames;i++){

			// There is a free frame
			if(frame_table[i] & FREE_FRAME){
				// TODO - allocation, updating page table/frame table, and translate - translate() streamline?
				assigned = 1;
				frame_index = i;

				// Set the F bit to zero in that frame then |= it with the index (update frame table)
				frame_table[i] &= 0;
				frame_table[i] |= address;

				// Update the page table
				page_table[address] |= READIT;
				page_table[address] |= i;

				printf("\tphys_address = %x\n",frame_index);
			}
			// If the process has been assigned a frame, break from checking - you chose the first free frame
			if(assigned) break;
		}
		// There were no free frames - replacement time
		if(!assigned){
			frame_index = replacement();

			frame_table[frame_index] &= 0;
			frame_table[frame_index] |= address; 

			page_table[address] |= READIT;
			page_table[address] |= frame_index;
		}
		if(!strcmp(rep_policy, "fifo")){
			enqueue(frame_index, fifo_q);
		}
	}
	printf("END_READ\n");
}

void write(uint address){
	int i, assigned;
	uint frame_index;
	printf("START_WRITE\n");
	if(address < 0 || address >= num_pages){
		printf("\tSegmentation fault: illegal_page = %i\n",address);
		printf("END_WRITE\n");
		return;
	}
	assigned = 0;
	w_count++;
	// If the page is valid
	printf("\tvirt_address = %x\n", address);
	// If the page is valid
	if(page_table[address] & VCHECK){
		assigned = 1;
		// do translation and such
		printf("\tphys_address = %x\n", page_table[address] << (uint) (32-3-log2(block_size)));
		page_table[address] |= WROTEIT;

	}
	else{
		printf("\tPage fault: page = %u\n",address);
		// Check frame table to look for free frames
		for(i=0;i<num_frames;i++){
			// There is a free frame
			if(frame_table[i] & FREE_FRAME){
				// TODO - allocation, updating page table/frame table, and translate - translate() streamline?
				assigned = 1;
				frame_index = i;

				// Set the F bit to zero in that frame then |= it with the index (update frame table)
				frame_table[i] &= 0;
				frame_table[i] |= address;

				// Update the page table
				page_table[address] |= WROTEIT;
				page_table[address] |= frame_index;

				printf("\tphys_address = %x\n",frame_index);
			}
			// If the process has been assigned a frame, break from checking - you chose the first free frame
			if(assigned) break;
		}
		// There were no free frames - replacement time
		if(!assigned){
			//printf("Replacement: \n");
			frame_index = replacement();
			printf("\tframe index: %d\n", frame_index);
			frame_table[frame_index] &= 0;
			frame_table[frame_index] |= address; 

			page_table[address] |= WROTEIT;
			page_table[address] |= frame_index;
			
		}
		if(!strcmp(rep_policy, "fifo")){
			enqueue(frame_index, fifo_q);
		}
	}
	printf("END_WRITE\n");
}