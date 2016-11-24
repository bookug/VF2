#compile parameters

CC = g++
CFLAGS = -c -Wall -fprofile-arcs -ftest-coverage -coverage #-pg #-O2
EXEFLAG = -fprofile-arcs -ftest-coverage -coverage #-pg #-O2
	 
#add -lreadline -ltermcap if using readline or objs contain readline
library = -lgcov -coverage

objdir = ./objs/
objfile = $(objdir)Util.o $(objdir)IO.o $(objdir)Search.o $(objdir)Pattern.o $(objdir)Graph.o $(objdir)Code.o

all: dig

dig: main/dig.cpp $(objfile)
	$(CC) $(EXEFLAG) -o dig main/dig.cpp $(objfile) $(library)

$(objdir)Util.o: util/Util.cpp util/Util.h
	$(CC) $(CFLAGS) util/Util.cpp -o $(objdir)Util.o

$(objdir)Code.o: util/Code.cpp util/Code.h
	$(CC) $(CFLAGS) util/Code.cpp -o $(objdir)Code.o

$(objdir)Graph.o: graph/Graph.cpp graph/Graph.h
	$(CC) $(CFLAGS) graph/Graph.cpp -o $(objdir)Graph.o

$(objdir)Pattern.o: graph/Pattern.cpp graph/Pattern.h
	$(CC) $(CFLAGS) graph/Pattern.cpp -o $(objdir)Pattern.o

$(objdir)Search.o: search/Search.cpp search/Search.h
	$(CC) $(CFLAGS) search/Search.cpp -o $(objdir)Search.o

$(objdir)IO.o: io/IO.cpp io/IO.h
	$(CC) $(CFLAGS) io/IO.cpp -o $(objdir)IO.o

.PHONY: clean dist tarball test sumlines

clean:
	rm -f $(objdir)*.o 
dist: clean
	rm -f *.txt dig

tarball:
	tar -czvf gSpan.tar.gz main util search io graph Makefile README.md objs script

test: main/test.o $(objfile)
	$(CC) $(EXEFLAG) -o test main/test.cpp $(objfile) $(library)

sumline:
	bash script/sumline.sh

