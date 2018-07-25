all: clean cripto-test

cripto-test: simonalgo modes main
	gcc -o "cripto-test" *.o

main:
	gcc -c main.c

simonalgo:
	gcc -c simonalgo.c

modes:
	gcc -c modes.c

clean:
	rm -rf *.o
	rm -rf cripto
