#include "COptionsState.h"
#include "CBitMapFont.h"
#include "CGame.h"
#include "CMainMenuState.h"
#include "SGD Wrappers/CSGD_FModManager.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SaveSystem.h"
#include "CCredits.h"
#include "CHowToPlay.h"
#include "CAssetState.h"
#include "CKeybindState.h"

COptionsState::COptionsState()
{
	//Behold as I set things to null.  Or 0.  Whatever.
	m_pGame = NULL;
	m_pFont = NULL;
	m_pTM = NULL;
	m_pDI = NULL;
	m_pFMod = NULL;
	m_pSaveFile = NULL;

	m_nMenuSelection = 0;
	m_nOptionsBG = -1;
	m_fFXVolume = 0.0f;
	m_fMusicVolume = 0.0f;
}

COptionsState* COptionsState::GetInstance()
{
	static COptionsState instance;
	return &instance;
}

void COptionsState::Enter()
{
	m_pGame = CGame::GetInstance();
	m_pFont = CBitMapFont::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pFMod = CSGD_FModManager::GetInstance();
	m_nMenuSelection = 0;
	m_nOptionsBG = CAssetState::GetInstance()->GetImageID("OptionsBG");

	m_pSaveFile = m_pGame->GetSaveFile();
	CAssetState::GetInstance()->SetAllMusicVolumes(m_pSaveFile->GetSaveData()->fMusicVol);
	CAssetState::GetInstance()->SetAllSFXVolumes(m_pSaveFile->GetSaveData()->fSFXVol);
}

void COptionsState::Exit()
{
	m_pGame->SyncWithFile(m_pGame->GetCurrentSaveSlot());
	m_pSaveFile->Save();
}

bool COptionsState::Input()
{
	DWORD dwStateResult;         // Used to store if a controller is connected
	DWORD dwResult;
	XINPUT_STATE state;     // Data type that stores all the current states
	XINPUT_KEYSTROKE key;
	// of a controller.

	ZeroMemory( &state, sizeof(XINPUT_STATE) ); // Zeros out the states of
	ZeroMemory( &key,sizeof(XINPUT_KEYSTROKE));
	// the controller.

	// Get all the current states of controller 1
	dwStateResult = XInputGetState( 0, &state );
	dwResult = XInputGetKeystroke(XUSER_INDEX_ANY,XINPUT_FLAG_GAMEPAD,&key);

	if( dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS )
	{
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
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_UP || key.VirtualKey == VK_PAD_LTHUMB_UP))
		{
			--m_nMenuSelection;
			if(m_nMenuSelection < 0)
				m_nMenuSelection = 5;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_DOWN || key.VirtualKey == VK_PAD_LTHUMB_DOWN))
		{
			++m_nMenuSelection;
			if(m_nMenuSelection > 5)
				m_nMenuSelection = 0;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
		{
			switch(m_nMenuSelection)
			{
			case 0:
				{
					m_pGame->PushState(CHowToPlay::GetInstance());
					break;
				}
			case 1:
				{
					m_pGame->PushState(CCredits::GetInstance());
					break;
				}
			case 4:
				m_pGame->PushState(CKeybindState::GetInstance());
				break;
			case 5:
				{
					m_pSaveFile->GetSaveData()->fMusicVol = CAssetState::GetInstance()->GetAllMusicVolumes();
					m_pSaveFile->GetSaveData()->fSFXVol = CAssetState::GetInstance()->GetAllSFXVolumes();
					m_pGame->SyncWithFile(m_pGame->GetCurrentSaveSlot());

					m_pGame->PopState();
					break;
				}
			}
		}

			if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_B)
			{
				m_pGame->PopState();
			}

			if(state.Gamepad.sThumbLX < -(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*2) || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
			{
				switch(m_nMenuSelection)
				{
				case 2:
					{
						/*m_fMusicVolume -= 0.01f;
						if(m_fMusicVolume < 0)
							m_fMusicVolume = 0.0f;*/
						CAssetState::GetInstance()->SetAllMusicVolumes(CAssetState::GetInstance()->GetAllMusicVolumes()- 0.001f);

						break;
					}
				case 3:
					{
						/*m_fFXVolume -= 0.01f;
						if(m_fFXVolume < 0)
							m_fFXVolume = 0.0f;*/
						CAssetState::GetInstance()->SetAllSFXVolumes(CAssetState::GetInstance()->GetAllSFXVolumes()- 0.001f);

						break;
					}
				}
			}

			if(state.Gamepad.sThumbLX > (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*2) || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
			{
				switch(m_nMenuSelection)
				{
				case 2:
					{
						/*m_fMusicVolume += 0.01f;
						if(m_fMusicVolume > 1)
							m_fMusicVolume = 1.0f;*/
						CAssetState::GetInstance()->SetAllMusicVolumes(CAssetState::GetInstance()->GetAllMusicVolumes() + 0.001f);

						break;
					}
				case 3:
					{
						/*m_fFXVolume += 0.01f;
						if(m_fFXVolume > 1)
							m_fFXVolume = 1.0f;*/
						CAssetState::GetInstance()->SetAllSFXVolumes(CAssetState::GetInstance()->GetAllSFXVolumes() + 0.001f);

						break;
					}
				}
			}
	}
	else
	{
		//Controller Disconnected
	}

	//Keyboard Input
	if(m_pDI->KeyPressedEx(DIK_DOWN))
	{
		++m_nMenuSelection;
		if(m_nMenuSelection > 5)
			m_nMenuSelection = 0;
	}

	if(m_pDI->KeyPressedEx(DIK_UP))
	{
		--m_nMenuSelection;
		if(m_nMenuSelection < 0)
			m_nMenuSelection = 5;
	}

	if(m_pDI->KeyPressedEx(DIK_RETURN))
	{
		switch(m_nMenuSelection)
		{
		case 0:
			{
				m_pGame->PushState(CHowToPlay::GetInstance());
				break;
			}
		case 1:
			{
				m_pGame->PushState(CCredits::GetInstance());
				break;
			}
		case 4:
			m_pGame->PushState(CKeybindState::GetInstance());
			break;
		case 5:
			{
				m_pSaveFile->GetSaveData()->fMusicVol = CAssetState::GetInstance()->GetAllMusicVolumes();
				m_pSaveFile->GetSaveData()->fSFXVol = CAssetState::GetInstance()->GetAllSFXVolumes();
				m_pGame->SyncWithFile(m_pGame->GetCurrentSaveSlot());

				m_pGame->PopState();
				break;
			}
		}
	}

	if(m_pDI->KeyDown(DIK_LEFT))
	{
		switch(m_nMenuSelection)
		{
		case 2:
			{
				/*m_fMusicVolume -= 0.01f;
				if(m_fMusicVolume < 0)
					m_fMusicVolume = 0.0f;*/
				CAssetState::GetInstance()->SetAllMusicVolumes(CAssetState::GetInstance()->GetAllMusicVolumes()- 0.001f);

				break;
			}
		case 3:
			{
				/*m_fFXVolume -= 0.01f;
				if(m_fFXVolume < 0)
					m_fFXVolume = 0.0f;*/
				CAssetState::GetInstance()->SetAllSFXVolumes(CAssetState::GetInstance()->GetAllSFXVolumes()- 0.001f);

				break;
			}
		}
	}

	if(m_pDI->KeyDown(DIK_RIGHT))
	{
		switch(m_nMenuSelection)
		{
		case 2:
			{
				/*m_fMusicVolume += 0.01f;
				if(m_fMusicVolume > 1)
					m_fMusicVolume = 1.0f;*/
				CAssetState::GetInstance()->SetAllMusicVolumes(CAssetState::GetInstance()->GetAllMusicVolumes() + 0.001f);

				break;
			}
		case 3:
			{
				/*m_fFXVolume += 0.01f;
				if(m_fFXVolume > 1)
					m_fFXVolume = 1.0f;*/
				CAssetState::GetInstance()->SetAllSFXVolumes(CAssetState::GetInstance()->GetAllSFXVolumes() + 0.001f);

				break;
			}
		}
	}

	return true;
}

void COptionsState::Render()
{
	char cBuffer[12];

	m_pTM->Draw(m_nOptionsBG, 0, 0, 0.8f, 0.8f);

	m_pFont->Print("Welcome To The Options!", m_pGame->GetWindowWidth() / 2, 10, true);

	if(m_nMenuSelection == 0)
	{
		m_pFont->Print("How to play", 30, 100, false, true);
	}
	else
	{
		m_pFont->Print("how to play", 30, 100);
	}

	if(m_nMenuSelection == 1)
	{
		m_pFont->Print("credits", 30, 200, false, true);
	}
	else
	{
		m_pFont->Print("credits", 30, 200);
	}

	sprintf_s(cBuffer, "%i", (int)(CAssetState::GetInstance()->GetAllMusicVolumes() * 100));

	if(m_nMenuSelection == 2)
	{
		m_pFont->Print("Music Volume", 30, 300, false, true);
		m_pFont->Print(cBuffer, 600, 300, false, true);
	}
	else
	{
		m_pFont->Print("Music Volume", 30, 300);
		m_pFont->Print(cBuffer, 600, 300);
	}

	sprintf_s(cBuffer, "%i", (int)(CAssetState::GetInstance()->GetAllSFXVolumes() * 100));
	if(m_nMenuSelection == 3)
	{
		m_pFont->Print("Sound FX Volume", 30, 400, false, true);
		m_pFont->Print(cBuffer, 600, 400, false, true);
	}
	else
	{
		m_pFont->Print("Sound FX Volume", 30, 400);
		m_pFont->Print(cBuffer, 600, 400);
	}

	//Dear self.  If we really want panning, put it here.  But that shit is dumb.
	if(m_nMenuSelection == 4)
		m_pFont->Print("Keybinding", 30, 500, false, true);
	else
		m_pFont->Print("Keybinding", 30, 500);

	if(m_nMenuSelection == 5)
		m_pFont->Print("Back!", 30, 600, false, true);
	else
		m_pFont->Print("Back!", 30, 600);
}

void COptionsState::Update(float fDT)
{
	//Turning off the music if I'm on the sound FX volume.
	if(m_nMenuSelection == 3)
	{
		m_pFMod->StopSound(CAssetState::GetInstance()->GetSoundID("MainMenuMusic", -1, SND_MUSIC));
		//Playing a sound effect on loop to be able to adjust the sound effect volume.
		if(!m_pFMod->IsSoundPlaying(CAssetState::GetInstance()->GetSoundID("AdjustFXID")))
		{
			//m_pFMod->PlaySound(CAssetState::GetInstance()->GetSoundID("AdjustFXID"));
			//CAssetState::GetInstance()->SetAllSFXVolumes(CAssetState::GetInstance()->GetAllSFXVolumes());
			CAssetState::GetInstance()->PlaySound("AdjustFXID");
		}
	}
	else
	{
		if(!m_pFMod->IsSoundPlaying(CAssetState::GetInstance()->GetSoundID("MainMenuMusic", -1, SND_MUSIC)))
		{
			//m_pFMod->PlaySound(CAssetState::GetInstance()->GetSoundID());
			CAssetState::GetInstance()->PlaySound("MainMenuMusic", -1, SND_MUSIC);
		}
		m_pFMod->StopSound(CAssetState::GetInstance()->GetSoundID("AdjustFXID"));
	}

	m_pSaveFile->GetSaveData()->fMusicVol = CAssetState::GetInstance()->GetAllMusicVolumes();
	m_pSaveFile->GetSaveData()->fSFXVol = CAssetState::GetInstance()->GetAllSFXVolumes();
	m_pSaveFile->Save();

	//m_pFMod->SetVolume(CMainMenuState::GetInstance()->GetTitleMusicID(), m_fMusicVolume);
	//m_pFMod->SetVolume(CAssetState::GetInstance()->GetSoundID("AdjustFXID"), m_fFXVolume);

	m_pGame->SyncWithFile(m_pGame->GetCurrentSaveSlot());
}

COptionsState::~COptionsState()
{
}