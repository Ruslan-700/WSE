#pragma once

#include "WSEOperationContext.h"

class WSEAgentOperationsContext : public WSEOperationContext
{
public:
	WSEAgentOperationsContext();

protected:
	virtual void OnLoad();
};
