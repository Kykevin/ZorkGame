#pragma once
#include "Object.h"

#include <map>
#include <vector>

class Trigger :
	public Object
{
public:
	Trigger(){
		active = true;
	}
	virtual ~Trigger(){};
	bool isSingle() { return single; }
	void deactive() { active = false; }
	bool getActiveStatus(){ return active; }
private:
	map<string, string> conditions;
	vector<string> commands;
	vector<pair<string,string> > actions;
	bool single;
	bool active;
	friend class ScriptManager;
	friend class Game;
};

