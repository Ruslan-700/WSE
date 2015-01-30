#pragma once

#include "WSEOperationContext.h"

class WSEInputOperationsContext : public WSEOperationContext
{
public:
	WSEInputOperationsContext();

protected:
	virtual void OnLoad();
};
