#pragma once

#include "WSEOperationContext.h"
#include <Windows.h>

typedef char MD5Hash[33];

class WSEStringOperationsContext : public WSEOperationContext
{
public:
	WSEStringOperationsContext();

protected:
	virtual void OnLoad();

public:
	bool MD5(const byte *buffer, size_t size, MD5Hash out_hash);
};
