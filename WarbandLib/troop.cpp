#include "troop.h"

#include "warband.h"

using namespace wb;

void troop::update_health()
{
	if ((this->get_attribute(0) + 2 * this->get_skill(36) + 35) < this->health)
		this->health = (float)(this->get_attribute(0) + 2 * this->get_skill(36) + 35);
}

int troop::get_skill(int skill_no) const
{
	int value = (this->skills[skill_no / 8] >> 4 * (skill_no % 8)) & 0xF;

	if (value < 0)
		value = 0;

	if (value > warband->skills[skill_no].max_level)
		value = warband->skills[skill_no].max_level;

	return value;
}

void troop::set_skill(int skill_no, int value)
{
	if (value < 0)
		value = 0;

	if (value > warband->skills[skill_no].max_level)
		value = warband->skills[skill_no].max_level;

	this->skills[skill_no / 8] &= ~(0xF << 4 * (skill_no % 8));
	this->skills[skill_no / 8] |= (value & 0xF) << 4 * (skill_no % 8);
	this->update_health();
}

int troop::get_attribute(int attribute) const
{
	return this->attributes[attribute];
}

void troop::set_attribute(int attribute, int value)
{
	if (value < 0)
		value = 0;

	//if (value > 100)
		//value = 100;

	this->attributes[attribute] = value;
	this->update_health();
}
