.PHONY: compile run clean test
compile femto: main.o cutpaste.o filehandle.o  statdisplay.o undoredo.o writesave.o
	@gcc -o femto main.o cutpaste.o filehandle.o  statdisplay.o undoredo.o writesave.o -lncurses
run: femto
	@./femto $(ARGS)
clean:
	rm -f *.o *.h.gch femto

main.o: main.c cutpaste.h filehandle.h statdisplay.h undoredo.h writesave.h
	@gcc -c main.c cutpaste.h filehandle.h statdisplay.h undoredo.h writesave.h -lncurses
cutpaste.o: cutpaste.h cutpaste.c
	@gcc -c cutpaste.c -lncurses
filehandle.o: filehandle.h filehandle.c
	@gcc -c filehandle.c -lncurses
statdisplay.o: statdisplay.h statdisplay.c
	@gcc -c statdisplay.c -lncurses
undoredo.o: undoredo.h undoredo.c
	@gcc -c undoredo.c -lncurses
writesave.o: writesave.h writesave.c
	@gcc -c writesave.c -lncurses
