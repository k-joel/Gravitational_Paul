#pragma once

#include "IGameState.h"
#include "CGame.h"

class CSGD_Direct3D;
class CSGD_TextureManager;
class CGame;
class CSGD_DirectInput;
class CCustomLevelState;

#include <vector>
using std::vector;

class CLevelSelect : public IGameState
{
private:
	CSGD_Direct3D*						m_pD3D;
	CSGD_TextureManager*				m_pTM;
	CGame*								m_pGame;
	CSGD_DirectInput*					m_pDI;
	CCustomLevelState*					m_pCustom;

	int									m_nSelection;
	int									m_nRowSelect;
	int									m_nAlpha;
	int									m_nBeta;
	//int									*Stages;
	int									Stages[10];
	int									m_nPcount;
	int									m_nBGid;
	bool								*m_pUnlocks;
	float								*m_pTimes;
	float								m_fTimer;
	int									m_nScreenShots[20];
	int									tester;
	int									m_nBox;
	GEMS								m_sGemCount[20];
	int									m_nGemCounter[20];

	struct tPlayer
	{
		bool  gemCollected[15];
		int	  gemCount;
	} m_sPlayer[20];

	////////////////////////////////////////////////////
	/// The constructors and destructors and copy and
	/// assignment operators for a proper singleton
	///
	///////////////////////////////////////////////////////
	CLevelSelect();

	~CLevelSelect();

	CLevelSelect& operator = (const CLevelSelect&);

	CLevelSelect(const CLevelSelect&);

public:
	static CLevelSelect* GetInstance();

	void Enter();

	bool Input();

	void Update(float fDT);

	void Render();

	void Exit();

	//Accessors
	int GetBackGroundID(void) {return m_nBGid;}
}; 