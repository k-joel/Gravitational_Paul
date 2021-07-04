/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CGame.cpp"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Defines the main CGame class to be used to run the game logic. This will contain the Game States and have player
//			data in it. This is a singleton class
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CGame.h"

//additional #includes here
#include "IGameState.h"
#include "CGamePlayState.h"
#include "CAnimationManager.h"
#include "CHazards.h"
#include "CObjectManager.h"
#include "Profiler/CCodeProfiler.h"
#include "CObjectFactory.h"
#include "CParticleManager.h"
#include "CMainMenuState.h"
#include "SGD Wrappers/CSGD_FModManager.h"
#include "CUpgradeState.h"
#include "CFileSelectState.h"
#include "CPlayer.h"
#include "CAssetState.h"
#include "COutroState.h"
#include "CBitMapFont.h"
#include "CKeybindState.h"

CGame::CGame()
{
	m_pD3D = NULL;
	m_pTM = NULL;
	m_pDS = NULL;
	m_pDI = NULL;
	m_pWM = NULL;
	m_pMS = NULL;

	m_pCurrFile = NULL;
	m_fSFXVol = 1.0f;
	m_fMusicVol = 1.0f;
	m_nPanning = 0; //default sound value sintil options menu working
	m_nDifficulty = DIFF_EASY;
	m_bIsCustom = false;

	m_bUnlocks = NULL;
	m_fTimes = NULL;

	m_fAchievTimer =  0.0f;
	m_nAchievAlpha = 255;
	m_nCurrAchiev = 0;
	m_bShowAchiev = false;
	m_bGunCheat = false;

	m_pFont = NULL;
	m_nKeyboardKeybind = 0; //default
	m_nControllerKeybind = 0; //default

	//Keys Set to actual values in enter
	m_cKMoveLeft = 0;
	m_cKMoveRight = 0;
	m_cKJump = 0;
	m_cKShootPush = 0;
	m_cKShootPull = 0;
	m_cKCloseAll = 0;

	m_wCMoveLeft = 0;
	m_wCMoveRight = 0;
	m_wCJump = 0;
	m_wCShootPush = 0;
	m_wCShootPull = 0;
	m_wCCloseAll = 0;

	m_bLevelUnlock = false;
	m_bCollisionUnlock = false; 
}

CGame::~CGame()
{
}
bool CGame::Main()
{
	//calc elapsed time
	DWORD dwStartTimeStamp = GetTickCount();
	m_fElapsedTime = (float)(dwStartTimeStamp - m_dwPreviousTimeStamp) / 1000.0f;
	m_dwPreviousTimeStamp = dwStartTimeStamp;

	//limit fDT in debug mode
#ifdef _DEBUG
	if(m_fElapsedTime > 0.15f)
	{
		m_fElapsedTime = 0.15f;
	}
#endif

	//get input
	if(Input() == false)
		return false;

	//Update Data by Time
	Update(m_fElapsedTime);

	//draw the Game images
	Render();

	return true;
}
void CGame::Init(HWND hWnd, HINSTANCE hInstance, int nWindowWidth, int nWindowHeight, bool bIsWindowed)
{
	//set all wrapper pointers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pDS = CSGD_DirectSound::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pWM = CSGD_WaveManager::GetInstance();
	m_pMS = CMessageSystem::GetInstance();
	m_pES = CEventSystem::GetInstance();

	m_pFont = CBitMapFont::GetInstance();

	//startup all D3D opjects
	m_pD3D->InitDirect3D(hWnd, nWindowWidth, nWindowHeight, bIsWindowed, false);
	m_pTM->InitTextureManager( m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite());
	m_pDS->InitDirectSound(hWnd);
	m_pWM->InitWaveManager( hWnd, m_pDS->GetDSObject() );
	CSGD_FModManager::GetInstance()->InitFModManager(hWnd);

	m_pCurrFile = new CGameSaveFile;
	m_pCurrFile->SetFileName("Resource/SaveData/TestSave.gps");
	//this line will be moved to the load area
	m_pCurrFile->Load();
	SyncWithFile(0);

#ifdef _DEBUG
	m_pDI->InitDirectInput(hWnd, hInstance, DI_KEYBOARD  | DI_MOUSE, DI_MOUSE);

#else
	m_pDI->InitDirectInput(hWnd, hInstance, DI_KEYBOARD  | DI_MOUSE, DI_MOUSE);
#endif

	//Init message system
	m_pMS->InitMessageSystem( MessageProc );

	//set common values
	m_nWindowWidth = nWindowWidth;
	m_nWindowHeight = nWindowHeight;
	m_bIsWindowed = bIsWindowed;

	//set time here for accuracy
	m_dwTimeStamp = GetTickCount();
	m_dwPreviousTimeStamp = GetTickCount();

	//Load animations
	CAnimationManager::GetInstance()->LoadAnimations("Resource/Scripts/CbB_PlayerAnimFinal.bin", false);
	CAnimationManager::GetInstance()->LoadAnimations("Resource/Scripts/CbB_WalkerAnim.bin", false);
	CAnimationManager::GetInstance()->LoadAnimations("Resource/Scripts/CbB_SniperAnim.bin", false);
	CAnimationManager::GetInstance()->LoadAnimations("Resource/Scripts/CbB_HeavyAnim.bin", false);
	CAnimationManager::GetInstance()->LoadAnimations("Resource/Scripts/CbB_BombardierAnim.bin", false);
	CAnimationManager::GetInstance()->LoadAnimations("Resource/SCripts/CbB_ClosedShot.bin", false);
	CAnimationManager::GetInstance()->SetAnimInfo();

	//Load particles
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_GravWell.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_PushWell.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_PullWell.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_GravTrail.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_Flames.bin");

	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_Explosion.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_Termnial.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_Dust.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_blood.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_Cash.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_BHSpiral.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_ItemSparkle.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_PushShot.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_PullShot.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_PlayerShield.bin");
	CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_ImmuneAura.bin");

	// Don't load or access any sound assets in cgame
	//CSGD_FModManager::GetInstance()->SetVolume(scream,m_fSFXVol);
	//CSGD_FModManager::GetInstance()->SetVolume(termEnter,m_fSFXVol);

	// Level info
	CGame::GetInstance()->SetDifficulty(DIFF_EASY);
	CGame::GetInstance()->SetLoadLevel(1);
	m_bUnlocks = new bool[20];
	m_fTimes = new float[20];
	m_bGunCheat = false;

	//Keybind keys set to default here
	m_nKeyboardKeybind = 0;
	m_nControllerKeybind = 0;

	m_cKMoveLeft = DIK_A;
	m_cKMoveRight = DIK_D;
	m_cKJump = DIK_W;
	m_cKShootPush = 1;
	m_cKShootPull = 0;
	m_cKCloseAll = 2;

	m_wCMoveLeft = VK_PAD_LTHUMB_LEFT;
	m_wCMoveRight = VK_PAD_LTHUMB_RIGHT;
	m_wCJump = VK_PAD_LTHUMB_UP;
	m_wCShootPush = VK_PAD_LTRIGGER;
	m_wCShootPull = VK_PAD_RTRIGGER;
	m_wCCloseAll = VK_PAD_RSHOULDER;

	m_bLevelUnlock = false;
	m_bCollisionUnlock = false; 

	// Always keep pushstate at the END of the enter calls, otherwise bad shit happens.
	//PushState(CGamePlayState::GetInstance());
	CGame::GetInstance()->PushState(CAssetState::GetInstance());
}

bool CGame::Input()
{
	m_pDI->ReadDevices(); //ONLY HERE ONLY ONCE PER FRAME

	//full screen toggle alt + enter
	if(m_pDI->KeyPressed(DIK_RETURN) && (m_pDI->KeyDown(DIK_LALT) || m_pDI->KeyDown(DIK_RALT)))
	{
		m_bIsWindowed = !m_bIsWindowed;
		m_pD3D->ChangeDisplayParam(m_nWindowWidth, m_nWindowHeight, m_bIsWindowed);
	}
	else
	{
		if(m_pDI->KeyPressed(DIK_1))
		{
			if(++m_nCurrAchiev > 9)
				m_nCurrAchiev = 0;
			AchievmentUnlocked(m_nCurrAchiev);
		}
		
		//call input on top state
		return m_vStates.back()->Input();
	}

	return true;
}

void CGame::Update(float fDT)
{
	m_pWM->Update();
	m_vStates.back()->Update(fDT);

	if(m_fAchievTimer > 0.0f) //if an achievment is still displayed
	{
		//increment the timer
		m_fAchievTimer -= fDT;
		if(m_fAchievTimer <= 3.0f)
		{
			m_nAchievAlpha = (int)(255.0f * (m_fAchievTimer / 3.0f));
		}
	}
	else
	{
		m_bShowAchiev = false;
	}

	m_pES->ProcessEvents();

	m_pMS->ProcessMessages();
}

void CGame::Render()
{
	m_pD3D->Clear(0, 0, 0); //clear to black
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();
	//drawing now started

	//rendering here

	// render the last state
	m_vStates.back()->Render();

	if(m_bShowAchiev == true)
	{
		DrawAchiev();
	}

	//end drawing area
	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();
	m_pD3D->Present();
}

void CGame::Shutdown()
{
	RemoveAllStates();

	CGame::GetInstance()->KillCompletes();
	CSGD_FModManager::GetInstance()->ShutdownFModManager();

	if(m_pMS)
	{
		m_pMS->ShutdownMessageSystem();
		m_pMS = NULL;
	}
	if(m_pES)
	{
		m_pES->ShutdownEventSystem();
		m_pES = NULL;
	}

	if(m_pDI)
	{
		m_pDI->ShutdownDirectInput();
		m_pDI = NULL;
	}

	if(m_pWM)
	{
		m_pWM->ShutdownWaveManager();
		m_pWM = NULL;
	}
	if(m_pDS)
	{
		m_pDS->ShutdownDirectSound();
		m_pDS = NULL;
	}
	if(m_pTM)
	{
		m_pTM->ShutdownTextureManager();
		m_pTM = NULL;
	}
	if(m_pD3D)
	{
		m_pD3D->ShutdownDirect3D();
		m_pD3D = NULL;
	}
	delete m_pCurrFile;

	CCodeProfiler::GetInstance()->WriteFiles();
}

CGame* CGame::GetInstance()
{
	static CGame instance;
	return &instance;
}

void CGame::PushState(IGameState *pState)
{
	m_vStates.push_back(pState);
	pState->Enter();
}

void CGame::PopState()
{
	if(m_vStates.size())
	{
		m_vStates.back()->Exit();
		m_vStates.pop_back();
	}
}

void CGame::RemoveAllStates()
{
	for(vector<IGameState*>::iterator iter = m_vStates.begin(); iter != m_vStates.end(); iter++ )
		(*iter)->Exit();

	m_vStates.clear();
}
void CGame::SyncWithFile(int slotNum)
{
	m_nCurrentSaveSlot = slotNum;
	m_nDifficulty = m_pCurrFile->GetSaveData()->tAllSaves[slotNum].nDifficulty;
	m_fSFXVol = m_pCurrFile->GetSaveData()->fSFXVol;
	m_fMusicVol = m_pCurrFile->GetSaveData()->fMusicVol;
	m_nPanning = m_pCurrFile->GetSaveData()->nPanning;
	CKeybindState::GetInstance()->SwitchCControls( m_pCurrFile->GetSaveData()->nControlSchemes[0]);
	CKeybindState::GetInstance()->SwitchKControls( m_pCurrFile->GetSaveData()->nControlSchemes[1]);
}
void MessageProc(CBaseMessage* pMsg)
{
	switch(pMsg->GetMsgID())
	{
	case MSG_CREATE_HAZARD:
		{
		}
		break;
	case MSG_DESTROY_HAZARD:
		{
			CGame* pGame = CGame::GetInstance();
			CDestroyHazardMessage* pDHM = (CDestroyHazardMessage*)pMsg;
			//CGamePlayState::GetInstance()->m_pOM->RemoveObject(pDHM->GetHazard());
			//CGamePlayState::GetInstance()->m_pOM->RemoveObject(pDHM->GetHazard());
			CObjectManager::GetInstance()->RemoveObject(pDHM->GetHazard());
		}
	case MSG_CURRENCY_COLLECT:
		{
			CGame* pGame = CGame::GetInstance();
			CCurrencyCollectedMessage* pCCM = (CCurrencyCollectedMessage*)pMsg;
			pGame->m_pCurrFile->GemCollected(pGame->m_nCurrentSaveSlot, pGame->m_nCurrentLevel, pCCM->GetGemIndex());
			CGamePlayState::GetInstance()->CollisionPickup(0);
		}
		break;
	case MSG_EXIT_LEVEL:
		{
			CExitLevelMessage* pExit = (CExitLevelMessage*)pMsg;

				CGame* pGame = CGame::GetInstance();
				CGamePlayState::GetInstance()->SetLevelCOmplete(pExit->GetLevelComplete());
				CParticleManager::GetInstance()->KillParticle("Blood");
				CParticleManager::GetInstance()->KillParticle("Dust");
				if(pExit->GetLevelComplete())
					pGame->m_pCurrFile->LevelComplete(pGame->m_nCurrentSaveSlot, pGame->m_nCurrentLevel, pExit->GetLevelTime());

				if(pGame->GetLoadLevel() == 20)
				{
					Sleep(1000);
					pGame->PopState();
					pGame->PushState(COutroState::GetInstance());
				}

			break;
		}
	}
}

bool* CGame::GetCurrStageCompletes()
{
	m_bUnlocks[0] = true;
	for(int i = 1; i < 20; i++)
		m_bUnlocks[i] = m_pCurrFile->GetSaveData()->tAllSaves[m_nCurrentSaveSlot].tAllLevels[i - 1].bComplete;

	return m_bUnlocks;
}

void CGame::KillCompletes()
{
	for(int i = 0; i < 20; i++)
	{
		m_bUnlocks[i] = false;
		m_fTimes[i] = 0;
	}

	delete [] m_bUnlocks;
	m_bUnlocks = NULL;

	delete [] m_fTimes;
	m_fTimes = NULL;
}

float* CGame::GetLevelTimes()
{
	for(int i = 0; i < 20; i++)
		m_fTimes[i] = m_pCurrFile->GetSaveData()->tAllSaves[m_nCurrentSaveSlot].tAllLevels[i].fTimeRecord;

	return m_fTimes;
}

GEMS CGame::GetGemsPerLevel(int level)
{
	for(int i = 0; i < 15; i++)
		m_sGems.gems[i] = m_pCurrFile->GetSaveData()->tAllSaves[m_nCurrentSaveSlot].tAllLevels[level].bGemCollected[i];

	return m_sGems;
}

void CGame::CheckSaveAchieves(unsigned int whichOne)
{
	if(m_pCurrFile->CheckAchievment(whichOne))
	{
		AchievmentUnlocked(whichOne);
	}
	else
		return;
}

void CGame::AchievmentUnlocked(unsigned int whichOne)
{
	m_fAchievTimer = 6.0f;
	m_nAchievAlpha = 255;
	m_nCurrAchiev = whichOne;
	m_bShowAchiev = true;
}

void CGame::DrawAchiev()
{
	string AchievName;
	string AchievDesc;
	int	   AchievIconID;
	int	   AchievBoxID = CAssetState::GetInstance()->GetImageID("AchievBoxID");

	switch(m_nCurrAchiev)
	{
	case ACHIEV_EASYBEAT:
		{
			AchievName = "Step Up Your Game";
			AchievDesc = "Completed All Standard Stages on Easy Mode";
			AchievIconID = CAssetState::GetInstance()->GetImageID("AchievBeatEasyIcon");

			break;
		}
	case ACHIEV_NORMBEAT:
		{
			AchievName = "Just Like Everybody Else";
			AchievDesc = "Completed All Standard Stages on Normal Mode";
			AchievIconID = CAssetState::GetInstance()->GetImageID("AchievBeatNormIcon");

			break;
		}
	case ACHIEV_HARDBEAT:
		{
			AchievName = "It's Lonely at the Top";
			AchievDesc = "Completed All Standard Stages on Hard Mode";
			AchievIconID = CAssetState::GetInstance()->GetImageID("AchievBeatHardIcon");

			break;
		}
	case ACHIEV_CHAMP:
		{
			AchievName = "Gravitational Champ";
			AchievDesc = "Clear a Single Slot on Hard Mode Without\nDying EVEN ONCE EVER";
			AchievIconID = CAssetState::GetInstance()->GetImageID("AchievChampIcon");

			break;
		}
	case ACHIEV_CHUMP:
		{
			AchievName = "Gravitational Chump";
			AchievDesc = "Get a Game Over While On Easy Mode in \nLevel 1";
			AchievIconID = CAssetState::GetInstance()->GetImageID("AchievChumpIcon");
			break;
		}
	case ACHIEV_1MAX:
		{
			AchievName = "One Trick Pony";
			AchievDesc = "Max One Stat on your Gravity Gun";
			AchievIconID = CAssetState::GetInstance()->GetImageID("AchievPonyIcon");

			break;
		}
	case ACHIEV_ALLMAX:
		{
			AchievName = "I'm the Best at What I Do!";
			AchievDesc = "Maxed all Gravity Gun Stats";
			AchievIconID = CAssetState::GetInstance()->GetImageID("AchievBestIcon");

			break;
		}
	case ACHIEV_MCDUCK:
		{
			AchievName = "The McDuck";
			AchievDesc = "Collect All Currency In the Game's \nStandard Levels";
			AchievIconID = CAssetState::GetInstance()->GetImageID("AchievDuckIcon");

			break;
		}
	case ACHIEV_CUSTOMLVL:
		{
			AchievName = "Level Designer Resume";
			AchievDesc = "Complete A User Created Level";
			AchievIconID = CAssetState::GetInstance()->GetImageID("AchievResumeIcon");

			break;
		}
	case ACHIEV_NOUPGRADE:
		{
			AchievName = "Won't Void My Warranty";
			AchievDesc = "Completed the Game on Normal or \nHard mode without upgrading the Gravity Gun at all";
			AchievIconID = CAssetState::GetInstance()->GetImageID("AchievWarrantyIcon");

			break;
		}
	default:
		{
			return;
		}
	}

		//create render box for the achieve panel
		RECT boxRect;
		boxRect.top = 0;
		boxRect.left = 0;
		boxRect.right = 512;
		boxRect.bottom = 96;

		int boxTop = 80;
		int boxLeft = 256;

		//draw the box with proper alpha
		m_pTM->Draw(AchievBoxID, boxLeft, boxTop, 1.0f, 1.0f, &boxRect, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAchievAlpha, 255, 255, 255));
		//draw the icon over it properly
		m_pTM->Draw(AchievIconID, boxLeft + 15, boxTop + 15,1.0f, 1.0f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAchievAlpha, 255, 255, 255));

		//write text out to it
		m_pFont->Print(AchievName.c_str(), boxLeft + 84, boxTop + 15, false, false, .80f, D3DCOLOR_ARGB(m_nAchievAlpha, 218, 208, 33));
		m_pFont->Print(AchievDesc.c_str(), boxLeft + 84, boxTop + 49, false, false, .40f, D3DCOLOR_ARGB(m_nAchievAlpha, 255, 255, 255));
}