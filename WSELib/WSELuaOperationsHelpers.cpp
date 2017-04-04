#include "WSELuaOperationsHelpers.h"

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

int checkLArgs(lua_State *L, int minCount, int maxCount, ...)
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
		lArgT a = va_arg(args, lArgT);

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

		errorType.erase(0, 1);

		luaL_error(L, "arg %i must be of type %s", i, errorType);
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

void printStack(lua_State *L)
{
	int size = lua_gettop(L);
	gPrintf("Stack top: %i", size);

	for (int i = 1; i <= size; i++)
	{
		gPrintf("arg%i: %s", i, lua_typename(L, lua_type(L, i)));
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

void lPushWSEMt(lua_State *L, const char *name, const char *nameSpace = "game")
{
	if (strlen(nameSpace))
	{
		lua_getfield(L, LUA_GLOBALSINDEX, nameSpace);
		lua_getfield(L, -1, name);
		lua_getfield(L, -1, "mt");

		lua_insert(L, -3);
		lua_pop(L, 2);
	}
	else
	{
		lua_getfield(L, LUA_GLOBALSINDEX, name);
		lua_getfield(L, -1, "mt");

		lua_insert(L, -2);
		lua_pop(L, 1);
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

	lPushWSEMt(L, "vector3", "");
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

	lPushWSEMt(L, "rotation");
	lua_setmetatable(L, -2);
}

void lPushPos(lua_State *L, const rgl::matrix &pos)
{
	lua_newtable(L);

	lPushVec3(L, pos.o);
	lPushRot(L, pos.rot);

	lua_setfield(L, -3, "rot");
	lua_setfield(L, -2, "o");

	lPushWSEMt(L, "pos");
	//gPrint(lua_typename(L, lua_type(L, -1)));
	lua_setmetatable(L, -2);
}