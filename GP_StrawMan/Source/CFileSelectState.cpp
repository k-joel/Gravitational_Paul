/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CFileSelectionState"
//
// Author: Justin Dumas (JuD)
//
// Purpose: This State will allow for the selection of a file by the user or a new file. This will make sleection of saves
//			possible even with the 360 controller.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CFileSelectState.h"

//additional #includes
#include "CGame.h"
#include "CLevelSelect.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_FModManager.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "CBitMapFont.h"
#include "CAssetState.h"
#include "CAchievmentsState.h"

#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1024
#define DEF_LEVELS 20
#define DEF_PREVIEWPATH "Resource/Graphics/CbB_level%idemo.png"

CFileSelectState::CFileSelectState()
{
		m_pGame = NULL;
		m_pTM = NULL;
		m_pFM = NULL;
		m_pDI = NULL;
		m_pFont = NULL;

		m_bFilesFound = false;
		m_bGameHasFile = false;
		m_bConfirming = false;
		m_nCurrState  = FILE_ENTERED;

		m_nCurrSelection = 0;
		m_nConfirmationSelection = 0;
		m_nListTop = 0;
		m_nListBot = 9;
		m_nLastFileIndex = 0;

		m_bNameFilled = false;
		m_nChosenSlot = 0;
		m_nMenuMax = 0;
		m_nStartingChar = 'a';
		m_nNameSize = 0;
		m_szPlayerName[0] = '_';
		m_szPlayerName[1] = '\0';
		m_bFileExists = false;

		m_nBGImageID = -1;
		m_nBoxImageID = -1;

		for(unsigned int i = 0; i < 21; ++i)
		{
			m_nPreviewPics[i] = -1;
		}

		m_bDeletingSlot = false;
		m_bKillIt = false;
		m_bDeleteMode = false;
		m_bEmpty = false;
}

CFileSelectState::~CFileSelectState()
{
}

void CFileSelectState::Enter()
{
	m_pGame = CGame::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pFM = CSGD_FModManager::GetInstance();
	m_pFont = CBitMapFont::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();

	m_nBGImageID = CAssetState::GetInstance()->GetImageID("FileSelectBG");
	m_nBoxImageID = CAssetState::GetInstance()->GetImageID("SlotBoxID");
	for(unsigned int i = 0; i < DEF_LEVELS; ++i)
	{
		m_nPreviewPics[i] = CAssetState::GetInstance()->GetImageID("StagePreview", i + 1);
	}

	PopulateList();
	m_nCurrState = FILE_ENTERED;
}

void CFileSelectState::Exit()
{
	
}

bool CFileSelectState::Input()
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

	switch(m_nCurrState) //choose the state
	{
	case FILE_ENTERED:
		{
			//If The fantastic controller is connected
			if( dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS )
			{
				//The Must have Deadzone Input
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


				if(state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				{
					m_nCurrSelection++;
					Sleep(190);
				}
				if(state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
				{
					m_nCurrSelection--;
					Sleep(190);
				}

				if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
				{
					switch(m_nCurrSelection)
					{
					case 0:
						{
							m_nCurrState = FILE_NEW;
							m_nCurrSelection = 0;
							break;
						}
					case 1:
						{
							m_nCurrState = FILE_LOAD;
							PopulateList();
							m_nCurrSelection = 0;
							break;
						}
					case 2:
						{
							m_nCurrState = FILE_SLOT;
							m_nCurrSelection = 0;
							break;
						}
					case 3:
						{
							m_pGame->PushState(CLevelSelect::GetInstance());
							break;
						}
					case 4:
						{
							m_pGame->PushState(CAchievementsState::GetInstance());
						}
					}
				}
				if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_B)
				{
					m_pGame->PopState();
				}
			}
			if(m_pDI->KeyPressed(DIK_UP))
				m_nCurrSelection--;
			if(m_pDI->KeyPressed(DIK_DOWN))
				m_nCurrSelection++;

			if(m_pDI->KeyPressed(DIK_RETURN))
			{
				switch(m_nCurrSelection)
				{
				case 0:
					{
						m_nCurrState = FILE_NEW;
						m_nCurrSelection = 0;
						break;
					}
				case 1:
					{
						m_nCurrState = FILE_LOAD;
						PopulateList();
						m_nCurrSelection = 0;
						break;
					}
				case 2:
					{
						m_nCurrState = FILE_SLOT;
						m_nCurrSelection = 0;
						break;
					}
				
				case 3:
					{
						m_pGame->PushState(CLevelSelect::GetInstance());
						break;
					}
				case 4:
					{
						m_pGame->PushState(CAchievementsState::GetInstance());
						break;
					}
				}
			}

			if(m_pDI->KeyPressed(DIK_ESCAPE))
			{
				m_pGame->PopState();
			}

			break;
		}
	case FILE_NEW:
		{
			if(!m_bFileExists)
			{
				//If that sweet controller is connected
				if(dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS)
				{
					//The Must have Dead Zone Input
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

					if(state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
					{
						if(m_nCurrSelection > 20)
						{
							m_nCurrSelection -= 21;
						}
						else
						{
							m_nCurrSelection += 7;
						}
						Sleep(190);
					}
					if(state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
					{
						if(m_nCurrSelection < 7)
						{
							m_nCurrSelection += 21;
						}
						else
						{
							m_nCurrSelection -= 7;
						}
						Sleep(190);
					}
					//Im going to assume this is left last time I was wrong
					if(state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
					{
						if(m_nCurrSelection % 7 == 0)
							m_nCurrSelection += 6;
						else
							m_nCurrSelection--;
						Sleep(190);
					}
					//Im going to assum this is right
					if(state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
					{
						if(m_nCurrSelection % 7 == 6 || m_nCurrSelection == 26)
							m_nCurrSelection -= 6;
						else
							m_nCurrSelection++;
						Sleep(190);
					}

					if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
					{
						if(m_nCurrSelection < 26)
						{
							AddLetter();
						}
						else
						{
							CreateSave();
						}
					}
					if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_X)
					{
						BackspaceLetter();
					}
					if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_START)
					{
						CreateSave();
					}
					if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_B)
					{
						m_nCurrState = FILE_ENTERED;
						m_nCurrSelection = 0;
					}
				}

				if(m_pDI->KeyPressed(DIK_LEFT))
				{
					if(m_nCurrSelection % 7 == 0)
						m_nCurrSelection += 6;
					else
						m_nCurrSelection--;
				}
				if(m_pDI->KeyPressed(DIK_RIGHT))
				{
					if(m_nCurrSelection % 7 == 6 || m_nCurrSelection == 26)
						m_nCurrSelection -= 6;
					else
						m_nCurrSelection++;
				}
				if(m_pDI->KeyPressed(DIK_UP))
				{
					if(m_nCurrSelection < 7)
					{
						m_nCurrSelection += 21;
					}
					else
					{
						m_nCurrSelection -= 7;
					}
				}
				if(m_pDI->KeyPressed(DIK_DOWN))
				{
					if(m_nCurrSelection > 20)
					{
						m_nCurrSelection -= 21;
					}
					else
					{
						m_nCurrSelection += 7;
					}
				}

				if(m_pDI->KeyPressed(DIK_SPACE))
				{
					if(m_nCurrSelection < 26)
					{
						AddLetter();
					}
					else
					{
						CreateSave();
					}
				}
				if(m_pDI->KeyPressed(DIK_RETURN))
				{
					CreateSave();
				}
				if(m_pDI->KeyPressed(DIK_BACKSPACE))
				{
					BackspaceLetter();
				}

				if(m_pDI->KeyPressed(DIK_ESCAPE))
				{
					m_nCurrState = FILE_ENTERED;
					m_nCurrSelection = 0;
				}
			}
			else
			{
				if(dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS)
				{
					if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
					{
						m_bFileExists = false;
					}
				}

				if(m_pDI->KeyPressed(DIK_RETURN))
				{
					m_bFileExists = false;
					m_bEmpty = false;
					m_szPlayerName[0] = '_';
					m_szPlayerName[1] = '\0';
					m_nNameSize = 0;
				}
			}

			break;
		}
	case FILE_LOAD:
		{
			if(dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS)
			{
				if(state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
				{
					m_nCurrSelection++;
					Sleep(150);
				}
				if(state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
				{
					m_nCurrSelection--;
					Sleep(150);
				}
				if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
				{
					LoadFileIntoGame(m_szAllFiles[m_nCurrSelection]);
				}
				if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_B)
				{
					m_nCurrState = FILE_ENTERED;
					m_nCurrSelection = 0;
				}
			}

			if(m_pDI->KeyPressed(DIK_UP))
				m_nCurrSelection--;
			if(m_pDI->KeyPressed(DIK_DOWN))
				m_nCurrSelection++;
			if(m_pDI->KeyPressed(DIK_RETURN))
			{
				LoadFileIntoGame(m_szAllFiles[m_nCurrSelection]);
			}
			if(m_pDI->KeyPressed(DIK_ESCAPE))
			{
				m_nCurrState = FILE_ENTERED;
				m_nCurrSelection = 0;
			}

			break;
		}
	case FILE_SLOT:
		{
			if(false == m_bDeletingSlot)
			{
				if(!m_bSettingDiff)
				{
					if(dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS)
					{
						if(state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
						{
							m_nCurrSelection++;
							Sleep(150);
						}
						if(state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
						{
							m_nCurrSelection--;
							Sleep(150);
						}
						if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
						{
							if(m_nCurrSelection == 3)
							{
								m_bDeleteMode = !m_bDeleteMode;
							}
							else if(m_bDeleteMode)
							{
								m_bDeletingSlot = true;
							}
							else if(m_pGame->GetSaveFile()->GetSaveData()->tAllSaves[m_nCurrSelection].bFreshSlot == true)
							{
								m_bSettingDiff = true;
								m_nDiffSelection = 0;
							}
							else
							{
								SelectSlot();
							}
						}
						if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_B)
						{
							m_nCurrState = FILE_ENTERED;
							m_nCurrSelection = 0;
						}
					}

					if(m_pDI->KeyPressed(DIK_UP))
						m_nCurrSelection--;

					if(m_pDI->KeyPressed(DIK_DOWN))
						m_nCurrSelection++;

					if(m_pDI->KeyPressed(DIK_RETURN))
					{
						if(m_nCurrSelection == 3)
						{
							m_bDeleteMode = !m_bDeleteMode;
						}
						else if(m_bDeleteMode)
						{
							m_bDeletingSlot = true;
						}
						else if(m_pGame->GetSaveFile()->GetSaveData()->tAllSaves[m_nCurrSelection].bFreshSlot == true)
						{
							m_bSettingDiff = true;
							m_nDiffSelection = 0;
						}
						else
						{
							SelectSlot();
						}
					}

					if(m_pDI->KeyPressed(DIK_ESCAPE))
					{
						m_nCurrState = FILE_ENTERED;
						m_nCurrSelection = 0;
					}
				}
				else
				{
					if(dwResult == ERROR_SUCCESS)
					{
						if(state.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
						{
							m_nDiffSelection++;
							Sleep(150);
						}
						if(state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
						{
							m_nDiffSelection--;
							Sleep(150);
						}

						if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
						{
							m_bSettingDiff = false;
							SelectSlot();
						}
						if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_B)
						{
							m_bSettingDiff = false;
						}
					}

					if(m_pDI->KeyPressed(DIK_UP))
						m_nDiffSelection--;

					if(m_pDI->KeyPressed(DIK_DOWN))
						m_nDiffSelection++;

					if(m_pDI->KeyPressed(DIK_RETURN))
					{
						m_bSettingDiff = false;
						SelectSlot();
					}

					if(m_pDI->KeyPressed(DIK_ESCAPE))
					{
						m_bSettingDiff = false;
					}
				}
			}
			else
			{
				if(dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS)
				{
					if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT || state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
					{
						m_bKillIt = !m_bKillIt;
						Sleep(150);
					}
					if(state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
					{
						m_bKillIt = !m_bKillIt;
						Sleep(150);
					}
					if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
					{
						if(m_bKillIt)
						{
							m_pGame->GetSaveFile()->ClearSaveSlot(m_nCurrSelection);
							m_pGame->GetSaveFile()->Save();
							m_bDeletingSlot = false;
							m_bDeleteMode = false;
							m_bKillIt = false;
							m_nCurrSelection = 0;
							if(m_nCurrSelection == m_pGame->GetCurrentSaveSlot())
							{
								m_pGame->SyncWithFile(m_nCurrSelection);
							}
							
						}
						else
						{
							m_bDeletingSlot = false;
							m_bDeleteMode = false;
							m_nCurrSelection = false;
						}
					}
				}

				if(m_pDI->KeyPressed(DIK_RIGHT) || m_pDI->KeyPressed(DIK_LEFT))
				{
					m_bKillIt = !m_bKillIt;
				}

				if(m_pDI->KeyPressed(DIK_RETURN))
				{
					if(m_bKillIt)
					{
						m_pGame->GetSaveFile()->ClearSaveSlot(m_nCurrSelection);
						m_pGame->GetSaveFile()->Save();
						m_bDeletingSlot = false;
						m_bDeleteMode = false;
						m_bKillIt = false;
						m_nCurrSelection = 0;
						if(m_nCurrSelection == m_pGame->GetCurrentSaveSlot())
						{
							m_pGame->SyncWithFile(m_nCurrSelection);
						}
						
					}
					else
					{
						m_bDeletingSlot = false;
						m_bDeleteMode = false;
						m_nCurrSelection = false;
					}
				}
			}

			break;
		}
	}

	return true;
}

void CFileSelectState::Update(float fDT)
{
	switch(m_nCurrState) //choose the state
	{
	case FILE_ENTERED:
		{
			PopulateList();
			if(m_bFilesFound == false)
				m_nMenuMax = 0;
			else if(m_bGameHasFile == false)
				m_nMenuMax = 1;
			else
				m_nMenuMax = 4; // removes menu options if not there

			//limits the selection choice
			if(m_nCurrSelection > m_nMenuMax)
				m_nCurrSelection = 0;

			if(m_nCurrSelection < 0)
				m_nCurrSelection = m_nMenuMax;

			break;
		}
	case FILE_NEW:
		{
			if(m_nCurrSelection == 27)
				m_nCurrSelection = 26;

			break;
		}
	case FILE_LOAD:
		{
			if(m_nCurrSelection < 0)
				m_nCurrSelection = m_nLastFileIndex;
			if(m_nCurrSelection > m_nLastFileIndex)
				m_nCurrSelection = 0;

			if(m_nCurrSelection < m_nListTop)
			{
				m_nListTop = m_nCurrSelection;
				m_nListBot = m_nListTop + 9;
			}
			if(m_nCurrSelection > m_nListBot)
			{
				m_nListBot = m_nCurrSelection;
				m_nListTop = m_nListBot - 9;
			}

			break;
		}
	case FILE_SLOT:
		{
			if(false == m_bSettingDiff)
			{
				if(m_nCurrSelection < 0)
					m_nCurrSelection = 3;
				if(m_nCurrSelection > 3)
					m_nCurrSelection = 0;
				break;
			}
			else
			{
				if(m_nDiffSelection < 0)
					m_nDiffSelection = 2;
				if(m_nDiffSelection > 2)
					m_nDiffSelection = 0;
				break;
			}
		}
	}
}
void CFileSelectState::Render()
{
	m_pTM->Draw(m_nBGImageID, 0, 0);

	switch(m_nCurrState) //choose the state
	{
	case FILE_ENTERED:
		{
			//display main file loading options - will only display the stage select file if there is a file loaded
			if(m_nCurrSelection == 0)
				m_pFont->Print("New Game", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 6, true, false, 1.25f, D3DCOLOR_XRGB(255, 0, 0));
			else
				m_pFont->Print("New Game", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 6, true, false, 1.25f, D3DCOLOR_XRGB(255, 255, 255));

			if(m_nCurrSelection == 1)
				m_pFont->Print("Load Game", SCREEN_WIDTH / 2, 2 * (SCREEN_HEIGHT / 6), true, false, 1.25f, D3DCOLOR_XRGB(255, 0, 0));
			else if (m_bFilesFound == false)
				m_pFont->Print("Load Game", SCREEN_WIDTH / 2, 2 * (SCREEN_HEIGHT / 6), true, false, 1.25f, D3DCOLOR_XRGB(140, 140, 140));
			else
				m_pFont->Print("Load Game", SCREEN_WIDTH / 2, 2 * (SCREEN_HEIGHT / 6), true, false, 1.25f, D3DCOLOR_XRGB(255, 255, 255));

			if(m_bGameHasFile)
			{
				if(m_nCurrSelection == 2)
					m_pFont->Print("Change Save Slot", SCREEN_WIDTH / 2, 3 * (SCREEN_HEIGHT / 6), true, false, 1.25f, D3DCOLOR_XRGB(255, 0, 0));
				else
					m_pFont->Print("Change Save Slot", SCREEN_WIDTH / 2, 3 * (SCREEN_HEIGHT / 6), true, false, 1.25f, D3DCOLOR_XRGB(255, 255, 255));

				if(m_nCurrSelection == 3)
					m_pFont->Print("Stage Select", SCREEN_WIDTH / 2, 4 * (SCREEN_HEIGHT / 6), true, false, 1.25f, D3DCOLOR_XRGB(255, 0, 0));
				else
					m_pFont->Print("Stage Select", SCREEN_WIDTH / 2, 4 * (SCREEN_HEIGHT / 6), true, false, 1.25f, D3DCOLOR_XRGB(255, 255, 255));
				
				if(m_nCurrSelection == 4)
					m_pFont->Print("Achievements List", SCREEN_WIDTH / 2, 5 * (SCREEN_HEIGHT / 6), true, false, 1.25f, D3DCOLOR_XRGB(255, 0, 0));
				else
					m_pFont->Print("Achievements List", SCREEN_WIDTH / 2, 5 * (SCREEN_HEIGHT / 6), true, false, 1.25f, D3DCOLOR_XRGB(255, 255, 255));
			
			}
			break;
		}
	case FILE_NEW:
		{
			if(m_bFileExists)
			{
				if(!m_bEmpty)
				{
					m_pFont->Print("File Already Exists", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 50, true, false, 1.50f, D3DCOLOR_XRGB(255, 255, 255));
					m_pFont->Print("Please Choose a Different Name", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) , true, false, 1.50f, D3DCOLOR_XRGB(255, 255, 255));
					m_pFont->Print("Press Enter to Continue", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 75, true, false, .75f, D3DCOLOR_XRGB(255, 0, 0));
				}
				else
				{
					m_pFont->Print("Empty File Not Allowed", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 50, true, false, 1.50f, D3DCOLOR_XRGB(255, 255, 255));
					m_pFont->Print("Please Enter a Name", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) , true, false, 1.50f, D3DCOLOR_XRGB(255, 255, 255));
					m_pFont->Print("Press Enter to Continue", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 75, true, false, .75f, D3DCOLOR_XRGB(255, 0, 0));
				}
			}
			else
			{
				m_pFont->Print("Enter Player Name", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 8, true, false, 1.50f, D3DCOLOR_XRGB(255, 255, 255));
				m_pFont->Print(m_szPlayerName, SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 4) - 25 , true, false, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
				DrawKeyboard();
			}
			m_pFont->Print("Space / A: Select", SCREEN_WIDTH - 260, SCREEN_HEIGHT - 110, false, false, .65f, D3DCOLOR_XRGB(255, 255, 255));
			m_pFont->Print("BackSpace / X: Delete", SCREEN_WIDTH - 260, SCREEN_HEIGHT - 85, false, false, .65f, D3DCOLOR_XRGB(255, 255, 255));

			break;
		}
	case FILE_LOAD:
		{
			m_pFont->Print("Select File", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 6, true, false, 1.50f, D3DCOLOR_XRGB(255, 255, 255));
			DrawFileList();

			break;
		}
	case FILE_SLOT:
		{
			if(m_bSettingDiff)
			{
				m_pFont->Print("Select Difficulty", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 10, true, false, 1.50f, D3DCOLOR_XRGB(255, 0, 0));

				if(m_nDiffSelection == 0)
				{
					m_pFont->Print("Easy", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, true, false, 1.0f, D3DCOLOR_XRGB(255, 0, 0));
					m_pFont->Print("Easy Mode offers a nice introduction for beginners.", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 60, true, false, .60f);
					m_pFont->Print("Players will face slower enemies and have 300 seconds per level.", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 90, true, false, .60f);
					m_pFont->Print("In Addition, Players will recieve 33% more Sprockets per pick-up.", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120, true, false, .60f);
					m_pFont->Print("This is also the only Mode with a hit buffer before death.", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 150, true, false, .60f);
				}
				else
					m_pFont->Print("Easy", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, true, false, 1.0f, D3DCOLOR_XRGB(255, 255, 255));

				if(m_nDiffSelection == 1)
				{
					m_pFont->Print("Normal", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 4) + 35, true, false, 1.0f, D3DCOLOR_XRGB(255, 0, 0));
					m_pFont->Print("Normal Mode offers is a step up in sadism.", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 60, true, false, .60f);
					m_pFont->Print("Players will face normal speed enemies", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 90, true, false, .60f);
					m_pFont->Print("and only have 200 seconds per level.", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120, true, false, .60f);
					m_pFont->Print("Currency pick-ups will be normal, and the Hit buffer is Gone.", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 150, true, false, .60f);
				}
				else
					m_pFont->Print("Normal", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 4) + 35, true, false, 1.0f, D3DCOLOR_XRGB(255, 255, 255));

				if(m_nDiffSelection == 2)
				{
					m_pFont->Print("Hard", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 4) + 70, true, false, 1.0f, D3DCOLOR_XRGB(255, 0, 0));
					m_pFont->Print("Hard Mode will Destroy Your Dreams.", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 60, true, false, .60f);
					m_pFont->Print("Enemies will be scary fast, and you will be rushed", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 90, true, false, .60f);
					m_pFont->Print("With only 100 seconds to complete a level. And Because times ", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 120, true, false, .60f);
					m_pFont->Print("are tight, there has been a 33% sprocket Payout cut.", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 150, true, false, .60f);
				}
				else
					m_pFont->Print("Hard", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 4) + 70, true, false, 1.0f, D3DCOLOR_XRGB(255, 255, 255));

				RECT boxRect;
				boxRect.top = 0;
				boxRect.left = 0;
				boxRect.right = 768;
				boxRect.bottom = 154;
				m_pTM->Draw(m_nBoxImageID, SCREEN_WIDTH / 8, (SCREEN_HEIGHT / 2) + 50, 1.0f, 1.0f, &boxRect, 0.0f, 0.0f, 0.0f);
			}
			else
			{
				if(m_bDeleteMode)
				{
					m_pFont->Print("Delete Which Slot?", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 10, true, false, 1.50f, D3DCOLOR_XRGB(255, 0, 0));
				}
				else
				{
					m_pFont->Print("Select Game Slot", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 10, true, false, 1.50f);
				}
				if(m_bDeletingSlot == false)
				{
					DrawGameSlots();
				}
				else
				{
					char buffer[100];
					sprintf_s(buffer, "Delete Slot %i?", m_nCurrSelection);
					m_pFont->Print("Are You Sure You To", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 70, true, false, 1.25);
					m_pFont->Print(buffer, SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 35, true, false, 1.25);

					if(m_bKillIt == false)
					{
						m_pFont->Print("No", (SCREEN_WIDTH / 2) - 50, (SCREEN_HEIGHT / 2) + 20, true, false, 1.0f, D3DCOLOR_XRGB(255, 0, 0));
						m_pFont->Print("Yes", (SCREEN_WIDTH / 2) + 50, (SCREEN_HEIGHT / 2) + 20, true, false, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
					}
					else
					{
						m_pFont->Print("No", (SCREEN_WIDTH / 2) - 50, (SCREEN_HEIGHT / 2) + 20, true, false, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
						m_pFont->Print("Yes", (SCREEN_WIDTH / 2) + 50, (SCREEN_HEIGHT / 2) + 20, true, false, 1.0f, D3DCOLOR_XRGB(255, 0, 0));
					}
				}
			}
			break;
		}
	}

	if(m_nCurrState != FILE_NEW)
		m_pFont->Print("Enter / A: Select", SCREEN_WIDTH - 260, SCREEN_HEIGHT - 60, false, false, .65f, D3DCOLOR_XRGB(255, 255, 255));
	else
		m_pFont->Print("Enter / Start: Done", SCREEN_WIDTH - 260, SCREEN_HEIGHT - 60, false, false, .65f, D3DCOLOR_XRGB(255, 255, 255));

	m_pFont->Print("Escape / B: Back", SCREEN_WIDTH - 260, SCREEN_HEIGHT - 35, false, false, .65f, D3DCOLOR_XRGB(255, 255, 255));

	if(m_bGameHasFile == false)
	{
		m_pFont->Print("Player: None", 35, SCREEN_HEIGHT - 35, false, false, .65f, D3DCOLOR_XRGB(255, 255, 255));
	}
	else
	{
		char slotBuff[50];
		string buffer = "Player: ";
		string name;
		name = m_pGame->GetSaveFile()->GetSaveData()->szPlayerName;
		buffer += name;
		sprintf_s(slotBuff, " Slot: %i",m_pGame->GetCurrentSaveSlot() + 1);
		buffer += slotBuff;
		m_pFont->Print(buffer.c_str(), 35, SCREEN_HEIGHT - 35, false, false, .65f, D3DCOLOR_XRGB(255, 255, 255));
	}
}

void CFileSelectState::PopulateList()
{
	//setstate values to nothing found
	m_bFilesFound = false;
	m_nLastFileIndex = 0;

	//declare values needed for chekcing and counting
	unsigned int counter = 0;
	bool working = true;

	//strings for reading and checking the data
	string buffer;

	//a find data object - gues what it does, actually it stores the data for the results of findfirsta dn enxt file
	WIN32_FIND_DATA myFinder;

	//handle created to feed to find next and find first file
	//findfirstfile found to determine if files were found at a;; right away
	HANDLE myHandle = FindFirstFile(DEF_FILEPATH,&myFinder);

	if(myHandle != INVALID_HANDLE_VALUE) //did i get a valid handle off of that result
	{
		   buffer = myFinder.cFileName; //store the string filename in my buffer
		   m_szAllFiles[counter] = buffer; //store that buffer in slot 0;
		   m_bFilesFound = true; // ok we have files found so the load screen is valid

		   while(true == working) //while we're still working
		   {
				  FindNextFile(myHandle,&myFinder); //now find the next file, using that same finder to keep my place in the directory

				  //if the next file found does not equal the previous
				  if(myFinder.cFileName!=buffer)
				  {
						 buffer = myFinder.cFileName; //store the file name in the buffer
						 ++counter; //up the counter for my string array
						 m_szAllFiles[counter] = buffer; //store the buffer in the string array
						 m_nLastFileIndex++; //up the last file index
				  }
				  else //same file returned twice
				  {
						  //end of files reached
						  working = false;
				  }
		   }
	}
}

CFileSelectState* CFileSelectState::GetInstance()
{
	static CFileSelectState instance;
	return &instance;
}

void CFileSelectState::LoadFileIntoGame(string fileName)
{
	char buffer[100] = DEF_PATHADDNAME;
	strcat_s(buffer, fileName.c_str());

	m_pGame->SetSaveFilePath(buffer);
	m_pGame->LoadFile();
	m_pGame->SyncWithFile(0);
	m_bGameHasFile = true;
	m_nCurrState = FILE_SLOT;
	m_nCurrSelection = 0;
}

void CFileSelectState::DrawFileList()
{
	for(unsigned int i = m_nListTop; i <= m_nListBot  && i <= m_nLastFileIndex; ++i)
	{
		if(m_nCurrSelection == i)
			m_pFont->Print(m_szAllFiles[i].c_str(), SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 3) + ((i - m_nListTop) * 25), true, false, .75f, D3DCOLOR_XRGB(255, 0, 0));
		else
			m_pFont->Print(m_szAllFiles[i].c_str(), SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 3) + ((i - m_nListTop) * 25), true, false, .60f, D3DCOLOR_XRGB(255, 255, 255));
	}
}

void CFileSelectState::DrawKeyboard()
{
	char drawChar = m_nStartingChar;
	int Row = 0;
	int Col = 0;
	int LastCol = 7;
	string	buffer;
	for(int i = 0; i < 27; ++i)
	{
		buffer = drawChar;
		if(i < 26)
		{
			if(m_nCurrSelection == i)
				m_pFont->Print(buffer.c_str(), (SCREEN_WIDTH / 3) + (Col * 60), (SCREEN_HEIGHT / 2) + (Row * 40), true, false, 1.0f, D3DCOLOR_XRGB(255, 0, 0));
			else
				m_pFont->Print(buffer.c_str(), (SCREEN_WIDTH / 3) + (Col * 60), (SCREEN_HEIGHT / 2) + (Row * 40), true, false, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
		}
		else
		{
			if(m_nCurrSelection == i)
				m_pFont->Print("Done", (SCREEN_WIDTH / 3) + (Col * 60) - 8, (SCREEN_HEIGHT / 2) + (Row * 40), false, false, 1.0f, D3DCOLOR_XRGB(255, 0, 0));
			else
				m_pFont->Print("Done", (SCREEN_WIDTH / 3) + (Col * 60) - 8, (SCREEN_HEIGHT / 2) + (Row * 40), false, false, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
		}
		Col++;
		if(Col >= LastCol)
		{
			Row++;
			Col = 0;
		}
		drawChar++;
	}
}
void CFileSelectState::AddLetter()
{
	if(m_nNameSize < 32 && m_nCurrSelection < 26)
	{
		char newLetter = 'a';
		newLetter += m_nCurrSelection;
		m_szPlayerName[m_nNameSize] = newLetter;
		m_szPlayerName[m_nNameSize + 1] = '_';
		m_szPlayerName[m_nNameSize + 2] = '\0';
		m_nNameSize++;
	}
}
void CFileSelectState::BackspaceLetter()
{
	if(m_nNameSize > 0)
	{
		m_szPlayerName[m_nNameSize - 1] = '_';
		m_szPlayerName[m_nNameSize ] = '\0';
		m_nNameSize--;
	}
}
void CFileSelectState::CreateSave()
{
	char* nameBuff = new char[m_nNameSize + 1];
	for(unsigned int i = 0; i < (unsigned int) m_nNameSize; ++i)
		nameBuff[i] = m_szPlayerName[i];
	nameBuff[m_nNameSize] = '\0';
	string buffer =  nameBuff; //copy player name to buffer

	buffer += ".gps"; //add the file ending

	if(CheckForSave(buffer) && !(m_nNameSize < 1)) //if save doesnt exist
	{
		CGameSaveFile* file = m_pGame->GetSaveFile();
		if(m_nNameSize < 32)
			m_szPlayerName[m_nNameSize] = '\0';
		file->ClearEntireFile();
		file->SetPlayerName(nameBuff);
		string path = DEF_PATHADDNAME;
		path += buffer;
		file->SetFileName(path.c_str());
		file->Save();
		m_pGame->SetGameSlot(0);
		m_pGame->SyncWithFile(0);
		m_nCurrState = FILE_SLOT;
		m_nCurrSelection = 0;
		m_bGameHasFile = true;
	}
	else //if it does
	{
		if(m_nNameSize < 1)
		{
			m_bEmpty = true;
		}
		m_bFileExists = true;
	}
	delete[] nameBuff;
}
bool CFileSelectState::CheckForSave(string checkName)
{
	for(unsigned int i = 0; i <= m_nLastFileIndex; ++i)
	{
		if( 0 == strcmp(m_szAllFiles[i].c_str(), checkName.c_str())) //check if this file name exists in the directory we checked
		{
			return false; //if so return false, this is an invalid save
		}
	}
	return true; //else good to go
}

void CFileSelectState::DrawGameSlots()
{
	RECT boxRect;
	boxRect.top = 0;
	boxRect.left = 0;
	boxRect.right = 768;
	boxRect.bottom = 154;

	RECT prevRect;

	for(unsigned int i = 0; i < 3 ; ++i)
	{
		if(m_nCurrSelection == i)
			m_pTM->Draw(m_nBoxImageID, SCREEN_WIDTH / 8, (SCREEN_HEIGHT / 6) + (i * 154), 1.0f, 1.0f, &boxRect, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
		else
			m_pTM->Draw(m_nBoxImageID, SCREEN_WIDTH / 8, (SCREEN_HEIGHT / 6) + (i * 154), 1.0f, 1.0f, &boxRect);

		char buffer[100];

		sprintf_s(buffer, "Slot %i", i + 1);
		m_pFont->Print(buffer, (SCREEN_WIDTH / 8) + 32, (SCREEN_HEIGHT / 6) + (i * 154) + 35, false, false, 1.0f, D3DCOLOR_XRGB(255, 255, 255));

		if(m_pGame->GetSaveFile()->GetSaveData()->tAllSaves[i].bFreshSlot == false)
		{
			unsigned int level = 0;
			for(;level < 20 && m_pGame->GetSaveFile()->GetSaveData()->tAllSaves[i].tAllLevels[level].bComplete == true;++level);
			
			if(level < 20)
				sprintf_s(buffer, "Level: %i", level + 1);
			else
				sprintf_s(buffer, "Complete");
			

			m_pFont->Print(buffer, 600, (SCREEN_HEIGHT / 6)  + (i * 154) + 20, false, false, .80f, D3DCOLOR_XRGB(255, 255, 255));

			int difficulty = m_pGame->GetSaveFile()->GetSaveData()->tAllSaves[i].nDifficulty;
			switch(difficulty)
			{
			case DIFF_EASY:
				{
					sprintf_s(buffer, "Easy");
					break;
				}
			case DIFF_NORMAL:
				{
					sprintf_s(buffer, "Normal");
					break;
				}
			case DIFF_HARD:
				{
					sprintf_s(buffer, "Hard");
					break;
				}
			}

			m_pFont->Print(buffer, 600, (SCREEN_HEIGHT / 6)  + (i * 154) + 60, false, false, .80f, D3DCOLOR_XRGB(255, 255, 255));

			int prevX = 750;
			int prevY = (SCREEN_HEIGHT / 6)  + (i * 154) + 20;

			if(level < 20) //if check until all preview images are done
				m_pTM->Draw(m_nPreviewPics[level], prevX, prevY, 0.25f, 0.25f);
			else
				m_pTM->Draw(m_nPreviewPics[19], prevX, prevY, 0.25f, 0.25f);
		}
		else
		{
			sprintf_s(buffer, "Empty");
			m_pFont->Print(buffer,  600, (SCREEN_HEIGHT / 6)  + (i * 154) + 40, false, false, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
		}
	}

	if(m_bDeleteMode)
	{
		if(m_nCurrSelection == 3)
			m_pFont->Print("Switch To Select Mode", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 6)  + (3 * 154) + 40, true, false, 1.0f, D3DCOLOR_XRGB(255, 0, 0));
		else
			m_pFont->Print("Switch To Select Mode", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 6)  + (3 * 154) + 40, true, false, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
	}
	else
	{
		if(m_nCurrSelection == 3)
			m_pFont->Print("Switch To Delete Mode", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 6)  + (3 * 154) + 40, true, false, 1.0f, D3DCOLOR_XRGB(255, 0, 0));
		else
			m_pFont->Print("Switch To Delete Mode", SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 6)  + (3 * 154) + 40, true, false, 1.0f, D3DCOLOR_XRGB(255, 255, 255));
	}
}
void CFileSelectState::SelectSlot()
{
	m_pGame->SetGameSlot(m_nCurrSelection);
	if(m_pGame->GetSaveFile()->GetSaveData()->tAllSaves[m_nCurrSelection].bFreshSlot == true)
	{
		m_pGame->GetSaveFile()->GetSaveData()->tAllSaves[m_nCurrSelection].bFreshSlot = false;
		m_pGame->GetSaveFile()->GetSaveData()->tAllSaves[m_nCurrSelection].nDifficulty = m_nDiffSelection;
	}

	m_pGame->GetSaveFile()->Save();
	m_pGame->SyncWithFile(m_nCurrSelection);
	m_nCurrState = FILE_ENTERED;
	m_nCurrSelection = 3;
	m_pGame->PushState(CLevelSelect::GetInstance());
}