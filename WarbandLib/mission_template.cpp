#include "mission_template.h"

#include "wb.h"

using namespace wb;

int mission_template::addTrigger(const trigger &newTrigger, int templateNo)
{
	int tIndex = this->triggers.addTrigger(newTrigger);
	trigger &actualNewTrigger = this->triggers.triggers[tIndex];

	actualNewTrigger.conditions.id.format("Mission Template [%i] %s Trigger [%i] Conditions", templateNo, this->id.static_c_str(), tIndex);
	actualNewTrigger.consequences.id.format("Mission Template [%i] %s Trigger [%i] Consequences", templateNo, this->id.static_c_str(), tIndex);

	return tIndex;
}

bool mission_template::removeTrigger(int index)
{
	return this->triggers.removeTrigger(index);
}