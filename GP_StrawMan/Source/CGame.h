/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CGame.h"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Declares the main CGame class to be used to run the game logic. This will contain the Game States and have player
//			data in it. This is a singleton class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DEF_CGAME_H
#define DEF_CGAME_H

#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_DirectSound.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_WaveManager.h"
#include "CMessageSystem.h"
#include "CEventSystem.h"
#include "SaveSystem.h"

#include <XInput.h>                       // Defines XBOX controller API

#include <vector>
using std::vector;

class IGameState;
class CBitMapFont;
class CGameSaveFile;

struct GEMS
{
	bool gems[15];
};

class CGame
{
private:

	//SGD Wrappers
	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectSound*		m_pDS;
	CSGD_DirectInput*		m_pDI;
	CSGD_WaveManager*		m_pWM;
	CMessageSystem*			m_pMS;
	CEventSystem*			m_pES;

	//GameState Vector
	vector<IGameState*>		m_vStates;

	//ingame systems

	CBitMapFont*			m_pFont;
	//program data / stats
	int						m_nWindowWidth;
	int						m_nWindowHeight;
	//windowed below a few lines in save area
	DWORD					m_dwTimeStamp;
	float					m_fElapsedTime;
	DWORD					m_dwPreviousTimeStamp;

	//savefile data
	CGameSaveFile*			m_pCurrFile;
	//save file data - will be synced on load, will load last sound data otherwise
	bool					m_bIsWindowed;
	bool					m_bIsCustom; //Bool to tell if the level being loaded is a custom level or normal.
	int						m_nDifficulty;
	float					m_fSFXVol;
	float					m_fMusicVol;
	int						m_nPanning;
	int						m_nCurrentLevel;
	int						m_nCurrentSaveSlot;
	bool					*m_bUnlocks;
	float					*m_fTimes;
	GEMS					m_sGems;
	
	//numbers needed for achievement tracking
	float					m_fAchievTimer;
	int						m_nAchievAlpha;
	int						m_nCurrAchiev;
	bool					m_bShowAchiev;
	bool					m_bCheater;
	bool					m_bGunCheat;

	//It's Keybinding time here be the variables for keybinding
	//These variables keep track of the current keybind layout for the keyboard and controller
	//used in How to play and Keybind State
	int				m_nKeyboardKeybind;
	int				m_nControllerKeybind;
	//This should be the correct variable type for the keyboard
	unsigned char	m_cKMoveLeft;
	unsigned char	m_cKMoveRight;
	unsigned char	m_cKJump;
	//These are the 3 well variables if they are set to the mouse they will be 0,1,2 otherwise like other keys.
	unsigned char	m_cKShootPush;
	unsigned char	m_cKShootPull;
	unsigned char	m_cKCloseAll;

	//This should be the correct variable type for the Xbox Controller
	WORD			m_wCMoveLeft;
	WORD			m_wCMoveRight;		
	WORD			m_wCJump;		
	WORD			m_wCShootPush;		
	WORD			m_wCShootPull;		
	WORD			m_wCCloseAll;		

	bool			m_bLevelUnlock; 
	bool			m_bCollisionUnlock; 





	//singleton private methods
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “constructor”
//
// Purpose: initializes values for the class to run. Declared private as a singlton
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGame();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “destructor”
//
// Purpose: Cleans up any memeory needed before deleting the class. declared private for singleton use
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
~CGame();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “copy Constructor”
//
// Purpose: Creats a new instance of the class from an old one. Declared private for singeton use.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGame(const CGame&);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “assignment operator”
//
// Purpose: Copies the data from one instance to another. Decalred hidden for singlton use.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGame& operator = (const CGame&);

public:

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Update”
//
// Purpose: Updates the CGame and current top state opject based on time increment, CGame is where the initial time increment
//			comes from for all game classes update.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Update(float fDT);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Input”
//
// Purpose: Gathers Information from the user as well as handes the buffering of the keys. This input has top tier priority over
//			all others, and will exceute first
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Input();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Render”
//
// Purpose: Draws all CGame images to screen as well as calling render on the current game state.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Render();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Init”
//
// Purpose: Sets up all inital values for the CGame. This is used to establish the game, as it cannot be initalized with an
//			overloaded constructor as it is a singleton.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Init(HWND hWnd, HINSTANCE hInstance, int nWindowWidth, int nWindowHeight, bool bIsWindowed);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Main”
//
// Purpose: This is the main game logic for the program. This will increment time, update all of CGame and the current gamestate,
//			and then render it all to screen. This function will pass instrcution to winmain when the program has terminated.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Main();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Shutdown”
//
// Purpose: Shuts down all game systems, cleans up memory, and prepares the game for closing. This is called in winmain once the
//			main game loop ahs exited.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Shutdown();

	//stateswap functions

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “PushState”
//
// Purpose: Pushes the passed in Game state onto the stack, and then enters it. Will only update the "top" state.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PushState(IGameState* pState);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “PopState”
//
// Purpose: Exits the current GameState, pops it off of the stack, and then will update the new top state.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PopState();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “RemoveAllStates”
//
// Purpose: Exits and removes all states fromt the vector one by one.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RemoveAllStates();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “GetInstance”
//
// Purpose: Returns a pointer to the static singleton instance of CGame
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static CGame* GetInstance();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: "MessageProc"
//
// Purpose: Returns a pointer to the static singleton instance of CGame
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
friend static void MessageProc(CBaseMessage* pMsg);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “SyncWithFile”
//
// Purpose: This function takes in a slot number and syncs the game to those settings. This is called after loading the file,
//			or anytime a change is made to it (for autosaving)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SyncWithFile(int slotNum);

//Accesors
bool GetIsCustom() {return m_bIsCustom;}
int GetWindowHeight() {return m_nWindowHeight;}
int GetWindowWidth() {return m_nWindowWidth;}
int GetDifficulty() {return m_nDifficulty;}
int GetLoadLevel()  {return m_nCurrentLevel;}
int	GetRangeLevel() {return m_pCurrFile->GetSaveData()->tAllSaves[m_nCurrentSaveSlot].nUpgrades[STAT_RANGE];}
int	GetDurationLevel(){return m_pCurrFile->GetSaveData()->tAllSaves[m_nCurrentSaveSlot].nUpgrades[STAT_DURATION];}
int	GetRechargeLevel(){return m_pCurrFile->GetSaveData()->tAllSaves[m_nCurrentSaveSlot].nUpgrades[STAT_RECHARGE];}
int	GetTotalCurrency(){return m_pCurrFile->GetSaveData()->tAllSaves[m_nCurrentSaveSlot].nUnspentCurrency;}
CGameSaveFile* GetSaveFile() {return m_pCurrFile;}
int GetCurrentSaveSlot() {return m_nCurrentSaveSlot;}
bool* GetCurrStageCompletes();
float*  GetLevelTimes();
bool	GetGunCheat() {return m_bGunCheat;}

bool GetLevelUnlock() {return m_bLevelUnlock;}
bool GetCollisionUnlock() {return m_bCollisionUnlock;}

void SetLevelUnlock(bool level) {m_bLevelUnlock = level;}
void SetCollisionUnlock(bool collide) {m_bCollisionUnlock = collide;}

//Keybinding Accessors
int			  GetKeyboardLayout() {return m_nKeyboardKeybind;}
int			  GetControllerLayout() {return m_nControllerKeybind;}

//Keyboard
unsigned char GetKMoveLeft()	{return m_cKMoveLeft;}
unsigned char GetKMoveRight()	{return m_cKMoveRight;}
unsigned char GetKJump()		{return m_cKJump;}
unsigned char GetKShootPush()	{return m_cKShootPush;}
unsigned char GetKShootPull()	{return m_cKShootPull;}
unsigned char GetKCloseAll()	{return m_cKCloseAll;}

//Controller
WORD		  GetCMoveLeft()	{return m_wCMoveLeft;}
WORD		  GetCMoveRight()	{return m_wCMoveRight;}
WORD		  GetCJump()		{return m_wCJump;}
WORD		  GetCShootPush()	{return m_wCShootPush;}
WORD		  GetCShootPull()	{return m_wCShootPull;}
WORD		  GetCCloseAll()	{return m_wCCloseAll;}



//Mutators
void SetIsCustom(bool bCustom) {m_bIsCustom = bCustom;}
void SetDifficulty(int diff) {m_nDifficulty = diff;}
void SetLoadLevel(int level) {m_nCurrentLevel = level;}
void SetSaveFilePath(string newPath){m_pCurrFile->SetFileName(newPath.c_str());}
void LoadFile(){m_pCurrFile->Load();}
void SetGameSlot(int newSlot){if(newSlot < 3 && newSlot > 0)m_nCurrentSaveSlot = newSlot;}
void SetGunCheat(bool gunCheat) {m_bGunCheat = gunCheat;}

//Yay Keybinding Mutators
void	SetKeyboardLayout(int layout)				{m_nKeyboardKeybind = layout;}
void	SetControllerLayout(int layout)				{m_nControllerKeybind = layout;}

//Keyboard
void	SetKMoveLeft(unsigned char moveleft)		{m_cKMoveLeft = moveleft;}
void	SetKMoveRight(unsigned char moveright)		{m_cKMoveRight = moveright;}
void	SetKJump(unsigned char jump)				{m_cKJump = jump;}
void	SetKShootPush(unsigned char shootpush)		{m_cKShootPush = shootpush;}
void	SetKShootPull(unsigned char shootpull)		{m_cKShootPull = shootpull;}
void	SetKCloseAll(unsigned char closeall)		{m_cKCloseAll = closeall;}

//Controller
void	SetCMoveLeft(WORD moveleft)			{m_wCMoveLeft = moveleft;}
void	SetCMoveRight(WORD moveright)		{m_wCMoveRight = moveright;}
void	SetCJump(WORD jump)					{m_wCJump = jump;}
void	SetCShootPush(WORD shootpush)		{m_wCShootPush = shootpush;}
void	SetCShootPull(WORD shootpull)		{m_wCShootPull = shootpull;}
void	SetCCloseAll(WORD closeall)			{m_wCCloseAll = closeall;}



//Kills the complete bool array
void KillCompletes();


//checks the save file for the achievment and if it is new, tells cgame to unlock it
void CheckSaveAchieves(unsigned int whichOne);
//unlock achievement - called by checksaveachieves if the save file if the achievment is new
void AchievmentUnlocked(unsigned int whichOne);

//draw achievs draws the achievment box to screen no matter what state its unlocked in
void DrawAchiev();

GEMS GetGemsPerLevel(int level);
};

#endif