
build: tema1.c MagicBand.h
	gcc -Wall tema1.c -o tema1

run: build
	./tema1

clean:
	rm -f tema1 

valgrind: 
	valgrind --leak-check=yes ./tema1