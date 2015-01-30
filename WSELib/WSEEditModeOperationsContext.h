#pragma once

#include "WSEOperationContext.h"

class WSEEditModeOperationsContext : public WSEOperationContext
{
public:
	WSEEditModeOperationsContext();

protected:
	virtual void OnLoad();
};
