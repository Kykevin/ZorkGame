#pragma once

#include "Room.h"
#include "Item.h"
#include "Container.h"
#include "Creature.h"
#include "Trigger.h"

#include "rapidxml.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <vector>

using namespace rapidxml;
using namespace std;

class Game;

class ScriptManager
{
public:
	ScriptManager();
	virtual ~ScriptManager();
	void parseFile(const char * fileName, vector<Room>& rooms, vector<Item>& items,
		vector<Container>& containers, vector<Creature>& creatures, vector<Trigger>& triggers);
	xml_document<> doc;
	xml_node<> * root_node;
private:
	void parseRoom(const xml_node<> * currNode, Room& room);
	void parseItem(const xml_node<> * currNode, Item& item);
	void parseContainer(const xml_node<> * currNode, Container& container);
	void parseCreature(const xml_node<> * currNode, Creature& creature);
	void parseTrigger(const xml_node<> * currNode, Trigger& trigger);
};

