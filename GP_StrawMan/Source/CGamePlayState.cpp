/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CGamePlayState.cpp"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Defines the Gameplay State for the main game. This will derived from IGameState
//			to execute Game logic.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CGamePlayState.h"
//additional #includes here
//#include "CGame.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_WaveManager.h"
#include "SGD Wrappers/CSGD_FModManager.h"
#include "CBitMapFont.h"
#include "CTileManager.h"
#include "CMainMenuState.h"
#include "CGravObject.h"
#include "CGravWell.h"
#include "CGravSystem.h"
#include "CPlayer.h"
#include "CEventSystem.h"
#include "CObjectManager.h"
#include "CWalkerEnemy.h"
#include "CHeavyEnemy.h"
#include "CSniperEnemy.h"
#include "CBombardEnemy.h"
#include "CAbsorbPlatform.h"
#include "CMovablePlatform.h"
#include "CTripwire.h"
#include "CTurret.h"
#include "CHUD.h"
#include "CHazards.h"
#include "CMessageSystem.h"
#include "CGame.h"
#include "Profiler/CCodeProfiler.h"
#include "CZone.h"
#include "CPickups.h"
#include "CUpgradeState.h"
#include "COptionsState.h"
#include "SaveSystem.h"
#include "CParticleManager.h"
#include "CCustomLevelState.h"
#include "CLevelSelect.h"
#include "CAssetState.h"
#include "COUtroState.h"
#include "CAnimationManager.h"
#include "CCheatState.h"

using namespace WorldConstants; //Uses world constants namespace for easy access to the variables needed

// for debugging
#include <iostream>
using namespace std;

#define DEF_LAST_LEVEL 20

CGamePlayState::CGamePlayState()
{
	//initialize all values
	m_pTM = NULL;
	m_pGS = NULL;
	m_pWM = NULL;
	m_pText = NULL;
	m_pOM = NULL;
	m_pOF = NULL;
	m_pFMod = NULL;
	m_pTileMan = NULL;
	m_pSaveFile = NULL;

	m_pText = NULL;

	m_pPlayer = NULL;
	m_pCHUD = NULL;

	m_nBGImageID = -1;
	m_nPlayerImageID = -1;
	m_nObjectImageID = -1;
	m_nTargetImageID = -1;
	m_nWellImageID = -1;
	m_nWellClosedImageID = -1;
	m_nBGImageID = -1;
	m_nWalkerImageID = -1;
	m_nHeavyImageID = -1;
	m_nSniperImageID = -1;
	m_nPlatformImageID = -1;
	m_nTurretImageID = -1;
	m_nTripwireImageID = -1;
	m_nFallSpikeImageID = -1;
	m_nExitImageID = -1;
	m_nPauseBoxID = -1;

	//Pickup image ID init
	m_nCurrencyImageID = -1;
	m_nBounceImageID = -1;
	m_nWeightedImageID = -1;

	m_nCollideSoundID = -1;
	m_nFireShotSoundID = -1;
	//m_nOutOfEnergySoundID = -1;
	m_nTerminalFallSoundID = -1;

	m_nBGMusicSoundID = -1;

	m_nPlayerLives = 3;
	m_fWeaponEnergy = 300.0f;
	if(CGame::GetInstance()->GetDifficulty() == DIFF_EASY)
		m_bEasyHitBuffer = true;
	else
		m_bEasyHitBuffer = false;

	m_bIsBounce = false;
	m_bIsWeighted = false;
	m_fPowerUpTimer = 0.0f;
	m_nLevelCurrency = 0;
	m_nTotalCurrency = 0;
	m_fCurrentTime = 0.0f;
	m_fStageTime = 0.0f;
	m_bLivesOut = false;

	m_sAccess = "givemegoodies";
	m_nSpot = 0;
	m_bAccess = false;
	m_cCheat = "aaaaaaaaaaaaa";
}

CGamePlayState::~CGamePlayState()
{
}

CGamePlayState* CGamePlayState::GetInstance()
{
	//return static instance
	static CGamePlayState instance;
	return &instance;
}

void CGamePlayState::Enter()
{
	//PROFILER_START

	//get instance of systems here
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pGS = CGravSystem::GetInstance();
	m_pOM = CObjectManager::GetInstance();
	m_pOF = CObjectFactory<string, CBaseObject>::GetInstance();
	m_pCHUD = CHUD::GetInstance();
	m_pFMod = CSGD_FModManager::GetInstance();
	m_pTileMan = CTileManager::GetInstance();
	m_pSaveFile = CGame::GetInstance()->GetSaveFile();

	//Shut off the menu music!!
	m_pFMod->StopSound(CMainMenuState::GetInstance()->GetTitleMusicID());

	//load resources here
	CAssetState* pAS = CAssetState::GetInstance();

	// images
	m_nObjectImageID = pAS->GetImageID("ObjectImageID");
	m_nWellImageID = pAS->GetImageID("WellImageID");
	m_nWellClosedImageID = pAS->GetImageID("WellClosedImageID");
	m_nPlayerImageID = pAS->GetImageID("PlayerImageID");
	m_nTargetImageID = pAS->GetImageID("TargetImageID");
	m_nBGImageID = pAS->GetImageID("BGImageID");
	m_nWalkerImageID = pAS->GetImageID("WalkerImageID");
	m_nHeavyImageID = pAS->GetImageID("HeavyImageID");
	m_nSniperImageID = pAS->GetImageID("SniperImageID");
	m_nBombardImageID = pAS->GetImageID("BombardImageID");
	m_nPlatformImageID = pAS->GetImageID("PlatformImageID");
	m_nTurretImageID = pAS->GetImageID("TurretImageID");
	m_nTripwireImageID = pAS->GetImageID("TripwireImageID");
	m_nFallSpikeImageID = pAS->GetImageID("FallSpikeImageID");
	m_nExitImageID = pAS->GetImageID("ExitImageID");
	m_nPauseBoxID = pAS->GetImageID("PauseBGID");

	//Pickup image init
	m_nCurrencyImageID = pAS->GetImageID("CurrencyImageID");
	m_nBounceImageID = pAS->GetImageID("BounceImageID");
	m_nWeightedImageID = pAS->GetImageID("WeightedImageID");

	// sounds
	m_nCollideSoundID = pAS->GetSoundID("CollideSoundID");
	m_nFireShotSoundID = pAS->GetSoundID("FireShotSoundID");
	m_nShotDissipateSoundID = pAS->GetSoundID("ShotDissipateSoundID");
	m_nShotBounceSoundID = pAS->GetSoundID("ShotBounceSoundID");
	m_nTerminalFallSoundID = pAS->GetSoundID("TerminalFallSoundID");

	m_nStart1SoundID = pAS->GetSoundID("StartSound1");
	m_nStart2SoundID = pAS->GetSoundID("StartSound2");
	m_nStart3SoundID = pAS->GetSoundID("StartSound3");

	m_nEnd1SoundID = pAS->GetSoundID("EndSound1");
	m_nEnd2SoundID = pAS->GetSoundID("EndSound2");
	m_nEnd3SoundID = pAS->GetSoundID("EndSound3");

	m_nDeath1SoundID = pAS->GetSoundID("DeathSound1");
	m_nDeath2SoundID = pAS->GetSoundID("DeathSound2");
	m_nDeath3SoundID = pAS->GetSoundID("DeathSound3");

	//Loading in a random theme for the background music.
	//char cBuffer[64];
	int nRandom = rand() % 5 + 1;

	/*sprintf_s(cBuffer, "Resource/Sound/CbB_stagemusic%i.mp3", nRandom);

	m_nBGMusicSoundID = m_pFMod->LoadSound(cBuffer);*/
	m_nBGMusicSoundID = CAssetState::GetInstance()->GetSoundID("StageMusic", nRandom, SND_MUSIC);

	//When I find this asset I will put this back in - John
	//m_nOutOfEnergySoundID = m_pFMod->LoadSound("Resource/Sound/CbB_OutOfEnergy.mp3");

	//set sound volumes here
	//m_pWM->SetVolume(m_nCollideSoundID, m_pSaveFile->GetSaveData()->fSFXVol);

	//set up bitmap font here

	//initialize demo objects here
	//will set up obj factory + manager here eventually

	if(CGame::GetInstance()->GetDifficulty() == DIFF_EASY)
		m_bEasyHitBuffer = true;
	else
		m_bEasyHitBuffer = false;

	m_pOF->RegisterClassType<CBaseObject>("BaseObject");
	m_pOF->RegisterClassType<CGravObject>("GravObject");
	m_pOF->RegisterClassType<CPlayer>("Player");
	m_pOF->RegisterClassType<CGravWell>("GravWell");
	m_pOF->RegisterClassType<CWalker>("Walker");
	m_pOF->RegisterClassType<CHeavy>("Heavy");
	m_pOF->RegisterClassType<CSniper>("Sniper");
	m_pOF->RegisterClassType<CBombardier>("Bombardier");
	m_pOF->RegisterClassType<CAbsorbPlatform>("AbsorbPlatform");
	m_pOF->RegisterClassType<CMovablePlatform>("MovablePlatform");
	m_pOF->RegisterClassType<CTurret>("Turret");
	m_pOF->RegisterClassType<CTripwire>("Tripwire");
	m_pOF->RegisterClassType<CZone>("Zone");
	m_pOF->RegisterClassType<CHazards>("CHazards");
	m_pOF->RegisterClassType<CPickups>("Pickups");

	//demo objects set up
	//Add all objects here

	//CreatePermWell();
	//CreateWalker();
	//CreateHeavy();
	//CreateSniper();
	//CreateSniper();
	//CreateBombardier();
	//CreateHazard();
	//CreateAbsorbPlatform(600, 620);
	//CreateMovablePlatform(200, 100);
	//CreateTurret();
	//CreateTripwire();
	//CreateReverseZone();
	//CreatePickUp(400,240,PICKUP_CURRENCY, 0);

	//CreatePickUp(100,140,PICKUP_BOUNCE);

	//CreatePickUp(600,100,PICKUP_BOUNCE);

	//CreatePickUp(100,100,PICKUP_WEIGHTED);

	//CreateHazard( 0, (float)(CGame::GetInstance()->GetWindowHeight() / 2), false , HAZ_SPIKES , HAZ_RIGHT);
	//CreateHazard( (float)CGame::GetInstance()->GetWindowWidth(), (float)(CGame::GetInstance()->GetWindowHeight() / 2), false , HAZ_SPIKES , HAZ_LEFT);
	//CreateHazard((float)(CGame::GetInstance()->GetWindowWidth() / 2), (float)(CGame::GetInstance()->GetWindowHeight() / 2), false, HAZ_SPIKES, HAZ_BOTTOM);

	//player profile information
	m_nPlayerLives = 3;

	//m_pFMod->PlaySoundA(m_nBGMusicSoundID);
	//m_pFMod->SetVolume(m_nBGMusicSoundID, m_pSaveFile->GetSaveData()->fMusicVol);
	CAssetState::GetInstance()->PlaySound(m_nBGMusicSoundID, SND_MUSIC);
	//Loading in the level
	m_bLivesOut = false;

	CreatePlayer(0.0f, 0.0f);
	if(!CGame::GetInstance()->GetIsCustom())
		m_pTileMan->LoadMap();
	else
	{
		m_pTileMan->LoadCustomLevel(CCustomLevelState::GetInstance()->GetCustomLevel());
	}
//, m_tPlayerSpawnLoc.y);
	//Setting the player spawn location YO.  W0RD.
	m_pPlayer->SetPosX(m_tPlayerSpawnLoc.x);
	m_pPlayer->SetPosY(m_tPlayerSpawnLoc.y);

	m_bIsBounce = false;
	m_bIsWeighted = false;
	m_fPowerUpTimer = 0.0f;
	this->m_fWeaponEnergy = 300.0f;
	m_bLevelComplete = false;
	m_fStageTime = EASY_TIME - ((float)CGame::GetInstance()->GetDifficulty() * 100.0f);
	m_fCurrentTime = 0.0f;
	m_bPaused = false;
	m_nSelection = 0;
	//CParticleManager::GetInstance()->KillAllParticles();
	CParticleManager::GetInstance()->KillParticle("Spiral");
	CParticleManager::GetInstance()->SpawnParticle("Dust",(float)(CGame::GetInstance()->GetWindowWidth() / 2), (float)(CGame::GetInstance()->GetWindowHeight()));

	m_fPrevVol = CAssetState::GetInstance()->GetAllMusicVolumes();

	//Time To Play a random saying

	int startSounds = 3;
	if(CGame::GetInstance()->GetLoadLevel() >= 3)
		startSounds = 7;
	CAssetState::GetInstance()->PlaySound("StartSound", rand() % startSounds + 1);
	/*
	switch(rand()%startSounds)
	{
	case 0:
		//m_pFMod->PlaySound(m_nStart1SoundID);
		CAssetState::GetInstance()->PlaySound(m_nStart1SoundID);
		break;
	case 1:
		//m_pFMod->PlaySound(m_nStart2SoundID);
		CAssetState::GetInstance()->PlaySound(m_nStart2SoundID);
		break;
	case 2:
		//m_pFMod->PlaySound(m_nStart3SoundID);
		CAssetState::GetInstance()->PlaySound(m_nStart3SoundID);
		break;
	default:
		break;
	}
	*/

	test = "fatfucker";
	cheat = "zzzzzzzzz";
	spot = 0;

	m_sAccess = "givemegoodies";
	m_nSpot = 0;
	m_bAccess = false;
	m_cCheat = "zzzzzzzzzzzzz";

	//PROFILER_END
}

bool CGamePlayState::Input()
{
	DWORD dwStateResult;         // Used to store if a controller is connected
	DWORD dwResult;
	XINPUT_STATE state;     // Data type that stores all the current states
	XINPUT_KEYSTROKE key;
	// of a controller.

	ZeroMemory( &state, sizeof(XINPUT_STATE) ); // Zeros out the states of
	// the controller.

	// Get all the current states of controller 1
	dwStateResult = XInputGetState( 0, &state );
	dwResult = XInputGetKeystroke(XUSER_INDEX_ANY,XINPUT_FLAG_GAMEPAD,&key);

	if(dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS)
	{
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_BACK)
		{
			CGame::GetInstance()->PopState(); //(pops state out to main menu)
			return true;
		}
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_START)
		{
			m_bPaused = !m_bPaused;
		}
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A && m_bPaused == true)
		{
			switch(m_nSelection)
			{
			case 0:
			 {
				 m_bPaused = !m_bPaused;
			 }
			 break;
			case 1:
			 {
				 CGame::GetInstance()->PushState(CUpgradeState::GetInstance());
			 }
			 break;
			case 2:
			 {
				 CGame::GetInstance()->PopState();
			 }
			 break;
			}
		}
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_UP || key.VirtualKey == VK_PAD_LTHUMB_UP) && m_bPaused == true)
		{
			m_nSelection--;
			if(m_nSelection < 0)
				m_nSelection = 2;
		}
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_DOWN || key.VirtualKey == VK_PAD_LTHUMB_DOWN) && m_bPaused == true)
		{
			m_nSelection++;
			if(m_nSelection > 2)
				m_nSelection = 0;
		}
	}

	CSGD_DirectInput* m_pDI = CSGD_DirectInput::GetInstance();

	//escapes to Menu if escape is pressed (for the tech demo)
	if(m_pDI->KeyPressed(DIK_ESCAPE))
	{
		CGame::GetInstance()->PopState(); //(pops state out to main menu)
		return true;
	}

	if(m_pDI->KeyPressed(DIK_P))
		m_bPaused = !m_bPaused;

	if(m_pDI->KeyPressed(DIK_RETURN) && m_bPaused == true)
	{
		switch(m_nSelection)
		{
		case 0:
			 {
				 m_bPaused = !m_bPaused;
			 }
			 break;
			case 1:
			 {
				 CGame::GetInstance()->PushState(CUpgradeState::GetInstance());
			 }
			 break;
			case 2:
			 {
				 CGame::GetInstance()->PopState();
			 }
			 break;
		}
	}

	if(m_pDI->KeyPressed(DIK_UP) && m_bPaused == true)
	{
		m_nSelection--;
		if(m_nSelection < 0)
			m_nSelection = 2;
	}

	if(m_pDI->KeyPressed(DIK_DOWN) && m_bPaused == true)
	{
		m_nSelection++;
		if(m_nSelection > 2)
			m_nSelection = 0;
	}

	if(m_pDI->CheckBufferedKeysEx() > 0 && m_bPaused == true)
	{
		//cheat[spot] = m_pDI->CheckBufferedKeysEx();
		m_cCheat[m_nSpot] = m_pDI->CheckBufferedKeysEx(); 

		if(m_cCheat[0] == m_sAccess[0])
		{
			int tempSpot = m_nSpot;
			m_nSpot = 1;
			while(m_nSpot < 13)
			{
				if(m_cCheat[m_nSpot] == m_sAccess[m_nSpot])
				{
					m_nSpot++;
				}
				else
				{
					if(m_nSpot > tempSpot)
					{
					}
					else
					{
						m_nSpot = 0;
						m_cCheat = "zzzzzzzzzzzzz";
					}
					break;
				}

				if(m_nSpot == 13)
				{
					m_nSpot = 0; 
					m_cCheat = "zzzzzzzzzzzzz"; 
					m_bAccess = !m_bAccess; 
				}
			}
		}

	}

	return true;
}

void CGamePlayState::Update(float fDT)
{
	//PROFILER_START

	//check to see if the game is paused
	if(m_bPaused)
	{
		if(m_bAccess)
		{
					m_bAccess = false;
					CGame::GetInstance()->PushState(CCheatState::GetInstance()); 
		}

		return;
	}
	//get gametime
	static float fGameTime = 0.0f;
	fGameTime += fDT;

	if(m_pFMod->IsSoundPlaying(m_nStart1SoundID) || m_pFMod->IsSoundPlaying(m_nStart2SoundID)||m_pFMod->IsSoundPlaying(m_nStart3SoundID)
		||m_pFMod->IsSoundPlaying(m_nDeath1SoundID)||m_pFMod->IsSoundPlaying(m_nDeath2SoundID)||m_pFMod->IsSoundPlaying(m_nDeath3SoundID))
		CAssetState::GetInstance()->SetAllMusicVolumes(m_fPrevVol*0.2f);
	else
		CAssetState::GetInstance()->SetAllMusicVolumes(m_fPrevVol);

	if(GetPowerUpTimer() > 0.0f)
		SetPowerUpTimer(GetPowerUpTimer() - fDT);
	else if(GetPowerUpTimer())
	{
		SetPowerUpTimer(0.0f);
		SetBounce(false);
		SetWeighted(false);
	}

	m_fCurrentTime += fDT;
	if(m_fCurrentTime >= m_fStageTime && CGame::GetInstance()->GetLoadLevel() != 30)
	{
		if(CAssetState::GetInstance()->IsSoundPlaying("WarningSoundID"))
			CAssetState::GetInstance()->StopSound("WarningSoundID"); 

		m_bLevelComplete = true;
	}
	else if(m_fCurrentTime >= m_fStageTime && CGame::GetInstance()->GetLoadLevel() == 30)
	{
		m_fStageTime = 0;
	}

	if(m_pPlayer != NULL)
	m_pShot = m_pPlayer->GetShot(); //makes sure the shot pointer is looking at the players shot

	m_pOM->UpdateObjects(fDT); // now update

	m_nTotalCurrency = CGame::GetInstance()->GetTotalCurrency();

	RechargeEnergy(fDT); //recharge gun energy

	m_pCHUD->Update(fDT); //Updating the CHUD

	//check new collisions
	m_pOM->CheckCollisions();

	//remove dead objects before new loop
	m_pOM->RemoveDead();
	m_pFMod->Update();

	if(m_bLivesOut == true)
	{
		CGame::GetInstance()->PopState();
	}

	CParticleManager::GetInstance()->UpdateParticles(fDT);
	CAnimationManager::GetInstance()->UpdateAnimations(fDT);

	if(m_bLevelComplete)
	{
		if(CAssetState::GetInstance()->IsSoundPlaying("WarningSoundID"))
			CAssetState::GetInstance()->StopSound("WarningSoundID"); 

		if(m_fCurrentTime < m_fStageTime)
		{
			int winSounds = 4;
			CAssetState::GetInstance()->PlaySound("EndSound", rand() % winSounds + 1);

			/*
			if(CGame::GetInstance()->GetLoadLevel() >= 3)
				winSounds = 6;
			switch(rand()%winSounds)
			{
			case 0:
				//m_pFMod->PlaySound(m_nEnd1SoundID);
				CAssetState::GetInstance()->PlaySound(m_nEnd1SoundID);
				break;
			case 1:
				//m_pFMod->PlaySound(m_nEnd2SoundID);
				CAssetState::GetInstance()->PlaySound(m_nEnd2SoundID);
				break;
			case 2:
				//m_pFMod->PlaySound(m_nEnd3SoundID);
				CAssetState::GetInstance()->PlaySound(m_nEnd3SoundID);
				break;
			default:
				break;
			}*/
		}
	
		while(m_pFMod->IsSoundPlaying(m_nEnd3SoundID) || m_pFMod->IsSoundPlaying(m_nEnd2SoundID)||m_pFMod->IsSoundPlaying(m_nEnd1SoundID))
		{
			CAssetState::GetInstance()->SetAllMusicVolumes(m_fPrevVol*0.2f);
		}
		CAssetState::GetInstance()->SetAllMusicVolumes(m_fPrevVol);

		

		if(CGame::GetInstance()->GetLoadLevel() == DEF_LAST_LEVEL && m_fCurrentTime < m_fStageTime) //if we're in the last level
		{
			CGame::GetInstance()->PopState();
			CGame::GetInstance()->PopState();
			CGame::GetInstance()->PushState(COutroState::GetInstance());
		}
		else
		{
			if(CAssetState::GetInstance()->IsSoundPlaying("WarningSoundID"))
			CAssetState::GetInstance()->StopSound("WarningSoundID"); 

			CGame::GetInstance()->PopState();
		}
			
	}

	//PROFILER_END
}

void CGamePlayState::Render()
{
	//PROFILER_START

	//BG for Demo
	CSGD_TextureManager::GetInstance()->Draw(m_nBGImageID, 0, 0, 2.0, 2.0);

	m_pTileMan->Render();
	CParticleManager::GetInstance()->RenderParticles();
	m_pOM->RenderObjects();
	CAnimationManager::GetInstance()->RenderAnimations();
	m_pCHUD->Render();
	if(m_bPaused)
	{
		CBitMapFont* m_pFont = CBitMapFont::GetInstance();
		CGame* pGame = CGame::GetInstance();

		m_pTM->Draw(m_nPauseBoxID, (pGame->GetWindowWidth() / 2) - 192, (pGame->GetWindowHeight() / 2) - 80, 1.5f, 1.0f);
			m_pFont->Print("Pause Menu", pGame->GetWindowWidth() / 2 , pGame->GetWindowHeight() / 2 - 40, true, false, 1.3f, D3DCOLOR_ARGB(255, 200, 128, 0));

		if(m_nSelection == 0)
			m_pFont->Print("Resume", pGame->GetWindowWidth() / 2 , pGame->GetWindowHeight() / 2, true, false, 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0));
		else
			m_pFont->Print("Resume", pGame->GetWindowWidth() / 2 , pGame->GetWindowHeight() / 2, true, false, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255));

		if(m_nSelection == 1)
			m_pFont->Print("Upgrade Stats", pGame->GetWindowWidth() / 2 , pGame->GetWindowHeight() / 2 + 50, true, false, 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0));
		else
			m_pFont->Print("Upgrade Stats", pGame->GetWindowWidth() / 2 , pGame->GetWindowHeight() / 2 + 50, true, false, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255));

		if(m_nSelection == 2)
			m_pFont->Print("Exit", pGame->GetWindowWidth() / 2 , pGame->GetWindowHeight() / 2 + 100, true, false, 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0));
		else
			m_pFont->Print("Exit", pGame->GetWindowWidth() / 2, pGame->GetWindowHeight() / 2 + 100, true, false, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if(CGame::GetInstance()->GetLoadLevel() == 30)
	{
		//This rect is needed for intersect rect
		RECT rIntersect = {};

		RECT startFont;
		startFont.left = 32;
		startFont.top = 416;
		startFont.right = 256;
		startFont.bottom = 640;

		RECT walkerFont;
		walkerFont.left = 352;
		walkerFont.top = 480;
		walkerFont.right = 640;
		walkerFont.bottom = 672;

		RECT spikeFont;
		spikeFont.left = 672;
		spikeFont.top = 480;
		spikeFont.right = 928;
		spikeFont.bottom = 704;

		RECT pushFont;
		pushFont.left = 288;
		pushFont.top = 256;
		pushFont.right = 768;
		pushFont.bottom = 400;

		RECT exitFont;
		exitFont.left = 320;
		exitFont.top = 64;
		exitFont.right = 928;
		exitFont.bottom = 292;

		CBitMapFont* m_pFont = CBitMapFont::GetInstance();
		if(IntersectRect(&rIntersect,&m_pPlayer->GetCollideRect(),&startFont))
			m_pFont->Print("Welcome to the tutorial level \nHere you will learn basic gameplay.",50,500,false,false,.45f);
		if(IntersectRect(&rIntersect,&m_pPlayer->GetCollideRect(),&walkerFont))
			m_pFont->Print("This is a walker enemy \nhe likes to walk back and forth \nEnemies can be affected by gravity like you",330,500,false,false,.45f);
		if(IntersectRect(&rIntersect,&m_pPlayer->GetCollideRect(),&spikeFont))
			m_pFont->Print("These are spikes if you touch them you will die \nbut your enemies can die by spikes as well",600,500,false,false,.45f);
		if(IntersectRect(&rIntersect,&m_pPlayer->GetCollideRect(),&pushFont))
			m_pFont->Print("This enemy is in your way, why dont you push him into those spikes other there.",300,290,false,false,.45f);
		if(IntersectRect(&rIntersect,&m_pPlayer->GetCollideRect(),&exitFont))
			m_pFont->Print("This is very basic gameplay, there is a lot more to discover and die by \nhave fun out there",300,100,false,false,.45f);
	}

	//PROFILER_END

	// debug info
#ifdef _DEBUG
	/*CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	char cBuff[128];
	sprintf_s(cBuff, _countof(cBuff), "Player Vel X: %f | Player Vel Y: %f", CSGD_DirectInput::GetInstance()->MouseGetPosX(),
																			CSGD_DirectInput::GetInstance()->MouseGetPosY() );*/

#endif
}

void CGamePlayState::Exit()
{
	m_pFMod->StopSound(m_nBGMusicSoundID);
	CGravSystem::GetInstance()->ClearGravField();
	m_pOM->RemoveAllObjects();
	CHUD::GetInstance()->StopWarning();  
	

	//m_pWM->UnloadWave(m_nCollideSoundID);

	//m_pFMod->UnloadSound(m_nOutOfEnergySoundID);

	//reset any Game data to default / begining values

	//unreister classes frm object factory
	m_pOF->UnregisterClassType("BaseObject");
	m_pOF->UnregisterClassType("GravObject");
	m_pOF->UnregisterClassType("Player");
	m_pOF->UnregisterClassType("GravWell");
	m_pOF->UnregisterClassType("Walker");
	m_pOF->UnregisterClassType("Heavy");
	m_pOF->UnregisterClassType("Sniper");
	m_pOF->UnregisterClassType("Bombardier");
	m_pOF->UnregisterClassType("CAbsorbPlatform");
	m_pOF->UnregisterClassType("CMovablePlatform");
	m_pOF->UnregisterClassType("CTurret");
	m_pOF->UnregisterClassType("CTripwire");
	m_pOF->UnregisterClassType("CZone");
	m_pOF->UnregisterClassType("CHazards");
	m_pOF->UnregisterClassType("Pickups");

	m_pPlayer->Release();
	m_pPlayer = NULL;

	if(m_pShot != NULL)
	{
		m_pShot->Release();
		m_pShot = NULL;
	}
	CAnimationManager::GetInstance()->ClearAllAnimations();
	CParticleManager::GetInstance()->KillAllParticles();

	// delete the map
	m_pTileMan->UnloadMap();

	//Fire off the level music
	if(!CSGD_FModManager::GetInstance()->IsSoundPlaying(CMainMenuState::GetInstance()->GetTitleMusicID()))
	{
		//CSGD_FModManager::GetInstance()->PlaySoundA(CMainMenuState::GetInstance()->GetTitleMusicID());
		CAssetState::GetInstance()->PlaySound(CMainMenuState::GetInstance()->GetTitleMusicID(), SND_MUSIC);
	}
}

//the following functions create dynamic objects and send them to the OM

void CGamePlayState::RechargeEnergy(float fDT)
{
	if(m_fWeaponEnergy < 300.0f)
	{
		if(CGame::GetInstance()->GetGunCheat())
			m_fWeaponEnergy += fDT * (20.0f + (RECHARGE_INCREMENT * (float)(5)));
		else
			m_fWeaponEnergy += fDT * (20.0f + (RECHARGE_INCREMENT * (float)(CGame::GetInstance()->GetRechargeLevel())));

	}
	if(m_fWeaponEnergy > 300.0f)
		m_fWeaponEnergy = 300.0f;
}
//Creates a pickup
void CGamePlayState::CreatePickUp(float fPosX,float fPosY, int nPickupType, int currencyIndex)
{
	CPickups* pPickup = (CPickups*)m_pOF->CreateObject("Pickups");

	switch(nPickupType)
	{
	case PICKUP_CURRENCY:

		pPickup->SetPickupType(nPickupType);

		pPickup->SetObjState(OBJ_NORMAL);
		pPickup->SetPosX(fPosX);
		pPickup->SetPosY(fPosY);

		pPickup->SetVelX(0.0f);
		pPickup->SetVelY(0.0f);

		pPickup->SetSize(2.0f * WorldConstants::fTileSize,2.0f * WorldConstants::fTileSize);
		pPickup->SetCurrencyIndex(currencyIndex);
		//Set Currency animation ID here
		pPickup->SetAnimation(GetCurrencyImageID());
		break;

	case PICKUP_BOUNCE:

		pPickup->SetPickupType(nPickupType);

		pPickup->SetObjState(OBJ_NORMAL);
		pPickup->SetPosX(fPosX);
		pPickup->SetPosY(fPosY);

		pPickup->SetVelX(0.0f);
		pPickup->SetVelY(0.0f);

		pPickup->SetSize(2.0f * WorldConstants::fTileSize,2.0f * WorldConstants::fTileSize);

		//Set Bounce animation ID here
		pPickup->SetAnimation(GetBounceImageID());
		break;

	case PICKUP_WEIGHTED:

		pPickup->SetPickupType(nPickupType);

		pPickup->SetObjState(OBJ_NORMAL);
		pPickup->SetPosX(fPosX);
		pPickup->SetPosY(fPosY);

		pPickup->SetVelX(0.0f);
		pPickup->SetVelY(0.0f);

		pPickup->SetSize(2.0f * WorldConstants::fTileSize,2.0f * WorldConstants::fTileSize);

		//Set Bounce animation ID here
		pPickup->SetAnimation(GetWeightedImageID());
		break;
	};

	m_pOM->AddObject(pPickup);
	pPickup->Release();
}

void CGamePlayState::CreateModZone(float posX, float posY, bool ReverseNegate, bool OnOff, float radius, float toggleTime)
{
	CZone* pZone = (CZone*)m_pOF->CreateObject("Zone");
	
	pZone->SetPosX(posX);
	pZone->SetPosY(posY);

	pZone->SetPerm(false);

	toggleTime -= 1.0f * CGame::GetInstance()->GetDifficulty();
	pZone->SetToggleTime(toggleTime);
	radius *= (CGame::GetInstance()->GetDifficulty() + 1);
	pZone->SetRange(radius);
	if(ReverseNegate)
	{
		pZone->SetZoneType(ZONE_REVERSE);

		if(CGame::GetInstance()->GetDifficulty() == DIFF_EASY)
			pZone->SetAnimation(CAssetState::GetInstance()->GetImageID("ReverseZoneEasy"));
		else if(CGame::GetInstance()->GetDifficulty() == DIFF_NORMAL)
			pZone->SetAnimation(CAssetState::GetInstance()->GetImageID("ReverseZoneNormal"));
		else if(CGame::GetInstance()->GetDifficulty() == DIFF_HARD)
			pZone->SetAnimation(CAssetState::GetInstance()->GetImageID("ReverseZoneHard"));

	}
	else
	{
		pZone->SetZoneType(ZONE_NEGATE);

		if(CGame::GetInstance()->GetDifficulty() == DIFF_EASY)
			pZone->SetAnimation(CAssetState::GetInstance()->GetImageID("NegateZoneEasy"));
		else if(CGame::GetInstance()->GetDifficulty() == DIFF_NORMAL)
			pZone->SetAnimation(CAssetState::GetInstance()->GetImageID("NegateZoneNormal"));
		else if(CGame::GetInstance()->GetDifficulty() == DIFF_HARD)
			pZone->SetAnimation(CAssetState::GetInstance()->GetImageID("NegateZoneHard"));
	}

	pZone->SetSize(WorldConstants::fTileSize, WorldConstants::fTileSize);


	m_pOM->AddObject(pZone);
	pZone->Release();
}
void CGamePlayState::CreateWalker(float posX, float posY, bool bImmune)
{
	CWalker* pWalk = (CWalker*)m_pOF->CreateObject("Walker");

	pWalk->SetObjState(OBJ_NORMAL);
	pWalk->SetPosX(posX);
	pWalk->SetPosY(posY);
	//set velocity based on difficulty here eventually

	pWalk->SetVelY(0.0f);
	pWalk->SetSize(WorldConstants::fTileSize, 2.0f * WorldConstants::fTileSize);
	pWalk->SetAnimation(m_nWalkerImageID);

	pWalk->SetPlayer(m_pPlayer);
	pWalk->SetGravResist(0.2f + (0.1f * CGame::GetInstance()->GetDifficulty()));
	pWalk->SetRange((3.0f + ( 1.0f * CGame::GetInstance()->GetDifficulty())) * WorldConstants::fTileSize);
	pWalk->SetImmune(bImmune);

	m_pOM->AddObject(pWalk);

	pWalk->Release();
}

void CGamePlayState::CreateHeavy(float posX, float posY, bool bImmune, bool PushPull, bool OnOff,  bool ChangeForce)
{
	CHeavy* pHeavy = (CHeavy*)m_pOF->CreateObject("Heavy");

	pHeavy->SetObjState(OBJ_NORMAL);
	pHeavy->SetPosX(posX);
	pHeavy->SetPosY(posY);
	//set velocity based on difficulty here eventually

	pHeavy->SetVelY(0.0f);
	pHeavy->SetSize(2.0f * WorldConstants::fTileSize, 2.0f * WorldConstants::fTileSize);
	pHeavy->SetAnimation(m_nHeavyImageID);

	//enemy specific data
	pHeavy->SetPlayer(m_pPlayer);
	pHeavy->SetGravResist(.15f + (.10f * (CGame::GetInstance()->GetDifficulty() + 1)));
	pHeavy->SetRange(3.0f * WorldConstants::fTileSize);
	pHeavy->SetImmune(bImmune);

	//heavy specific data
	pHeavy->SetWellImage(m_nWellImageID);
	pHeavy->SetRange(96.0f +  (32.0f * CGame::GetInstance()->GetDifficulty()));
	pHeavy->SetDuration(3.0f + (1.0f * CGame::GetInstance()->GetDifficulty()));

	pHeavy->SetIsTimed( OnOff ); //if toggle time is passed 0.0f this will be a permanantly on heavy
	pHeavy->SetDuration(3.0f);
	pHeavy->SetPushPull(PushPull);

	if(CGame::GetInstance()->GetDifficulty() != DIFF_HARD)
		pHeavy->SetChangeDirect(ChangeForce);
	else
		pHeavy->SetChangeDirect(true);

	pHeavy->SetIsAnimated(true);

	m_pOM->AddObject(pHeavy);

	pHeavy->Release();
}

void CGamePlayState::CreateSniper(float posX, float posY, bool bImmune, float AimRange)
{
	CSniper* pSnipe = (CSniper*)m_pOF->CreateObject("Sniper");

	pSnipe->SetObjState(OBJ_NORMAL);
	pSnipe->SetPosX(posX);
	pSnipe->SetPosY(posY);
	//set velocity based on difficulty here eventually

	pSnipe->SetVelY(0.0f);
	pSnipe->SetVelX(0.0f);
	pSnipe->SetSize(WorldConstants::fTileSize, 2.0f * WorldConstants::fTileSize);
	pSnipe->SetAnimation(m_nSniperImageID);

	pSnipe->SetGravResist(0.2f + (0.1f * CGame::GetInstance()->GetDifficulty()));
	//pSnipe->SetRange(AimRange);
	pSnipe->SetImmune(bImmune);

	//sniper specific var sets
	pSnipe->SetBulletImage(m_nWellClosedImageID);

	pSnipe->SetIsAnimated(true);

	m_pOM->AddObject(pSnipe);

	pSnipe->Release();
}
void CGamePlayState::CreateBombardier(float posX, float posY, bool bImmune, float AimRange, bool bPushPull)
{
	CBombardier* pBomb = (CBombardier*)m_pOF->CreateObject("Bombardier");

	pBomb->SetObjState(OBJ_NORMAL);
	pBomb->SetPosX(posX);
	pBomb->SetPosY(posY);

	//set velocity based on difficulty here eventually

	pBomb->SetVelY(0.0f);
	pBomb->SetVelX(0.0f);
	pBomb->SetSize(WorldConstants::fTileSize, 2.0f * WorldConstants::fTileSize);
	pBomb->SetAnimation(m_nBombardImageID);

	pBomb->SetGravResist(0.2f);
	pBomb->SetRange(DEFAULT_SNIPER_RANGE);
	pBomb->SetImmune(bImmune);

	//sniper specific var sets
	pBomb->SetBulletImage(m_nWellClosedImageID);

	//bombard specific stats
	pBomb->SetWellImage(m_nWellImageID);

	pBomb->SetPushPull(bPushPull);

	pBomb->SetWellTime(3.0f);
	pBomb->SetPlayer(m_pPlayer);
	pBomb->SetIsAnimated(true);

	m_pOM->AddObject(pBomb);

	pBomb->Release();
}

void CGamePlayState::CreatePlayer(float spawnX, float spawnY)
{
	m_pPlayer = (CPlayer*)m_pOF->CreateObject("Player");

	m_pPlayer->SetObjState(OBJ_NORMAL);
	m_pPlayer->SetPosX(spawnX);
	m_pPlayer->SetPosY(spawnY);
	m_pPlayer->SetMousePt(240.0f, 368.0f );

	m_pPlayer->SetVelX(0.0f);
	m_pPlayer->SetVelY(0.0f);

	m_pPlayer->SetSize(WorldConstants::fTileSize, 2.0f * WorldConstants::fTileSize );

	m_pPlayer->SetAnimation(m_nPlayerImageID);
	m_pPlayer->SetTargetImage(m_nTargetImageID);
	m_pPlayer->SetSoundID(m_nCollideSoundID);
	m_pPlayer->SetIsAnimated(true);
	if(CGame::GetInstance()->GetDifficulty() == DIFF_EASY)
		m_pPlayer->ShieldToggle(true);

	//m_pPlayer =  pPlayer; //no add ref needed as we are no releasing from the creation.
	m_pOM->AddObject( m_pPlayer );
}
void CGamePlayState::ResetPlayer()
{
	m_pPlayer->SetObjState(OBJ_NORMAL);
	m_pPlayer->SetPosX(m_tPlayerSpawnLoc.x);
	m_pPlayer->SetPosY(m_tPlayerSpawnLoc.y);
	m_pPlayer->SetMousePt(240.0f, 368.0f );

	m_pPlayer->SetVelX(0.0f);
	m_pPlayer->SetVelY(0.0f);
}
void CGamePlayState::CreatePlayerShot(bool PushPull)
{
	if(m_fWeaponEnergy >= SHOT_ENERGY)
	{
		if(m_pShot != NULL)
		{
			m_pShot->Terminate();
		}

		CGravWell* pNewWell = (CGravWell*)m_pOF->CreateObject("GravWell");

		pNewWell->SetShotBounce(m_nShotBounceSoundID);
		pNewWell->SetShotDissipate(m_nShotDissipateSoundID);

		pNewWell->SetPosVec(m_pPlayer->GetPosVec());

		pNewWell->SetVelVec(WorldConstants::fGravShotSpeed * m_pPlayer->GetShotVec());

		pNewWell->SetSize(.5f * WorldConstants::fTileSize, .5f * WorldConstants::fTileSize);

		if(CGame::GetInstance()->GetGunCheat())
			pNewWell->SetDuration(3.0f + (DURATION_INCREMENT * (float)(5)));
		else
			pNewWell->SetDuration(3.0f + (DURATION_INCREMENT * (float)(CGame::GetInstance()->GetDurationLevel())));
			
		pNewWell->SetForce(fGravity * 2.0f);
		pNewWell->SetPushPull(PushPull);
		if(CGame::GetInstance()->GetGunCheat())
			pNewWell->SetRange(fTileSize * (10.0f + (RANGE_INCREMENT * (float)(5))));
		else
			pNewWell->SetRange(fTileSize * (10.0f + (RANGE_INCREMENT * (float)(CGame::GetInstance()->GetRangeLevel()))));



		//Powerups set here default is false show arrange itself accordingly
		pNewWell->SetIsBounce(GetBounce());
		pNewWell->SetIsWeighted(GetWeighted());

		pNewWell->SetAnimation(m_nWellImageID);
		pNewWell->SetClosedImage(m_nWellClosedImageID);

		pNewWell->SetObjState(OBJ_NORMAL);
		pNewWell->SetOwner(m_pPlayer);
		m_pPlayer->SetShot(pNewWell);
		m_pShot = pNewWell;
		pNewWell->SetAnimID(CAnimationManager::GetInstance()->StartAnimation("ClosedShot", pNewWell));

		m_pOM->AddObject(pNewWell);
		pNewWell->Release();

		m_fWeaponEnergy -= SHOT_ENERGY;

		//m_pFMod->PlaySoundA(m_nFireShotSoundID);
		CAssetState::GetInstance()->PlaySound(m_nFireShotSoundID);
	}
	else
	{
		//m_pFMod->PlaySoundA(m_nOutOfEnergySoundID);
	}
}

void CGamePlayState::CreateAbsorbPlatform(float posX, float posY)
{
	CAbsorbPlatform* pAbPlat = (CAbsorbPlatform*)m_pOF->CreateObject("AbsorbPlatform");

	pAbPlat->SetObjState(OBJ_NORMAL);
	pAbPlat->SetPosX(posX);

	pAbPlat->SetPosY(posY);
	//set velocity based on difficulty here eventually

	pAbPlat->SetVelY(0.0f);
	pAbPlat->SetSize(73.0f/*WorldConstants::fTileSize * 5.0f*/, WorldConstants::fTileSize * 1.5f);
	pAbPlat->SetAnimation(m_nPlatformImageID);

	pAbPlat->SetGravResist(0.0f);

	//AbPlat specific data
	pAbPlat->SetWellImage(m_nWellImageID);

	m_pOM->AddObject(pAbPlat);

	pAbPlat->Release();
}

void CGamePlayState::CreateMovablePlatform(float posX, float posY)
{
	CMovablePlatform* pMovPlat = (CMovablePlatform*)m_pOF->CreateObject("MovablePlatform");

	pMovPlat->SetObjState(OBJ_NORMAL);
	pMovPlat->SetPosX(posX);
	pMovPlat->SetPosY(posY);
	//set velocity based on difficulty here eventually

	pMovPlat->SetVelY(0.0f);
	pMovPlat->SetSize(73.0f/*5.0f* WorldConstants::fTileSize*/, 1.5f * WorldConstants::fTileSize);
	pMovPlat->SetAnimation(m_nPlatformImageID);

	pMovPlat->SetGravResist(0.0f);

	m_pOM->AddObject(pMovPlat);

	pMovPlat->Release();
}

void CGamePlayState::CreateTurret()
{
	CTurret* m_pTurret = (CTurret*)m_pOF->CreateObject("Turret");

	m_pTurret->SetObjState(OBJ_NORMAL);
	m_pTurret->SetPosX(32.0f);
	m_pTurret->SetPosY(200.0f);

	m_pTurret->SetVelY(0.0f);
	m_pTurret->SetSize(WorldConstants::fTileSize, WorldConstants::fTileSize);
	m_pTurret->SetAnimation(m_nTurretImageID);
	m_pTurret->SetPlayer(m_pPlayer);

	m_pOM->AddObject(m_pTurret);
	m_pTurret->Release();
}

void CGamePlayState::CreateTripwire()
{
	CTripwire *pTripwire = (CTripwire*)m_pOF->CreateObject("Tripwire");

	pTripwire->SetObjState(OBJ_NORMAL);
	pTripwire->SetPosX(400.0f);
	pTripwire->SetPosY(500.0f);

	pTripwire->SetVelY(0.0f);
	pTripwire->SetSize(32.0f, 16.0f);
	pTripwire->SetAnimation(m_nTripwireImageID);
//	pTripwire->SetTurret(m_pTurret);

	m_pOM->AddObject(pTripwire);
	pTripwire->Release();
}

void CGamePlayState::CreateHazard(float posX, float posY, bool Respawn, int hazType, int hazPlace)
{
	CHazards* pHazard = (CHazards*)m_pOF->CreateObject("CHazards");

	pHazard->SetPosX(posX); //This is actually int division.... was this on purpose?
	pHazard->SetPosY(posY);
	pHazard->SetSize(2.0f * WorldConstants::fTileSize,2.0f *  WorldConstants::fTileSize);
	pHazard->SetOrgPos();
	pHazard->SetHazType(hazType);
	pHazard->SetHazPlace(hazPlace);
	pHazard->SetType(TYPE_HAZARD);

	if(pHazard->GetHazType() != HAZ_LAVA)
		pHazard->SetAnimation(CAssetState::GetInstance()->GetImageID("SpikeImageID"));
	else
	{
		pHazard->SetAnimation(CAssetState::GetInstance()->GetImageID("LavaImageID"));
		CParticleManager::GetInstance()->SpawnParticle("Flames", posX-16.0f, posY);
	}
	pHazard->SetMyRect();
	CObjectManager::GetInstance()->AddObject(pHazard);
	pHazard->Release();
}

void CGamePlayState::CollisionPickup(int nPickUpType)
{
	switch((int)nPickUpType)
		{
		case PICKUP_CURRENCY:
			//This one will increment a value
			AddLevelCurrency();
			break;
		case PICKUP_BOUNCE:
			//both of these will set a bool to true and a timer to 0
			if(GetWeighted())
				SetWeighted(false);
			SetBounce(true);
			SetPowerUpTimer(WorldConstants::fPowerUpTimer);
			break;
		case PICKUP_WEIGHTED:
			//both of these will set a bool to true and a timer to 0
			if(GetBounce())
				SetBounce(false);
			SetWeighted(true);
			SetPowerUpTimer(WorldConstants::fPowerUpTimer);
			break;
		};
}

void CGamePlayState::CreateExitDoor(float posX, float posY)
{
	CBaseObject* pExit = (CBaseObject*)m_pOF->CreateObject("BaseObject");

	pExit->SetPosX(posX);
	pExit->SetPosY(posY);
	pExit->SetSize(2.0f * WorldConstants::fTileSize,2.0f *  WorldConstants::fTileSize);
	pExit->SetAnimation(m_nExitImageID);
	pExit->SetType(TYPE_EXIT);

	CObjectManager::GetInstance()->AddObject(pExit);
	pExit->Release();
}

int CGamePlayState::GetDurationLevel()
{
	CGame* pGame = CGame::GetInstance();
	return pGame->GetDurationLevel();
}

int CGamePlayState::GetRangeLevel()
{
	CGame* pGame = CGame::GetInstance();
	return pGame->GetRangeLevel();
}

int CGamePlayState::GetRechargeLevel()
{
	CGame* pGame = CGame::GetInstance();
	return pGame->GetRechargeLevel();
}
void CGamePlayState::SetPlayerSpawn(float spawnX, float spawnY)
{
	m_tPlayerSpawnLoc.x = spawnX;
	m_tPlayerSpawnLoc.y = spawnY;
}
void CGamePlayState::PlayerHit()
{
	if(CGame::GetInstance()->GetCollisionUnlock())
	{	
		m_pPlayer->BounceBack(m_pPlayer, AXIS_BOTH, false);
		m_pPlayer->HitFlash();
		return;
	}

	if(m_bEasyHitBuffer == true || m_pPlayer->GetIsFlashing() == true)
	{
		if(m_pPlayer->GetIsFlashing() == false)
		{	
			m_bEasyHitBuffer = false;
			m_pPlayer->ShieldToggle(false);
			m_pPlayer->HitFlash();
		}
		
		m_pPlayer->BounceBack(m_pPlayer, AXIS_BOTH, false);
	
	}
	else
	{
		if(m_pPlayer->GetIsFlashing() == false)
		{

		

			m_pPlayer->SetVelX(0.0f);
			m_pPlayer->SetVelY(0.0f);
			m_pPlayer->SetPosVec(m_tPlayerSpawnLoc);
			CTileManager::GetInstance()->HardResetPseudoWalls(m_pPlayer);
			m_pPlayer->SetTermVel(false);
			m_pPlayer->HitFlash();

			//if the player has died at all, set and save it for that slot (forthe achiev)
			//John Added dieing in tutorial level doesnt count here
			if(false == CGame::GetInstance()->GetSaveFile()->GetSaveData()->tAllSaves[CGame::GetInstance()->GetCurrentSaveSlot()].bEverDied && CGame::GetInstance()->GetLoadLevel() != 30)
			{
				CGame::GetInstance()->GetSaveFile()->GetSaveData()->tAllSaves[CGame::GetInstance()->GetCurrentSaveSlot()].bEverDied = true;
				CGame::GetInstance()->GetSaveFile()->Save();
			}
			//sleep 3000 secs -> will

			m_nPlayerLives--;

			if(CGame::GetInstance()->GetDifficulty() == DIFF_EASY)
			{
				m_bEasyHitBuffer = true;
				m_pPlayer->ShieldToggle(true);

			}
			else if(CGame::GetInstance()->GetLoadLevel() == 30)
			{
				m_bEasyHitBuffer = true;
				m_pPlayer->ShieldToggle(true);
			}

			int sounds = 4;
			CAssetState::GetInstance()->PlaySound("DeathSound", rand() % sounds + 1);
			/*
			if(CGame::GetInstance()->GetLoadLevel() >= 3)
				sounds = 6;
			switch(rand()%sounds)
			{
			case 0:
				CAssetState::GetInstance()->PlaySound(m_nDeath1SoundID);
				break;
			case 1:
				CAssetState::GetInstance()->PlaySound(m_nDeath2SoundID);
				break;
			case 3:
				CAssetState::GetInstance()->PlaySound(m_nDeath3SoundID);
				break;
			default:
				break;
			}
			*/
		}

		if(m_nPlayerLives < 0 && CGame::GetInstance()->GetLoadLevel() != 30)
		{
			//check for chump achievment
			if(CGame::GetInstance()->GetLoadLevel() == 1 && CGame::GetInstance()->GetDifficulty() == DIFF_EASY)
			{
				CGame::GetInstance()->CheckSaveAchieves(ACHIEV_CHUMP);
			}
			m_bLivesOut = true;
		}
		else if(CGame::GetInstance()->GetLoadLevel() == 30 && m_nPlayerLives < 0)
		{
			//Setting the player lives to zero if they go less than in the tutorial level
			m_nPlayerLives = 0;
		}
	}
}