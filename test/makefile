assembler: program.o compiler.o firstRun.o decodeLine.o preCompiler.o
	gcc -Wall -ansi -pedantic -g program.o compiler.o firstRun.o decodeLine.o preCompiler.o -o assembler -lm
program.o: program.c program.h
	gcc -c -ansi -Wall -pedantic program.c -o program.o
compiler.o: compiler.c compiler.h
	gcc -c -ansi -Wall -pedantic compiler.c -o compiler.o
firstRun.o: firstRun.c firstRun.h
	gcc -c -ansi -Wall -pedantic firstRun.c -o firstRun.o
decodeLine.o: decodeLine.c decodeLine.h
	gcc -c -ansi -Wall -pedantic decodeLine.c -o decodeLine.o
preCompiler.o: preCompiler.c preCompiler.h
	gcc -c -ansi -Wall -pedantic preCompiler.c -o preCompiler.o
clean:
	rm *.o
