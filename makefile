
main: main.o my_q.o sched.o
	gcc -o main main.o my_q.o sched.o

debug: main.o my_q.o sched.o
	gcc -g -ggdb -o main main.o my_q.o sched.o

main.o:
	gcc -O -Wall -c main.c

my_q.o: my_q.h
	gcc -O -Wall -c my_q.c

sched.o: sched.h
	gcc -O -Wall -c sched.c

clean:
	rm -f *.o main