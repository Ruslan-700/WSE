#pragma once

#include "WSEDynMultiArray2.h"
#include "WSEObjectManager.h"
#include "WSEOperationContext.h"

#define MAX_DIMENSIONS 14
#define SORT_F_DESCENDING		1
#define SORT_F_CASE_INSENSITIVE	2

class WSEArrayOperationsContext : public WSEOperationContext
{
public:
	WSEArrayOperationsContext();

	void *GetArray(int id);
	void setSortMode(int mode, int maxMode, bool isCustomScript);

protected:
	virtual void OnLoad();
	virtual void OnUnload();

	bool cmpInt(const int &a, const int &b);
	bool cmpStr(const std::string &a, const std::string &b);
	bool cmpPos(const rgl::matrix &a, const rgl::matrix &b);
	bool customCmp(int arg1, int arg2);

public:
	WSEObjectType *m_type;

	std::function<bool(const int&, const int&)> cmpIntCB;
	std::function<bool(const std::string&, const std::string&)> cmpStrCB;
	std::function<bool(const rgl::matrix&, const rgl::matrix&)> cmpPosCB;

protected:
	bool sortDescending;
	bool sortCaseInsensitive;
	int sortCmpScriptNo;
};