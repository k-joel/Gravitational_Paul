#include "CEventSystem.h"

void CEventSystem::RegisterClient(EVENTID eventID, IListener* pClient)
{
	if(!pClient || AlreadyRegistered(eventID, pClient)) return; 

	m_Database.insert(make_pair(eventID, pClient)); 
}

void CEventSystem::UnregisterClient(EVENTID eventID, IListener *pClient)
{
	pair<multimap<EVENTID, IListener*>::iterator,
		multimap<EVENTID, IListener*>::iterator> range; 

	range = m_Database.equal_range(eventID); 

	for(multimap<EVENTID, IListener*>::iterator mmIter = range.first;
		mmIter != range.second; mmIter++)
	{
		if((*mmIter).second == pClient)
		{
			mmIter = m_Database.erase(mmIter); 
			break; 
		}
	}
}

void CEventSystem::UnregisterClientAll(IListener *pClient)
{
	multimap<string, IListener*>::iterator mmIter = m_Database.begin();

	while(mmIter !=m_Database.end())
	{
		if((*mmIter).second == pClient)
		{
			mmIter = m_Database.erase(mmIter);
		}
		else
			mmIter++;
	}
}

void CEventSystem::DispatchEvents(CEvent *pEvent)
{
		//	Make an iterator that will iterate all of our clients that
	//	should be receiveing this event
	pair<multimap<EVENTID, IListener*>::iterator,
		 multimap<EVENTID, IListener*>::iterator> range;

	//	Find all of the clients that are able to receive this event.
	range = m_Database.equal_range(pEvent->GetEventID());

	//	Go through the linked list of clients that are able to receive this event.
	for(multimap<EVENTID, IListener*>::iterator mmIter = range.first;
					mmIter != range.second; mmIter++)
	{
		//	Pass this event to the client

		(*mmIter).second->HandleEvent(pEvent);
	}
}

bool CEventSystem::AlreadyRegistered(EVENTID eventID, IListener *pClient)
{
	bool bIsAlreadyRegistered = false;

	//	Make an iterator that will iterate all of our clients that
	//	should be receiveing this event
	pair<multimap<EVENTID, IListener*>::iterator,
		 multimap<EVENTID, IListener*>::iterator> range;

	//	Find all of the clients that are able to receive this event.
	range = m_Database.equal_range(eventID);

	//	Go through the list of clients that are able to receive this event.
	for(multimap<EVENTID, IListener*>::iterator mmIter = range.first;
					mmIter != range.second; mmIter++)
	{
		//	check if the pointer is equal to the client
		if((*mmIter).second == pClient)
		{
			bIsAlreadyRegistered = true;
			break;
		}
	}

	return bIsAlreadyRegistered;
}

void CEventSystem::SendEvent(EVENTID eventID, void *pData)
{
	CEvent newEvent(eventID, pData);

	m_Events.push_back(newEvent);
}

void CEventSystem::ProcessEvents()
{
	while(m_Events.size())
	{
		DispatchEvents(&m_Events.front());
		m_Events.pop_front();
	}
}

void CEventSystem::ClearEvents()
{
	m_Events.clear(); 
}

void CEventSystem::ShutdownEventSystem()
{
	m_Events.clear(); 
}
