CC = g++
CFLAGS = -Wall
DEPS = Constants.h Container.h Creature.h Game.h Item.h Object.h rapidxml.hpp Room.h ScriptManager.h Trigger.h 
OBJ = Container.o Game.o Room.o ScriptManager.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

Room.o: Room.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

ScriptManager.o: ScriptManager.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

Container.o: Container.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

Game.o: Game.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

zork: $(OBJ)
	$(CC) -o zork $(OBJ)