#include "WSESkinOperationsContext.h"

#include "WSE.h"
#include "warband.h"

void FaceKeysInit(WSESkinOperationsContext *context)
{
	int fkreg;

	context->ExtractRegister(fkreg);

	memset(&context->m_face_key_registers[fkreg], 0, sizeof(wb::face_keys));
}

void FaceKeysCopy(WSESkinOperationsContext *context)
{
	int fkreg_1, fkreg_2;
	
	context->ExtractRegister(fkreg_1);
	context->ExtractRegister(fkreg_2);

	context->m_face_key_registers[fkreg_1] = context->m_face_key_registers[fkreg_2];
}

int FaceKeysGetHair(WSESkinOperationsContext *context)
{
	int fkreg;

	context->ExtractRegister(fkreg);

	return context->m_face_key_registers[fkreg].get_hair();
}

void FaceKeysSetHair(WSESkinOperationsContext *context)
{
	int fkreg, value;
	
	context->ExtractRegister(fkreg);
	context->ExtractValue(value);

	context->m_face_key_registers[fkreg].set_hair(value);
}

int FaceKeysGetBeard(WSESkinOperationsContext *context)
{
	int fkreg;

	context->ExtractRegister(fkreg);

	return context->m_face_key_registers[fkreg].get_beard();
}

void FaceKeysSetBeard(WSESkinOperationsContext *context)
{
	int fkreg, value;
	
	context->ExtractRegister(fkreg);
	context->ExtractValue(value);

	context->m_face_key_registers[fkreg].set_beard(value);
}

int FaceKeysGetFaceTexture(WSESkinOperationsContext *context)
{
	int fkreg;

	context->ExtractRegister(fkreg);

	return context->m_face_key_registers[fkreg].get_face_texture();
}

void FaceKeysSetFaceTexture(WSESkinOperationsContext *context)
{
	int fkreg, value;
	
	context->ExtractRegister(fkreg);
	context->ExtractValue(value);

	context->m_face_key_registers[fkreg].set_face_texture(value);
}

int FaceKeysGetHairTexture(WSESkinOperationsContext *context)
{
	int fkreg;

	context->ExtractRegister(fkreg);

	return context->m_face_key_registers[fkreg].get_hair_texture();
}

void FaceKeysSetHairTexture(WSESkinOperationsContext *context)
{
	int fkreg, value;
	
	context->ExtractRegister(fkreg);
	context->ExtractValue(value);

	context->m_face_key_registers[fkreg].set_hair_texture(value);
}

int FaceKeysGetHairColor(WSESkinOperationsContext *context)
{
	int fkreg;

	context->ExtractRegister(fkreg);

	return (int)(context->m_face_key_registers[fkreg].get_hair_color() * 100.0f);
}

void FaceKeysSetHairColor(WSESkinOperationsContext *context)
{
	int fkreg, value;
	
	context->ExtractRegister(fkreg);
	context->ExtractValue(value);

	if (value < 0)
		value = 0;
	else if (value > 100)
		value = 100;
		
	context->m_face_key_registers[fkreg].set_hair_color(value / 100.0f);
}

int FaceKeysGetAge(WSESkinOperationsContext *context)
{
	int fkreg;

	context->ExtractRegister(fkreg);

	return (int)(context->m_face_key_registers[fkreg].get_age() * 100.0f);
}

void FaceKeysSetAge(WSESkinOperationsContext *context)
{
	int fkreg, value;
	
	context->ExtractRegister(fkreg);
	context->ExtractValue(value);

	if (value < 0)
		value = 0;
	else if (value > 100)
		value = 100;
		
	context->m_face_key_registers[fkreg].set_age(value / 100.0f);
}

int FaceKeysGetMorphKey(WSESkinOperationsContext *context)
{
	int fkreg, key_no;
	
	context->ExtractRegister(fkreg);
	context->ExtractBoundedValue(key_no, 0, 42);

	return (int)(context->m_face_key_registers[fkreg].get_morph_key(key_no) * 7.0f);
}

void FaceKeysSetMorphKey(WSESkinOperationsContext *context)
{
	int fkreg, key_no, value;
	
	context->ExtractRegister(fkreg);
	context->ExtractBoundedValue(key_no, 0, 42);
	context->ExtractValue(value);

	context->m_face_key_registers[fkreg].set_morph_key(key_no, value / 7.0f);
}

void FaceKeysStoreString(WSESkinOperationsContext *context)
{
	int fkreg;
	std::string str;

	context->ExtractRegister(fkreg);
	context->ExtractString(str);

	if (str[1] == 'x' || str[1] == 'X')
		str = str.substr(2);
	
	context->m_face_key_registers[fkreg].keys[0] = _strtoui64(str.substr(0, 16).c_str(), nullptr, 16);
	context->m_face_key_registers[fkreg].keys[1] = _strtoui64(str.substr(16, 16).c_str(), nullptr, 16);
	context->m_face_key_registers[fkreg].keys[2] = _strtoui64(str.substr(32, 16).c_str(), nullptr, 16);
	context->m_face_key_registers[fkreg].keys[3] = _strtoui64(str.substr(48, 16).c_str(), nullptr, 16);
}

WSESkinOperationsContext::WSESkinOperationsContext() : WSEOperationContext("skin", 2700, 2799)
{
}

void WSESkinOperationsContext::OnLoad()
{
	RegisterOperation("face_keys_init", FaceKeysInit, Both, None, 1, 1,
		"Zeroes <0>",
		"face_key_register");

	RegisterOperation("face_keys_copy", FaceKeysCopy, Both, None, 2, 2,
		"Copies <1> to <0>",
		"face_key_register_1", "face_key_register_2");

	RegisterOperation("face_keys_get_hair", FaceKeysGetHair, Both, Lhs, 2, 2,
		"Stores <1>'s hair value into <0>",
		"destination", "face_key_register");

	RegisterOperation("face_keys_set_hair", FaceKeysSetHair, Both, None, 2, 2,
		"Sets <0>'s hair value",
		"face_key_register", "hair_no");

	RegisterOperation("face_keys_get_beard", FaceKeysGetBeard, Both, Lhs, 2, 2,
		"Stores <1>'s beard value into <0>",
		"destination", "face_key_register");

	RegisterOperation("face_keys_set_beard", FaceKeysSetBeard, Both, None, 2, 2,
		"Sets <0>'s beard value",
		"face_key_register", "beard_no");

	RegisterOperation("face_keys_get_face_texture", FaceKeysGetFaceTexture, Both, Lhs, 2, 2,
		"Stores <1>'s face texture value into <0>",
		"destination", "face_key_register");

	RegisterOperation("face_keys_set_face_texture", FaceKeysSetFaceTexture, Both, None, 2, 2,
		"Sets <0>'s face texture value",
		"face_key_register", "face_texture_no");

	RegisterOperation("face_keys_get_hair_texture", FaceKeysGetHairTexture, Both, Lhs, 2, 2,
		"Stores <1>'s hair texture value into <0>",
		"destination", "face_key_register");

	RegisterOperation("face_keys_set_hair_texture", FaceKeysSetHairTexture, Both, None, 2, 2,
		"Sets <0>'s hair texture value",
		"face_key_register", "hair_texture_no");

	RegisterOperation("face_keys_get_hair_color", FaceKeysGetHairColor, Both, Lhs, 2, 2,
		"Stores <1>'s hair color value (in %%) into <0>",
		"destination", "face_key_register");

	RegisterOperation("face_keys_set_hair_color", FaceKeysSetHairColor, Both, None, 2, 2,
		"Sets <0>'s hair color value (in %%)",
		"face_key_register", "hair_color");

	RegisterOperation("face_keys_get_age", FaceKeysGetAge, Both, Lhs, 2, 2,
		"Stores <1>'s age value (in %%) into <0>",
		"destination", "face_key_register");

	RegisterOperation("face_keys_set_age", FaceKeysSetAge, Both, None, 2, 2,
		"Sets <0>'s age value (in %%)",
		"face_key_register", "age");

	RegisterOperation("face_keys_get_morph_key", FaceKeysGetMorphKey, Both, Lhs, 3, 3,
		"Stores <1>'s morph key value (0-7) into <0>",
		"destination", "face_key_register", "key_no");

	RegisterOperation("face_keys_set_morph_key", FaceKeysSetMorphKey, Both, None, 3, 3,
		"Sets <0>'s morph key value (0-7) to <value>",
		"face_key_register", "key_no", "value");

	RegisterOperation("face_keys_store_string", FaceKeysStoreString, Both, None, 2, 2,
		"Initializes <0> using <1>",
		"face_key_register", "string");
}
