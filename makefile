assembler: program.o compiler.o secondRun.o firstRun.o decodeLine.o baseConvertion.o binaryLine.o preCompiler.o
	gcc -Wall -ansi -pedantic -g program.o compiler.o secondRun.o firstRun.o decodeLine.o baseConvertion.o binaryLine.o preCompiler.o -o assembler -lm
program.o: program.c program.h
	gcc -c -ansi -Wall -pedantic program.c -o program.o
compiler.o: compiler.c compiler.h
	gcc -c -ansi -Wall -pedantic compiler.c -o compiler.o
secondRun.o: secondRun.c secondRun.h
	gcc -c -ansi -Wall -pedantic secondRun.c -o secondRun.o
firstRun.o: firstRun.c firstRun.h
	gcc -c -ansi -Wall -pedantic firstRun.c -o firstRun.o
decodeLine.o: decodeLine.c decodeLine.h
	gcc -c -ansi -Wall -pedantic decodeLine.c -o decodeLine.o
baseConvertion.o: baseConvertion.c baseConvertion.h
	gcc -c -ansi -Wall -pedantic baseConvertion.c -o baseConvertion.o
binaryLine.o: binaryLine.c binaryLine.h
	gcc -c -ansi -Wall -pedantic binaryLine.c -o binaryLine.o
preCompiler.o: preCompiler.c preCompiler.h
	gcc -c -ansi -Wall -pedantic preCompiler.c -o preCompiler.o
clean:
	rm *.o
remove:
	rm **/*.ob **/*.ext **/*.ent **/*.am
