#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "vmm.h"
#include "replacement.h"

fte victim; 
int block_log;


/* Evict the chosen frame and take care of requisite bookkeeping
 *
 * @param v_index	index in frame table of the victim frame (chosen through replacement policy)
 */
void evict(uint v_index){
	uint p_index = frame_table[v_index] << (32-1-block_log);
	p_index >>= (32-1-block_log);

	//printf("Replacement: evicted_page=%i [frame %i], ", p_index, v_index);

	if(page_table[p_index] & MCHECK) printf("\tReplacement: evicted_page=%i [frame %i], writeout: TRUE\n", p_index, v_index);
	else printf("\tReplacement: evicted_page=%i [frame %i], writeout: FALSE\n", p_index, v_index);
	// invalidate the page corresponding to the frame
	page_table[p_index] &= 0;
	replacements++;
}

uint rand_replace(){
	// Select a random number between 0 and num_frames-1
	// evict(frame_table[num]);

	r = rand() % (num_frames);
	//printf("Random number is %d\n",r);
	evict(r);
	return r;
}

uint fifo_replace(){
	int v = getlast(fifo_q);
	evict(v);
	return v;
}

uint lru_replace(){

}

uint clock_replace(){
	// Infinite loop - breaks out once it finds a "cold" entry
	while(1){
		// If page_table index of clock hand is hot,
		if(page_table[clock_hand] & RCHECK){
			// Turn the entry at clock_hand to cold
			page_table[clock_hand] &= CHILL;
			//printf("Chill out\n");
			if(clock_hand == num_frames-1) clock_hand = 0;
			else {
				clock_hand++;
				printf("Clock hand is now at %d\n",clock_hand);
			}
		}
		else{
			evict(clock_hand);
			printf("Clock hand is at %d\n",clock_hand);
			page_table[clock_hand] |= RCHECK;
			return clock_hand;
		}
	}
}

uint replacement(){
	block_log = log2(block_size);
	uint vframe;
	if(!strcmp(rep_policy, "random")){
		//printf("Replacement policy is random\n");
		vframe = (rand_replace());
	}
	else if(!strcmp(rep_policy, "fifo")){
		//printf("Replacement policy is fifo\n");
		vframe = (fifo_replace());
	}
	else if(!strcmp(rep_policy, "lru")){
		//printf("Replacement policy is LRU\n");
		vframe = (lru_replace());
	}
	else if(!strcmp(rep_policy, "clock")){
		//printf("Replacement policy is CLOCK\n");
		vframe = (clock_replace());
	}
	else{
		printf("Error: Invalid replacement policy\n");
		exit(0);
	}
	return vframe;
}