#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "vmm.h"
#include "prints.h"
#include "replacement.h"
#include "commands.h"


FILE *fp;



int main(int argc, char** argv){
	// Check if instruction size is <= 32
	int block_log = log2(block_size);
	if(log2(num_pages) + block_log>=32){
		printf("Error: Instructions must be <= 32 bits\n");
		exit(0);
	}

	if(argc < 2){
		printf("Error: You need a file input\n");
		exit(0);
	}
	else{
		fp = argv[1];
	}
	int i;

	// If replacement policy is fifo, initialize the queue
	if(!strcmp(rep_policy, "fifo")){
		fifo_q = newqueue();
	}
	else if(!strcmp(rep_policy,"random")){
		srand(time(NULL));
	}
	else if(!strcmp(rep_policy, "clock")){
		clock_hand = 0;
	}

	// Set all entries of frame table to be free - 1 followed by 31 zeroes
	for(i=0;i<num_frames;i++){
		frame_table[i] = FREE_FRAME;
	}

	fp = fopen(argv[1],"r");
	char command[5];
	uint address;
	char line[256];
	char buffer[256];

	//printf("starting tokenization\n");
    while(fgets(line, sizeof(line), fp)){
    	printf("About to tokenize line\n");
		strcpy(buffer,line);
		char *p = strtok(buffer, " \t\n");

		if(!strcmp(p,"PP")){
			print_pt();
			p = strtok(NULL, " \t\n");
		}
		else if(!strcmp(p,"PF")){
			print_ft();
			p = strtok(NULL, " \t\n");
		}
		else{
			for(i=0;i<2;i++){
				//printf("Token reading\n");
				//printf("%s\n",p);
				if(i==0){
					strcpy(command,p);
					//printf("command: %s\n ",command);
				}
				else{
					//printf("Trying to get virtual address\n");
					address = strtoul(p,NULL,16);
					//printf("String to uint successful\n");
					address >>= block_log;
				}
				//if(i<2) p = strtok(NULL, " \t\n");
				p = strtok(NULL, " \t\n");
				//printf("\n");
			}
			if(!strcmp(command,"R")) read(address);
			else if(!strcmp(command,"W")) write(address);
			else{
				printf("Error: invalid command\n");
				//exit(0);
			}
			printf("\n");
			//p = strtok(NULL, " \t\n");
		}
		printf("Got here\n");
    }

	//print_pt();
	return 0;
}