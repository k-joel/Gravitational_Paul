#include "CCustomLevelState.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "CGamePlayState.h"
#include "CGame.h"
#include "CBitMapFont.h"
#include "CLevelSelect.h"
#include "CTileManager.h"
#include "CAssetState.h"

#define DEF_FILEPATH "Resource/Custom Levels/*.bin"
#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1024

#include <XInput.h>

CCustomLevelState::CCustomLevelState()
{
	m_pGame = NULL;
	m_pGS = NULL;
	m_pDI = NULL;
	m_pFont = NULL;
	m_pTM = NULL;
	m_nBackGroundID = -1;
	m_nLevelID = -1;
	m_nListTop = 0;
	m_nListBot = 9;
	m_szCustomLevel = "";

	m_nMaxPages = 0;
	m_nCurrentPage = 0;
}

CCustomLevelState::~CCustomLevelState()
{
}

void CCustomLevelState::Enter()
{
	m_pGame = CGame::GetInstance();
	m_pGS = CGamePlayState::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pFont = CBitMapFont::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_nBackGroundID = CLevelSelect::GetInstance()->GetBackGroundID();
	m_nLevelID = CAssetState::GetInstance()->GetImageID("CustomLevelID");
	m_nCurrSelection = 0;
	PopulateList();
	m_nMaxPages = m_nLastFileIndex / 10 + 1;
	m_szCustomLevel = "";
	m_nCurrentPage = 1;
}

void CCustomLevelState::Exit()
{
	m_nListTop = 0;
	m_nListBot = 9;
	m_pGame->SetIsCustom(false);
}

bool CCustomLevelState::Input()
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

	if(dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS)
	{
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_UP || key.VirtualKey == VK_PAD_LTHUMB_UP)  && m_nListTop - 10 >= 0)
		{
			m_nListBot = m_nListTop - 1;
			m_nListTop = m_nListTop - 10;

			m_nCurrSelection = m_nListTop;
		}
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_DOWN || key.VirtualKey == VK_PAD_LTHUMB_DOWN) && m_nListBot != m_nLastFileIndex)
		{
			m_nListTop = m_nListBot + 1;

			if(m_nListTop + 10 > m_nLastFileIndex)
				m_nListBot = m_nLastFileIndex;
			else
				m_nListBot = m_nListTop + 9;

			m_nCurrSelection = m_nListTop;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_LEFT || key.VirtualKey == VK_PAD_LTHUMB_LEFT))
		{
			m_nCurrSelection--;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_RIGHT || key.VirtualKey == VK_PAD_LTHUMB_RIGHT))
		{
			m_nCurrSelection++;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_B)
		{
			m_pGame->PopState();
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
		{
			m_szCustomLevel = m_szAllFiles[m_nCurrSelection];
			m_pGame->SetIsCustom(true);
			m_pGame->PushState(CGamePlayState::GetInstance());
		}
	}

	//Quitting out if the user hits escape.
	if(m_pDI->KeyPressedEx(DIK_ESCAPE))
		m_pGame->PopState();

	//Increasing the page number
	if(m_pDI->KeyPressedEx(DIK_DOWN) && m_nListBot != m_nLastFileIndex)
	{
		m_nListTop = m_nListBot + 1;

		if(m_nListTop + 10 > m_nLastFileIndex)
			m_nListBot = m_nLastFileIndex;
		else
			m_nListBot = m_nListTop + 9;

		m_nCurrSelection = m_nListTop;
	}

	//Decreasing the page number
	if(m_pDI->KeyPressedEx(DIK_UP) && m_nListTop - 10 >= 0)
	{
		m_nListBot = m_nListTop - 1;
		m_nListTop = m_nListTop - 10;

		m_nCurrSelection = m_nListTop;
	}

	//Decreasing the current selection number
	if(m_pDI->KeyPressed(DIK_LEFT))
	{
		m_nCurrSelection--;
	}

	//Increasing the current selection number
	if(m_pDI->KeyPressed(DIK_RIGHT))
		m_nCurrSelection++;

	//Loading up the selected custom level
	if(m_pDI->KeyPressedEx(DIK_RETURN))
	{
		m_szCustomLevel = m_szAllFiles[m_nCurrSelection];
		m_pGame->SetIsCustom(true);
		m_pGame->PushState(CGamePlayState::GetInstance());
	}

	return true;
}

void CCustomLevelState::Update(float fDT)
{
	//if(m_nCurrSelection < 0)
	//	m_nCurrSelection = m_nListBot - 1;

	//if(m_nCurrSelection > m_nListBot - 1)
	//	m_nCurrSelection = 0;

	if(m_nCurrSelection < m_nListTop)
	{
		m_nCurrSelection = m_nListBot;
	}

	if(m_nCurrSelection > m_nListBot)
	{
		m_nCurrSelection = m_nListTop;
	}
}

void CCustomLevelState::Render()
{
	m_pTM->Draw(m_nBackGroundID, 0, 0);

	//Drawing the stage names.  YES.
	char buffer[64];
	int xOffset = 0;
	int yOffset = 0;
	int nIndex = 0;

	for(int x = m_nListTop; x <= m_nListBot && x <= m_nLastFileIndex; ++x)
	{
		//This crap right here is being used to cut off the ".bin" of the filename
		string temp;
		for(int i = 0; i < m_szAllFiles[x].length() - 4; ++i)
		{
			temp = temp + m_szAllFiles[x][i];
		}

		sprintf_s(buffer, "%s", temp.c_str());

		if(x == m_nCurrSelection)
			m_pFont->Print(buffer, 36 + (200 * xOffset), 106 + (250 * yOffset), false, true, 0.50f);
		else
			m_pFont->Print(buffer, 36 + (200 * xOffset), 106 + (250 * yOffset), false, false, 0.50f);

		 m_pTM->Draw(m_nLevelID, 36 + (200 * xOffset), 136 + (250 * yOffset), 0.30f, 0.30f);

		++xOffset;
		++nIndex;

		if(nIndex == 5)
		{
			++yOffset;
			xOffset = 0;
		}

		if(nIndex > 10)
			break;
	}

	CBitMapFont::GetInstance()->Print("Press Down for more Levels Up for Previous Ones", 20, (m_pGame->GetWindowWidth() / 5 * 2) + 200, false, false, 1, D3DCOLOR_ARGB(255,255,0,0));
	CBitMapFont::GetInstance()->Print("Press ESC or B to go back", 20, (m_pGame->GetWindowWidth() / 5 * 2) + 250, false, false, 1, D3DCOLOR_ARGB(255,255,0,0));
}

CCustomLevelState* CCustomLevelState::GetInstance()
{
	static CCustomLevelState instance;
	return &instance;
}

void CCustomLevelState::PopulateList()
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

void CCustomLevelState::DrawList()
{
	for(unsigned int i = m_nListTop; i <= (unsigned int)m_nListBot  && i <= (unsigned int)m_nLastFileIndex; ++i)
	{
		if(m_nCurrSelection == i)
			m_pFont->Print(m_szAllFiles[i].c_str(), SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 3) + ((i - m_nListTop) * 25), true, false, .75f, D3DCOLOR_XRGB(255, 0, 0));
		else
			m_pFont->Print(m_szAllFiles[i].c_str(), SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 3) + ((i - m_nListTop) * 25), true, false, .60f, D3DCOLOR_XRGB(255, 255, 255));
	}
}