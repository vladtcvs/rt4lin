OF=character.o  data_structures.o  draw.o  enemy.o  init.o  life_bonus.o  main.o  menu.o  passive.o  ship.o wall.o
HF=data_structures.h  draw.h  init.h  menu.h  ship.h wall.h

INC=/usr/include/SDL2/
LD=-lpng -lSDL2 -lSDL2_ttf

CXX=clang++ -I$(INC) -g

OUT=rt4lin

all: rt4lin


main.o: main.cc $(HF)
	$(CXX) -c main.cc


wall.o: wall.cc $(HF)
	$(CXX) -c wall.cc

ship.o: ship.cc $(HF)
	$(CXX) -c ship.cc

draw.o: draw.cc $(HF)
	$(CXX) -c draw.cc

enemy.o: enemy.cc $(HF)
	$(CXX) -c enemy.cc

character.o: character.cc $(HF)
	$(CXX) -c character.cc

init.o: init.cc $(HF)
	$(CXX) -c init.cc

data_structures.o: data_structures.cc $(HF)
	$(CXX) -c data_structures.cc

passive.o: passive.cc $(HF)
	$(CXX) -c passive.cc

menu.o: menu.cc $(HF)
	$(CXX) -c menu.cc

life_bonus.o: life_bonus.cc $(HF)
	$(CXX) -c life_bonus.cc



rt4lin: $(OF)
	$(CXX) -o rt4lin $(OF) $(LD) -g

clean:
	rm *.o

