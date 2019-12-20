CC=g++
FLAGS=-g

all: 
	$(CC) $(FLAGS) main.cpp  assembler.cpp -o test.out

clean:
	rm -f *.out
