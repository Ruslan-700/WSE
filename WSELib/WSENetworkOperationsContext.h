#pragma once

#include "WSEOperationContext.h"

class WSENetworkOperationsContext : public WSEOperationContext
{
public:
	WSENetworkOperationsContext();

protected:
	virtual void OnLoad();
};
