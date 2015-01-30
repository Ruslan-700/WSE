#pragma once

#include "WSEOperationContext.h"

class WSEEngineOperationsContext : public WSEOperationContext
{
public:
	WSEEngineOperationsContext();

protected:
	virtual void OnLoad();
};
