#ifndef CUPGRADESTATE_H_
#define CUPGRADESTATE_H_

#include "IGameState.h"
#include <string>
using namespace std;
enum { MENU_RECHARGE, MENU_RANGE, MENU_DURATION, MENU_LEAVE, MENU_MAXIMUM };


class CSGD_Direct3D;
class CSGD_TextureManager;
class CGame;
class CSGD_DirectInput;


class CUpgradeState : public IGameState
{
private:

	

	CSGD_Direct3D*						m_pD3D;
	CSGD_TextureManager*				m_pTM;
	CGame*								m_pGame;
	CSGD_DirectInput*					m_pDI;

	int									m_nCurrSelection;
	string								m_szMenuItems[MENU_MAXIMUM];
	int									m_nImgIDBG;
	int									m_nUpgradeSoundID;
	int									m_nFailedUpgradeID;
	int									m_nBoxImageID;
	bool								m_bUpgradeConfirm;
	int									m_nConfirmSelection;

	//Proper Singleton and shit
	CUpgradeState();
	~CUpgradeState();
	CUpgradeState(const CUpgradeState&);
	CUpgradeState& operator=(const CUpgradeState&);



public:

	static CUpgradeState* GetInstance();

	void Enter();

	bool Input();

	void Update(float fDT);

	void Render();

	void Exit();

	bool CheckUpgrade(int nStat);


};

#endif