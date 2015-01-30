#include "item_kind.h"

using namespace wb;

item_kind_type item_kind::get_type() const
{
	return (item_kind_type)(this->properties & 0xFF);
}

bool item_kind::is_weapon() const
{
	switch(this->get_type())
	{
		case itp_type_one_handed:
		case itp_type_two_handed:
		case itp_type_polearm:
		case itp_type_bow:
		case itp_type_crossbow:
		case itp_type_thrown:
		case itp_type_pistol:
		case itp_type_musket:
			return true;
	}

	return false;
}

bool item_kind::is_melee_weapon() const
{
	switch(this->get_type())
	{
		case itp_type_one_handed:
		case itp_type_two_handed:
		case itp_type_polearm:
			return true;
	}

	return false;
}

bool item_kind::is_ranged_weapon() const
{
	switch(this->get_type())
	{
		case itp_type_bow:
		case itp_type_crossbow:
		case itp_type_thrown:
		case itp_type_pistol:
		case itp_type_musket:
			return true;
	}

	return false;
}

bool item_kind::is_shield() const
{
	return this->get_type() == itp_type_shield;
}

bool item_kind::is_horse() const
{
	return this->get_type() == itp_type_horse;
}

bool item_kind::is_animal() const
{
	return this->get_type() == itp_type_animal;
}

bool item_kind::is_ammo() const
{
	switch(this->get_type())
	{
		case itp_type_arrows:
		case itp_type_bolts:
		case itp_type_bullets:
		case itp_type_thrown:
			return true;
	}

	return false;
}

int item_kind::get_attachment_bone_no() const
{
	if (this->properties & 0xF00)
		return 14;

	switch (this->get_type())
	{
	case wb::itp_type_one_handed:
	case wb::itp_type_two_handed:
	case wb::itp_type_polearm:
	case wb::itp_type_crossbow:
	case wb::itp_type_thrown:
	case wb::itp_type_pistol:
	case wb::itp_type_musket:
		return 19;
	case wb::itp_type_bow:
		return 14;
	case wb::itp_type_shield:
		return 12;
	}

	return -1;
}
