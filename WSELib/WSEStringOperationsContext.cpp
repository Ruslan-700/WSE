#include "WSEStringOperationsContext.h"

#include <WinCrypt.h>
#include <cstdio>
#include <cstring>
#include "WSE.h"
#include "warband.h"

bool StrEquals(WSEStringOperationsContext *context)
{
	CStringW str1, str2;
	bool caseInsensitive;
	
	context->ExtractWideString(str1);
	context->ExtractWideString(str2);
	context->ExtractBoolean(caseInsensitive);

	return caseInsensitive ? !str1.CompareNoCase(str2) : !str1.Compare(str2);
}

bool StrContains(WSEStringOperationsContext *context)
{
	CStringW str1, str2;
	bool caseInsensitive;
	
	context->ExtractWideString(str1);
	context->ExtractWideString(str2);
	context->ExtractBoolean(caseInsensitive);

	if (caseInsensitive)
	{
		str1.MakeLower();
		str2.MakeLower();
	}

	return str1.Find(str2) != -1;
}

bool StrStartsWith(WSEStringOperationsContext *context)
{
	CStringW str1, str2;
	bool caseInsensitive;
	
	context->ExtractWideString(str1);
	context->ExtractWideString(str2);
	context->ExtractBoolean(caseInsensitive);

	CStringW str3 = str1.Left(str2.GetLength());

	return caseInsensitive ? !str3.CompareNoCase(str2) : !str3.Compare(str2);
}

bool StrEndsWith(WSEStringOperationsContext *context)
{
	CStringW str1, str2;
	bool caseInsensitive;
	
	context->ExtractWideString(str1);
	context->ExtractWideString(str2);
	context->ExtractBoolean(caseInsensitive);

	CStringW str3 = str1.Right(str2.GetLength());

	return caseInsensitive ? !str3.CompareNoCase(str2) : !str3.Compare(str2);
}

bool StrIsAlpha(WSEStringOperationsContext *context)
{
	CStringW str1;
	int index;
	
	context->ExtractWideString(str1);
	context->ExtractValue(index, -1);
	
	if (index == -1)
	{
		for (int i = 0; i < str1.GetLength(); ++i)
		{
			if (!iswalpha(str1[i]))
				return false;
		}

		return true;
	}
	else if (index < 0 || index >= str1.GetLength())
	{
		context->ScriptError("index (%d) out of string bounds (length: %d).", index, str1.GetLength());
	}

	return iswalpha(str1[index]) != 0;
}

bool StrIsDigit(WSEStringOperationsContext *context)
{
	CStringW str1;
	int index;
	
	context->ExtractWideString(str1);
	context->ExtractValue(index, -1);
	
	if (index == -1)
	{
		for (int i = 0; i < str1.GetLength(); ++i)
		{
			if (!iswdigit(str1[i]))
				return false;
		}

		return true;
	}
	else if (index < 0 || index >= str1.GetLength())
	{
		context->ScriptError("index (%d) out of string bounds (length: %d).", index, str1.GetLength());
	}

	return iswdigit(str1[index]) != 0;
}

bool StrIsWhitespace(WSEStringOperationsContext *context)
{
	CStringW str1;
	int index;
	
	context->ExtractWideString(str1);
	context->ExtractValue(index, -1);
	
	if (index == -1)
	{
		for (int i = 0; i < str1.GetLength(); ++i)
		{
			if (!iswspace(str1[i]))
				return false;
		}

		return true;
	}
	else if (index < 0 || index >= str1.GetLength())
	{
		context->ScriptError("index (%d) out of string bounds (length: %d).", index, str1.GetLength());
	}

	return iswspace(str1[index]) != 0;
}

int StrLength(WSEStringOperationsContext *context)
{
	CStringW str1;

	context->ExtractWideString(str1);

	return str1.GetLength();
}

int StrIndexOf(WSEStringOperationsContext *context)
{
	CStringW str1, str2;
	int start, end;
	
	context->ExtractWideString(str1);
	context->ExtractWideString(str2);
	context->ExtractValue(start, 0);
	context->ExtractValue(end, str1.GetLength() - 1);

	if (start < 0 || end < 0 || start >= str1.GetLength() || end >= str1.GetLength())
		return -1;
	else if (start > end)
		return -1;
	
	int index = str1.Find(str2, start);

	if (index == -1)
		return index;
	
	return index <= end ? index : -1;
}

int StrLastIndexOf(WSEStringOperationsContext *context)
{
	CStringW str1, str2;
	int start, end;
	
	context->ExtractWideString(str1);
	context->ExtractWideString(str2);
	context->ExtractValue(start, 0);
	context->ExtractValue(end, str1.GetLength() - 1);

	if (start < 0 || end < 0 || start >= str1.GetLength() || end >= str1.GetLength())
		return -1;
	else if (start > end)
		return -1;

	int index = -1;
	int prev_index = index;

	do
	{
		prev_index = index;

		if (start >= end)
			break;

		index = str1.Find(str2, start);
		start = index + 1;
	}
	while (index != -1);
	
	return prev_index <= end ? prev_index : -1;
}

int StrGetChar(WSEStringOperationsContext *context)
{
	CStringW str1;
	int index;

	context->ExtractWideString(str1);
	context->ExtractValue(index);

	if (index < 0 || index >= str1.GetLength())
		context->ScriptError("index (%d) out of string bounds (length: %d).", index, str1.GetLength());

	return str1.GetAt(index);
}

int StrToNum(WSEStringOperationsContext *context)
{
	CStringW str1;
	int useFixedPointMultiplier;

	context->ExtractWideString(str1);
	context->ExtractValue(useFixedPointMultiplier);

	double value = _wtof(str1);

	if (useFixedPointMultiplier)
		value *= warband->basic_game.fixed_point_multiplier;
	
	return (int)value;
}

int StrCompare(WSEStringOperationsContext *context)
{
	CStringW str1, str2;
	bool caseInsensitive;
	
	context->ExtractWideString(str1);
	context->ExtractWideString(str2);
	context->ExtractBoolean(caseInsensitive);

	if (caseInsensitive)
		return str1.CompareNoCase(str2);
	else
		return str1.Compare(str2);
}

int StrSplit(WSEStringOperationsContext *context)
{
	int sreg, max;
	CStringW str1, str2;
	bool skipEmpty;
	
	context->ExtractRegister(sreg);
	context->ExtractWideString(str1);
	context->ExtractWideString(str2);
	context->ExtractBoolean(skipEmpty);
	context->ExtractValue(max);

	int index = 0;
	int cur = 0;
	
	while (index <= str1.GetLength())
	{
		int findIndex = str1.Find(str2, index);

		if (findIndex < 0)
			findIndex = str1.GetLength();

		if (max && cur >= max)
			return cur;

		if (sreg >= NUM_REGISTERS)
			return cur;

		CStringW substr = str1.Mid(index, findIndex - index);

		if (skipEmpty && substr.IsEmpty())
		{
			index = findIndex + str2.GetLength();
			continue;
		}

		warband->basic_game.string_registers[sreg++] = substr;
		index = findIndex + str2.GetLength();
		cur++;
	}

	return cur;
}

int __cdecl CompareCStringW(const void *first, const void *second)
{
	return ((CStringW *)first)->Compare(*(CStringW *)second);
}

int __cdecl CompareCStringWNoCase(const void *first, const void *second)
{
	return ((CStringW *)first)->CompareNoCase(*(CStringW *)second);
}

void StrSort(WSEStringOperationsContext *context)
{
	int sreg, count;
	bool caseInsensitive, descending;

	context->ExtractRegister(sreg);
	context->ExtractValue(count, 128);
	context->ExtractBoolean(caseInsensitive);
	context->ExtractBoolean(descending);

	count = sreg + count <= 128 ? count : 128 - sreg;

	CStringW *arr = new CStringW[count];

	for (int i = 0; i < count; ++i)
	{
		warband->basic_game.string_registers[sreg + i].widen(arr[i]);
	}

	if (caseInsensitive)
		qsort(arr, count, sizeof(CStringW), CompareCStringWNoCase);
	else
		qsort(arr, count, sizeof(CStringW), CompareCStringW);
	
	for (int i = 0; i < count; ++i)
	{
		warband->basic_game.string_registers[sreg + i] = arr[descending ? count - i - 1 : i];
	}

	delete[] arr;
}

void StrStoreLower(WSEStringOperationsContext *context)
{
	int sreg;
	CStringW str1;
	
	context->ExtractRegister(sreg);
	context->ExtractWideString(str1);

	str1.MakeLower();

	warband->basic_game.string_registers[sreg] = str1;
}

void StrStoreUpper(WSEStringOperationsContext *context)
{
	int sreg;
	CStringW str1;
	
	context->ExtractRegister(sreg);
	context->ExtractWideString(str1);

	str1.MakeUpper();

	warband->basic_game.string_registers[sreg] = str1;
}

void StrStoreTrim(WSEStringOperationsContext *context)
{
	int sreg, mode;
	CStringW str1;
	
	context->ExtractRegister(sreg);
	context->ExtractWideString(str1);
	context->ExtractValue(mode);

	switch (mode)
	{
	case 0:
		str1.Trim();
		break;
	case 1:
		str1.TrimLeft();
		break;
	case 2:
		str1.TrimRight();
		break;
	}

	warband->basic_game.string_registers[sreg] = str1;
}

void StrStoreReplace(WSEStringOperationsContext *context)
{
	int sreg;
	CStringW str1, str2, str3;
	
	context->ExtractRegister(sreg);
	context->ExtractWideString(str1);
	context->ExtractWideString(str2);
	context->ExtractWideString(str3);

	str1.Replace(str2, str3);

	warband->basic_game.string_registers[sreg] = str1;
}

void StrStoreMD5(WSEStringOperationsContext *context)
{
	int sreg;
	std::string str1;
	MD5Hash hash;
	
	context->ExtractRegister(sreg);
	context->ExtractString(str1);

	if (context->MD5((byte *)str1.c_str(), str1.length(), hash))
		warband->basic_game.string_registers[sreg] = hash;
	else
		warband->basic_game.string_registers[sreg].clear();
}

void StrStoreSubstring(WSEStringOperationsContext *context)
{
	int sreg, start, length;
	CStringW str1;
	
	context->ExtractRegister(sreg);
	context->ExtractWideString(str1);
	context->ExtractValue(start, 0);
	context->ExtractValue(length, str1.GetLength() - start);
	
	if (start < 0 || length < 0 || (start + length) > str1.GetLength())
		context->ScriptError("substring range (start: %d, length: %d) out of string bounds (length: %d).", start, length, str1.GetLength());

	warband->basic_game.string_registers[sreg] = str1.Mid(start, length);
}

void StrStoreReverse(WSEStringOperationsContext *context)
{
	int sreg;
	CStringW str1;
	
	context->ExtractRegister(sreg);
	context->ExtractWideString(str1);

	str1.MakeReverse();

	warband->basic_game.string_registers[sreg] = str1;
}

void StrStoreJoin(WSEStringOperationsContext *context)
{
	int destination, start, count;
	CStringW delimiter;
	bool hasDelimiter = false;
	
	context->ExtractRegister(destination);
	context->ExtractRegister(start);
	context->ExtractValue(count);

	if (context->HasMoreOperands())
	{
		context->ExtractWideString(delimiter);
		hasDelimiter = true;
	}

	CStringW result;
	CStringW sreg;

	int endCond = start + count > 128 ? 128 : start + count;
	
	for (int i = start; i < endCond; ++i)
	{
		warband->basic_game.string_registers[i].widen(sreg);
		result.Append(sreg);

		if (hasDelimiter && i < endCond - 1)
			result.Append(delimiter);
	}
	
	warband->basic_game.string_registers[destination] = result;
}

void StrStoreReplaceSpacesWithUnderscores(WSEStringOperationsContext *context)
{
	int sreg;
	CStringW str1;
	
	context->ExtractRegister(sreg);
	context->ExtractWideString(str1);

	str1.Replace(L" ", L"_");

	warband->basic_game.string_registers[sreg] = str1;
}

void StrStoreReplaceUnderscoresWithSpaces(WSEStringOperationsContext *context)
{
	int sreg;
	CStringW str1;
	
	context->ExtractRegister(sreg);
	context->ExtractWideString(str1);

	str1.Replace(L"_", L" ");

	warband->basic_game.string_registers[sreg] = str1;
}

void StrStoreMultiplayerProfileName(WSEStringOperationsContext *context)
{
	int sreg, profile_no;
	
	context->ExtractRegister(sreg);
	context->ExtractProfileNo(profile_no);

	warband->basic_game.string_registers[sreg] = warband->multiplayer_data.profile_manager.profiles[profile_no].name;
}

void StrStoreFaceKeys(WSEStringOperationsContext *context)
{
	int sreg, fkreg;
	
	context->ExtractRegister(sreg);
	context->ExtractRegister(fkreg);
	
	warband->basic_game.string_registers[sreg] = WSE->SkinOperations.m_face_key_registers[fkreg].to_string();
}

void StrStoreModuleSetting(WSEStringOperationsContext *context)
{
	int sreg;
	std::string setting;

	context->ExtractRegister(sreg);
	context->ExtractString(setting);

	warband->basic_game.string_registers[sreg] = WSE->ModuleSettingsIni.String("", spaces_to_underscores(setting));
}

void StrStoreServerPasswordAdmin(WSEStringOperationsContext *context)
{
	int sreg;

	context->ExtractRegister(sreg);

	warband->basic_game.string_registers[sreg] = warband->multiplayer_data.server_password_admin;
}

void StrStoreServerPasswordPrivate(WSEStringOperationsContext *context)
{
	int sreg;

	context->ExtractRegister(sreg);
	
	warband->basic_game.string_registers[sreg] = warband->multiplayer_data.server_password_private;
}

void StrStoreOverlayText(WSEStringOperationsContext *context)
{
#if defined WARBAND
	int sreg, overlay_no;

	context->ExtractRegister(sreg);
	context->ExtractOverlayNo(overlay_no);

	warband->cur_presentation->overlays[overlay_no]->get_text(warband->basic_game.string_registers[sreg]);
#endif
}

void StrStorePlayerIp(WSEStringOperationsContext *context)
{
	int sreg, player_no;

	context->ExtractRegister(sreg);
	context->ExtractPlayerNo(player_no);

	warband->basic_game.string_registers[sreg].clear();

	if (warband->basic_game.is_server())
		warband->basic_game.string_registers[sreg] = inet_ntoa(warband->multiplayer_data.players[player_no].mbnet_peer.address.ip);
}

void StrStoreGameVariable(WSEStringOperationsContext *context)
{
	int sreg;
	std::string setting;

	context->ExtractRegister(sreg);
	context->ExtractString(setting);

	warband->basic_game.string_registers[sreg] = WSE->GameVariablesIni.String("", spaces_to_underscores(setting));
}

void StrStoreSkillName(WSEStringOperationsContext *context)
{
	int sreg, skill_no;

	context->ExtractRegister(sreg);
	context->ExtractSkillNo(skill_no);

	warband->basic_game.string_registers[sreg] = warband->skills[skill_no].name;
}

void StrStoreFloat(WSEStringOperationsContext *context)
{
	int sreg, freg, precision;
	
	context->ExtractRegister(sreg);
	context->ExtractRegister(freg);
	context->ExtractValue(precision, 5);

	if (precision < 0)
		precision = 5;

	char buffer[512];

	sprintf_s(buffer, "%.*f", precision, WSE->FloatingPointOperations.m_float_registers[freg]);
	warband->basic_game.string_registers[sreg] = buffer;
}

void StrSanitize(WSEStringOperationsContext *context)
{
	int sreg;
	
	context->ExtractRegister(sreg);

	wchar_t *utf8_str = warband->basic_game.string_registers[sreg].to_utf8();
	CStringW str(utf8_str);
	
	str.Replace(L"{", L"");
	str.Replace(L"}", L"");
	str.Replace(L"<", L"");
	str.Replace(L">", L"");
	rgl::_delete_vec(utf8_str);
	warband->basic_game.string_registers[sreg] = str;
}

void StrStoreItemId(WSEStringOperationsContext *context)
{
	int sreg, item_no;

	context->ExtractRegister(sreg);
	context->ExtractItemKindNo(item_no);

	warband->basic_game.string_registers[sreg] = warband->item_kinds[item_no].id;
}

bool StrIsInteger(WSEStringOperationsContext *context)
{
	CStringW str1;
	
	context->ExtractWideString(str1);

	str1.Trim();

	int index = 0;

	if (str1[0] == '-')
		index = 1;
	
	for (int i = index; i < str1.GetLength(); ++i)
	{
		if (!iswdigit(str1[i]))
			return false;
	}

	return true;
}

WSEStringOperationsContext::WSEStringOperationsContext() : WSEOperationContext("string", 2600, 2699)
{
}

void WSEStringOperationsContext::OnLoad()
{
	RegisterOperation("str_equals", StrEquals, Both, Cf, 2, 3,
		"Fails if <0> is not equal to <1>",
		"string_1", "string_2", "case_insensitive");

	RegisterOperation("str_contains", StrContains, Both, Cf, 2, 3,
		"Fails if <0> doesn't contain <1>",
		"string_1", "string_2", "case_insensitive");

	RegisterOperation("str_starts_with", StrStartsWith, Both, Cf, 2, 3,
		"Fails if <0> doesn't start with <1>",
		"string_1", "string_2", "case_insensitive");

	RegisterOperation("str_ends_with", StrEndsWith, Both, Cf, 2, 3,
		"Fails if <0> doesn't end with <1>",
		"string_1", "string_2", "case_insensitive");

	RegisterOperation("str_is_alpha", StrIsAlpha, Both, Cf, 1, 2,
		"Fails if character <1> of <0> isn't alphabetic. If <1> is not defined or is -1, the entire string is checked",
		"string_1", "index");

	RegisterOperation("str_is_digit", StrIsDigit, Both, Cf, 1, 2,
		"Fails if character <1> of <0> isn't a digit. If <1> is not defined or is -1, the entire string is checked",
		"string_1", "index");

	RegisterOperation("str_is_whitespace", StrIsWhitespace, Both, Cf, 1, 2,
		"Fails if character <1> of <0> isn't whitespace. If <1> is not defined or is -1, the entire string is checked",
		"string_1", "index");

	RegisterOperation("str_length", StrLength, Both, Lhs, 2, 2,
		"Stores the length of <1> into <0>",
		"destination", "string_1");

	RegisterOperation("str_index_of", StrIndexOf, Both, Lhs, 3, 5,
		"Stores the index of the first occurrence of <2> in <1> into <0>. Search bounds can be specified with <3> and <4>",
		"destination", "string_1", "string_2", "start", "end");
	
	RegisterOperation("str_last_index_of", StrLastIndexOf, Both, Lhs, 3, 5,
		"Stores the index of the last occurrence of <2> in <1> into <0>. Search bounds can be specified with <3> and <4>",
		"destination", "string_1", "string_2", "start", "end");

	RegisterOperation("str_get_char", StrGetChar, Both, Lhs, 2, 3,
		"Stores the numeric value of the <2>th character in <1> into <0>",
		"destination", "string_1", "index");

	RegisterOperation("str_to_num", StrToNum, Both, Lhs, 2, 3,
		"Stores the numeric value of <1> into <0>. Decimal values will be rounded to integers, for more precision set <2> to non-zero",
		"destination_fixed_point", "string_1", "use_fixed_point_multiplier");

	RegisterOperation("str_compare", StrCompare, Both, Lhs, 3, 4,
		"Stores the relationship between <1> and <2> into <0> (-1: s1 < s2, 0: s1 = s2, 1: s1 > s2)",
		"destination", "string_1", "string_2", "case_insensitive");

	RegisterOperation("str_split", StrSplit, Both, Lhs, 4, 6,
		"Splits <2> using <3> into a range of string registers, starting from <1>, storing <5> substrings at most (default = unlimited), ignoring empty (zero length) substrings if <4> (default = false). Stores the amount of substrings split into <0>",
		"destination", "string_register", "string_1", "delimiter", "skip_empty", "max");

	RegisterOperation("str_sort", StrSort, Both, None, 1, 4,
		"Sorts a range of <1> string registers starting from <0>",
		"string_register", "count", "case_insensitive", "descending");

	RegisterOperation("str_store_lower", StrStoreLower, Both, None, 2, 2,
		"Stores the lowercase version of <1> into <0>",
		"string_register", "string_1");

	RegisterOperation("str_store_upper", StrStoreUpper, Both, None, 2, 2,
		"Stores the uppercase version of <1> into <0>",
		"string_register", "string_1");

	RegisterOperation("str_store_trim", StrStoreTrim, Both, None, 2, 3,
		"Stores the whitespace trimmed version of <1> into <0>. <2>: 0 (default) = trim leading and trailing, 1 = trim leading, 2 = trim trailing",
		"string_register", "string_1", "trim_mode");

	RegisterOperation("str_store_replace", StrStoreReplace, Both, None, 4, 4,
		"Stores <1> into <0>, replacing occurrences of <2> with <3>",
		"string_register", "string_1", "string_2", "string_3");

	RegisterOperation("str_store_md5", StrStoreMD5, Both, None, 2, 2,
		"MD5 encrypts <1> and stores it into <0>",
		"string_register", "string_1");

	RegisterOperation("str_store_substring", StrStoreSubstring, Both, None, 2, 4,
		"Stores a substring of <1> into <0>, starting from <2>. If <3> is not specified, everything on the right of <start> will be used",
		"string_register", "string_1", "start", "length");

	RegisterOperation("str_store_reverse", StrStoreReverse, Both, None, 2, 2,
		"Stores the reverse of <0> into <1>",
		"string_register", "string_1");

	RegisterOperation("str_store_join", StrStoreJoin, Both, None, 3, 4,
		"Joins <2> string registers starting from string register <1>, using <3> (default = empty string) and stores them into <0>",
		"string_register", "start_string_register", "count", "delimiter");

	RegisterOperation("str_store_replace_spaces_with_underscores", StrStoreReplaceSpacesWithUnderscores, Both, None, 2, 2,
		"Stores <1> into <0>, replacing all spaces with underscores",
		"string_register", "string_1");

	RegisterOperation("str_store_replace_underscores_with_spaces", StrStoreReplaceUnderscoresWithSpaces, Both, None, 2, 2,
		"Stores <1> into <0>, replacing all underscores with spaces",
		"string_register", "string_1");
	
	RegisterOperation("str_store_multiplayer_profile_name", StrStoreMultiplayerProfileName, Both, None, 2, 2,
		"Stores <1>'s name into <0>",
		"string_register", "profile_no");

	RegisterOperation("str_store_face_keys", StrStoreFaceKeys, Both, None, 2, 2,
		"Stores the string representation of <1> into <0>",
		"string_register", "face_key_register");

	RegisterOperation("str_store_module_setting", StrStoreModuleSetting, Both, None, 2, 2,
		"Stores the string value (empty if not found) of <1> in module.ini into <0>",
		"string_register", "setting");

	RegisterOperation("str_store_server_password_admin", StrStoreServerPasswordAdmin, Both, None, 1, 1,
		"Stores the server administrator password into <0>",
		"string_register");

	RegisterOperation("str_store_server_password_private", StrStoreServerPasswordPrivate, Both, None, 1, 1,
		"Stores the server private player password into <0>",
		"string_register");

	RegisterOperation("str_store_overlay_text", StrStoreOverlayText, Client, None, 2, 2,
		"Stores <1>'s text into <0>",
		"string_register", "overlay_no");

	RegisterOperation("str_store_player_ip", StrStorePlayerIp, Both, None, 2, 2,
		"Stores <1>'s IP address into <0> (works only on servers)",
		"string_register", "player_no");

	RegisterOperation("str_store_game_variable", StrStoreGameVariable, Both, None, 2, 2,
		"Stores the string value (empty if not found) of <1> in game_variables.txt into <0>",
		"string_register", "variable");

	RegisterOperation("str_store_skill_name", StrStoreSkillName, Both, None, 2, 2,
		"Stores the name of <1> into <0>",
		"string_register", "skill_no");

	RegisterOperation("str_store_float", StrStoreFloat, Both, None, 2, 3,
		"Stores the string representation of <1> into <0> showing <2> decimal digits at most",
		"string_register", "fp_register", "precision");

	RegisterOperation("str_sanitize", StrSanitize, Both, None, 1, 1,
		"Removes invalid characters from <0>",
		"string_register");

	RegisterOperation("str_store_item_id", StrStoreItemId, Both, None, 2, 2,
		"Stores the id of <1> into <0>",
		"string_register", "item_no");

	RegisterOperation("str_is_integer", StrIsInteger, Both, Cf, 1, 1,
		"Fails if <0> isn't a valid integer",
		"string_1");
}

bool WSEStringOperationsContext::MD5(const byte *buffer, size_t size, MD5Hash out_hash)
{
    HCRYPTPROV provider;
    HCRYPTHASH hash;
    BYTE hash_buf[0x7f];
    DWORD hash_buf_len = 16;

    if(!CryptAcquireContext(&provider, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_MACHINE_KEYSET))
		return false;

	if(!CryptCreateHash(provider, CALG_MD5, 0, 0, &hash))
		return false;

	if(!CryptHashData(hash, buffer, size, 0))
		return false;
	
	if(!CryptGetHashParam(hash, HP_HASHVAL, hash_buf, &hash_buf_len, 0))
		return false;

	memset(out_hash, 0, sizeof(MD5Hash));

	for (int i = 0; i < 16; ++i)
	{
		sprintf_s(out_hash + (i * 2), 3, "%02x", hash_buf[i]);
	}
	
    CryptDestroyHash(hash);
    CryptReleaseContext(provider, 0);
    return true;
}
