#pragma once 

#include "CEvent.h"


class IListener
{
public:
	IListener(void) {}
	virtual ~IListener(void) {}

	virtual void HandleEvent(CEvent* pEvent) = 0; 
}; 