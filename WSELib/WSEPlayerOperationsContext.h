#pragma once

#include "WSEOperationContext.h"

class WSEPlayerOperationsContext : public WSEOperationContext
{
public:
	WSEPlayerOperationsContext();

protected:
	virtual void OnLoad();
};
