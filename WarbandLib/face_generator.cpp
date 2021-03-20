#include "face_generator.h"

using namespace wb;

face_keys::face_keys()
{
	memset(this->keys, 0, sizeof(this->keys));
}

int face_keys::get_hair() const
{
	return (this->keys[0] >> FACE_KEYS_HAIR_SHIFT) & FACE_KEYS_VALUE_MASK;
}

void face_keys::set_hair(int value)
{
	this->keys[0] &= ~(FACE_KEYS_VALUE_MASK << FACE_KEYS_HAIR_SHIFT);
	this->keys[0] |= (value & FACE_KEYS_VALUE_MASK) << FACE_KEYS_HAIR_SHIFT;
}

int face_keys::get_beard() const
{
	return (this->keys[0] >> FACE_KEYS_BEARD_SHIFT) & FACE_KEYS_VALUE_MASK;
}

void face_keys::set_beard(int value)
{
	this->keys[0] &= ~(FACE_KEYS_VALUE_MASK << FACE_KEYS_BEARD_SHIFT);
	this->keys[0] |= (value & FACE_KEYS_VALUE_MASK) << FACE_KEYS_BEARD_SHIFT;
}

int face_keys::get_face_texture() const
{
	return (this->keys[0] >> FACE_KEYS_FACE_TEXTURE_SHIFT) & FACE_KEYS_VALUE_MASK;
}

void face_keys::set_face_texture(int value)
{
	this->keys[0] &= ~(FACE_KEYS_VALUE_MASK << FACE_KEYS_FACE_TEXTURE_SHIFT);
	this->keys[0] |= (value & FACE_KEYS_VALUE_MASK) << FACE_KEYS_FACE_TEXTURE_SHIFT;
}

int face_keys::get_hair_texture() const
{
	return (this->keys[0] >> FACE_KEYS_HAIR_TEXTURE_SHIFT) & FACE_KEYS_VALUE_MASK;
}

void face_keys::set_hair_texture(int value)
{
	this->keys[0] &= ~(FACE_KEYS_VALUE_MASK << FACE_KEYS_HAIR_TEXTURE_SHIFT);
	this->keys[0] |= (value & FACE_KEYS_VALUE_MASK) << FACE_KEYS_HAIR_TEXTURE_SHIFT;
}

float face_keys::get_hair_color() const
{
	return ((this->keys[0] >> FACE_KEYS_HAIR_COLOR_SHIFT) & FACE_KEYS_VALUE_MASK) / 63.0f;
}

void face_keys::set_hair_color(float value)
{
	this->keys[0] &= ~(FACE_KEYS_VALUE_MASK << FACE_KEYS_HAIR_COLOR_SHIFT);
	this->keys[0] |= ((unsigned __int64)round_half_up(value * 63.0f) & FACE_KEYS_VALUE_MASK) << FACE_KEYS_HAIR_COLOR_SHIFT;
}

float face_keys::get_age() const
{
	return ((this->keys[0] >> FACE_KEYS_AGE_SHIFT) & FACE_KEYS_VALUE_MASK) / 63.0f;
}

void face_keys::set_age(float value)
{
	this->keys[0] &= ~(FACE_KEYS_VALUE_MASK << FACE_KEYS_AGE_SHIFT);
	this->keys[0] |= ((unsigned __int64)round_half_up(value * 63.0f) & FACE_KEYS_VALUE_MASK) << FACE_KEYS_AGE_SHIFT;
}

float face_keys::get_skin_color() const
{
	return ((this->keys[0] >> FACE_KEYS_SKIN_COLOR_SHIFT) & FACE_KEYS_VALUE_MASK) / 63.0f;
}

void face_keys::set_skin_color(float value)
{
	this->keys[0] &= ~(FACE_KEYS_VALUE_MASK << FACE_KEYS_SKIN_COLOR_SHIFT);
	this->keys[0] |= ((unsigned __int64)round_half_up(value * 63.0f) & FACE_KEYS_VALUE_MASK) << FACE_KEYS_SKIN_COLOR_SHIFT;
}

float face_keys::get_morph_key(int index) const
{
	int qword = (index / 21) + 1;
	int shift = (index * 3) % 63;

	return ((this->keys[qword] >> shift) & FACE_KEYS_MORPH_KEY_MASK) / 7.0f;
}

void face_keys::set_morph_key(int index, float value)
{
	int qword = (index / 21) + 1;
	int shift = (index * 3) % 63;
	
	this->keys[qword] &= ~(FACE_KEYS_MORPH_KEY_MASK << shift);
	this->keys[qword] |= ((unsigned __int64)round_half_up(value * 7.0f) & FACE_KEYS_MORPH_KEY_MASK) << shift;
}

rgl::string face_keys::to_string() const
{
	char buf[67];

	//sprintf_s(buf, "0x%.16llX%.16llX%.16llX%.16llX", this->keys[0], this->keys[1], this->keys[2], this->keys[3]);
	sprintf_s(buf, "%.16llx%.16llx%.16llx%.16llx", this->keys[0], this->keys[1], this->keys[2], this->keys[3]);
	return buf;
}

void face_keys::from_string(const rgl::string &facekeys)
{
	if (facekeys.length() == 64)
	{
		this->keys[0] = _strtoui64(facekeys.substr(0, 16).c_str(), nullptr, 16);
		this->keys[1] = _strtoui64(facekeys.substr(16, 32).c_str(), nullptr, 16);
		this->keys[2] = _strtoui64(facekeys.substr(32, 48).c_str(), nullptr, 16);
		this->keys[3] = _strtoui64(facekeys.substr(48, 64).c_str(), nullptr, 16);
	}
}