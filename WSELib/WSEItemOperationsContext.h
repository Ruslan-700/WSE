#pragma once

#include "WSEOperationContext.h"

class WSEItemOperationsContext : public WSEOperationContext
{
public:
	WSEItemOperationsContext();

protected:
	virtual void OnLoad();
};
