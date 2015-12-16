#include "Room.h"
#include <algorithm>


Room::Room()
{

}


Room::~Room()
{
}

string Room::checkBorder(string direction){
	if (borders.find(direction) != borders.end()){
		return borders.find(direction)->second;
	}
	return "";
}

bool Room::hasItem(string itemName){
	vector<string>::iterator it;
	for (it = items.begin(); it != items.end(); ++it){
		if ((*it) == itemName) break;
	}
	if (it != items.end()){
		return true;
	}
	return false;
}



void Room::removeItem(string itemName){
	vector<string>::iterator it;
	for (it = items.begin(); it != items.end(); ++it){
		if ((*it) == itemName) break;
	}
	if (it != items.end()){
		items.erase(it);
	}
}

bool Room::hasContainer(string containerName){
	vector<string>::iterator it;
	for (it = containers.begin(); it != containers.end(); ++it){
		if ((*it) == containerName) break;
	}
	if (it != containers.end()){
		return true;
	}
	return false;
}
bool Room::hasCreature(string creatureName){
	vector<string>::iterator it;
	for (it = creatures.begin(); it != creatures.end(); ++it){
		if ((*it) == creatureName) break;
	}
	if (it != creatures.end()){
		return true;
	}
	return false;
}

bool Room::findChildren(string& name){
	if (hasItem(name)){ return true; }
	vector<string>::iterator it;
	for (it = containers.begin(); it != containers.end(); ++it){
		if ((*it) == name) break;
	}
	if (it != containers.end()){
		return true;
	}
	for (it = creatures.begin(); it != creatures.end(); ++it){
		if ((*it) == name) break;
	}
	if (it != creatures.end()){
		return true;
	}
	return false;
}

void Room::deleteChild(string& name){
	vector<string>::iterator it;
	for (it = items.begin(); it != items.end(); ++it){
		if ((*it) == name) break;
	}
	if (it != items.end()){ 
		items.erase(it);
		return; 
	}
	for (it = containers.begin(); it != containers.end(); ++it){
		if ((*it) == name) break;
	}
	if (it != containers.end()){
		containers.erase(it);
		return;
	}
	for (it = creatures.begin(); it != creatures.end(); ++it){
		if ((*it) == name) break;
	}
	if (it != creatures.end()){
		creatures.erase(it);
		return;
	}
	return;
}