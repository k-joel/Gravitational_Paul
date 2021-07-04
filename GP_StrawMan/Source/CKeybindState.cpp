#include "CKeybindState.h"
#include "CGame.h"
#include "CAssetState.h"
#include "CBitMapFont.h"

CKeybindState::CKeybindState()
{
	m_pGame = NULL;

	m_nCurrSelection = 0;

	m_nImageID = -1;


}
CKeybindState::~CKeybindState()
{

}

void CKeybindState::Enter()
{
	m_pGame = CGame::GetInstance();
	m_nImageID = CAssetState::GetInstance()->GetImageID("OptionsBG");
	m_nCurrSelection = 0;
}

void CKeybindState::Exit()
{
	m_nCurrSelection = 0;
	m_pGame->GetSaveFile()->Save();
}

CKeybindState* CKeybindState::GetInstance()
{
	static CKeybindState instance;
	return &instance;
}

bool CKeybindState::Input()
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

	if(dwStateResult == ERROR_SUCCESS || dwResult == ERROR_SUCCESS)
	{

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_B)
		{
			m_pGame->PopState();
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_UP || key.VirtualKey == VK_PAD_LTHUMB_UP))
		{
			m_nCurrSelection--;
			if(m_nCurrSelection < 0)
				m_nCurrSelection = 1;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_DOWN || key.VirtualKey == VK_PAD_LTHUMB_DOWN))
		{
			m_nCurrSelection++;
			if(m_nCurrSelection > 1)
				m_nCurrSelection = 0;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_LEFT || key.VirtualKey == VK_PAD_LTHUMB_LEFT))
		{
			if(0 == m_nCurrSelection)
			{
				m_pGame->SetKeyboardLayout(m_pGame->GetKeyboardLayout() - 1);
				if(m_pGame->GetKeyboardLayout() < 0)
					m_pGame->SetKeyboardLayout(2);
				SwitchKControls(m_pGame->GetKeyboardLayout());
			}
			else
			{
				m_pGame->SetControllerLayout(m_pGame->GetControllerLayout() - 1);
				if(m_pGame->GetControllerLayout() < 0)
					m_pGame->SetControllerLayout(2);
				SwitchCControls(m_pGame->GetControllerLayout());
			}
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_RIGHT || key.VirtualKey == VK_PAD_LTHUMB_RIGHT))
		{
			if(0 == m_nCurrSelection)
			{
				m_pGame->SetKeyboardLayout(m_pGame->GetKeyboardLayout() + 1);
				if(m_pGame->GetKeyboardLayout() > 2)
					m_pGame->SetKeyboardLayout(0);
				SwitchKControls(m_pGame->GetKeyboardLayout());
			}
			else
			{
				m_pGame->SetControllerLayout(m_pGame->GetControllerLayout() + 1);
				if(m_pGame->GetControllerLayout() > 2)
					m_pGame->SetControllerLayout(0);
				SwitchCControls(m_pGame->GetControllerLayout());
			}
		}
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressedEx(DIK_UP))
	{
		m_nCurrSelection--;
			if(m_nCurrSelection < 0)
				m_nCurrSelection = 1;
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressedEx(DIK_DOWN))
	{
		m_nCurrSelection++;
			if(m_nCurrSelection > 1)
				m_nCurrSelection = 0;
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressedEx(DIK_LEFT))
	{
		if(0 == m_nCurrSelection)
			{
				m_pGame->SetKeyboardLayout(m_pGame->GetKeyboardLayout() - 1);
				if(m_pGame->GetKeyboardLayout() < 0)
					m_pGame->SetKeyboardLayout(2);
				SwitchKControls(m_pGame->GetKeyboardLayout());
			}
			else
			{
				m_pGame->SetControllerLayout(m_pGame->GetControllerLayout() - 1);
				if(m_pGame->GetControllerLayout() < 0)
					m_pGame->SetControllerLayout(2);
				SwitchCControls(m_pGame->GetControllerLayout());
			}

	}

	if(CSGD_DirectInput::GetInstance()->KeyPressedEx(DIK_RIGHT))
	{
		if(0 == m_nCurrSelection)
			{
				m_pGame->SetKeyboardLayout(m_pGame->GetKeyboardLayout() + 1);
				if(m_pGame->GetKeyboardLayout() > 2)
					m_pGame->SetKeyboardLayout(0);
				SwitchKControls(m_pGame->GetKeyboardLayout());
			}
			else
			{
				m_pGame->SetControllerLayout(m_pGame->GetControllerLayout() + 1);
				if(m_pGame->GetControllerLayout() > 2)
					m_pGame->SetControllerLayout(0);
				SwitchCControls(m_pGame->GetControllerLayout());
			}

	}

	if(CSGD_DirectInput::GetInstance()->KeyPressedEx(DIK_ESCAPE))
	{
		m_pGame->PopState();
	}


	return true;
}


void CKeybindState::Render()
{
	CSGD_TextureManager::GetInstance()->Draw(m_nImageID,0,0);

	if(0 == m_nCurrSelection)
	{
		switch(m_pGame->GetKeyboardLayout())
		{
		case 0:
			CBitMapFont::GetInstance()->Print("Default Control Scheme",300,200,false,true);
			break;
		case 1:
			CBitMapFont::GetInstance()->Print("Alternate Control Scheme 1",300,200,false,true);
			break;
		case 2:
			CBitMapFont::GetInstance()->Print("Alternate Control Scheme 2",300,200,false,true);
			break;
		}

	}
	else
	{
		switch(m_pGame->GetKeyboardLayout())
		{
		case 0:
			CBitMapFont::GetInstance()->Print("Default Control Scheme",300,200,false,false);
			break;
		case 1:
			CBitMapFont::GetInstance()->Print("Alternate Control Scheme 1",300,200,false,false);
			break;
		case 2:
			CBitMapFont::GetInstance()->Print("Alternate Control Scheme 2",300,200,false,false);
			break;
		}

	}

	RenderKControls(m_pGame->GetKeyboardLayout());

	if(1 == m_nCurrSelection)
	{
		switch(m_pGame->GetControllerLayout())
		{
		case 0:
			CBitMapFont::GetInstance()->Print("Default Control Scheme",300,425,false,true);
			break;
		case 1:
			CBitMapFont::GetInstance()->Print("Alternate Control Scheme 1",300,425,false,true);
			break;
		case 2:
			CBitMapFont::GetInstance()->Print("Alternate Control Scheme 2",300,425,false,true);
			break;
		}

	}
	else
	{
		switch(m_pGame->GetControllerLayout())
		{
		case 0:
			CBitMapFont::GetInstance()->Print("Default Control Scheme",300,425,false,false);
			break;
		case 1:
			CBitMapFont::GetInstance()->Print("Alternate Control Scheme 1",300,425,false,false);
			break;
		case 2:
			CBitMapFont::GetInstance()->Print("Alternate Control Scheme 2",300,425,false,false);
			break;
		}

	}

	RenderCControls(m_pGame->GetControllerLayout());

	CBitMapFont::GetInstance()->Print("Press Escape/B: Go Back",50,750,false,false,.5f);
	CBitMapFont::GetInstance()->Print("Pess Left/Right: Switch Control Schemes",650,750,false,false,.5f);


}

void CKeybindState::RenderKControls(int controlscheme)
{
	switch(controlscheme)
	{
	case 0: //default
		CBitMapFont::GetInstance()->Print("Jump: W",300,250,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Move Right: D",300,275,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Move Left: A",300,300,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Shoot Pull Well: Left Mouse Button",300,325,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Shoot Push Well: Right Mouse Button",300,350,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Close Wells: Middle Mouse Button",300,375,false,false,.8f);
		break;
	case 1: 
		CBitMapFont::GetInstance()->Print("Jump: W",300,250,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Move Right: D",300,275,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Move Left: A",300,300,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Shoot Pull Well: Left Mouse Button",300,325,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Shoot Push Well: Right Mouse Button",300,350,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Close Wells: E",300,375,false,false,.8f);
		break;
	case 2:
		CBitMapFont::GetInstance()->Print("Jump: Up Arrow",300,250,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Move Right: Right Arrow",300,275,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Move Left: Left Arrow",300,300,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Shoot Pull Well: Left Mouse Button",300,325,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Shoot Push Well: Right Mouse Button",300,350,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Close Wells: Middle Mouse Button",300,375,false,false,.8f);
		break;
	}
}

void CKeybindState::SwitchKControls(int controlscheme)
{
	if(m_pGame == NULL)
		m_pGame = CGame::GetInstance();

	switch(controlscheme)
	{
	case 0:
		m_pGame->SetKMoveLeft(DIK_A);
		m_pGame->SetKMoveRight(DIK_D);
		m_pGame->SetKJump(DIK_W);
		m_pGame->SetKShootPull(0);
		m_pGame->SetKShootPush(1);
		m_pGame->SetKCloseAll(2);
		break;

	case 1:
		m_pGame->SetKMoveLeft(DIK_A);
		m_pGame->SetKMoveRight(DIK_D);
		m_pGame->SetKJump(DIK_W);
		m_pGame->SetKShootPull(0);
		m_pGame->SetKShootPush(1);
		m_pGame->SetKCloseAll(DIK_E);
		break;

	case 2:
		m_pGame->SetKMoveLeft(DIK_LEFT);
		m_pGame->SetKMoveRight(DIK_RIGHT);
		m_pGame->SetKJump(DIK_UP);
		m_pGame->SetKShootPull(0);
		m_pGame->SetKShootPush(1);
		m_pGame->SetKCloseAll(2);
		break;
	}

	m_pGame->GetSaveFile()->GetSaveData()->nControlSchemes[1] = controlscheme;
	
}

void CKeybindState::RenderCControls(int controlscheme)
{
	switch(controlscheme)
	{
	case 0: //default
		CBitMapFont::GetInstance()->Print("Jump: Left Thumbstick Up",300,475,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Move Right: Left Thumbstick Right",300,500,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Move Left: Left Thumbstick Left",300,525,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Shoot Pull Well: Right Trigger",300,550,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Shoot Push Well: Left Trigger",300,575,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Close Wells: Right Bumper",300,600,false,false,.8f);
		break;
	case 1:
		CBitMapFont::GetInstance()->Print("Jump: Dpad Up",300,475,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Move Right: Dpad Right",300,500,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Move Left: Dpad Left",300,525,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Shoot Pull Well: B",300,550,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Shoot Push Well: Y",300,575,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Close Wells: Left Bumper",300,600,false,false,.8f);
		break;
	case 2:
		CBitMapFont::GetInstance()->Print("Jump: Left Bumper",300,475,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Move Right: Left Thumbstick Right",300,500,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Move Left: Left Thumbstick Left",300,525,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Shoot Pull Well: Right Trigger",300,550,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Shoot Push Well: Left Trigger",300,575,false,false,.8f);
		CBitMapFont::GetInstance()->Print("Close Wells: Y",300,600,false,false,.8f);
		break;
	}


}

void CKeybindState::SwitchCControls(int controlscheme)
{

	if(m_pGame == NULL)
		m_pGame = CGame::GetInstance();
	switch(controlscheme)
	{
	case 0:
		m_pGame->SetCMoveLeft(VK_PAD_LTHUMB_LEFT);
		m_pGame->SetCMoveRight(VK_PAD_LTHUMB_RIGHT);
		m_pGame->SetCJump(VK_PAD_LTHUMB_UP);
		m_pGame->SetCShootPull(VK_PAD_RTRIGGER);
		m_pGame->SetCShootPush(VK_PAD_LTRIGGER);
		m_pGame->SetCCloseAll(VK_PAD_RSHOULDER);
		break;

	case 1:
		m_pGame->SetCMoveLeft(VK_PAD_DPAD_LEFT);
		m_pGame->SetCMoveRight(VK_PAD_DPAD_RIGHT);
		m_pGame->SetCJump(VK_PAD_DPAD_UP);
		m_pGame->SetCShootPull(VK_PAD_B);
		m_pGame->SetCShootPush(VK_PAD_Y);
		m_pGame->SetCCloseAll(VK_PAD_LSHOULDER);
		break;

	case 2:
		m_pGame->SetCMoveLeft(VK_PAD_LTHUMB_LEFT);
		m_pGame->SetCMoveRight(VK_PAD_LTHUMB_RIGHT);
		m_pGame->SetCJump(VK_PAD_LSHOULDER);
		m_pGame->SetCShootPull(VK_PAD_RTRIGGER);
		m_pGame->SetCShootPush(VK_PAD_LTRIGGER);
		m_pGame->SetCCloseAll(VK_PAD_Y);
		break;
	}

	m_pGame->GetSaveFile()->GetSaveData()->nControlSchemes[0] = controlscheme;

}



void  CKeybindState::Update(float fDT)
{

}