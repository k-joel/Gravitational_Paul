/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CHowToPlay.h"
//
// Author: TJ "Real Men Don't RTFM" Farnie (TJF)
// 
// Purpose: Singleton that tells the idio-USER how to play.  ..........hi mom!
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CHOWDOGSAREFUNNY_H
#define _CHOWDOGSAREFUNNY_H

#include "IGameState.h"

class CGame;
class CSGD_TextureManager;
class COptionsState;
class CSGD_DirectInput;

class CHowToPlay : public IGameState
{
private:
	CGame* m_pGame;		
	CSGD_TextureManager* m_pTM;		
	COptionsState* m_pOptions;
	CSGD_DirectInput* m_pDI;	

	int m_nImageID;

	//keeping these functions hidden in here to hide stuff.  AND MAKE IT A SINGLETON.
	CHowToPlay();
	CHowToPlay( const CHowToPlay&);
	~CHowToPlay();
	CHowToPlay& operator = (const CHowToPlay&);

public:
	static CHowToPlay* GetInstance();
	void Enter();
	void Exit();
	bool Input();
	void Update(float fDT);
	void Render();

	//These render functions will display the gameplay controls based on the current keybinding
	void RenderKControls(int controlscheme);
	void RenderCControls(int controlscheme);

};
#endif