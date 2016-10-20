LIB=-L.
INC=-I.
SRC=hw3.cpp glinit.cpp GLee.c utils.cpp	

all:
	gcc -c GLee.c
	g++ -c utils.cpp
	g++ -c glinit.cpp
	g++ hw3.cpp $(LIB) $(INC) -o hw3 GLee.o utils.o glinit.o -lGL -lGLU -lglut -ljpeg -lpthread
	
dist:
	@tar -czvf hw3.tar.gz $(SRC) Makefile --exclude "*~"
