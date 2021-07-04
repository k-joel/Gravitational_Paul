#pragma once 

#include <string> 
using std::string; 

typedef string EVENTID; 

class CEvent
{
private:
	EVENTID		m_EventID; 
	void*		m_pParam; 
public:
	CEvent(EVENTID eventID, void* param = NULL)
	{
		m_EventID = eventID; 
		m_pParam = param; 
	}

	~CEvent() {}

	inline EVENTID  GetEventID(void) {return m_EventID;}
	inline void*    GetParam(void)   {return m_pParam;}
};