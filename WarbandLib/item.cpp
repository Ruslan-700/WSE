#include "item.h"

#include "warband.h"

using namespace wb;

item::item()
{
	this->item_no = -1;
	this->item_flags = 0;
}

item::item(int item_no)
{
	this->item_no = item_no;
	this->item_flags = 0;
	this->refill();
}

item::item(int item_no, int item_modifier_no)
{
	this->item_no = item_no;
	this->item_flags = 0;
	this->set_modifier(item_modifier_no);
	this->refill();
}

void item::refill()
{
	if (this->item_no < 0)
		return;

	wb::item_kind &item_kind = warband->item_kinds[this->item_no];
	
	if (item_kind.is_shield() || item_kind.is_horse() || item_kind.is_animal())
		this->set_health(this->get_max_health());
	else if (item_kind.is_ammo())
		this->item_flags &= ~AMMO_MASK;
}

int item::get_max_ammo() const
{
	if (this->item_no < 0)
		return 0;

	int max_ammo = warband->item_kinds[this->item_no].max_ammo;

	if (max_ammo && this->get_modifier() == imod_large_bag)
	{
		float additional_ammo_f = max_ammo * 0.13f;
		int additional_ammo = round_half_up(additional_ammo_f);

		if (additional_ammo < 1)
			additional_ammo = 1;

		max_ammo += additional_ammo;
	}

	return max_ammo;
}

int item::get_ammo() const
{
	return this->get_max_ammo() - (this->item_flags & AMMO_MASK);
}

void item::set_ammo(int ammo)
{
	int max_ammo = this->get_max_ammo();

	if (ammo < 0)
		ammo = 0;

	if (ammo > max_ammo)
		ammo = max_ammo;

	this->item_flags &= ~AMMO_MASK;
	this->item_flags |= (max_ammo - ammo) & AMMO_MASK;
}

int item::get_max_health() const
{
	if (this->item_no < 0)
		return 1;
	
	int health = warband->item_kinds[this->item_no].hit_points;

	if (warband->item_kinds[this->item_no].get_type() == itp_type_horse && !health)
		health = 100;
	else if (!health)
		return 0;

	switch (this->get_modifier())
	{
	case imod_cracked:
		health -= 56;
		break;
	case imod_battered:
		health -= 26;
		break;
	case imod_thick:
		health += 47;
		break;
	case imod_reinforced:
		health += 83;
		break;
	case imod_lordly:
		health += 155;
		break;
	case imod_heavy:
		health += 10;
		break;
	case imod_stubborn:
		health += 5;
		break;
	}

	if (health < 1)
		health = 1;

	return health;
}

int item::get_health() const
{
	return this->item_flags & HEALTH_MASK;
}

void item::set_health(int health)
{
	int max_health = this->get_max_health();

	if (health < 0)
		health = 0;

	if (health > max_health)
		health = max_health;

	this->item_flags &= ~HEALTH_MASK;
	this->item_flags |= health & HEALTH_MASK;
}

int item::get_modifier() const
{
	return (this->item_flags & MODIFIER_MASK) >> MODIFIER_SHIFT;
}

void item::set_modifier(int modifier)
{
	if (modifier < 0)
		modifier = 0;

	if (modifier >= NUM_ITEM_MODIFIERS)
		modifier = 0;

	this->item_flags &= ~MODIFIER_MASK;
	this->item_flags |= (modifier << MODIFIER_SHIFT) & MODIFIER_MASK;
}

bool item::is_valid() const
{
	return this->item_no >= 0 && this->item_no < warband->num_item_kinds;
}

item_kind *item::get_item_kind() const
{
	if (!this->is_valid())
		return nullptr;

	return &warband->item_kinds[this->item_no];
}
