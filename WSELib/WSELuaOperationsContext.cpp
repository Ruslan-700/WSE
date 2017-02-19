#include <fstream>
#include <algorithm>
#include <regex>
#include "WSELuaOperationsContext.h"
#include "WSE.h"
#include "WSEScriptingContext.h"

void gPrint(const char *msg)
{
#if defined WARBAND
	warband->window_manager.display_message(msg, 0xFFFF5555, 0);
#endif

	warband->log_stream.write_c_str("\n");
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

bool fileExists(const std::string& name) {
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

void WSELuaOperationsContext::applyFlagListToOperationMap(std::unordered_map<std::string, std::vector<std::string>*> &flagLists, std::string listName, unsigned short flag, std::string opFile)
{
	auto l = flagLists.find(listName);
	if (l != flagLists.end())
	{
		for (size_t i = 0; i < l->second->size(); i++)
		{
			std::string curKey = l->second->at(i);
			auto op = operationMap.find(curKey);

			if (op == operationMap.end())
				gPrintf("WSELuaOperationsContext: Warning reading %s, trying to set flag %s for non-existing operation [%s]", opFile.c_str(), listName.c_str(), curKey.c_str());
			else
				op->second.flags |= flag;
		}
	}
}

WSELuaOperationsContext::WSELuaOperationsContext() : WSEOperationContext("array", 5100, 5199)
{
}

void WSELuaOperationsContext::OnLoad()
{
	//RegisterOperation("lua_call", luaCall, Both, Lhs, 3, 16,
	//	"Creates an array object of <1> (0: Integer, 1: String, 2: Position) and stores its ID into <0>. You can specify up to 14 dimensions, from <2> to <15>. The array will be initialized by default with 0 / empty string / 0-position.",
	//	"destination", "type_id", "Dim 0", "Dim 1", "Dim 2", "Dim 3", "Dim 4", "Dim 5", "Dim 6", "Dim 7", "Dim 8", "Dim 9", "Dim 10", "Dim 11", "Dim 12", "Dim 13");
	
	initLua();
}

void WSELuaOperationsContext::OnUnload()
{
	lua_close(luaState);
}

void WSELuaOperationsContext::printLastError()
{
	const char* msg = lua_tostring(luaState, -1);
	gPrint(msg);
	lua_pop(luaState, 1);
}

inline void WSELuaOperationsContext::initLua()
{
	luaState = luaL_newstate();
	luaL_openlibs(luaState);

	loadOperations();
	initLGameTable();
	doMainScript();
}

inline void WSELuaOperationsContext::loadOperations()
{
	std::string opFile = getLuaScriptDir() + "header_operations.py";

	if (!fileExists(opFile))
		return;

	std::ifstream opStream(opFile);
	std::string curLine;
	int curLineNum = 0;

	std::smatch curMatches;

	std::regex opRegEx( R"((\w+)=(\d+)#\(\1((?:,<\w+>)*)((?:,\[<\w+>\])*)\).*)" );
	std::regex listStartRegEx( R"((\w+)(\+)?=\[((?:\w+,)*(?:\w+)?)(\]?))" );

	std::unordered_map<std::string, std::vector<std::string>*> flagLists;

	while (std::getline(opStream, curLine))
	{
		curLineNum++;
		delBlank(curLine);
		//gPrint(curLine);

		if (curLine.length() == 0 || curLine[0] == '#')
			continue;
		
		if (std::regex_match(curLine, curMatches, opRegEx))
		{
			//for (size_t i = 0; i < curMatches.size(); i++)
			//	gPrintf("match #%i: %s", i, curMatches.str(i).c_str());

			if (curMatches.size() < 3)
				continue;

			gameOperation newOp;
			newOp.flags = 0;

			newOp.opcode = std::stoi(curMatches.str(2));

			size_t reqArgs = 0, addArgs = 0;

			if (curMatches[3].length())
				reqArgs = countChar(curMatches.str(3), '<');
			
			if (curMatches[4].length())
				addArgs = countChar(curMatches.str(4), '<');

			if (reqArgs + addArgs > 16)
			{
				gPrintf("WSELuaOperationsContext: Error reading %s, line %i: arg count > 16", opFile.c_str(), curLineNum);
			}
			else 
			{
				newOp.minOperands = static_cast<short>(reqArgs);
				newOp.maxOperands = static_cast<short>(reqArgs + addArgs);

				operationMap[curMatches.str(1)] = newOp;

				//gPrintf("added op %s, code %i, #min args %i, #max args %i", curMatches.str(1).c_str(), newOp.opcode, newOp.minOperands, newOp.maxOperands);
			}
		}
		else if (std::regex_match(curLine, curMatches, listStartRegEx))
		{
			std::string listName = curMatches.str(1);
			bool add = curMatches.str(2) == "+";
			std::string curKeysStr = curMatches.str(3);
			bool end = curMatches.str(4) == "]";

			auto l = flagLists.find(listName);

			if (!add)
			{
				if (l != flagLists.end())
					delete l->second;

				flagLists[listName] = new std::vector<std::string>();
			}
			else
			{
				if (l == flagLists.end()) 
				{
					gPrintf("WSELuaOperationsContext: Error reading %s, line %i: trying to extend a non-existing list", opFile.c_str(), curLineNum);
					continue;
				}
			}

			//gPrintf("curKeyStr (list start): [%s], len: %i", curKeysStr.c_str(), curKeysStr.length());
			if (curKeysStr.length())
			{
				std::vector<std::string> curKeys = split(curKeysStr, ',', true);

				for (size_t i = 0; i < curKeys.size(); i++)
					flagLists[listName]->push_back(curKeys[i]);
			}

			curKeysStr = "";
			while (!end && std::getline(opStream, curLine))
			{
				curLineNum++;
				delBlank(curLine);
				discardComment(curLine);

				size_t endPos = curLine.find(']');

				if (endPos != std::string::npos)
				{
					end = true;
					curLine.erase(endPos);
				}

				curKeysStr += curLine;
			}

			//gPrintf("curKeyStr: [%s], len: %i", curKeysStr.c_str(), curKeysStr.length());
			if (curKeysStr.length()) 
			{
				std::vector<std::string> curKeys = split(curKeysStr, ',', true);

				for (size_t i = 0; i < curKeys.size(); i++)
					flagLists[listName]->push_back(curKeys[i]);
			}			
		}
		else 
		{
			gPrintf("WSELuaOperationsContext: Warning reading %s, could not process line %i", opFile.c_str(), curLineNum);
		}
	}

	applyFlagListToOperationMap(flagLists, "lhs_operations", WSEOperationFlags::Lhs, opFile);
	applyFlagListToOperationMap(flagLists, "global_lhs_operations", WSEOperationFlags::Lhs, opFile);
	applyFlagListToOperationMap(flagLists, "can_fail_operations", WSEOperationFlags::Cf, opFile);

	auto l = flagLists.begin();
	while (l != flagLists.end())
	{
		delete l->second;
		l++;
	}
}

inline void WSELuaOperationsContext::initLGameTable()
{
	lua_newtable(luaState);
	
	lua_pushcfunction(luaState, lGameTableHandler);
	lua_setfield(luaState, -2, "execOperation");

	lua_setglobal(luaState, "game");

	const char *s =
		"game.mt = {}\n"
		"game.mt.__index = function(table, key)\n"
			"return function(...)\n"
				"game.execOperation(key,...)\n"
			"end\n"
		"end\n"
		"setmetatable(game, game.mt)\n";

	if (luaL_dostring(luaState, s))
		printLastError();
}

inline void WSELuaOperationsContext::doMainScript()
{
	std::string mainFile = getLuaScriptDir() + "main.lua";

	if (fileExists(mainFile))
	{
		int result = luaL_loadfile(luaState, mainFile.c_str());

		if (result)
		{
			printLastError();
		}
		else
		{
			result = lua_pcall(luaState, 0, LUA_MULTRET, 0);
			if (result)
				printLastError();
		}
	}
}

static int lGameTableHandler(lua_State *L)
{
	int n = lua_gettop(L);

#if defined DEBUG
	char ab[1000];
	sprintf_s(ab, "game table call - num args: %i", n);
	print(ab);

	for (int i = 1; i <= n; i++)
	{
		const char *s = lua_tostring(L, i);
		if (s != NULL)
		{
			sprintf_s(ab, "arg%i: %s", i, s);
			print(ab);
		}		
	}
#endif

	return 0;
}