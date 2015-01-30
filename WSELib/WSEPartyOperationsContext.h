#pragma once

#include "WSEOperationContext.h"

class WSEPartyOperationsContext : public WSEOperationContext
{
public:
	WSEPartyOperationsContext();

protected:
	virtual void OnLoad();
};
