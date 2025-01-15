.PHONY: compile run clean test
compile femto: main.o cutpaste.o filehandle.o  statdisplay.o undoredo.o writesave.o cursor.o
	@gcc -o femto main.o cutpaste.o filehandle.o statdisplay.o writesave.o cursor.o -lncurses -lm -Wall
run: femto
	@./femto $(ARGS)
clean:
	rm -f *.o *.h.gch femto

main.o: main.c cutpaste.h filehandle.h statdisplay.h undoredo.h writesave.h
	@gcc -c main.c cutpaste.h filehandle.h statdisplay.h undoredo.h writesave.h -lncurses -lm -Wall
cutpaste.o: cutpaste.h cutpaste.c
	@gcc -c cutpaste.c -lncurses -lm -Wall
filehandle.o: filehandle.h filehandle.c
	@gcc -c filehandle.c -lncurses -lm -Wall
statdisplay.o: statdisplay.h statdisplay.c
	@gcc -c statdisplay.c -lncurses -lm -Wall
writesave.o: writesave.h writesave.c
	@gcc -c writesave.c -lncurses -lm -Wall
cursor.o: cursor.h cursor.c
	@gcc -c cursor.c -lncurses -lm -Wall
