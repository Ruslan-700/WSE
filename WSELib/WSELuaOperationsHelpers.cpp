#include <regex>
#include <string>
#include <sstream>
#include "WSELuaOperationsHelpers.h"

const char *const lShortTypeNames[] = {
	"none", "nil", "bool", "usrdat", "num", "str",
	"table", "func", "usrdat", "thread", "proto", "cdata"
};

#define numLTypes 12

void gPrint(const char *msg)
{
#if defined WARBAND
	warband->window_manager.display_message(msg, 0xFFFF5555, 0);
#endif

	warband->log_stream.write_c_str(msg);
	warband->log_stream.write_c_str("\n");
}

void gPrint(const std::string &s)
{
	gPrint(s.c_str());
}

void gPrintf(const char *format, ...)
{
	char buf[1000];
	va_list args;

	va_start(args, format);
	vsprintf_s(buf, format, args);
	va_end(args);

	gPrint(buf);
}

void gPrintf(const std::string &format, ...)
{
	va_list args;

	va_start(args, format);
	gPrintf(format.c_str(), args);
	va_end(args);
}

void printLastLuaError(lua_State *L, const char *fileName, HANDLE hFile)
{
	const char *msg = lua_tostring(L, -1);
	std::string errMsg;

	if (fileName)
	{
		errMsg = "Lua error in ";
		errMsg += fileName;
		errMsg += ": ";
		errMsg += msg;
	}
	else
	{
		errMsg = "Lua error: ";
		errMsg += msg;
	}

	if (hFile == INVALID_HANDLE_VALUE)
		gPrint(errMsg);
	else
	{
		errMsg += "\r\n";
		DWORD a = 0;
		WriteFile(hFile, errMsg.c_str(), errMsg.length(), &a, NULL);
	}

	lua_pop(L, 1);
}

bool fileExists(const std::string& name)
{
	std::ifstream f(name.c_str());
	return f.good();
}

void removeChar(std::string &str, char c)
{
	str.erase(std::remove(str.begin(), str.end(), c), str.end());
}

void delBlank(std::string &str)
{
	removeChar(str, ' ');
	removeChar(str, '\t');
}

void discardComment(std::string &str)
{
	size_t n = str.find('#');

	if (n != std::string::npos)
	{
		str.erase(n);
	}
}

template<typename Out>
void split(const std::string &s, char delim, bool skipEmpty, Out result)
{
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		if (!skipEmpty || item.length() > 0)
			*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim, bool skipEmpty)
{
	std::vector<std::string> elems;
	split(s, delim, skipEmpty, std::back_inserter(elems));
	return elems;
}

size_t countChar(const std::string &s, char c)
{
	return std::count(s.begin(), s.end(), c);
}

std::string getLuaScriptDir()
{
	std::string dir = "Modules\\";
	dir += warband->cur_module_name.c_str();
	dir += "\\lua\\";

	return dir;
}

void setOperandToLocalVar(__int64 &operand, int localsIndex)
{
	operand = ((__int64)17 << 56) + localsIndex;
}

int checkLArgs(lua_State *L, int minCount, int maxCount, ...) //TODO -- maxCount??
{
	int numArgs = lua_gettop(L);

	if (numArgs < minCount || numArgs > maxCount)
	{
		luaL_error(L, "invalid arg count: %d - min is %d, max is %d", numArgs, minCount, maxCount);
	}

	va_list args;
	va_start(args, maxCount);

	for (int i = 1; i <= maxCount && i <= numArgs; i++)
	{
		lTypeF a = va_arg(args, lTypeF);

		if (a & lNum && lua_isnumber(L, i))
			continue;
		else if (a & lStr && lua_isstring(L, i))
			continue;
		else if (a & lFunc && lua_isfunction(L, i))
			continue;
		else if (a & lNil && lua_isnil(L, i))
			continue;
		else if (a & lTable && lua_istable(L, i))
			continue;
		else if (a & lBool && lua_isboolean(L, i))
			continue;
		else if (a & lLightUserData && lua_islightuserdata(L, i))
			continue;
		else if (a & lUserData && lua_isuserdata(L, i))
			continue;
		else if (a & lThread && lua_isthread(L, i))
			continue;
		else if (a % lPos && lIsPos(L, i))
			continue;

		std::string errorType = "";

		if (a & lNil)
			errorType += "|LUA_TNIL";
		if (a & lBool)
			errorType += "|LUA_TBOOLEAN";
		if (a & lLightUserData)
			errorType += "|LUA_TLIGHTUSERDATA";
		if (a & lNum)
			errorType += "|LUA_TNUMBER";
		if (a & lStr)
			errorType += "|LUA_TSTRING";
		if (a & lTable)
			errorType += "|LUA_TTABLE";
		if (a & lFunc)
			errorType += "|LUA_TFUNCTION";
		if (a & lUserData)
			errorType += "|LUA_TUSERDATA";
		if (a & lThread)
			errorType += "|LUA_TTHREAD";
		if (a & lPos)
			errorType += "|LUA_WSE_POS";

		errorType.erase(0, 1);

		luaL_error(L, "arg %d must have type %s but is %s", i, errorType.c_str(), lua_typename(L, lua_type(L, i)));
	}
	va_end(args);

	return numArgs;
}

int getTemplateNo(const char *id)
{
	for (int i = 0; i < warband->num_mission_templates; i++)
		if (warband->mission_templates[i].id == id)
			return i;

	return -1;
}

int getItemKindNo(const char *id)
{
	for (int i = 0; i < warband->num_item_kinds; i++)
		if (warband->item_kinds[i].id == id)
			return i;

	return -1;
}

void printStack(lua_State *L)
{
	int size = lua_gettop(L);
	gPrintf("Stack top: %i", size);

	for (int i = lua_gettop(L); i >= 1; i--)
	{
		gPrintf("slot%i: %s", i, lua_typename(L, lua_type(L, i)));
	}
}

void checkStackIndex(WSELuaOperationsContext *context, int index)
{
	if (index == 0 || abs(index) > lua_gettop(context->luaState))
		context->ScriptError("invalid stack index: %d", index);
}

bool lIsVec3(lua_State *L, int index)
{
	if (lua_type(L, index) == LUA_TTABLE)
	{
		lua_getfield(L, index, "x");
		int b1 = lua_isnumber(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, index, "y");
		int b2 = lua_isnumber(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, index, "z");
		int b3 = lua_isnumber(L, -1);
		lua_pop(L, 1);

		return b1 && b2 && b3;
	}
	return false;
}

bool lIsRot(lua_State *L, int index)
{
	if (lua_type(L, index) == LUA_TTABLE)
	{
		lua_getfield(L, index, "s");
		bool b1 = lIsVec3(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, index, "f");
		bool b2 = lIsVec3(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, index, "u");
		bool b3 = lIsVec3(L, -1);
		lua_pop(L, 1);

		return b1 && b2 && b3;
	}
	return false;
}

bool lIsPos(lua_State *L, int index)
{
	if (lua_type(L, index) == LUA_TTABLE)
	{
		lua_getfield(L, index, "o");
		bool b1 = lIsVec3(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, index, "rot");
		bool b2 = lIsRot(L, -1);
		lua_pop(L, 1);

		return b1 && b2;
	}
	
	return false;
}

rgl::vector4 lToVec3(lua_State *L, int index)
{
	lua_getfield(L, index, "x");
	float x = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "y");
	float y = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "z");
	float z = (float)lua_tonumber(L, -1);
	lua_pop(L, 1);

	return rgl::vector4(x, y, z);
}

rgl::orientation lToRot(lua_State *L, int index)
{
	rgl::orientation rot;

	lua_getfield(L, index, "s");
	rot.s = lToVec3(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "f");
	rot.f = lToVec3(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "u");
	rot.u = lToVec3(L, -1);
	lua_pop(L, 1);

	return rot;
}

rgl::matrix lToPos(lua_State *L, int index)
{
	rgl::matrix pos;

	lua_getfield(L, index, "o");
	pos.o = lToVec3(L, -1);
	lua_pop(L, 1);

	lua_getfield(L, index, "rot");
	pos.rot = lToRot(L, -1);
	lua_pop(L, 1);

	return pos;
}

void lPushChild(lua_State *L, const std::string &name)
{
	std::stringstream ss;
	std::string curPart;

	ss.str(name);

	lua_getfield(L, LUA_GLOBALSINDEX, "_G");
	while (std::getline(ss, curPart, '.'))
	{
		lua_getfield(L, -1, curPart.c_str());
		lua_remove(L, -2);
	}
}

void lPushVec3(lua_State *L, const rgl::vector4 &vec)
{
	lua_newtable(L);
	lua_pushnumber(L, (lua_Number)vec.z);
	lua_pushnumber(L, (lua_Number)vec.y);
	lua_pushnumber(L, (lua_Number)vec.x);

	lua_setfield(L, -4, "x");
	lua_setfield(L, -3, "y");
	lua_setfield(L, -2, "z");

	lPushChild(L, "vector3.mt");
	lua_setmetatable(L, -2);
}

void lPushRot(lua_State *L, const rgl::orientation &rot)
{
	lua_newtable(L);
	lPushVec3(L, rot.u);
	lPushVec3(L, rot.f);
	lPushVec3(L, rot.s);

	lua_setfield(L, -4, "s");
	lua_setfield(L, -3, "f");
	lua_setfield(L, -2, "u");

	lPushChild(L, "game.rotation.mt");
	lua_setmetatable(L, -2);
}

void lPushPos(lua_State *L, const rgl::matrix &pos)
{
	lua_newtable(L);

	lPushVec3(L, pos.o);
	lPushRot(L, pos.rot);

	lua_setfield(L, -3, "rot");
	lua_setfield(L, -2, "o");

	lPushChild(L, "game.pos.mt");
	lua_setmetatable(L, -2);
}

void loadGameConstantsFromFile(std::string filePath, std::vector<gameConstTable> &gameConstTables, std::string name)
{
	gameConstTable constTable;
	std::vector<gameConst> &constants = constTable.constants;

	if (!fileExists(filePath))
		return;

	std::ifstream fStream(filePath);
	std::string curLine;
	int curLineNum = 0;

	std::smatch curMatches;

	std::regex numRegEx(R"(^(\w+)=(((-)?0x[\da-fA-F]+)|((-)?\d+(\.\d+)?))$)");
	std::regex refRegEx(R"(^(\w+)=(\w+)$)");

	while (std::getline(fStream, curLine))
	{
		curLineNum++;
		delBlank(curLine);
		discardComment(curLine);

		if (!curLine.length())
			continue;

		gameConst con;

		if (std::regex_match(curLine, curMatches, numRegEx))
		{
			con.name = curMatches.str(1);
			con.val = std::stof(curMatches.str(2), NULL);
		}
		else if (std::regex_match(curLine, curMatches, refRegEx))
		{
			con.name = curMatches.str(1);
			int index = -1;

			for (size_t i = 0; i < constants.size(); i++)
				if (constants[i].name == curMatches.str(2))
				{
					index = i;
					break;
				}

			if (index == -1)
			{
				gPrintf("WSELuaOperationsContext: Error reading %s, line %i: could not find %s", filePath.c_str(), curLineNum, con.name.c_str());
				continue;
			}

			con.val = constants[index].val;
		}
		else
		{
			gPrintf("WSELuaOperationsContext: Warning reading %s, line %i: could not process line", filePath.c_str(), curLineNum);
			continue;
		}

		constants.push_back(con);
	}

	if (constants.size())
	{
		constTable.name = name;

		size_t i = 0;
		size_t suffix = 1;
		while (i < gameConstTables.size())
		{
			if (constTable.name == gameConstTables[i].name)
			{
				i = 0;
				constTable.name = name + "_" + std::to_string(suffix);
				suffix++;
			}
			else
				i++;
		}

		gameConstTables.push_back(constTable);
	}
}


/************   table arg checking    ************/
bool inline isAlphaNumeric(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool inline isStrNumber(const std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
		if (str[i] < '0' || str[i] > '9')
			return false;

	return true;
}

size_t inline findClosingBracketIndex(const std::string &s, size_t start, size_t end, char bracketClose, bool allowNesting)
{
	char bracketOpen = s[start];
	int openBrackets = 1;
	size_t index = start;

	while (++index <= end)
	{
		if (s[index] == bracketOpen)
			if (allowNesting)
				openBrackets++;
			else
			{
				std::string err = "Nesting at position " + std::to_string(index) + " not allowed for '" + bracketOpen;
				err += "' at position " + std::to_string(start);
				throw std::logic_error(err);
			}
		else if (s[index] == bracketClose)
		{
			openBrackets--;
			if (!openBrackets)
				return index;
		}
	}

	std::string err = std::string("Missing closing bracket for '") + bracketOpen;
	err += "' at position " + std::to_string(start);

	throw std::logic_error(err);
}

lTypeF inline getLTypeFlag(int type)
{
	return (lTypeF)(1 << type);
}

lTypeF inline getLTypeFlag(std::string &str)
{
	for (int i = 1; i < numLTypes; i++)
		if (lShortTypeNames[i] == str)
			return getLTypeFlag(i - 1);

	if (str == "any")
		return lAny;

	throw std::logic_error("unrecognized flag '" + str + "'");
}

struct tableCheckOptions
{
	int valF = -1;
	int keyF = -1;
	int min = -1;
	int max = -1;
};

struct tableCheckKey
{
	std::string name;
	bool optional = false;
};

struct tableCheckVal
{
	int val = 0;
	size_t definedTableStart;
	size_t definedTableEnd;

	bool isFlag = false;
	bool isDefinedTable = false;
};

struct tableCheckKeyValPair
{
	tableCheckKey key;
	tableCheckVal val;
};

size_t inline getKey(tableCheckKey &res, const std::string &str, size_t start, size_t end)
{
	if (str[start] == '[')
	{
		end = findClosingBracketIndex(str, start, end, ']', false);
		res.optional = true;
		res.name = str.substr(start + 1, end - start - 1);
	}
	else
	{
		size_t pos = str.find('=', start);
		if (pos == std::string::npos || pos > end)
			throw std::logic_error("Could not find end of key name at " + std::to_string(start));

		res.name = str.substr(start, pos - start);
		end = pos - 1;
	}

	return end;
}

size_t inline getVal(tableCheckVal &val, const std::string &str, size_t start, size_t end)
{
	size_t curIndex = start;
	std::string s = "";

	if (str[curIndex] == '{')
	{
		val.isDefinedTable = true;
		val.definedTableStart = curIndex;
		val.definedTableEnd = findClosingBracketIndex(str, curIndex, end, '}', true);
		val.val = lTable;
		return val.definedTableEnd;
	}

	while (curIndex <= end + 1)
	{
		if (curIndex < end + 1 && isAlphaNumeric(str[curIndex]))
			s += str[curIndex];
		else
		{
			if (!s.length())
				throw std::logic_error("Empty value at position " + std::to_string(curIndex));

			if (isStrNumber(s))
			{
				if (val.isFlag || (curIndex < end + 1 && str[curIndex] == '|'))
					throw std::logic_error("Can't combine number and flag, at " + std::to_string(curIndex));
				else
					val.val = std::stoi(s);
			}
			else
			{
				val.val |= getLTypeFlag(s);
				val.isFlag = true;
			}

			if (curIndex >= end || str[curIndex] != '|')
				return curIndex - 1;

			s = "";
		}
		curIndex++;
	}

	return std::string::npos;
}

size_t inline getKeyValPair(tableCheckKeyValPair &res, const std::string &str, size_t start, size_t end)
{
	size_t pos = getKey(res.key, str, start, end) + 2;
	return getVal(res.val, str, pos, end);
}

size_t inline getTableCheckOptions(tableCheckOptions &options, const std::string &str, size_t start, size_t end)
{
	end = findClosingBracketIndex(str, start, end, ')', false) - 1;
	size_t curIndex = start + 1;

	while (curIndex < end)
	{
		tableCheckKeyValPair curPair;
		size_t nextIndex = getKeyValPair(curPair, str, curIndex, end) + 2;

		if (curPair.key.optional)
			throw std::logic_error("Option key can't be optional, at position " + std::to_string(curIndex));

		if (curPair.val.isDefinedTable)
			throw std::logic_error("Option val can't be a table, at position " + std::to_string(curIndex));

		if (curPair.key.name == "key")
		{
			if (!curPair.val.isFlag)
				throw std::logic_error("Option [key] must be flag, at " + std::to_string(curIndex));
			options.keyF = curPair.val.val;
		}
		else if (curPair.key.name == "val")
		{
			if (!curPair.val.isFlag)
				throw std::logic_error("Option [val] must be flag, at " + std::to_string(curIndex));
			options.valF = curPair.val.val;
		}
		else if (curPair.key.name == "min")
		{
			if (curPair.val.isFlag)
				throw std::logic_error("Option [min] must be num, at " + std::to_string(curIndex));
			options.min = curPair.val.val;
		}
		else if (curPair.key.name == "max")
		{
			if (curPair.val.isFlag)
				throw std::logic_error("Option [max] must be num, at " + std::to_string(curIndex));
			options.max = curPair.val.val;
		}
		else
			throw std::logic_error("Unrecognized option key at position " + std::to_string(curIndex));

		curIndex = nextIndex;
	}

	return end + 1;
}

bool _checkTableStructure(lua_State *L, int sIndex, const std::string &str, size_t start, size_t end, std::string curTreePos, std::string &errMsg);

bool inline checkDefinedPair(lua_State *L, int sIndex, const std::string &str, const tableCheckKeyValPair &pair, std::string curTreePos, std::string &errMsg)
{
	if (pair.val.isFlag)
	{
		if (!(getLTypeFlag(lua_type(L, sIndex)) & pair.val.val))
		{
			//errMsg += std::to_string(lua_type(L, sIndex)) + "," + std::to_string(getLTypeFlag(lua_type(L, sIndex))) + "," + std::to_string(pair.val.val);
			errMsg += "val " + curTreePos + "." + pair.key.name + " has invalid type";
			return false;
		}
	}
	else if (pair.val.isDefinedTable)
	{
		if (lua_type(L, sIndex) != LUA_TTABLE)
		{
			errMsg += "val " + curTreePos + "." + pair.key.name + " must be a table";
			return false;
		}

		if (!_checkTableStructure(L, sIndex, str, pair.val.definedTableStart + 1, pair.val.definedTableEnd - 1, curTreePos + "->" + pair.key.name, errMsg))
			return false;
	}
	else //must be a num val
	{
		if (lua_type(L, sIndex) != LUA_TNUMBER)
		{
			errMsg += "val " + curTreePos + "." + pair.key.name + " must be a num";
			return false;
		}

		if (pair.val.val != lua_tointeger(L, sIndex))
		{
			errMsg += "val " + curTreePos + "." + pair.key.name + " must be " + std::to_string(pair.val.val);
			return false;
		}
	}

	return true;
}

std::string lToStr_t(lua_State *L, int sIndex)
{
	if (lua_type(L, sIndex) == LUA_TSTRING)
		return lua_tostring(L, sIndex);
	else if (lua_type(L, sIndex) == LUA_TNUMBER)
		return std::to_string(lua_tointeger(L, sIndex));
	
	std::string s = lua_typename(L, lua_type(L, sIndex));
	s += ",";
	s += lua_tostring(L, sIndex);

	return s;
}

bool _checkTableStructure(lua_State *L, int sIndex, const std::string &str, size_t start, size_t end, std::string curTreePos, std::string &errMsg)
{
	size_t curIndex = start;
	bool hasOptions;
	tableCheckOptions options;
	std::vector<tableCheckKeyValPair> pairs;

	while (curIndex < end)
	{
		if (str[curIndex] == '(')
		{
			curIndex = getTableCheckOptions(options, str, curIndex, end) + 1;
			hasOptions = true;
		}
		else
		{
			tableCheckKeyValPair newPair;
			curIndex = getKeyValPair(newPair, str, curIndex, end) + 2;
			pairs.push_back(newPair);
		}
	}

	if (hasOptions)
	{
		int numEntries = 0;
		if (sIndex < 0)
			sIndex--;

		lua_pushnil(L);  /* first key */
		while (lua_next(L, sIndex))
		{
			// key at -2, val at -1
			numEntries++;
			int pairsIndex = -1;

			if (lua_type(L, -2) == LUA_TNUMBER || lua_type(L, -2) == LUA_TSTRING)
			{
				std::string keyName;
				if (lua_type(L, -2) == LUA_TNUMBER)
					keyName = std::to_string(lua_tointeger(L, -2));
				else
					keyName = lua_tostring(L, -2);

				for (size_t i = 0; i < pairs.size(); i++)
					if (pairs[i].key.name == keyName)
						pairsIndex = i;
			}

			if (pairsIndex == -1)
			{
				if (options.keyF != -1 && !(getLTypeFlag(lua_type(L, -2)) & options.keyF))
				{
					errMsg += "key " + curTreePos + "." + lToStr_t(L, -2) + " has invalid type";
					lua_pop(L, 1);
					return false;
				}

				if (options.valF != -1 && !(getLTypeFlag(lua_type(L, -1)) & options.valF))
				{
					errMsg += "val " + curTreePos + "." + lToStr_t(L, -1) + " has invalid type";
					lua_pop(L, 1);
					return false;
				}
			}
			else
			{
				if (!checkDefinedPair(L, -1, str, pairs[pairsIndex], curTreePos, errMsg))
				{
					lua_pop(L, 1);
					return false;
				}
			}

			lua_pop(L, 1);
		}

		if (options.min != -1 && numEntries < options.min)
		{
			errMsg += curTreePos + " must have min " + std::to_string(options.min) + " entries";
			return false;
		}
		else if (options.max != -1 && numEntries > options.max)
		{
			errMsg += curTreePos + " must have max " + std::to_string(options.max) + " entries";
			return false;
		}
	}
	else
	{
		for (size_t i = 0; i < pairs.size(); i++)
		{
			lua_getfield(L, sIndex, pairs[i].key.name.c_str());

			if (lua_type(L, -1) == LUA_TNIL && pairs[i].key.optional)
				continue;
			else
				if (!checkDefinedPair(L, -1, str, pairs[i], curTreePos, errMsg))
				{
					lua_pop(L, 1);
					return false;
				}

			lua_pop(L, 1);
		}
	}

	return true;
}

void checkTableStructure(lua_State *L, int sIndex, std::string structure)
{
	if (lua_type(L, sIndex) != LUA_TTABLE)
		luaL_error(L, "argument must be a table");

	int sTop = lua_gettop(L);

	delBlank(structure);
	if (findClosingBracketIndex(structure, 0, structure.length() - 1, '}', true) != structure.length() - 1)
		throw std::logic_error("Structure does not stop behind closing bracket");

	std::string errMsg = "invalid argument, ";
	if (!_checkTableStructure(L, sIndex, structure, 1, structure.length() - 2, "arg", errMsg))
		luaL_error(L, errMsg.c_str());
}
/************   table arg checking end   ************/