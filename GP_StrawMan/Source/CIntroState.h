#pragma once
#include "IGameState.h"

#define MAXSTATES 14

class CAssetState;
class CIntroState : public IGameState
{
	CAssetState* m_pAS;

	float m_fTimers[MAXSTATES];
	int m_nAlphaCBB;
	int m_nAlphaGP;
	int m_nCurState;

	CIntroState();
	~CIntroState();
	CIntroState(const CIntroState&);
	CIntroState& operator=(const CIntroState&);
public:
	static CIntroState* GetInstance();
	void Enter();
	bool Input();
	void Update(float fDT);
	void Render();
	void Exit();
};