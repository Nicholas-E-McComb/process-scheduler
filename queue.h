/* queue.h */

/* Queue structure declarations, constants, and inline functions	*/


#define	EMPTY (-1)		/* null value for qnext or qprev index	*/
#define SYSERR (-1)

typedef unsigned int fte;

struct queue
{
	struct qentry *head; 
	struct qentry *tail; 
	int	size;	
};


struct qentry
{
	fte key;	
	struct qentry *next;
	struct qentry *prev;
};


/* Queue function prototypes (don't touch!) */

int	isempty(struct queue *q);
int	nonempty(struct queue *q);
int	isfull(struct queue *q);

fte	getfirst(struct queue *q);
fte	getlast(struct queue *q);
fte	q_remove(fte key, struct queue *q);
fte	dequeue(struct queue *q);
fte	enqueue(fte key, struct queue *q);
struct queue	*newqueue();
struct qentry	*getbykey(fte key, struct queue *q);