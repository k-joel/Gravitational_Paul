#pragma once 

#include "CMessage.h"
#include <queue> 
using std::queue; 

typedef void (*MESSAGEPROC)(CBaseMessage*); 

class CMessageSystem
{
private:
	queue<CBaseMessage*>			m_MsgQueue; 
	MESSAGEPROC						m_pMsgProc; 

	CMessageSystem() {m_pMsgProc = NULL;}
	CMessageSystem(const CMessageSystem&); 
	CMessageSystem& operator=(const CMessageSystem&); 

	~CMessageSystem() {}

public:
	static CMessageSystem* GetInstance(void); 

	//How many messages waiting to be processed
	inline int GetNumMessages(void) {return (int)m_MsgQueue.size();}

	//Sends the function pointer for the system
	void InitMessageSystem(MESSAGEPROC pMsgProc); 

	//Sends the message into the queue to await processing 
	void SendMsg(CBaseMessage* pMsg); 

	//Processes all the messages that are waiting inside the queue
	void ProcessMessages(void); 
	
	//Clears any messages that may be remaining 
	void ClearMessages(void); 

	//Clears any messages in the queue
	void ShutdownMessageSystem(void); 

};