#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "prints.h"
#include "vmm.h"

void print_pt(){
	int i;
	uint v,m,r,frame;
	pte current;
	int frame_bits = log2(block_size);

	printf("Page    | V, M, R, Frame\n");


	for(i=0;i<num_pages;i++){
		// Operations to find various bits
		current = page_table[i];
		v = current;
		v >>= 31;

		// If the entry is valid,
		if(v){
			m = current & MCHECK;
			m >>= 30;
			//m >>= 30;
			//m <<= 1;

			r = current & RCHECK;
			r >>= 29;
			//r >>= 29;
			//r <<= 2;

			frame = current;
			frame <<= 32-3-frame_bits;
			frame >>= 32-3-frame_bits;
			//frame >>= 32-3-frame_bits;

			printf("%d    	| %u, %u, %u, %u\n",i,v,m,r,frame);
		}
		else{
			printf("%d    	| %u, -, -, -\n",i,v);
		}	
	}
	printf("\n");
}

void print_ft(){
	int i,f,page;
	fte current;
	int page_bits = log2(block_size);

	printf("Frame    | F, Page\n");

	for(i=0;i<num_frames;i++){
		current = frame_table[i];
		f = current;
		f >>= 31;

		// If the frame is free
		if(f){
			printf("%d    	 | %u, -\n",i,f);
		}
		else{
			page = current;
			page <<= 32-1-page_bits;
			page >>= 32-1-page_bits;
			printf("%d    	 | %u, %u\n", i, f, page);
		}
	}
	printf("\n");
}