#include "CCheatState.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "CBitMapFont.h"
#include "CGame.h"
#include "CAssetState.h"

CCheatState::CCheatState()
{
	m_bInvincible = false;
	m_bLevels = false; 
	m_bGuns = false; 
	m_nSpot = 0; 
	
}

CCheatState::~CCheatState()
{

}

void CCheatState::Enter()
{
	m_cGuns = "poweroverwhelming"; 
	m_cCheat = "zzzzzzzzzzzzzzzzz";

	m_cInvincible = "getoffhim"; 
	m_cCheat1 = "zzzzzzzzz"; 

	m_cLevels = "igiveup"; 
	m_cCheat2 = "zzzzzzz"; 

	m_bInvincible = CGame::GetInstance()->GetCollisionUnlock();
	m_bLevels = CGame::GetInstance()->GetLevelUnlock(); 
	m_bGuns = CGame::GetInstance()->GetGunCheat(); 
	m_nSpot = 0; 
	m_nSpot1 = 0; 
	m_nSpot2 = 0; 

}

void CCheatState::Update(float fDT)
{
}

bool CCheatState::Input()
{
	CSGD_DirectInput* m_pDI = CSGD_DirectInput::GetInstance(); 

	if(m_pDI->KeyPressed(DIK_ESCAPE))
		CGame::GetInstance()->PopState(); 

	if(m_pDI->CheckBufferedKeysEx() > 0)
	{
		m_cCheat[m_nSpot] = m_pDI->CheckBufferedKeysEx(); 
		m_cCheat1[m_nSpot1] = m_cCheat[m_nSpot]; 
		m_cCheat2[m_nSpot2] = m_cCheat[m_nSpot]; 
		
		if(m_cCheat[0] == m_cGuns[0])
		{
			int tempSpot = m_nSpot;
			m_nSpot = 1; 
			while(m_nSpot < 17)
			{
				if(m_cCheat[m_nSpot] == m_cGuns[m_nSpot])
					m_nSpot++; 
				else 
				{
					if(m_nSpot > tempSpot)
					{
					
					}
					else 
					{
						m_nSpot = 0; 
						m_cCheat = "zzzzzzzzzzzzzzzzz"; 
					} 
					break; 
				}

				if(m_nSpot == 17)
				{
					m_nSpot = 0; 
					m_cCheat = "zzzzzzzzzzzzzzzzz"; 
					m_bGuns = !m_bGuns; 
					CGame::GetInstance()->SetGunCheat(m_bGuns); 
				}
			}	
		}

		if(m_cCheat1[0] == m_cInvincible[0])
		{
			int tempSpot = m_nSpot1;
			m_nSpot1 = 1; 
			while(m_nSpot1 < 9)
			{
				if(m_cCheat1[m_nSpot1] == m_cInvincible[m_nSpot1])
					m_nSpot1++; 
				else 
				{
					if(m_nSpot1 > tempSpot)
					{
					
					}
					else 
					{
						m_nSpot1 = 0; 
						m_cCheat1 = "zzzzzzzzz"; 
					} 
					break; 
				}

				if(m_nSpot1 == 9)
				{
					m_bInvincible = !m_bInvincible;
					m_nSpot1 = 0; 
					m_cCheat1 = "zzzzzzzzz"; 
					CGame::GetInstance()->SetCollisionUnlock(m_bInvincible); 

				}
			}		
		}

		if(m_cCheat2[0] == m_cLevels[0])
		{
			int tempSpot = m_nSpot2;
			m_nSpot2 = 1; 
			while(m_nSpot2 < 7)
			{
				if(m_cCheat2[m_nSpot2] == m_cLevels[m_nSpot2])
					m_nSpot2++; 
				else 
				{
					if(m_nSpot2 > tempSpot)
					{
					
					}
					else 
					{
						m_nSpot2 = 0; 
						m_cCheat2 = "zzzzzzz"; 
					}
					break; 
				}

				if(m_nSpot2 == 7)
				{
					m_bLevels = !m_bLevels;
					m_nSpot2 = 0; 
					m_cCheat2 = "zzzzzzz"; 
					CGame::GetInstance()->SetLevelUnlock(m_bLevels); 
				}
			}		
		}
	}


	return true; 
}

void CCheatState::Render()
{
	CSGD_TextureManager::GetInstance()->Draw(CAssetState::GetInstance()->GetImageID("HowToPlayID"),0,0); 
	
	//CSGD_TextureManager::GetInstance()->Draw(CSGD_TextureManager::GetInstance()->LoadTexture("Resource/Graphics/CbB_keyboard.png"),CGame::GetInstance()->GetWindowWidth() / 2, CGame::GetInstance()->GetWindowHeight() / 2); 

	if(m_bGuns)
		CBitMapFont::GetInstance()->Print("Guns",CGame::GetInstance()->GetWindowWidth() / 2, CGame::GetInstance()->GetWindowHeight() / 2); 

	if(m_bLevels)
		CBitMapFont::GetInstance()->Print("Levels",CGame::GetInstance()->GetWindowWidth() / 2, CGame::GetInstance()->GetWindowHeight() / 2 + 50); 

	if(m_bInvincible)
		CBitMapFont::GetInstance()->Print("Invincible",CGame::GetInstance()->GetWindowWidth() / 2, CGame::GetInstance()->GetWindowHeight() / 2 + 100); 

	CBitMapFont::GetInstance()->Print("Enter Cheats",20, 20); 

	
	//CBitMapFont::GetInstance()->Print("Hint:",CGame::GetInstance()->GetWindowWidth() - 100, 20); 

}

void CCheatState::Exit()
{

}


CCheatState* CCheatState::GetInstance()
{
	static CCheatState instance; 
	return &instance; 
}