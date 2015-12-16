#pragma once
#include "Object.h"
#include "Trigger.h"

#include <vector>
#include <string>

using namespace std;

class Container :
	public Object
{
public:
	Container();
	virtual ~Container();
	bool hasItem(string itemName);
	bool addItem(string itemName);
	vector<string>* getItems(){ return &items; };
	void removeItem(string itemName);

	bool isLocked(){ return locked; }
	vector<Trigger> * getTriggers(){
		return &triggers;
	}
	void removeTigger(const vector<Trigger>::iterator& it){
		triggers.erase(it);
	}
	virtual bool findChildren(string& name){ return hasItem(name); }
	virtual void deleteChild(string& name) { removeItem(name); }
private:
	bool locked;
	vector<string> accept;
	vector<string> items;
	vector<Trigger> triggers;

	friend class ScriptManager;
};

