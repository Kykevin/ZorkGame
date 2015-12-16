#include "ScriptManager.h"


ScriptManager::ScriptManager()
{
}


ScriptManager::~ScriptManager()
{
}

void ScriptManager::parseFile(const char * fileName, vector<Room>& rooms, vector<Item>& items,
	vector<Container>& containers, vector<Creature>& creatures, vector<Trigger>& triggers){
	ifstream file(fileName);
	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);
	// Find our root node
	xml_node<> * rootNode = doc.first_node("map");
	xml_node<> * tempNode;
	for (tempNode = rootNode->first_node("room"); tempNode; tempNode = tempNode->next_sibling("room")){
		Room room;
		parseRoom(tempNode, room);
		room.objType = "room";
		rooms.push_back(room);
	}
	for (tempNode = rootNode->first_node("item"); tempNode; tempNode = tempNode->next_sibling("item")){
		Item item;
		parseItem(tempNode, item);
		item.objType = "item";
		items.push_back(item);
	}
	for (tempNode = rootNode->first_node("container"); tempNode; tempNode = tempNode->next_sibling("container")){
		Container container;
		parseContainer(tempNode, container);
		container.objType = "container";
		containers.push_back(container);
	}
	for (tempNode = rootNode->first_node("creature"); tempNode; tempNode = tempNode->next_sibling("creature")){
		Creature creature;
		parseCreature(tempNode, creature);
		creature.objType = "creature";
		creatures.push_back(creature);
	}
}

void ScriptManager::parseRoom(const xml_node<> * currNode, Room& room){
	xml_node<> * tempNode = currNode->first_node("name");
	room.name = tempNode->value();

	//cout << tempNode->value() << endl;

	tempNode = currNode->first_node("status");
	if (tempNode != NULL){
		room.status = tempNode->value();
	}
	tempNode = currNode->first_node("description");
	room.description = tempNode->value();
	tempNode = currNode->first_node("type");
	if (tempNode == NULL) room.type = "regular";
	else room.type = tempNode->value();
	for (tempNode = currNode->first_node("border"); tempNode; tempNode = tempNode->next_sibling("border")){
		room.borders[tempNode->first_node("direction")->value()] = tempNode->first_node("name")->value();
	}
	for (tempNode = currNode->first_node("container"); tempNode; tempNode = tempNode->next_sibling("container")){
		room.containers.push_back(tempNode->value());
	}
	for (tempNode = currNode->first_node("item"); tempNode; tempNode = tempNode->next_sibling("item")){
		room.items.push_back(tempNode->value());
	}
	for (tempNode = currNode->first_node("creature"); tempNode; tempNode = tempNode->next_sibling("creature")){
		room.creatures.push_back(tempNode->value());
	}
	for (tempNode = currNode->first_node("trigger"); tempNode; tempNode = tempNode->next_sibling("trigger")){
		Trigger trigger;
		parseTrigger(tempNode, trigger);
		room.triggers.push_back(trigger);
	}
}

void ScriptManager::parseItem(const xml_node<> * currNode, Item& item){
	xml_node<> * tempNode = currNode->first_node("name");
	item.name = tempNode->value();

	//cout << tempNode->value() << endl;

	tempNode = currNode->first_node("status");
	if (tempNode != NULL)
		item.status = tempNode->value();
	tempNode = currNode->first_node("description");
	if (tempNode != NULL)
		item.description = tempNode->value();
	tempNode = currNode->first_node("turnon");
	if (tempNode != NULL){
		xml_node<> * tempNode2;
		for (tempNode2 = tempNode->first_node(); tempNode2; tempNode2 = tempNode2->next_sibling()){
			item.turnOnActions.push_back(make_pair(tempNode2->name(), tempNode2->value()));
		}

	}
	tempNode = currNode->first_node("writing");
	if (tempNode != NULL)
		item.writing = tempNode->value();
	for (tempNode = currNode->first_node("trigger"); tempNode; tempNode = tempNode->next_sibling("trigger")){
		Trigger trigger;
		parseTrigger(tempNode, trigger);
		item.triggers.push_back(trigger);
	}
}

void ScriptManager::parseContainer(const xml_node<> * currNode, Container& container){
	xml_node<> * tempNode = currNode->first_node("name");
	container.name = tempNode->value();

	//cout << tempNode->value() << endl;

	tempNode = currNode->first_node("status");
	if (tempNode != NULL)
		container.status = tempNode->value();
	tempNode = currNode->first_node("description");
	if (tempNode != NULL)
		container.description = tempNode->value();
	container.locked = false;
	for (tempNode = currNode->first_node("accept"); tempNode; tempNode = tempNode->next_sibling("accept")){
		container.accept.push_back(tempNode->value());
		container.locked = true;
	}
	for (tempNode = currNode->first_node("item"); tempNode; tempNode = tempNode->next_sibling("item")){
		container.items.push_back(tempNode->value());
	}
	for (tempNode = currNode->first_node("trigger"); tempNode; tempNode = tempNode->next_sibling("trigger")){
		Trigger trigger;
		parseTrigger(tempNode, trigger);
		container.triggers.push_back(trigger);
	}
}

void ScriptManager::parseCreature(const xml_node<> * currNode, Creature& creature){
	xml_node<> * tempNode = currNode->first_node("name");
	creature.name = tempNode->value();

	//cout << tempNode->value() << endl;

	tempNode = currNode->first_node("status");
	if (tempNode != NULL)
		creature.status = tempNode->value();
	tempNode = currNode->first_node("description");
	if (tempNode != NULL)
		creature.description = tempNode->value();
	for (tempNode = currNode->first_node("vulnerability"); tempNode; tempNode = tempNode->next_sibling("vulnerability")){
		creature.vulnerability.push_back(tempNode->value());
	}
	tempNode = currNode->first_node("attack");
	if (tempNode != NULL){
		xml_node<> * tempNode2;
		for (tempNode2 = tempNode->first_node(); tempNode2; tempNode2 = tempNode2->next_sibling()){
			creature.attackActions.push_back(make_pair(tempNode2->name(), tempNode2->value()));
		}
	}
	for (tempNode = currNode->first_node("trigger"); tempNode; tempNode = tempNode->next_sibling("trigger")){
		Trigger trigger;
		parseTrigger(tempNode, trigger);
		creature.triggers.push_back(trigger);
	}
}

void ScriptManager::parseTrigger(const xml_node<> * currNode, Trigger& trigger){
	xml_node<> * tempNode = currNode->first_node("condition");
	xml_node<> * tempNode2 = tempNode->first_node("object");
	trigger.conditions["object"] = tempNode2->value();
	tempNode2 = tempNode->first_node("status");
	if (tempNode2 != NULL){
		trigger.conditions["status"] = tempNode2->value();
	}
	tempNode2 = tempNode->first_node("has");
	if (tempNode2 != NULL){
		trigger.conditions["has"] = tempNode2->value();
	}
	tempNode2 = tempNode->first_node("owner");
	if (tempNode2 != NULL){
		trigger.conditions["owner"] = tempNode2->value();
	}
	tempNode = currNode->first_node("type");
	if (tempNode != NULL && tempNode->value() == "permanent"){
		trigger.single = false;
	}
	else{
		trigger.single = true;
	}
	for (tempNode = currNode->first_node("command"); tempNode; tempNode = tempNode->next_sibling("command")){
		trigger.commands.push_back(tempNode->value());
	}
	tempNode = currNode->first_node("condition");
	tempNode = tempNode->next_sibling();
	for (; tempNode; tempNode = tempNode->next_sibling()){
		trigger.actions.push_back(make_pair(tempNode->name(), tempNode->value()));
	}
}
