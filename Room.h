#pragma once

#include "Object.h"
#include "Trigger.h"

#include <vector>
#include <map>

using namespace std;

class Room :
	public Object
{
public:
	Room();
	virtual ~Room();
	//using Object::getName;
	string checkBorder(string direction);
	bool hasItem(string itemName);
	void addItem(string itemName){
		items.push_back(itemName);
	}
	void removeItem(string itemName);

	bool hasContainer(string containerName);
	bool hasCreature(string creatureName);
	bool isExit(){ 
		if (type == "exit") {
			return true;
		}
		return false;
	}
	vector<Trigger> * getTriggers(){
		return &triggers;
	}
	void removeTigger(const vector<Trigger>::iterator& it){
		triggers.erase(it);
	}
	void addContainer(string containerName){
		containers.push_back(containerName);
	}
	void addCreature(string creatureName){
		creatures.push_back(creatureName);
	}
	vector<string> * getContainers(){ return &containers; }
	vector<string> * getItems(){ return &items; }
	vector<string> * getCreatures(){ return &creatures; }

	virtual bool findChildren(string& name);

	virtual void deleteChild(string& name);

	map<string, string> * getBorders(){ return &borders; }
private:
	string type;
	map<string,string> borders;
	vector<string> containers;
	vector<string> items;
	vector<string> creatures;
	vector<Trigger> triggers;

	friend class ScriptManager;
};

