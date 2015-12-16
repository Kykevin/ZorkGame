#pragma once

#include "Object.h"
#include "Trigger.h"

#include <vector>


using namespace std;


class Item :
	public Object
{
public:
	Item(){}
	virtual ~Item(){}
	vector<pair<string, string> > * getTurnOnActions() { return &turnOnActions; }
	string getWritten(){ return writing; }
	vector<Trigger> * getTriggers(){
		return &triggers;
	}
	void removeTigger(const vector<Trigger>::iterator& it){
		triggers.erase(it);
	}
private:
	string writing;
	vector<pair<string, string> > turnOnActions;
	vector<Trigger> triggers;

	friend class ScriptManager;
};

