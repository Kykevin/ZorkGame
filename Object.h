#pragma once


#include <string>

using namespace std;

class Object
{
public:
	Object(){}
	~Object(){}
	string getName(){
		return name;
	}
	string getStatus(){
		return status;
	}
	void setStatus(string& status){
		this->status = status;
	}
	string getDescription(){
		return description;
	}
	virtual bool findChildren(string& name){ return false; }
	virtual void deleteChild(string& name){ return; }
	string objType;
protected:
	string name;
	string status;
	string description;
	friend class ScriptManager;
};

