
#include "CAttractState.h"
#include "CGame.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "CBitMapFont.h"
#include "CAssetState.h"



CAttractState::CAttractState()
{
	m_fAttractTimer = 0.0f;
	m_nImageID = -1;
}

CAttractState::~CAttractState()
{

}

void CAttractState::Enter()
{
	m_fAttractTimer = 0.0f;
	m_pGame = CGame::GetInstance();
	m_nImageID = CAssetState::GetInstance()->GetImageID("AttractCurrency");


}

void CAttractState::Exit()
{
	m_fAttractTimer = 0.0f;
	m_nImageID = -1;

}

void CAttractState::Update(float fDT)
{
	m_fAttractTimer += fDT;

	if(m_fAttractTimer > 8.0f && m_fAttractTimer < 16.0f)
	{
		m_nImageID = CAssetState::GetInstance()->GetImageID("AttractEnemies");
	}
	else if(m_fAttractTimer > 16.0f && m_fAttractTimer < 24.0f)
	{
		m_nImageID = CAssetState::GetInstance()->GetImageID("AttractObstacles");
	}
	else if(m_fAttractTimer > 24.0f && m_fAttractTimer < 32.0f)
	{
		m_nImageID = CAssetState::GetInstance()->GetImageID("AttractFinish");
	}
	else if(m_fAttractTimer > 33.0f)
	{
		m_pGame->PopState();
	}

}

bool CAttractState::Input()
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
				m_pGame->PopState();
	}

	if(CSGD_DirectInput::GetInstance()->CheckKeys())
	{
		m_pGame->PopState();
		return true;
	}
	return true;
}

void CAttractState::Render()
{
	CSGD_TextureManager::GetInstance()->Draw(m_nImageID,0,0);

	if(m_fAttractTimer > 1.0f && m_fAttractTimer < 8.0f)
	{
		//Draw Some Bitmap Font Here
		CBitMapFont::GetInstance()->Print("Collect Sprockets to upgrade your gravity gun \n and become unstoppable",170,600,false,false,.8f);
	}
	else if(m_fAttractTimer > 8.0f && m_fAttractTimer < 16.0f)
	{
		CBitMapFont::GetInstance()->Print("Avoid or destroy enemies \n using your super gravity powers",200,530,false,false,.8f);
	}
	else if(m_fAttractTimer > 16.0f && m_fAttractTimer < 24.0f)
	{
		CBitMapFont::GetInstance()->Print("Use gravity to avoid dangerous obstacles",200,180,false,false,.8f);
	}
	else if(m_fAttractTimer > 24.0f && m_fAttractTimer < 32.0f)
	{
		CBitMapFont::GetInstance()->Print("Complete Levels in record time \n and attract all the women and cash ever.",200,230,false,false,.8f);
	}
	//Make some if else statements here
	


}

CAttractState* CAttractState::GetInstance()
{
	static CAttractState instance;
	return &instance;
}



