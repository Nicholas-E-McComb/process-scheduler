#include "queue.h"

//int num_pages, num_frames, block_size;
#define num_pages 8
#define num_frames 4
#define block_size 1024
#define rep_policy "clock"
#define FREE_FRAME 0x80000000
#define VCHECK 0x80000000
#define MCHECK 0x40000000
#define RCHECK 0x20000000
#define READIT 0xA0000000
#define WROTEIT 0xE0000000
#define CHILL 0xDFFFFFFF

//char[10] rep_policy; // Given that the longest replacement policy we're supposed to implement is 6 characters, 
					 // 10 should be plenty to deal with improperly formed inputs

typedef unsigned int uint;

typedef unsigned int pte;
pte page_table[num_pages-1];

typedef unsigned int fte;
fte frame_table[num_frames-1];

int inst_count, r_count, w_count, page_faults, replacements, r, clock_hand;

struct queue* fifo_q;