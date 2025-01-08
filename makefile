.PHONY: compile run clean test
compile femto: main.o cutpaste.o filehandle.o  statdisplay.o undoredo.o writesave.o
	@gcc -o femto main.o cutpaste.o filehandle.o  statdisplay.o undoredo.o writesave.o -lncurses
run: femto
	@./femto $(ARGS)
clean:
	rm -f *.o femto test_main

main.o: main.c cutpaste.h filehandle.h  statdisplay.h undoredo.h writesave.h
	@gcc -c main.c cutpaste.h filehandle.h  statdisplay.h undoredo.h writesave.h
cutpaste.o: cutpaste.h cutpaste.c
	@gcc -c cutpaste.c
filehandle.o: filehandle.h filehandle.c
	@gcc -c filehandle.c
statdisplay.o: statdisplay.h statdisplay.c
	@gcc -c statdisplay.c
undoredo.o: undoredo.h undoredo.c
	@gcc -c undoredo.c
writesave.o: writesave.h writesave.c
	@gcc -c writesave.c -lncurses

test_main: test_main.o cutpaste.o filehandle.o  statdisplay.o undoredo.o writesave.o
	@gcc -o test_main test_main.o cutpaste.o filehandle.o  statdisplay.o undoredo.o writesave.o
test: test_main
	@./test_main $(ARGS)
test_main.o: test_main.c cutpaste.h filehandle.h  statdisplay.h undoredo.h writesave.h
	@gcc -c test_main.c cutpaste.h filehandle.h  statdisplay.h undoredo.h writesave.h
