#include "CCredits.h"

#include "CGame.h"
#include "COptionsState.h"
#include "CBitMapFont.h"
#include "CAssetState.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "CMainMenuState.h"

#define INCREMENT_TIME 0.05f
#define NORMAL_X 100
#define TITLE_X 50
#define WINDOW_HEIGHT_CAMAL 768

CCredits::CCredits()
{
	m_pTM = NULL;
	m_pFont = NULL;
	m_pOptions = NULL;
	m_pGame = NULL;
	m_nOffset = 0;
	m_fElapsedTime = 0.0f;
}

CCredits::~CCredits()
{
}

bool CCredits::Input()
{
	DWORD dwStateResult;
	XINPUT_KEYSTROKE key;
	// of a controller.

	ZeroMemory( &key, sizeof(XINPUT_KEYSTROKE));
	// the controller.

	// Get all the current states of controller 1
	dwStateResult = XInputGetKeystroke(XUSER_INDEX_ANY,XINPUT_FLAG_GAMEPAD,&key);

	if(dwStateResult == ERROR_SUCCESS)
	{
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_B)
		{
			CGame::GetInstance()->PopState();
			m_pGame->PushState(CMainMenuState::GetInstance());
		}
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressedEx(DIK_ESCAPE))
	{
		CGame::GetInstance()->PopState();
		m_pGame->PushState(CMainMenuState::GetInstance());
	}

	return true;
}

void CCredits::Render()
{
	//So.... here I'm basically showing a bunch of credits BELOW the window height.  Then subtracting an offset so they slowly scroll up.
	//Genious?  HELL TO THE YEAH.
	CSGD_Direct3D::GetInstance()->Clear(0, 0, 0);

	m_pTM->Draw(m_nImageID,0,0);

	m_pFont->Print("Executive Producers", TITLE_X, WINDOW_HEIGHT_CAMAL - m_nOffset, false, true);
	m_pFont->Print("John Oleske", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() - m_nOffset, false, false);
	m_pFont->Print("Mike \"Wigs\" Wigand", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 2 - m_nOffset, false, false);

	m_pFont->Print("Associate Producers", TITLE_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 4 - m_nOffset, false, true);
	m_pFont->Print("Rob Martinez", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 5 - m_nOffset, false, false);
	m_pFont->Print("Ron Powell", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 6 - m_nOffset, false, false);
	m_pFont->Print("Sean Hathaway", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 7 - m_nOffset, false, false);

	m_pFont->Print("Sound Guys", TITLE_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 9 - m_nOffset, false, true);
	m_pFont->Print("Casey Coffman", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 10 - m_nOffset, false, false);
	m_pFont->Print("Chris Marks", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 11 - m_nOffset, false, false);

	m_pFont->Print("Art Guy", TITLE_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 13 - m_nOffset, false, true);
	m_pFont->Print("Chris Jahosky", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 14 - m_nOffset, false, false);

	m_pFont->Print("Programmers", TITLE_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 16 - m_nOffset, false, true);
	m_pFont->Print("John Wileczek", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 17 - m_nOffset, false, false);
	m_pFont->Print("Justin Dumas", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 18 - m_nOffset, false, false);
	m_pFont->Print("Josh Davis", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 19 - m_nOffset, false, false);
	m_pFont->Print("Kahran Ghosalkar", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 20 - m_nOffset, false, false);
	m_pFont->Print("TJ Farnie", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 21 - m_nOffset, false, false);

	m_pFont->Print("Music", TITLE_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 23 - m_nOffset, false, true);

	m_pFont->Print("Waste Water Wipeout", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 25 - m_nOffset, false, false);
	m_pFont->Print("by tefnek & zircon", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 26 - m_nOffset, false, false);
	m_pFont->Print("www.ocremix.org", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 27 - m_nOffset, false, false);
	
	m_pFont->Print("Lionheart", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 29 - m_nOffset, false, false);
	m_pFont->Print("by ZeRo-BaSs", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 30 - m_nOffset, false, false);
	m_pFont->Print("www.newgrounds.com/audio", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 31 - m_nOffset, false, false);

	m_pFont->Print("still blastin\'", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 33 - m_nOffset, false, false);
	m_pFont->Print("by NemesisTheory", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 34 - m_nOffset, false, false);
	m_pFont->Print("www.newgrounds.com/audio", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 35 - m_nOffset, false, false);

	m_pFont->Print("stainless steel", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 37 - m_nOffset, false, false);
	m_pFont->Print("by ae", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 38 - m_nOffset, false, false);
	m_pFont->Print("www.ocremix.org", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 39 - m_nOffset, false, false);

	m_pFont->Print("jamesbrowninspace", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 41 - m_nOffset, false, false);
	m_pFont->Print("by djpretzel", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 42 - m_nOffset, false, false);
	m_pFont->Print("www.ocremix.org", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 43 - m_nOffset, false, false);
	
	m_pFont->Print("alpha blade", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 45 - m_nOffset, false, false);
	m_pFont->Print("by beatdrop", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 46 - m_nOffset, false, false);
	m_pFont->Print("www.ocremix.org", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 47 - m_nOffset, false, false);
	
	m_pFont->Print("Subterranean Kamikaze", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 49 - m_nOffset, false, false);
	m_pFont->Print("by Sixto Sounds & zircon", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 50 - m_nOffset, false, false);
	m_pFont->Print("www.ocremix.org", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 51 - m_nOffset, false, false);

	m_pFont->Print("Special Thanks", TITLE_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 53 - m_nOffset, false, true);
	m_pFont->Print("Tim Beneke", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 54 - m_nOffset, false, false);
	m_pFont->Print("Norm Kelly", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 55 - m_nOffset, false, false);
	m_pFont->Print("Matt Tjarks", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 56 - m_nOffset, false, false);
	m_pFont->Print("Sir Isaac Newton", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 57 - m_nOffset, false, false);
	m_pFont->Print("Chips", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 58 - m_nOffset, false, false);
	m_pFont->Print("Caffeine", NORMAL_X, WINDOW_HEIGHT_CAMAL + m_pFont->GetCellHeight() * 59 - m_nOffset, false, false);

}
void CCredits::Enter()
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pOptions = COptionsState::GetInstance();
	m_pFont = CBitMapFont::GetInstance();
	m_pGame = CGame::GetInstance();

	CSGD_DirectInput::GetInstance()->ClearInput();

	m_nImageID = CAssetState::GetInstance()->GetImageID("CreditsID"); 
}
void CCredits::Exit()
{
	m_fElapsedTime = 0.0f;
	m_nOffset = 0;
}
void CCredits::Update(float fDT)
{
	m_fElapsedTime += fDT;

	if(m_fElapsedTime > INCREMENT_TIME)
	{
		m_nOffset += 2;
		m_fElapsedTime = 0.0f;
	}

	if(m_nOffset / 2 >= 1380)
		m_pGame->PushState(CMainMenuState::GetInstance());
		
}

CCredits* CCredits::GetInstance()
{
	static CCredits instance;
	return &instance;
}