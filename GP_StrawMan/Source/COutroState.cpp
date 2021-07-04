#include "COutroState.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "CMainMenuState.h"
#include "CAssetState.h"
#include "CParticleManager.h"
#include "CGame.h"
#include "CCredits.h"

COutroState::COutroState()
{
	m_nCurState = 0;
	m_fTimer = 0.0f;
}

COutroState::~COutroState()
{
}

COutroState* COutroState::GetInstance()
{
	static COutroState instance;
	return &instance;
}

void COutroState::Enter()
{
	CParticleManager::GetInstance()->SpawnParticle("Default", 512, 50);
}

bool COutroState::Input()
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
		{
			m_nCurState++;
			m_fTimer = 0.0f;
			if(m_nCurState >= 1)
			{
				CParticleManager::GetInstance()->KillParticle("Default");
			}

			if(m_nCurState >= 2)
			{
				CGame::GetInstance()->PopState();
				CGame::GetInstance()->PushState(CCredits::GetInstance());
			}
		}
	}

	if(CSGD_DirectInput::GetInstance()->CheckBufferedKeysEx())
	{
		m_nCurState++;
		m_fTimer = 0.0f;
		if(m_nCurState >= 1)
		{
			CParticleManager::GetInstance()->KillParticle("Default");
		}

		if(m_nCurState >= 2)
		{
			CGame::GetInstance()->PopState();
			CGame::GetInstance()->PushState(CCredits::GetInstance());
		}
	}

	return true;
}

void COutroState::Update(float fDT)
{
	CParticleManager::GetInstance()->UpdateParticles(fDT);
	m_fTimer += fDT;
	if(m_fTimer >= 10.0f)
		m_nCurState++;
}

void COutroState::Render()
{
	if(m_nCurState == 0)
		CSGD_TextureManager::GetInstance()->Draw(CAssetState::GetInstance()->GetImageID("Outro"), 0, 0);

	if(m_nCurState >= 1)
		CSGD_TextureManager::GetInstance()->Draw(CAssetState::GetInstance()->GetImageID("OutroGO"), 0, 0);

	CParticleManager::GetInstance()->RenderParticles();
}

void COutroState::Exit()
{
}