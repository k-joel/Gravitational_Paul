/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"COptionsState.h"
//
// Author: TJ "I've Got Options" Farnie (TJF)
// 
// Purpose: Singleton that contains the game options.  ...which is basically just volume settings.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef OPTIONSSTATE_H
#define OPTIONSSTATE_H

#include "IGameState.h"

//Forward declerations
class CGame;
class CSGD_TextureManager;
class CBitMapFont;
class CSGD_DirectInput;
class CSGD_FModManager;
class CGameSaveFile;


class COptionsState : public IGameState
{
private:
	CGameSaveFile*			m_pSaveFile;
	CGame*					m_pGame;
	CBitMapFont*			m_pFont;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CSGD_FModManager*		m_pFMod;
	int						m_nMenuSelection;
	int						m_nOptionsBG;
	float					m_fFXVolume;
	float					m_fMusicVolume;

	//Functions that make this bad boy a singleton
	COptionsState();
	COptionsState( const COptionsState&);
	~COptionsState();
	COptionsState& operator = (const COptionsState&);

public:
	//Function that returns a pointer to the singleton instance
	static COptionsState* GetInstance();

	//Getting the input
	bool Input();

	//Rendiggity endering.
	void Render();

	//And... updating?  I guess?
	void Update(float fDT);

	//For entering the state
	void Enter();

	//For exiting the state
	void Exit();
};

#endif