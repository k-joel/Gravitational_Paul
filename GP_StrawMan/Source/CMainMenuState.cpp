/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CMainMenuState.h"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Defines the main intro menu state for the Game, this will be the inital state for the game. This is a singleton.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CMainMenuState.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "CGame.h"
#include "CGamePlayState.h"
#include "CBitMapFont.h"
#include "SGD Wrappers/CSGD_FModManager.h"
#include "CLevelSelect.h"
#include "CParticleManager.h"
#include "CFileSelectState.h"
#include "COptionsState.h"
#include "CCustomLevelState.h"
#include "CAssetState.h"
#include "CAttractState.h"

//TESTING UPGRADE STATE
#include "CUpgradeState.h"

//additional #includes here

CMainMenuState::CMainMenuState()
{
	m_pD3D = NULL;
	m_pTM = NULL;
	m_pGame = NULL;
	m_pDI = NULL;
	m_fAttractTime = 0.0f;

	m_szMenuItems[0] = "Play";
	m_szMenuItems[1] = "Options";
	m_szMenuItems[2] = "Exit";

	m_nMusicID = -1;
	m_nImgIDBG = -1;
}

CMainMenuState::~CMainMenuState()
{
}

CMainMenuState* CMainMenuState::GetInstance()
{
	static CMainMenuState instance;
	return &instance;
}

void CMainMenuState::Enter()
{
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pGame = CGame::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_nCurrSelection = MENU_PLAY;
	m_nImgIDBG = CAssetState::GetInstance()->GetImageID("MainMenuBG");
	m_nMusicID = CAssetState::GetInstance()->GetSoundID("MainMenuMusic", -1, SND_MUSIC);
	m_fAttractTime = 0.0f;
	if(!CSGD_FModManager::GetInstance()->IsSoundPlaying(m_nMusicID))
	{
		//CSGD_FModManager::GetInstance()->PlaySoundA(m_nMusicID);
		CAssetState::GetInstance()->PlaySound(m_nMusicID, SND_MUSIC);
	}

	CParticleManager::GetInstance()->SpawnParticle("Spiral", 400.0f, 384.0f);
	//CParticleManager::GetInstance()->SpawnParticle("Dust",(float)(CGame::GetInstance()->GetWindowWidth() / 2), (float)(CGame::GetInstance()->GetWindowHeight() - 10));
}
bool CMainMenuState::Input()
{
	//////////////////////////////////////////////////////////////////////////
	// MENU SELECTION
	//////////////////////////////////////////////////////////////////////////

	DWORD dwResult;         // Used to store if a controller is connected
	DWORD dwStateResult;
	XINPUT_STATE state;     // Data type that stores all the current states
	XINPUT_KEYSTROKE key;
	// of a controller.

	ZeroMemory( &state, sizeof(XINPUT_STATE) ); // Zeros out the states of
	ZeroMemory( &key, sizeof(XINPUT_KEYSTROKE));
	// the controller.

	// Get all the current states of controller 1
	dwResult = XInputGetState( 0, &state );
	dwStateResult = XInputGetKeystroke(XUSER_INDEX_ANY,XINPUT_FLAG_GAMEPAD,&key);

	if( dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS )
	{
		// Controller is connected.
		// -----------INSERT BUTTON CHECKS HERE----------

		//DPAD AND A BUTTON CHECKS

		//Deadzone Deadzone does whatever a deadzone can
		if( (state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
            (state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) )
            {
                  state.Gamepad.sThumbLX = 0;
                  state.Gamepad.sThumbLY = 0;
            }

      if( (state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
            state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
            (state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
            state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) )
            {
                  state.Gamepad.sThumbRX = 0;
                  state.Gamepad.sThumbRY = 0;
            }
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_LTHUMB_UP || key.VirtualKey == VK_PAD_DPAD_UP))
		{
			--m_nCurrSelection;
			if(m_nCurrSelection < MENU_PLAY)
				m_nCurrSelection = MENU_EXIT;

			m_fAttractTime = 0.0f;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_LTHUMB_DOWN || key.VirtualKey == VK_PAD_DPAD_DOWN))
		{
			++m_nCurrSelection;
			if(m_nCurrSelection > MENU_EXIT)
				m_nCurrSelection = MENU_PLAY;

			m_fAttractTime = 0.0f;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
		{
			switch(m_nCurrSelection)
			{
			case MENU_PLAY:
				//m_pGame->PushState(CLevelSelect::GetInstance());
				m_pGame->PushState(CFileSelectState::GetInstance());
				break;
			case MENU_OPTIONS:
				m_pGame->PushState(COptionsState::GetInstance());
				break;
			case MENU_EXIT:
				return false;
				break;
			}
			m_fAttractTime = 0.0f;
		}
	}
	else
	{
		// Controller is disconnected.
	}

	if(m_pDI->KeyPressedEx(DIK_UP))
	{
		--m_nCurrSelection;
		if(m_nCurrSelection < MENU_PLAY)
			m_nCurrSelection = MENU_EXIT;

		m_fAttractTime = 0.0f;
	}

	if(m_pDI->KeyPressedEx(DIK_DOWN))
	{
		++m_nCurrSelection;
		if(m_nCurrSelection > MENU_EXIT)
			m_nCurrSelection = MENU_PLAY;

		m_fAttractTime = 0.0f;
	}

	if(m_pDI->KeyPressedEx(DIK_RETURN))
	{
		switch(m_nCurrSelection)
		{
		case MENU_PLAY:
			//m_pGame->PushState(CLevelSelect::GetInstance());
			m_pGame->PushState(CFileSelectState::GetInstance());
			break;
		case MENU_OPTIONS:
			m_pGame->PushState(COptionsState::GetInstance());
			break;
		case MENU_EXIT:
			return false;
			break;
		}
		m_fAttractTime = 0.0f;
	}

	return true;
}

void CMainMenuState::Update(float fDT)
{
	CParticleManager::GetInstance()->UpdateParticles(fDT);
	m_fAttractTime = m_fAttractTime + fDT;

	if(m_fAttractTime > 10.0f)
	{
		m_fAttractTime = 0.0f;
		m_pGame->PushState(CAttractState::GetInstance());
	}
}

void CMainMenuState::Render()
{
	m_pTM->Draw(m_nImgIDBG, 0, 0);

	CParticleManager::GetInstance()->RenderParticles();

	//m_pTM->Draw(CAssetState::GetInstance()->GetImageID("AlphaBlackID"), 0, 100, 1.0f, 2.0f, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(150, 0, 0, 0));
	CBitMapFont::GetInstance()->Print("Gravitational Paul", 550, 420, true, false, 2.0f, D3DCOLOR_XRGB(0, 150, 255));
	//m_pTM->Draw(CAssetState::GetInstance()->GetImageID("Test"), 512, 384);

	CBitMapFont::GetInstance()->SetCurFont(FONT_B);
	int nOffsetY = 520; // FROM THE TOP
	//m_pTM->Draw(CAssetState::GetInstance()->GetImageID("AlphaBlackID"), 0, nOffsetY, 1.0f, 3.2f, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(150, 0, 0, 0));
	//////////////////////////////////////////////////////////////////////////
	// MENU
	//////////////////////////////////////////////////////////////////////////
	for(int i = 0; i < MENU_MAX; i++)
	{
		bool bSelected = false;

		if( i == m_nCurrSelection )
			bSelected = true;

		CBitMapFont::GetInstance()->Print(m_szMenuItems[i].c_str(), 750, nOffsetY + ( CBitMapFont::GetInstance()->GetCellHeight() * i ), true, bSelected);
	}
	CBitMapFont::GetInstance()->SetCurFont(FONT_A);
}

void CMainMenuState::Exit()
{
	CParticleManager::GetInstance()->KillParticle("Spiral");
}