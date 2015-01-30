#pragma once

#include "WSEOperationContext.h"

class WSEOptionsOperationsContext : public WSEOperationContext
{
public:
	WSEOptionsOperationsContext();

protected:
	virtual void OnLoad();
};
