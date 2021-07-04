#include "CHowToPlay.h"

#include "CBitMapFont.h"
#include "CGame.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "COptionsState.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "CAssetState.h"
#include "CGamePlayState.h"

CHowToPlay::CHowToPlay()
{
	m_pGame				= NULL;
	m_pTM				= NULL;
	m_pOptions			= NULL;
	m_pDI				= NULL;

	m_nImageID			= -1;
}

CHowToPlay::~CHowToPlay()
{
}
void CHowToPlay::Enter()
{
	m_pGame = CGame::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pOptions = COptionsState::GetInstance();

	m_nImageID = CAssetState::GetInstance()->GetImageID("HowToPlayID");
}

void CHowToPlay::Exit()
{
}

bool CHowToPlay::Input()
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
			m_pGame->PopState();
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
		{
			m_pGame->SetLoadLevel(30);
			m_pGame->PushState(CGamePlayState::GetInstance());
		}
	}

	if(m_pDI->KeyPressedEx(DIK_ESCAPE))
	{
		m_pGame->PopState();
	}

	if(m_pDI->KeyPressedEx(DIK_RETURN))
	{
		m_pGame->SetLoadLevel(30);
		m_pGame->PushState(CGamePlayState::GetInstance());
	}

	return true;
}

void CHowToPlay::Update(float fDT)
{
}

CHowToPlay* CHowToPlay::GetInstance()
{
	static CHowToPlay instance;
	return &instance;
}

void CHowToPlay::Render()
{
	m_pTM->Draw(m_nImageID, 0, 0, 1.0f, 1.5f);

	//Display Menu Controls to the screen
	CBitMapFont::GetInstance()->Print(" Menu Controls ",500,100,true,true);

	CBitMapFont::GetInstance()->Print("Keyboard",20,150);
	CBitMapFont::GetInstance()->Print("Navigation: Arrow Keys",20,200,false,false,.8f);
	CBitMapFont::GetInstance()->Print("Select: Enter",20,225,false,false,.8f);
	CBitMapFont::GetInstance()->Print("Back: Escape",20,250,false,false,.8f);

	CBitMapFont::GetInstance()->Print("Xbox Controller",550,150);
	CBitMapFont::GetInstance()->Print("Navigation: Left Thumbstick/Dpad",550,200,false,false,.8f);
	CBitMapFont::GetInstance()->Print("Select: A",550,225,false,false,.8f);
	CBitMapFont::GetInstance()->Print("Back: B",550,250,false,false,.8f);
	CBitMapFont::GetInstance()->Print("Backspace: X",550,275,false,false,.8f);

	//Gameplay Controls will be updated when the user changes their keybinding and shit
	CBitMapFont::GetInstance()->Print("Gameplay Controls",500,400,true,true);

	CBitMapFont::GetInstance()->Print("Keyboard",20,500);

	RenderKControls(m_pGame->GetKeyboardLayout());
	

	CBitMapFont::GetInstance()->Print("Xbox Controller",550,500);

	RenderCControls(m_pGame->GetControllerLayout());

	CBitMapFont::GetInstance()->Print("Press Escape/B: Go Back",20,750,false,false,.5f);
	CBitMapFont::GetInstance()->Print("Press Enter/A: Go to tutorial level",700,750,false,false,.5f);
	

}

void CHowToPlay::RenderKControls(int controlscheme)
{
	switch(controlscheme)
	{
	case 0: //default
		CBitMapFont::GetInstance()->Print("Jump: W",20,550,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Move Right: D",20,575,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Move Left: A",20,600,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Shoot Pull Well: Left Mouse Button",20,625,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Shoot Push Well: Right Mouse Button",20,650,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Close Wells: Middle Mouse Button",20,675,false,false,.7f);
		break;
	case 1: 
		CBitMapFont::GetInstance()->Print("Jump: W",20,550,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Move Right: D",20,575,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Move Left: A",20,600,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Shoot Pull Well: Left Mouse Button",20,625,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Shoot Push Well: Right Mouse Button",20,650,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Close Wells: E",20,675,false,false,.7f);
		break;
	case 2:
		CBitMapFont::GetInstance()->Print("Jump: Up Arrow",20,550,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Move Right: Right Arrow",20,575,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Move Left: Left Arrow",20,600,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Shoot Pull Well: Left Mouse Button",20,625,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Shoot Push Well: Right Mouse Button",20,650,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Close Wells: Middle Mouse Button",20,675,false,false,.7f);
		break;
	}
}

void CHowToPlay::RenderCControls(int controlscheme)
{
	switch(controlscheme)
	{
	case 0: //default
		CBitMapFont::GetInstance()->Print("Jump: Left Thumbstick Up",550,550,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Move Right: Left Thumbstick Right",550,575,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Move Left: Left Thumbstick Left",550,600,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Shoot Pull Well: Right Trigger",550,625,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Shoot Push Well: Left Trigger",550,650,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Close Wells: Right Bumper",550,675,false,false,.7f);
		break;
	case 1:
		CBitMapFont::GetInstance()->Print("Jump: Dpad Up",550,550,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Move Right: Dpad Right",550,575,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Move Left: Dpad Left",550,600,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Shoot Pull Well: B",550,625,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Shoot Push Well: Y",550,650,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Close Wells: Left Bumper",550,675,false,false,.7f);
		break;
	case 2:
		CBitMapFont::GetInstance()->Print("Jump: Left Bumper",550,550,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Move Right: Left Thumbstick Right",550,575,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Move Left: Left Thumbstick Left",550,600,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Shoot Pull Well: Right Trigger",550,625,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Shoot Push Well: Left Trigger",550,650,false,false,.7f);
		CBitMapFont::GetInstance()->Print("Close Wells: Y",550,675,false,false,.7f);
		break;
	}
}