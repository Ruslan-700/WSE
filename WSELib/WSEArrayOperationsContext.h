#pragma once

#include <functional>
#include "WSEObjectManager.h"
#include "WSEOperationContext.h"

#define MAX_DIMENSIONS 14
#define SORT_F_DESCENDING		1
#define SORT_F_CASE_INSENSITIVE	2

struct WSEArraySortOptions
{
	bool sortDescending;
	bool sortCaseInsensitive;
	int sortCmpScriptNo;

	bool setSortMode(int mode, int maxMode, bool isCustomScript);
};

bool cmpInt(const int &a, const int &b, size_t index1, size_t index2, const WSEArraySortOptions &opt);
bool cmpStr(const std::string &a, const std::string &b, size_t index1, size_t index2, const WSEArraySortOptions &opt);
bool cmpPos(const rgl::matrix &a, const rgl::matrix &b, size_t index1, size_t index2, const WSEArraySortOptions &opt);
bool customCmp(size_t index1, size_t index2, const WSEArraySortOptions &opt);

enum cmp_operation{
	eq = 0,
	neq = 1,
	gt = 2,
	ge = 3,
	lt = 4,
	le = 5
};

class WSEArrayOperationsContext : public WSEOperationContext
{
public:
	WSEArrayOperationsContext();

	void *GetArray(int id);

protected:
	virtual void OnLoad();
	virtual void OnUnload();

public:
	WSEObjectType *m_type;
	WSEArraySortOptions sortOptions;
};