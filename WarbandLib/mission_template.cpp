#include "mission_template.h"

#include "wb.h"

using namespace wb;

int mission_template::addTrigger(const trigger &newTrigger, int templateNo, const rgl::string& id_suffix)
{
	int tIndex = this->triggers.addTrigger(newTrigger);
	trigger &actualNewTrigger = this->triggers.triggers[tIndex];

	actualNewTrigger.conditions.id.format("Mission Template [%i] %s Trigger [%i] Conditions%s", templateNo, this->id.static_c_str(), tIndex, id_suffix.c_str());
	actualNewTrigger.consequences.id.format("Mission Template [%i] %s Trigger [%i] Consequences%s", templateNo, this->id.static_c_str(), tIndex, id_suffix.c_str());

	return tIndex;
}

bool mission_template::removeTrigger(int index)
{
	return this->triggers.removeTrigger(index);
}