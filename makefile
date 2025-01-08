.PHONY: compile run clean
compile femto: main.o cutpaste.o filehandle.o savequit.o statdisplay.o undoredo.o writesave.o
	@gcc -o femto main.o cutpaste.o filehandle.o savequit.o statdisplay.o undoredo.o writesave.o
run: femto
	@./femto
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
clean:
	rm -f *.o femto