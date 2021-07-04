/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CGamePlayState.h"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Declares the Gameplay State for the main game. This will derived from IGameState
//			to execute Game logic.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DEF_GAMEPLAY_STATE
#define DEF_GAMEPLAY_STATE
#include "IGameState.h" //inherits from iGameState

//additional #includes here
#include "CObjectFactory.h"
#include <string>
using std::string;
#include "CBaseObject.h"
//forward declarations here
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_FModManager;
class CGravSystem;
class CGravWell;
class CGravObject;
class CBitMapFont;
class CPlayer;
class CObjectManager;
class CWalker;
class CTurret;
class CHUD;
class CTileManager;
class CGameSaveFile;

#define SHOT_ENERGY 100.0f

#define EASY_TIME 300.0f


//upgrade #defines
#define RANGE_INCREMENT 2.0f
#define DURATION_INCREMENT 1.0f
#define RECHARGE_INCREMENT 4.0f

class CGamePlayState : public IGameState
{
private:

	//system class pointers
	CSGD_TextureManager*		m_pTM;
	CSGD_WaveManager*			m_pWM;
	CGravSystem*				m_pGS; 
	CObjectFactory<string, CBaseObject>* m_pOF;
	CObjectManager*				m_pOM;
	CSGD_FModManager*			m_pFMod;
	CTileManager*				m_pTileMan;

	//class pointer vars
	CBitMapFont*				m_pText;
	CGameSaveFile*				m_pSaveFile;

	
	CPlayer*					m_pPlayer;
	CGravWell*					m_pShot;
	CHUD*						m_pCHUD;

	//state time variables

	//Image IDs

	int							m_nBGImageID;
	int							m_nPlayerImageID;
	int							m_nObjectImageID;
	int							m_nWellImageID;
	int							m_nWellClosedImageID;
	int							m_nTargetImageID;
	int							m_nWalkerImageID;
	int							m_nHeavyImageID;
	int							m_nSniperImageID;
	int							m_nBombardImageID;
	int							m_nPlatformImageID;
	int							m_nTurretImageID;
	int							m_nTripwireImageID;
	int							m_nFallSpikeImageID;
	int							m_nExitImageID;
	int							m_nPauseBoxID;

	//Pick Up Image IDs
	int							m_nCurrencyImageID;
	int							m_nBounceImageID;
	int							m_nWeightedImageID;

	//Sound FX IDs

	int							m_nCollideSoundID;
	int							m_nFireShotSoundID;
	int							m_nOutOfEnergySoundID;
	int							m_nShotDissipateSoundID;
	int							m_nShotBounceSoundID;
	int							m_nTerminalFallSoundID;

	//LOL IDs
	int							m_nStart1SoundID;
	int							m_nStart2SoundID;
	int							m_nStart3SoundID;

	int							m_nEnd1SoundID;
	int							m_nEnd2SoundID;
	int							m_nEnd3SoundID;

	int							m_nDeath1SoundID;
	int							m_nDeath2SoundID;
	int							m_nDeath3SoundID;

	//Music IDs
	int							m_nBGMusicSoundID;

	float						m_fPrevVol;

	//Additional HUD and user variables
	int							m_nPlayerLives; //Player lives
	float						m_fWeaponEnergy; //remaining energy for player to use
	bool						m_bEasyHitBuffer; //boool for if the player is on easy mode and gets a buffer (or still has it)
	int							m_nLevelCurrency;
	int							m_nTotalCurrency;
	D3DXVECTOR2					m_tPlayerSpawnLoc;

	//Pause Menu Variables  
	bool						m_bPaused; 
	int							m_nSelection; 

	string cheat; 
	string test; 
	int spot; 




	//Pickup Variables
	//There are accessors and mutators for all of these except the timer /*GetCurrency(),GetBounce(),GetWeighted()*/
	int							m_nCurrency; //Keep track of currency the player picks up throughout the level
	bool						m_bIsBounce; //Will get set to true inside of gameplay and passed along
	bool						m_bIsWeighted; // Same Shit Different Name
	float						m_fPowerUpTimer; // pretty self explanatory its a timer
	float						m_fStageTime; //the max time for the level, this will scale for the difficulty
	float						m_fCurrentTime;
	bool						m_bLevelComplete; //A NEW RECORD!
	bool						m_bLivesOut;

	//Cheat Code char and bool variables 
	int							m_nSpot; //which index in the cheat char array you are in 
	
	string						m_sAccess; 
	bool						m_bAccess; 
	string						m_cCheat; 

	

	//Scream Timer take this one out  

	//singletion functions private

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “constructor”
//
// Purpose: creates the class with usable inital memorya nd starting values
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGamePlayState();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “destructor”
//
// Purpose: cleans up any memory needed and then destoys the class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
~CGamePlayState();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Copy constructor”
//
// Purpose: Creates a new instance of the class from an old one. Declared priavte for singleton use.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGamePlayState(const CGamePlayState&);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “assignment operator”
//
// Purpose: copies one instance of the class to another. declared private for singlton use
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGamePlayState& operator = (const CGamePlayState&);

public:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “GetInstance”
//
// Purpose: creates a static pointer to the only instance of the game state. Because all memory is cleared in Exit, this can
//			then be safely deleted at program end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static CGamePlayState* GetInstance();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Enter”
//
// Purpose: Enters the GamePlay State, initializing the values to the needed level.
//			Once levels have been introduced, this will only load relvant resources
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Enter();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Input”
//
// Purpose: Gathers input from the user like the parent implementation. Calls specific game logic inputs
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Input();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Update”
//
// Purpose: Updates all game objects by time, including systems, and objects via the manager. This will progress the game
//			based on the logic, and call messages for progression or game over accoridingly
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Update(float fDT);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Render”
//
// Purpose: Draws active game objectss to the screen along with hud information
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Render();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Exit”
//
// Purpose: Unloads level and all relevant resources, updates CGame's player data accordingly
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Exit();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CreatePlayerShot”
//
// Purpose: Creates a player shot of the passed in bool type (push = true, pull = false) Will eventually
//			check energy and be called by messages as well as add them to the object manager
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreatePlayerShot(bool PushPull);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “RechargeEnergy”
//
// Purpose: This recharges the player energy based on the CGame Rechage rate. This is called once per frame in update.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RechargeEnergy(float fDT);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Creation Functions”
//
// Purpose: The Following functions create the demo objects of each class. These will be moved and refined in the Ogbject Factory
//			and manager Eventually
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void CreatePermWell(bool bType = false);
void ResetPermWell(bool bType);
void CreatePlayer(float spawnX = 200.0f, float spawnY = 50.0f);
void ResetPlayer();


////functions streamlined for Tile Manager Use
void CreateWalker(float posX, float posY, bool bImmune = false);
void CreateHeavy(float posX, float posY, bool bImmune = false, bool PushPull = false, bool OnOff = true,  bool ChangeForce = false);
void CreateSniper(float posX, float posY, bool bImmune = false, float AimRange = 7.0f * WorldConstants::fTileSize );
void CreateBombardier(float posX, float posY, bool bImmune = false, float AimRange = 7.0f * WorldConstants::fTileSize, bool bPushPull = false);
void CreateMovablePlatform(float posX, float posY);
void CreateAbsorbPlatform(float posX, float posY);
void CreateHazard(float posX, float posY, bool Respawn, int hazType, int hazPlace); 
void CreateModZone(float posX, float posY, bool ReverseNegate, bool OnOff = true, float radius = (2.5f * WorldConstants::fTileSize), float toggleTime = 5.0f); //reverse = true, negate = false
void CreateExitDoor(float posX, float posY);

void CreateTurret();
void CreateTripwire();
void CreatePickUp(float fPosX,float fPosY, int nPickupType, int currencyIndex = -1);


void CreateReverseZone();

//Pickup Collision Function
void CollisionPickup(int nPickUpType);
//void CreateNegateZone();
//////////////////////////////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////////////////////////////
CPlayer* GetPlayer() const { return m_pPlayer; }

//Accessors
int GetLives(void) {return m_nPlayerLives;}
int GetBackgroundImageID(void) {return m_nBGImageID;}
bool GetBounce(void) {return m_bIsBounce;}
int GetPlayerImageID(void) {return m_nPlayerImageID;}
bool GetWeighted(void) {return m_bIsWeighted;}
int GetObjectImageID(void) {return m_nObjectImageID;}
int  GetCurrency(void) {return m_nCurrency;}
int GetWellImageID(void) {return m_nWellImageID;}
float GetPowerUpTimer() {return m_fPowerUpTimer;}
int GetWellClosedImageID(void) {return m_nWellClosedImageID;}
int GetTargetImageID(void) {return m_nTargetImageID;}
int GetWalkerImageID(void) {return m_nWalkerImageID;}
int GetHeavyImageID(void) {return m_nHeavyImageID;}
int GetSniperImageID(void) {return m_nSniperImageID;}
int GetBombardImageID(void) {return m_nBombardImageID;}
int GetPlatformImageID(void) {return m_nPlatformImageID;}
int GetTurretImageID(void) {return m_nTurretImageID;}
int GetTripwireImageID(void) {return m_nTripwireImageID;}
int GetCurrencyImageID(void) {return m_nCurrencyImageID;}
int GetBounceImageID(void)	 {return m_nBounceImageID;}
int GetWeightedImageID(void) {return m_nWeightedImageID;}
int GetExitImageID(void) {return m_nExitImageID;}
int GetLevelCurrency(){return m_nLevelCurrency;}
int GetTotalCurrency(){return m_nTotalCurrency;}
bool GetLevelComplete(void) {return m_bLevelComplete;}
float GetCurrentLevelTime(){return (m_fStageTime - m_fCurrentTime);}
float GetRecordTime(){return m_fCurrentTime;}
bool GetHasHitBuffer(){return m_bEasyHitBuffer;}
bool GetIsWeighted(){return m_bIsWeighted;}
bool GetIsBounce(){return m_bIsBounce;}

float GetCurrentEnergy(){return m_fWeaponEnergy;}
CPlayer* GetPlayerPointer(void) {return m_pPlayer;}

//Mutators
void SetLives(int nLives) {m_nPlayerLives = nLives;}
void SetBounce(bool bBounce) {m_bIsBounce = bBounce;}
void SetWeighted(bool bWeighted) {m_bIsWeighted = bWeighted;}
void AddLevelCurrency(){m_nLevelCurrency++;}
void SetPowerUpTimer(float fPowerUpTimer) {m_fPowerUpTimer = fPowerUpTimer;}
void SetPlayerSpawn(float spawnX, float spawnY);
void SetLevelCOmplete(bool bLevelComplete) {m_bLevelComplete = bLevelComplete;}


//accessors

//accessors that call cGame (in .cpp)
int GetDurationLevel();
int GetRangeLevel();
int GetRechargeLevel();

//fucntion called when player has been hit.
void PlayerHit();

};

#endif