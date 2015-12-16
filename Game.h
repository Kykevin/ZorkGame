#pragma once

#include "ScriptManager.h"
#include "Room.h"
#include "Item.h"
#include "Creature.h"
#include "Container.h"
#include "Trigger.h"
#include "Object.h"



#include <vector>


class Game
{
public:
  Game(const char * fileName);
  virtual ~Game(){}

private:
	void startGame();
	void performActionQueue(vector<pair<string, string> > * actionQueue);
	void performCommand(string command);
	Object* findObject(string name);
	bool checkTrigger(string command);
	bool performTrigger(Trigger& trigger,string command);


	vector<Room> rooms;
	vector<Item> items;
	vector<Container> containers;
	vector<Creature> creatures;
	vector<Trigger> triggers;

	bool endGame;

	Room* presentRoom;
	Container* presentContainer;
	vector<string> inventory;


	ScriptManager scriptManager;

	friend class ScriptManager;
};

