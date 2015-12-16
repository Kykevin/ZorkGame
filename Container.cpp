#include "Container.h"
#include <algorithm>


Container::Container()
{
}


Container::~Container()
{
}

bool Container::hasItem(string itemName){
	for (vector<string>::iterator it = items.begin(); it != items.end(); ++it){
		if ((*it) == itemName) return true;
	}
	return false;
}

bool Container::addItem(string itemName){
	if (locked){
		bool chk = false;
		for (vector<string>::iterator it = accept.begin(); it != accept.end(); ++it){
			if ((*it) == itemName) { 
				locked = false;
				chk = true;
				items.push_back(itemName);
			}
		}
		return chk;
	}
	else{
		items.push_back(itemName);
		return true;
	}
}

void Container::removeItem(string itemName){
	bool chk = false;
	for (vector<string>::iterator it = items.begin(); it != items.end(); ++it){
		if ((*it) == itemName) {
			items.erase(it);
			return;
		}
	}
}