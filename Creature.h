#pragma once
#include "Object.h"
#include "Trigger.h"

#include <vector>

using namespace std;

class Creature :
	public Object
{
public:
	Creature(){}
	virtual ~Creature(){}
	bool isVulnerableTo(string itemName){
		for (vector<string>::iterator it = vulnerability.begin(); it != vulnerability.end(); ++it){
			if ((*it) == itemName) return true;
		}
		return false;
	}
	vector<pair<string, string> > * getAttackActions() { return &attackActions; }
	vector<Trigger> * getTriggers(){
		return &triggers;
	}
	void removeTigger(const vector<Trigger>::iterator& it){
		triggers.erase(it);
	}

private:
	vector<string> vulnerability;
	vector<pair<string, string> > attackActions;
	vector<Trigger> triggers;

	friend class ScriptManager;
};

