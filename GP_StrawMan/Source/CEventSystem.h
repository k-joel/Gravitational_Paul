#pragma once 

#include <list>
#include <map> 
using std::pair; 
using std::multimap;  
using std::list; 

#include "CEvent.h"
#include "IListener.h"

class CEventSystem
{
private:
	// Our database to hold clients to listen to events 
	multimap<EVENTID, IListener*> m_Database; 

	//Events waiting to be processed. 
	list<CEvent>    m_Events; 

	//Next two functions to only be used by this class 
	void DispatchEvents(CEvent* pEvent); 
	bool AlreadyRegistered(EVENTID eventID, IListener* pClient); 

	CEventSystem() {}
	CEventSystem(const CEventSystem&); 
	CEventSystem& operator=(const CEventSystem&);
	~CEventSystem() {}
public:

	static CEventSystem* GetInstance(void)
	{
		static CEventSystem instance; 
		return &instance;  
	}

	//This adds a client to the database. 
	//add client to specific bucket
	void RegisterClient(EVENTID eventID, IListener* pClient);

	//Unregisters the client 
	void UnregisterClient(EVENTID eventID, IListener* pClient); 

	//Removes the client from the database completely 
	void UnregisterClientAll(IListener* pClient); 

	//Sends the event
	void SendEvent(EVENTID eventID, void* pData = NULL); 

	void ProcessEvents(void); 

	//Clears all pending events 
	void ClearEvents(void); 

	//Unregisters all objects 
	void ShutdownEventSystem(void); 
}; 