all: vmm.c prints.o replacement.o commands.o
	gcc -Wall -o vmmu vmm.c prints.o replacement.o commands.o queue.c

prints.o: prints.h prints.c
	gcc -Wall -c prints.c

replacement.o: replacement.h replacement.c
	gcc -Wall -c replacement.c queue.c

commands.o: commands.h commands.c
	gcc -Wall -c commands.c queue.c

queue.o: queue.h queue.c
	gcc -Wall -c queue.c

clean: 
	rm -f hw7 *.o