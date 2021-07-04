#pragma once
#include "IGameState.h"

class COutroState : public IGameState
{
	int m_nCurState;
	float m_fTimer;

	COutroState();
	~COutroState();
	COutroState(const COutroState&);
	COutroState& operator=(const COutroState&);
public:
	static COutroState* GetInstance();
	void Enter();
	bool Input();
	void Update(float fDT);
	void Render();
	void Exit();
};