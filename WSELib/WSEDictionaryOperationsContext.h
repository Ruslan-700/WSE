#pragma once

#include "WSEDictionary.h"
#include "WSEObjectManager.h"
#include "WSEOperationContext.h"

class WSEDictionaryOperationsContext : public WSEOperationContext
{
public:
	WSEDictionaryOperationsContext();
	WSEDictionary *GetDictionary(int id);

protected:
	virtual void OnLoad();
	virtual void OnUnload();

public:
	WSEObjectType *m_type;
};
