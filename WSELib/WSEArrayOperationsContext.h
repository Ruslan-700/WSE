#pragma once

#include "WSEDynMultiArray2.h"
#include "WSEObjectManager.h"
#include "WSEOperationContext.h"

#define MAX_DIMENSIONS 14

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
};