#include "CIntroState.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "CBitMapFont.h"
#include "CMainMenuState.h"
#include "CAssetState.h"
#include "CGame.h"

#define THRESHOLD 2.0f

CIntroState::CIntroState()
{
	m_pAS = CAssetState::GetInstance();

	for(int i = 0; i < MAXSTATES; i++)
		m_fTimers[i] = 0.0f;

	m_nAlphaCBB = 0;
	m_nAlphaGP = 0;
	m_nCurState = 0;
}

CIntroState::~CIntroState()
{
}

CIntroState* CIntroState::GetInstance()
{
	static CIntroState instance;
	return &instance;
}

void CIntroState::Enter()
{
}

bool CIntroState::Input()
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
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_A || key.VirtualKey == VK_PAD_B ||
				key.VirtualKey == VK_PAD_X || key.VirtualKey == VK_PAD_Y || key.VirtualKey == VK_PAD_START || key.VirtualKey == VK_PAD_BACK))
				CGame::GetInstance()->PushState(CMainMenuState::GetInstance());
	}

	if(CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx())
			CGame::GetInstance()->PushState(CMainMenuState::GetInstance());

	return true;
}

void CIntroState::Update(float fDT)
{
	if(m_nCurState == MAXSTATES) return;
	m_fTimers[m_nCurState] += fDT;

	if((m_nCurState >= 3 && m_nCurState <= 6) || (m_nCurState >= 9 && m_nCurState <= 11))
	{
		if(m_fTimers[m_nCurState] >= 1.0f)
			m_nCurState++;
	}
	else if( m_nCurState >= 7 && m_nCurState <= 8)
	{
		if(m_fTimers[m_nCurState] >= 0.2)
			m_nCurState++;
	}
	else
		if(m_fTimers[m_nCurState] >= THRESHOLD)
			m_nCurState++;

	switch(m_nCurState)
	{
	case 0:
		m_nAlphaCBB = int(255.0f * m_fTimers[m_nCurState]/THRESHOLD);
		break;
	case 1:
		m_nAlphaCBB = 255;
		break;
	case 2:
		m_nAlphaCBB = int(255.0f * (THRESHOLD-m_fTimers[m_nCurState])/THRESHOLD);
		break;
	case 12:
		m_nAlphaGP = int(255.0f * m_fTimers[m_nCurState]/THRESHOLD);
		break;
	case 13:
		m_nAlphaGP = 255;
		break;
	}
}

void CIntroState::Render()
{
	if(m_nCurState <= 2)
		CSGD_TextureManager::GetInstance()->Draw(m_pAS->GetImageID("IntroCBB"), 0, 0, 1.0f, 1.0f, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaCBB, 255, 255, 255));

	if(m_nCurState == 1 || m_nCurState == 2)
		CBitMapFont::GetInstance()->Print("Presents", 512, 675, true, false, 1.4f);

	if(m_nCurState >= 12)
		CSGD_TextureManager::GetInstance()->Draw(m_pAS->GetImageID("IntroGP"), 0, 0, 1.0f, 1.0f, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlphaGP, 255, 255, 255));

	if(m_nCurState >= 13)
		CBitMapFont::GetInstance()->Print("Press any key to continue", 512, 650, true, false, 1.4f, D3DCOLOR_ARGB(255, 0, 160, 240));

	if(m_nCurState >= 3 && m_nCurState <= 11)
		CSGD_TextureManager::GetInstance()->Draw(m_pAS->GetImageID("IntroSS", m_nCurState-2), 0, 0);
}

void CIntroState::Exit()
{
}