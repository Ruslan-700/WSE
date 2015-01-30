#pragma once

#include "WSEOperationContext.h"

class WSESiteOperationsContext : public WSEOperationContext
{
public:
	WSESiteOperationsContext();
	void SetSiteTerrainCode(wb::site *site, int key_no, int shift, int bits, int value);

protected:
	virtual void OnLoad();
};
