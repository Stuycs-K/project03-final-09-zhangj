.PHONY: compile run clean test
compile femto: main.o filebuffer.o statdisplay.o writesave.o cursor.o execprint.o filehandle.o
	@gcc -o femto main.o filebuffer.o statdisplay.o writesave.o cursor.o execprint.o filehandle.o -lncurses -lm -Wall
run: femto
	@./femto $(ARGS)
clean:
	rm -f *.o *.h.gch femto

main.o: main.c filebuffer.h statdisplay.h writesave.h cursor.h execprint.h filehandle.h
	@gcc -c main.c filebuffer.h statdisplay.h writesave.h cursor.h execprint.h filehandle.h -lncurses -lm -Wall
filebuffer.o: filebuffer.h filebuffer.c
	@gcc -c filebuffer.c -lncurses -lm -Wall
statdisplay.o: statdisplay.h statdisplay.c
	@gcc -c statdisplay.c -lncurses -lm -Wall
writesave.o: writesave.h writesave.c filebuffer.h
	@gcc -c writesave.c filebuffer.h -lncurses -lm -Wall
cursor.o: cursor.h cursor.c
	@gcc -c cursor.c -lncurses -lm -Wall
execprint.o: execprint.h execprint.c
	@gcc -c execprint.c -lncurses -lm -Wall
filehandle.o: filehandle.h filehandle.c statdisplay.h
	@gcc -c filehandle.c statdisplay.h -lncurses -lm -Wall
