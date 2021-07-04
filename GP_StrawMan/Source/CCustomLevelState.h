/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CCustomLevelState.h"
//
// Author: TJ "Rob Beats Us, Please Help" Farnie (TJF)
// 
// Purpose: Singleton that contains the loading state for custom levels that the user made.  
// Any levels not contained here are hung up on the fridge because they're very, very special.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CCUSTOMLEVEL_H
#define _CCUSTOMLEVEL_H

#include "IGameState.h"
#include <string>

using namespace std;

//Forward declarations
class CGame;
class CSGD_DirectInput;
class CGamePlayState;
class CBitMapFont;
class CSGD_TextureManager;

class CCustomLevelState : public IGameState
{
private:
	//Members and such
	CGame* m_pGame;
	CSGD_DirectInput* m_pDI;
	CGamePlayState* m_pGS;
	CBitMapFont* m_pFont;
	CSGD_TextureManager* m_pTM;

	int m_nBackGroundID;
	int m_nLevelID;
	int m_nCurrSelection;
	int	m_nListBot;
	bool m_bFilesFound;
	int m_nLastFileIndex;
	string m_szAllFiles[200];
	int m_nListTop;
	string m_szCustomLevel;
	int m_nMaxPages;
	int m_nCurrentPage;

	//The following functions make this a singleton.
	CCustomLevelState();
	CCustomLevelState( const CCustomLevelState&);
	~CCustomLevelState();
	CCustomLevelState& operator = (const CCustomLevelState&);

public:
	//Returns a pointer to the singleton instance
	static CCustomLevelState* GetInstance(void);

	//Renders stuff
	void Render();

	//Enters the state
	void Enter();

	//Exits the state
	void Exit();

	//Updates stuff
	void Update(float fDT);

	//Inputs stuff
	bool Input();
	
	//Function I stole from Justin to populate a list with known files and such.
	void PopulateList();

	//Another function I jacked from Justin that just draws out the list
	void DrawList();

	//Accessors
	string GetCustomLevel(void) { return m_szCustomLevel;}

};

#endif