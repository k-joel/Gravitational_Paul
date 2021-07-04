#pragma once 
class CHazards; 

enum eMsgTypes {MSG_NULL = 0, MSG_CREATE_HAZARD, MSG_DESTROY_HAZARD, MSG_CURRENCY_COLLECT, MSG_EXIT_LEVEL}; 

class CBaseMessage 
{
private:
	int m_msgID; 

public:
	CBaseMessage(int id)
	{
		m_msgID = id; 
	}

	virtual ~CBaseMessage(void) {}

	int GetMsgID(void) {return m_msgID;}
}; 


class CCreateHazardMessage : public CBaseMessage
{
private:
	CHazards* m_pHazard; 
public:
	CCreateHazardMessage(CHazards* pHazard); 
	~CCreateHazardMessage(); 

	CHazards* GetHazard() {return m_pHazard;}
}; 


class CDestroyHazardMessage : public CBaseMessage
{
private: 
	CHazards* m_pHazard; 
public:
	CDestroyHazardMessage(CHazards* pHazard); 
	~CDestroyHazardMessage();

	CHazards* GetHazard() {return m_pHazard;}
}; 

class CExitLevelMessage : public CBaseMessage
{
private:
	bool m_bLevelComplete;
	float m_fLevelTime;
public:
	CExitLevelMessage(bool bLevelComplete, float fLevelTime);

	bool GetLevelComplete() {return m_bLevelComplete;}
	float GetLevelTime() {return m_fLevelTime;}
};

class CCurrencyCollectedMessage : public CBaseMessage
{
private:
	int GemIndex;
public:
	CCurrencyCollectedMessage(int newIndex);
	~CCurrencyCollectedMessage();

	int GetGemIndex(){return GemIndex;}
};

