#pragma once

#include "WSE.h"
#include <fstream>
#include <algorithm>

enum lTypeF
{
	lNone			= 0x0000,
	lNil			= 0x0001,
	lBool			= 0x0002,
	lLightUserData	= 0x0004,
	lNum			= 0x0008,
	lStr			= 0x0010,
	lTable			= 0x0020,
	lFunc			= 0x0040,
	lUserData		= 0x0080,
	lThread			= 0x0100,
	lPos			= 0x0200,
	lCdata			= 0x0400,
	lAny			= 0xFFFF,
};

struct gameConst
{
	std::string name;
	float val;
};

void gPrint(const char *msg);
void gPrint(const char *msg);
void gPrint(const std::string &s);
void gPrintf(const char *format, ...);
void gPrintf(const std::string &format, ...);

bool fileExists(const std::string& name);

void removeChar(std::string &str, char c);
void delBlank(std::string &str);
void discardComment(std::string &str);
template<typename Out>
void split(const std::string &s, char delim, bool skipEmpty, Out result);
std::vector<std::string> split(const std::string &s, char delim, bool skipEmpty);
size_t countChar(const std::string &s, char c);

std::string getLuaScriptDir();
void setOperandToLocalVar(__int64 &operand, int localsIndex);
int checkLArgs(lua_State *L, int minCount, int maxCount, ...);
int getTemplateNo(const char *id);
void printStack(lua_State *L);
void checkStackIndex(WSELuaOperationsContext *context, int index);

bool lIsVec3(lua_State *L, int index);
bool lIsRot(lua_State *L, int index);
bool lIsPos(lua_State *L, int index);

rgl::vector4 lToVec3(lua_State *L, int index);
rgl::orientation lToRot(lua_State *L, int index);
rgl::matrix lToPos(lua_State *L, int index);

void lPushVec3(lua_State *L, const rgl::vector4 &vec);
void lPushRot(lua_State *L, const rgl::orientation &rot);
void lPushPos(lua_State *L, const rgl::matrix &pos);

void addConstantsFromFileToTable(std::string fileName, lua_State *L, std::string name);

void checkTableStructure(lua_State *L, int sIndex, std::string structure);
