#pragma once

#include "WSEOperationContext.h"

class WSEDebugOperationsContext : public WSEOperationContext
{
public:
	WSEDebugOperationsContext();

protected:
	virtual void OnLoad();
};
