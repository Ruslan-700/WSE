#include "WSEDictionaryOperationsContext.h"

#include "WSE.h"
#include "warband.h"

int DictCreate(WSEDictionaryOperationsContext *context)
{
	return WSE->Objects.AllocObject(context->m_type, new WSEDictionary());
}

void DictDestroy(WSEDictionaryOperationsContext *context)
{
	int id;

	context->ExtractValue(id);

	WSEDictionary *dict = context->GetDictionary(id);
	
	delete dict;
	WSE->Objects.FreeObject(context->m_type, id);
}

void DictLoadFile(WSEDictionaryOperationsContext *context)
{
	int id, mode;
	std::string path;
	
	context->ExtractValue(id);
	context->ExtractPath(path);
	context->ExtractValue(mode);
	
	WSEDictionary *dict = context->GetDictionary(id);
	
	dict->Load(context->CreateFile(path, "wsedict"), mode);
}

void DictLoadDict(WSEDictionaryOperationsContext *context)
{
	int id1, id2, mode;
	
	context->ExtractValue(id1);
	context->ExtractValue(id2);
	context->ExtractValue(mode);
	
	WSEDictionary *dict1 = context->GetDictionary(id1);
	WSEDictionary *dict2 = context->GetDictionary(id2);

	dict1->Load(*dict2, mode);
}

void DictSave(WSEDictionaryOperationsContext *context)
{
	int id;
	std::string path;
	
	context->ExtractValue(id);
	context->ExtractPath(path);

	WSEDictionary *dict = context->GetDictionary(id);

	dict->Save(context->CreateFile(path, "wsedict"));
}

void DictClear(WSEDictionaryOperationsContext *context)
{
	int id;
	
	context->ExtractValue(id);

	WSEDictionary *dict = context->GetDictionary(id);

	dict->Clear();
}

bool DictIsEmpty(WSEDictionaryOperationsContext *context)
{
	int id;
	
	context->ExtractValue(id);

	WSEDictionary *dict = context->GetDictionary(id);

	return dict->GetSize() == 0;
}

bool DictHasKey(WSEDictionaryOperationsContext *context)
{
	int id;
	std::string key;
	
	context->ExtractValue(id);
	context->ExtractString(key);

	WSEDictionary *dict = context->GetDictionary(id);

	return dict->Has(key);
}

int DictGetSize(WSEDictionaryOperationsContext *context)
{
	int id;
	
	context->ExtractValue(id);

	WSEDictionary *dict = context->GetDictionary(id);

	return dict->GetSize();
}

void DictDeleteFile(WSEDictionaryOperationsContext *context)
{
	std::string path;
	
	context->ExtractPath(path);

	DeleteFile(context->CreateFile(path, "wsedict").c_str());
}

void DictGetStr(WSEDictionaryOperationsContext *context)
{
	int sreg, id;
	std::string key, def;
	
	context->ExtractValue(sreg);
	context->ExtractValue(id);
	context->ExtractString(key);
	context->ExtractString(def);

	WSEDictionary *dict = context->GetDictionary(id);

	warband->basic_game.string_registers[sreg] = dict->GetString(key, def);
}

int DictGetInt(WSEDictionaryOperationsContext *context)
{
	int id, def;
	std::string key;
	
	context->ExtractValue(id);
	context->ExtractString(key);
	context->ExtractValue(def);

	WSEDictionary *dict = context->GetDictionary(id);

	return dict->GetInt(key, def);
}

void DictSetStr(WSEDictionaryOperationsContext *context)
{
	int id;
	std::string key, value;
	
	context->ExtractValue(id);
	context->ExtractString(key);
	context->ExtractString(value);

	WSEDictionary *dict = context->GetDictionary(id);

	dict->SetString(key, value);
}

void DictSetInt(WSEDictionaryOperationsContext *context)
{
	int id, value;
	std::string key;
	
	context->ExtractValue(id);
	context->ExtractString(key);
	context->ExtractValue(value);

	WSEDictionary *dict = context->GetDictionary(id);

	dict->SetInt(key, value);
}

WSEDictionaryOperationsContext::WSEDictionaryOperationsContext() : WSEOperationContext("dictionary", 3200, 3299)
{
}

WSEDictionary *WSEDictionaryOperationsContext::GetDictionary(int id)
{
	WSEDictionary *dict = (WSEDictionary *)WSE->Objects.FetchObject(m_type, id);

	if (!dict)
		ScriptError("operand is not a valid dictionary");

	return dict;
}

void WSEDictionaryOperationsContext::OnLoad()
{
	m_type = WSE->Objects.AllocType("dictionary");

	RegisterOperation("dict_create", DictCreate, Both, Lhs, 1, 1,
		"Creates an empty dictionary object and stores it into <0>",
		"destination");
	
	RegisterOperation("dict_free", DictDestroy, Both, None, 1, 1,
		"Frees the dictionary object <0>. A dictionary can't be used after freeing it",
		"dict");

	RegisterOperation("dict_load_file", DictLoadFile, Both, None, 2, 3,
		"Loads a dictionary file into <0>. Setting <2> to 0 (default) clears <dict> and then loads the file, setting <2> to 1 doesn't clear <dict> but overrides any key that's already present, <2> to 2 doesn't clear <0> and doesn't overwrite keys that are already present",
		"dict", "file", "mode");

	RegisterOperation("dict_load_dict", DictLoadDict, Both, None, 2, 3,
		"Loads <1> into <0>. <2>: see above",
		"dict_1", "dict_2", "mode");

	RegisterOperation("dict_save", DictSave, Both, None, 2, 2,
		"Saves <0> into a file. For security reasons, <1> is just a name, not a full path, and will be stored into a WSE managed directory",
		"dict", "file");

	RegisterOperation("dict_clear", DictClear, Both, None, 1, 1,
		"Clears all key-value pairs from <0>",
		"dict");

	RegisterOperation("dict_is_empty", DictIsEmpty, Both, Cf, 1, 1,
		"Fails if <0> is not empty",
		"dict");

	RegisterOperation("dict_has_key", DictHasKey, Both, Cf, 2, 2,
		"Fails if <1> is not present in <0>",
		"dict", "key");

	RegisterOperation("dict_get_size", DictGetSize, Both, Lhs, 2, 2,
		"Stores the count of key-value pairs in <1> into <0>",
		"destination", "dict");

	RegisterOperation("dict_delete_file", DictDeleteFile, Both, None, 1, 1,
		"Deletes dictionary file <0> from disk",
		"file");

	RegisterOperation("dict_get_str", DictGetStr, Both, None, 3, 4,
		"Stores the string value paired to <2> into <0>. If the key is not found and <3> is set, <3> will be stored instead. If <3> is not set, an empty string will be stored",
		"string_register", "dict", "key", "default");

	RegisterOperation("dict_get_int", DictGetInt, Both, Lhs, 3, 4,
		"Stores the numeric value paired to <2> into <0>. If the key is not found and <3> is set, <3> will be stored instead. If <3> is not set, 0 will be stored",
		"destination", "dict", "key", "default");

	RegisterOperation("dict_set_str", DictSetStr, Both, None, 3, 3,
		"Adds (or changes) <2> as the string value paired to <1>",
		"dict", "key", "string_no");

	RegisterOperation("dict_set_int", DictSetInt, Both, None, 3, 3,
		"Adds (or changes) <2> as the numeric value paired to <1>",
		"dict", "key", "value");
}

void WSEDictionaryOperationsContext::OnUnload()
{
	WSE->Objects.FreeType(m_type);
}
