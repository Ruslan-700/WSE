#include "mission_template.h"

#include "wb.h"

using namespace wb;

int mission_template::add_trigger(const trigger &newTrigger, int templateNo, const rgl::string& id_suffix)
{
	int tIndex = this->triggers.add_trigger(newTrigger);
	trigger &actualNewTrigger = this->triggers.triggers[tIndex];

	actualNewTrigger.conditions.id.format("Mission Template [%i] %s Trigger [%i] Conditions%s", templateNo, this->id.static_c_str(), tIndex, id_suffix.c_str());
	actualNewTrigger.consequences.id.format("Mission Template [%i] %s Trigger [%i] Consequences%s", templateNo, this->id.static_c_str(), tIndex, id_suffix.c_str());

	return tIndex;
}

bool mission_template::remove_trigger(int index)
{
	return this->triggers.remove_trigger(index);
}