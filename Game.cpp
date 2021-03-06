#include "Game.h"

#include <iostream>
using namespace std;

Game::Game(const char * fileName)
{
  endGame = false;
	scriptManager.parseFile(fileName, rooms, items, containers, creatures, triggers);
	startGame();
}



void Game::startGame(){
	string command;
	presentRoom = (Room*)findObject("Entrance");
	cout << presentRoom->getDescription() << endl;
	while (true){
		if (endGame == true){
			return;
		}
		getline(cin, command);
		performCommand(command);
	}
}

void Game::performActionQueue(vector<pair<string, string> > * actionQueue){
	for (vector<pair<string, string> >::iterator it = actionQueue->begin();
		it != actionQueue->end(); ++it){
		if ((*it).first == "print"){
			cout << (*it).second << endl;
		}
	}
	for (vector<pair<string, string> >::iterator it = actionQueue->begin();
		it != actionQueue->end(); ++it){
		if ((*it).first == "action"){
			performCommand((*it).second);
		}
	}
	return;
}

void Game::performCommand(string command){
	//cout << "--" << command << endl;
	if (endGame == true){
		return;
	}
	if (checkTrigger(command)){
		return;
	}
	//direction commands
	string direction = "";
	if (command == "n"){
		direction = "north";
	}
	else if (command == "s"){
		direction = "south";
	}
	else if (command == "e"){
		direction = "east";
	}
	else if (command == "w"){
		direction = "west";
	}

	if (direction != ""){
		string nextRoomName = presentRoom->checkBorder(direction);
		if (nextRoomName != ""){
			presentRoom = (Room*)findObject(nextRoomName);
			//			presentContainer = NULL;
			cout << presentRoom->getDescription() << endl;
		}
		else {
			cout << "You can't go that way." << endl;
		}
	}

	//"i" command
	else if (command == "i"){
		bool haveItem = false;
		bool firstItem = true;
		cout << "Inventory: ";
		for (vector<string>::iterator it = inventory.begin(); it != inventory.end(); ++it){
			if (!firstItem){
				cout << ", ";
			}
			cout << (*it);
			haveItem = true;
			firstItem = false;
		}
		if (!haveItem){
			cout << "empty";
		}
		cout << endl;
	}

	//"take" item
	else if (command.substr(0, 5) == "take "){
		string itemName = command.substr(5);
		if (presentRoom->hasItem(itemName)){
			inventory.push_back(itemName);
			presentRoom->removeItem(itemName);
			cout << "Item " << itemName << " added to inventory" << endl;
		}
		else if (presentContainer != NULL && presentContainer->hasItem(itemName)) {
			inventory.push_back(itemName);
			presentContainer->removeItem(itemName);
			cout << "Item " << itemName << " added to inventory" << endl;
		}
		else {
			cout << "You can't do that" << endl;
		}
	}

	else if (command == "Game Over"){
		cout << "Victory!" << endl;
		endGame = true;
	}
	//"open" container

	else if (command.substr(0, 5) == "open "){
		if (command == "open exit"){
			if (presentRoom->isExit()){
				cout << "Game Over" << endl;
				endGame = true;
			}
			else {
				cout << "No you can't do that. " << endl;
			}
		}
		else{
			string containerName = command.substr(5);
			if (presentRoom->hasContainer(containerName)){

				presentContainer = (Container*)findObject(containerName);
				if (!presentContainer->isLocked()){
					cout << containerName;
					vector<string> * items = presentContainer->getItems();
					bool haveItem = false;
					bool firstItem = true;
					for (vector<string>::iterator it = items->begin(); it != items->end(); ++it){
						if (!firstItem){
							cout << ", ";
						}
						else {
							cout << " contains ";
						}
						cout << (*it);
						haveItem = true;
						firstItem = false;
					}
					if (!haveItem){
						cout << " is empty";
					}
					cout << "." << endl;
				}
				else {
					presentContainer = NULL;
					cout << "You cant open that right now." << endl;
				}

			}
			else {
				cout << "No " << containerName << " existed here." << endl;
			}
		}
	}



	else if (command.substr(0, 5) == "read "){
		string itemName = command.substr(5);
		bool chk = false;
		for (vector<string>::iterator it = inventory.begin(); it != inventory.end(); ++it){
			if ((*it) == itemName){
				chk = true;
				break;
			}
		}
		if (chk){
			string print = ((Item*)findObject(itemName))->getWritten();
			if (print == ""){
				cout << "Nothing written." << endl;
			}
			else {
				cout << print << endl;
			}
		}
		else {
			cout << "You don't have this item." << endl;
		}
	}

	else if (command.substr(0, 5) == "drop "){
		string itemName = command.substr(5);
		bool chk = false;
		vector<string>::iterator it;
		for (it = inventory.begin(); it != inventory.end(); ++it){
			if ((*it) == itemName){
				chk = true;
				break;
			}
		}
		if (chk){
			presentRoom->addItem(itemName);
			inventory.erase(it);
			cout << itemName << " dropped." << endl;
		}
		else {
			cout << "You don't have this item." << endl;
		}
	}

	else if (command.substr(0, 4) == "put "){
		int inPos = command.find(" in ");
		string itemName = command.substr(4, inPos - 4);
		string containerName = command.substr(inPos + 4);
		vector<string>::iterator it;
		for (it = inventory.begin(); it != inventory.end(); ++it){
			if ((*it) == itemName){
				break;
			}
		}
		if (it != inventory.end()){
			if (presentRoom->hasContainer(containerName)){
				presentContainer = (Container*)findObject(containerName);
				if (presentContainer->addItem(itemName)){
					inventory.erase(it);
					cout << "Item " << itemName << " added to " << containerName << endl;
				}
				else{
					cout << "You cant add " << itemName << " to " << containerName << " now." << endl;
				}
				presentContainer = NULL;
			}
			else {
				cout << "No " << containerName << " existed here." << endl;
			}
		}
		else {
			cout << "You don't have this item." << endl;
		}
	}
	else if (command.substr(0, 4) == "Add "){
		int inPos = command.find(" to ");
		string objName = command.substr(4, inPos - 4);
		string containerName = command.substr(inPos + 4);
		Object* obj = findObject(objName);
		Object* owner = findObject(containerName);
		if (owner->objType == "container"){
			((Container*)owner)->addItem(objName);
		}
		else {
			if (obj->objType == "item"){
				((Room*)owner)->addItem(objName);
			}
			if (obj->objType == "container"){
				((Room*)owner)->addContainer(objName);
			}
			if (obj->objType == "creature"){
				((Room*)owner)->addCreature(objName);
			}
		}
	}
	else if (command.substr(0, 7) == "Delete "){
		string name = command.substr(7);
		if (findObject(name)->objType != "room"){
			for (vector<Room>::iterator it = rooms.begin(); it != rooms.end(); it++){
				(*it).deleteChild(name);
			}
			for (vector<Container>::iterator it = containers.begin(); it != containers.end(); it++){
				(*it).deleteChild(name);
			}
		}
		else {
			Room* room = (Room*)findObject(name);
			map<string, string>::iterator it;
			if (room->getBorders()->find("north") != room->getBorders()->end()){
				((Room*)findObject((*(room->getBorders()))["north"]))->getBorders()->erase("south");
			}
			if (room->getBorders()->find("south") != room->getBorders()->end()){
				((Room*)findObject((*(room->getBorders()))["south"]))->getBorders()->erase("north");
			}
			if (room->getBorders()->find("east") != room->getBorders()->end()){
				((Room*)findObject((*(room->getBorders()))["east"]))->getBorders()->erase("west");
			}
			if (room->getBorders()->find("west") != room->getBorders()->end()){
				((Room*)findObject((*(room->getBorders()))["west"]))->getBorders()->erase("east");
			}
		}
	}
	else if (command.substr(0, 7) == "Update "){
		int withPos = command.find(" to ");
		string object = command.substr(7, withPos - 7);
		string status = command.substr(withPos + 4);
		findObject(object)->setStatus(status);
	}

	else if (command.substr(0, 8) == "turn on "){
		string itemName = command.substr(8);
		vector<string>::iterator it;
		for (it = inventory.begin(); it != inventory.end(); ++it){
			if ((*it) == itemName){
				break;
			}
		}
		if (it != inventory.end()){
			vector<pair<string, string> > * queue = ((Item*)findObject(itemName))->getTurnOnActions();
			if (queue == NULL){
				cout << "Nothing happenes." << endl;
			}
			else {
				cout << "You activate the " << itemName << "." << endl;
				performActionQueue(queue);
			}
		}
		else {
			cout << "You don't have this item." << endl;
		}
	}

	else if (command.substr(0, 7) == "attack "){
		int withPos = command.find(" with ");
		string creatureName = command.substr(7, withPos - 7);
		string itemName = command.substr(withPos + 6);
		vector<string>::iterator it;
		for (it = inventory.begin(); it != inventory.end(); ++it){
			if ((*it) == itemName){
				break;
			}
		}
		if (it != inventory.end()){
			if (presentRoom->hasCreature(creatureName)){
				Creature * creature = (Creature*)findObject(creatureName);
				if (creature->isVulnerableTo(itemName)){
					vector<pair<string, string> > * queue = creature->getAttackActions();
					cout << "You assault the " << creatureName << " with the " << itemName << "." << endl;
					if (queue != NULL){
						performActionQueue(queue);
					}
				}
				else {
					cout << "Nothing happens." << endl;
				}
			}
			else {
				cout << "No " << creatureName << " here." << endl;
			}

		}
		else {
			cout << "You don't have this item." << endl;
		}
	}
	else {
		cout << "I don't understant your words." << endl;
	}
	checkTrigger("");
	return;
}

Object* Game::findObject(string name){
	for (vector<Room>::iterator it = rooms.begin(); it != rooms.end(); it++){
		if ((*it).getName() == name){
			return (Object*)&(*it);
		}
	}
	for (vector<Item>::iterator it = items.begin(); it != items.end(); it++){
		if ((*it).getName() == name){
			return (Object*)&(*it);
		}
	}
	for (vector<Container>::iterator it = containers.begin(); it != containers.end(); it++){
		if ((*it).getName() == name){
			return (Object*)&(*it);
		}
	}
	for (vector<Creature>::iterator it = creatures.begin(); it != creatures.end(); it++){
		if ((*it).getName() == name){
			return (Object*)&(*it);
		}
	}
	return NULL;
}

bool Game::checkTrigger(string command){
	vector<Trigger>::iterator it;
	vector<Trigger> *triggers;
	vector<string> *names;
	vector<string>::iterator nameIt;
	Object* temp;

	names = &inventory;
	for (nameIt = names->begin(); nameIt != names->end(); ++nameIt){
		temp = findObject(*nameIt);
		triggers = ((Item*)temp)->getTriggers();
		for (it = triggers->begin(); it != triggers->end(); ++it){
			performTrigger(*it, command);

		}
	}


	if (presentRoom == NULL){
		return false;
	}
	triggers = presentRoom->getTriggers();
	for (it = triggers->begin(); it != triggers->end(); ++it){
		if (performTrigger(*it, command))
			return true;
	}
	names = presentRoom->getItems();
	for (nameIt = names->begin(); nameIt != names->end(); ++nameIt){
		temp = findObject(*nameIt);
		triggers = ((Item*)temp)->getTriggers();
		for (it = triggers->begin(); it != triggers->end(); ++it){
			if (performTrigger(*it, command))
				return true;
		}
	}

	names = presentRoom->getContainers();
	for (nameIt = names->begin(); nameIt != names->end(); ++nameIt){
		temp = findObject(*nameIt);
		triggers = ((Container*)temp)->getTriggers();
		for (it = triggers->begin(); it != triggers->end(); ++it){
			if (performTrigger(*it, command))
				return true;
		}
		vector<string>* names2;
		vector<string>::iterator nameIt2;
		Object* temp2;

		names2 = ((Container*)temp)->getItems();
		for (nameIt2 = names2->begin(); nameIt2 != names2->end(); ++nameIt2){
			temp2 = findObject(*nameIt2);
			triggers = ((Item*)temp2)->getTriggers();
			for (it = triggers->begin(); it != triggers->end(); ++it){
				if (performTrigger(*it, command))
					return true;
			}
		}
	}

	names = presentRoom->getCreatures();
	for (nameIt = names->begin(); nameIt != names->end(); ++nameIt){
		temp = findObject(*nameIt);
		triggers = ((Creature*)temp)->getTriggers();
		for (it = triggers->begin(); it != triggers->end(); ++it){
			if (performTrigger(*it, command))
				return true;
		}
	}

	return false;
}

bool Game::performTrigger(Trigger& trigger, string command){
	if (trigger.getActiveStatus() == false){
		return false;
	}
	vector<string>::iterator it1;
	for (it1 = trigger.commands.begin(); it1 != trigger.commands.end(); ++it1){
		if ((*it1) == command){
			break;
		}
	}
	if (trigger.commands.size() == 0 || it1 != trigger.commands.end()){
		if (trigger.conditions.find("owner") != trigger.conditions.end()){
			if (trigger.conditions["owner"] == "inventory"){
				vector<string>::iterator it;
				for (it = inventory.begin(); it != inventory.end(); ++it){
					if ((*it) == trigger.conditions["object"]){
						break;
					}
				}
				if (it != inventory.end()){
					trigger.deactive();
					performActionQueue(&(trigger.actions));
					return true;
				}
			}
			else if (findObject(trigger.conditions["owner"])->findChildren(trigger.conditions["object"])){
				trigger.deactive();
				performActionQueue(&(trigger.actions));
				return true;
			}
		}
		else {
			if (findObject(trigger.conditions["object"])->getStatus() == trigger.conditions["status"]){
				trigger.deactive();
				performActionQueue(&(trigger.actions));
				return true;
			}
		}
	}
	return false;
}

int main(int argc, char * argv[]){
	if (argc >= 2){
		Game game(argv[1]);
	}
	else {
		cout << "Please input file name in the command line argument." << endl;
	}
	return 0;
}
