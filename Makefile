CC = gcc -g

CFLAGS= 

LIBS= 

.PHONY:	interp	clean

dp:	main.o interp.o
	$(CC)	$(LIBS)	-o interp main.o interp.o

main.o:		main.c
	$(CC)	$(CFLAGS)	-c main.c

cmd.o:  interp.c interp.h
	$(CC)   $(CFLAGS)   -c interp.c

clean:
	-rm *.o
	-rm interp

