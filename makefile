runFile: decodeLine.o baseConvertion.o
	gcc -Wall -ansi -pedantic -g baseConvertion.o decodeLine.o -o runFile -lm
decodeLine.o: decodeLine.c decodeLine.h
	gcc -c -ansi -Wall -pedantic decodeLine.c -o decodeLine.o
baseConvertion.o: baseConvertion.c baseConvertion.h
	gcc -c -ansi -Wall -pedantic baseConvertion.c -o baseConvertion.o
clean:
	rm *.o
