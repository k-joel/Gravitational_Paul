/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CCredits.h"
//
// Author: TJ "Roll That Beautiful Bean Footage" Farnie (TJF)
// 
// Purpose: The credits.  ....it shows the credits.  Woo.  Maybe it'll be a singleton?  Yeah,   
// why the Hell not.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CCREDITS_H
#define _CCREDITS_H

#include "IGameState.h"

//Forward declerations
class CBitMapFont;
class COptionsState;
class CSGD_TextureManager;
class CGame;

class CCredits : public IGameState
{
private:
	CBitMapFont* m_pFont;
	COptionsState* m_pOptions;
	CSGD_TextureManager* m_pTM;
	CGame* m_pGame;
	float m_fElapsedTime;
	int m_nOffset;

	int m_nImageID;

	//The following functions must be private in order for this to be a singleton. YO.
	CCredits();
	CCredits( const CCredits&);
	~CCredits();
	CCredits& operator = (const CCredits&);

public:
	static CCredits* GetInstance();
	void Enter();
	void Exit();
	void Render();
	void Update(float fDT);
	bool Input();

};

#endif