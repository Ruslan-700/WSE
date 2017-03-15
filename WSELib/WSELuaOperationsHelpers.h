#pragma once

#include "WSE.h"
#include <fstream>
#include <algorithm>

enum lArgT
{
	lNil = 0x1,
	lBool = 0x2,
	lLightUserData = 0x4,
	lNum = 0x8,
	lStr = 0x10,
	lTable = 0x20,
	lFunc = 0x40,
	lUserData = 0x80,
	lThread = 0x100
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

rgl::vector4 lGetVec3(lua_State *L, int index);
rgl::matrix lGetPos(lua_State *L, int index);
void lPushVec3(lua_State *L, const rgl::vector4 &vec);
void lPushRot(lua_State *L, const rgl::orientation &rot);
void lPushPos(lua_State *L, const rgl::matrix &pos);
