/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CHUD.h"
//
// Author: TJ "Beware the CHUDs" Farnie (TJF)
// 
// Purpose: Declares the HUD object, which will be used to relay information to the player about the game.
//			It will keep track of the gun's energy level, as well as various other items of interest
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CHUD_H
#define CHUD_H

#include <windows.h>
//Forward declerations
class CSGD_TextureManager;
class CSGD_Direct3D;
class CGamePlayState;
class CBitMapFont;

class CHUD
{
private:
	//Private data members:
	CSGD_TextureManager*	m_pTM; //Pointer to the texture manager
	CSGD_Direct3D*			m_pD3D; //Pointer to the direct 3d thang
	CGamePlayState*			m_pGPS; //Pointer to the gameplay state
	CBitMapFont*			m_pFont;
	int						m_nHUDPictureID; //ID for the texture used to draw the HUD
	int						m_nLifeIconID; //ID for the life icon
	int						m_nBarImageID;
	int						m_nFillerImageID;
	int						m_nHUDUpperBound; //Upper boundery of the HUD (cleaner to use than horrible division all over the place
	int						m_nHUDLowerBound; //Lower boundery of the HUD (yes, it's the screen height, but I don't want those calls all over the place
	float					m_fElapsedTime; //Elapsed time "bucket"
	int						m_nTimer; //Timer for the level
	RECT					m_tBarFillRect; //rect used for the fill bar
	DWORD					m_lBarColor;
	RECT					m_tPowerUpRect;
	DWORD					m_lPowerUpColor;
	bool					m_bPowerUpOn;
	float					m_fPowerUpTime;
	int						m_nUnspentCurrency;
	DWORD					m_lTimerColor;

	//The following methods have been made private in order to keep this class a singleton.
	CHUD();
	~CHUD();
	CHUD( const CHUD&);
	CHUD& operator = (const CHUD&);

public:
	//Used to update the various elements of the HUD
	void Update(float fElapsedTime); 

	//Used to actually draw the HUD.
	void Render(); 

	//Used to get the instance of the singleton HUD
	static CHUD* GetInstance();

	//Accessors
	int GetTimer(void) {return m_nTimer;}
	int GetUpperBound(void) {return m_nHUDUpperBound;}
	//Mutations

	void StopWarning(void); 


};

#endif