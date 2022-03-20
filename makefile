assembler: assembler.o secondRun.o firstRun.o decodeLine.o binaryLine.o preAssembler.o symbol.o
	gcc -Wall -ansi -pedantic -g assembler.o secondRun.o firstRun.o decodeLine.o symbol.o binaryLine.o preAssembler.o -o assembler -lm
assembler.o: assembler.c assembler.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o
secondRun.o: secondRun.c secondRun.h
	gcc -c -ansi -Wall -pedantic secondRun.c -o secondRun.o
firstRun.o: firstRun.c firstRun.h
	gcc -c -ansi -Wall -pedantic firstRun.c -o firstRun.o
decodeLine.o: decodeLine.c decodeLine.h
	gcc -c -ansi -Wall -pedantic decodeLine.c -o decodeLine.o
binaryLine.o: binaryLine.c binaryLine.h
	gcc -c -ansi -Wall -pedantic binaryLine.c -o binaryLine.o
preAssembler.o: preAssembler.c preAssembler.h
	gcc -c -ansi -Wall -pedantic preAssembler.c -o preAssembler.o
symbol.o: symbol.c symbol.h
	gcc -c -ansi -Wall -pedantic symbol.c -o symbol.o
clean:
	rm *.o
remove:
	rm **/*.ob **/*.ext **/*.ent **/*.am
