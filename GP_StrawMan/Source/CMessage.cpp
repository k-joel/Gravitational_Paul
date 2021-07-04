#include "CMessage.h"
#include "CHazards.h"

CCreateHazardMessage::CCreateHazardMessage(CHazards *pHazard) :  CBaseMessage(MSG_CREATE_HAZARD)
{
	m_pHazard = pHazard; 
	m_pHazard->AddRef(); 
}
CCreateHazardMessage::~CCreateHazardMessage()
{
	m_pHazard->Release();
	m_pHazard = NULL; 
}

CDestroyHazardMessage::CDestroyHazardMessage(CHazards *pHazard) : CBaseMessage(MSG_DESTROY_HAZARD)
{
	m_pHazard = pHazard; 
	m_pHazard->AddRef(); 
}

CDestroyHazardMessage::~CDestroyHazardMessage()
{
	m_pHazard->Release(); 
	m_pHazard = NULL; 
}

CExitLevelMessage::CExitLevelMessage(bool bLevelComplete, float fLevelTime) : CBaseMessage(MSG_EXIT_LEVEL)
{
	m_bLevelComplete = bLevelComplete;
	m_fLevelTime = fLevelTime;
}


CCurrencyCollectedMessage::CCurrencyCollectedMessage(int newIndex) : CBaseMessage(MSG_CURRENCY_COLLECT)
{
	GemIndex = newIndex;	
}

CCurrencyCollectedMessage::~CCurrencyCollectedMessage()
{

}

