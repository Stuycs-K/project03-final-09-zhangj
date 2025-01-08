.PHONY: compile run clean test
compile femto: main.o cutpaste.o filehandle.o savequit.o statdisplay.o undoredo.o writesave.o
	@gcc -o femto main.o cutpaste.o filehandle.o savequit.o statdisplay.o undoredo.o writesave.o
run: femto
	@./femto
clean:
	rm -f *.o femto test_main

main.o: main.c cutpaste.h filehandle.h savequit.h statdisplay.h undoredo.h writesave.h
	@gcc -c main.c cutpaste.h filehandle.h savequit.h statdisplay.h undoredo.h writesave.h
cutpaste.o: cutpaste.h
	@gcc -c cutpaste.c
filehandle.o: filehandle.h
	@gcc -c filehandle.c
savequit.o: savequit.h
	@gcc -c savequit.c
statdisplay.o: statdisplay.h
	@gcc -c statdisplay.c
undoredo.o: undoredo.h
	@gcc -c undoredo.c
writesave.o: writesave.h
	@gcc -c writesave.c
	
test_main: test_main.o cutpaste.o filehandle.o savequit.o statdisplay.o undoredo.o writesave.o
	@gcc -o test_main test_main.o cutpaste.o filehandle.o savequit.o statdisplay.o undoredo.o writesave.o
test: test_main
	@./test_main
test_main.o: test_main.c cutpaste.h filehandle.h savequit.h statdisplay.h undoredo.h writesave.h
	@gcc -c test_main.c cutpaste.h filehandle.h savequit.h statdisplay.h undoredo.h writesave.h
