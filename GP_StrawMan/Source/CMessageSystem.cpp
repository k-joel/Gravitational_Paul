#include "CMessageSystem.h"

CMessageSystem* CMessageSystem::GetInstance(void)
{
	static CMessageSystem instance; 
	return &instance; 
}

void CMessageSystem::InitMessageSystem(MESSAGEPROC pMsgProc)
{
	if(!pMsgProc)
		return; 

	this->m_pMsgProc = pMsgProc;  
}

void CMessageSystem::SendMsg(CBaseMessage* pMsg)
{
	if(!pMsg) 
		return; 

	this->m_MsgQueue.push(pMsg);
}

void CMessageSystem::ProcessMessages(void)
{
	if(!this->m_pMsgProc) 
		return; 

	while(!this->m_MsgQueue.empty())
	{
		m_pMsgProc(m_MsgQueue.front()); 
		delete m_MsgQueue.front(); 
		m_MsgQueue.pop(); 
	}
}

void CMessageSystem::ClearMessages(void)
{
	while(!this->m_MsgQueue.empty())
	{
		delete m_MsgQueue.front(); 
		m_MsgQueue.pop(); 
	}
}

void CMessageSystem::ShutdownMessageSystem(void)
{
	ClearMessages(); 

	m_pMsgProc = NULL; 
}