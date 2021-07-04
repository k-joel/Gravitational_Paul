#include "CHUD.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include "CGame.h"
#include "CGamePlayState.h"
#include "CBitMapFont.h"
#include "CAssetState.h"

CHUD::CHUD()
{
	//Initializing the ID for the picture to -1 (meaning it doesn't exist yet)
	m_nHUDPictureID = -1;
	m_nLifeIconID = -1;

	//Getting the instance of the texture manager
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pGPS = CGamePlayState::GetInstance();
	m_pFont = CBitMapFont::GetInstance();
	m_nHUDPictureID = CAssetState::GetInstance()->GetImageID("HUDImageID");
	m_nLifeIconID = CAssetState::GetInstance()->GetImageID("LifeIconID");
	m_nBarImageID = CAssetState::GetInstance()->GetImageID("BarImageID");
	m_nFillerImageID = CAssetState::GetInstance()->GetImageID("FillerImageID");
	m_fElapsedTime = 0.0f;
	m_tBarFillRect.top = 0;
	m_tBarFillRect.left = 0;
	m_tBarFillRect.right = 192;
	m_tBarFillRect.bottom = 18;
	m_lBarColor = D3DCOLOR_XRGB(255, 255, 255);
	m_tPowerUpRect.top = 0;
	m_tPowerUpRect.left = 0;
	m_tPowerUpRect.right = 192;
	m_tPowerUpRect.bottom = 18;
	m_lPowerUpColor = D3DCOLOR_XRGB(255, 123, 0);
	m_nTimer = 0;
	m_fPowerUpTime = 0.0f;
	m_bPowerUpOn = false;
	m_nUnspentCurrency = 0;
	m_lTimerColor = 0;
}
CHUD::~CHUD()
{
}

CHUD* CHUD::GetInstance()
{
	static CHUD pInstance;
	return &pInstance;
}

void CHUD::Render()
{
	m_nHUDUpperBound = CGame::GetInstance()->GetWindowHeight() - 112;
	m_nHUDLowerBound = CGame::GetInstance()->GetWindowHeight() - 32;

	////Drawing the background image of the HUD
	//if(CGamePlayState::GetInstance()->GetHasHitBuffer())
	//	m_pTM->Draw(m_nHUDPictureID, 32, m_nHUDUpperBound, 7.5f, 1.25f);
	//else
	//	m_pTM->Draw(m_nHUDPictureID, 32, m_nHUDUpperBound, 7.5f, 1.25f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));

	////Drawing the life icon on the HUD
	//m_pTM->Draw(m_nLifeIconID, CGame::GetInstance()->GetWindowWidth() / 2 - 200, (m_nHUDUpperBound + (m_nHUDLowerBound - m_nHUDUpperBound) / 2) - 28);

	//m_pD3D->GetSprite()->Flush();

	////First, drawing where the energy bar is going to be
	////m_pD3D->DrawTextA("Insert Energy Bar Here", 20, (m_nHUDUpperBound + (m_nHUDLowerBound - m_nHUDUpperBound) / 2) - 10);

	//m_pTM->Draw(m_nBarImageID, 52, (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) - 10 ), 1.0f, .80f);
	//m_pTM->Draw(m_nFillerImageID, 106, (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) - 3 ), 1.0f, .80f, &m_tBarFillRect, 0.0f, 0.0f, 0.0f, m_lBarColor);
	//m_pFont->Print("E", 80,  (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) - 6 ), true, false, .75f, m_lBarColor);
	////Drawing the active gun power up (either weighted shot or the other one I forget.
	////m_pD3D->DrawTextA("POWERUP INFO", 20, );
	////m_pFont->Print("POWERUP INFO", 20, (m_nHUDUpperBound + (m_nHUDLowerBound - m_nHUDUpperBound) / 2) + 10);
	//if(m_bPowerUpOn)
	//{
	//	m_pTM->Draw(m_nBarImageID, 52, (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) + 30 ), 1.0f, .80f);

	//	m_pTM->Draw(m_nFillerImageID, 106, (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) + 37 ), 1.0f, .80f, &m_tPowerUpRect, 0.0f, 0.0f, 0.0f, m_lPowerUpColor);

	//	if(CGamePlayState::GetInstance()->GetIsBounce() == true)
	//	{
	//		m_pFont->Print("B", 80,  (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) + 30 ), true, false, .75f, m_lPowerUpColor);
	//	}
	//	if(CGamePlayState::GetInstance()->GetIsWeighted() == true)
	//	{
	//		m_pFont->Print("W", 80,  (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) + 30 ), true, false, .75f, m_lPowerUpColor);
	//	}
	//}

	////Drawing the life count
	//char buffer[32];
	//sprintf_s(buffer, " x %d", CGamePlayState::GetInstance()->GetLives());
	////m_pD3D->DrawTextA(buffer, CGame::GetInstance()->GetWindowWidth() / 2, m_nHUDUpperBound + (m_nHUDLowerBound - m_nHUDUpperBound) / 2);
	//m_pFont->Print(buffer, (CGame::GetInstance()->GetWindowWidth() / 2) - 168, m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 2 - 32));

	////Gem count, used for like....  upgrades?  Maybe?  I forget.  They're under the timer.
	//int currCurrency = CGame::GetInstance()->GetSaveFile()->CountLevelCurrency(CGame::GetInstance()->GetLoadLevel() - 1, CGame::GetInstance()->GetCurrentSaveSlot());
	//sprintf_s(buffer, "Sprockets");
	////m_pD3D->DrawTextA(buffer, CGame::GetInstance()->GetWindowWidth() - 80, (m_nHUDUpperBound + (m_nHUDLowerBound - m_nHUDUpperBound) / 2) + 10);
	//m_pFont->Print(buffer, CGame::GetInstance()->GetWindowWidth() - 280, (m_nHUDUpperBound + (m_nHUDLowerBound - m_nHUDUpperBound) / 2) -30, false, false, 1.0f, D3DCOLOR_XRGB(254, 255, 255) );

	//sprintf_s(buffer, "Stage : %i / 15", currCurrency);
	//m_pFont->Print(buffer, CGame::GetInstance()->GetWindowWidth() - 280, (m_nHUDUpperBound + (m_nHUDLowerBound - m_nHUDUpperBound) / 2), false, false, .50f , D3DCOLOR_XRGB(254, 255, 255) );

	//sprintf_s(buffer, "Total Sprockets: %i", m_nUnspentCurrency);
	//m_pFont->Print(buffer, CGame::GetInstance()->GetWindowWidth() - 280 , (m_nHUDUpperBound + (m_nHUDLowerBound - m_nHUDUpperBound) / 2) + 20,  false, false, .50f, D3DCOLOR_XRGB(254, 255, 255));

	////Drawing the timer on the far right
	//m_nTimer = (int)CGamePlayState::GetInstance()->GetCurrentLevelTime();
	//sprintf_s(buffer, "%d", m_nTimer);
	////m_pD3D->DrawTextA(buffer, CGame::GetInstance()->GetWindowWidth() - 40,(m_nHUDUpperBound + (m_nHUDLowerBound - m_nHUDUpperBound) / 2) - 10);
	//m_pFont->Print(buffer, (CGame::GetInstance()->GetWindowWidth() / 2) + 20,(m_nHUDUpperBound + (m_nHUDLowerBound - m_nHUDUpperBound) / 2) - 5, true, false, 1.75, m_lTimerColor );

	//m_pFont->Print("Time Remaining", (CGame::GetInstance()->GetWindowWidth() / 2) + 20, (m_nHUDUpperBound + (m_nHUDLowerBound - m_nHUDUpperBound) / 2) - 30, true, false, .75f, D3DCOLOR_XRGB(254, 255, 255));


	//gotta figure out how to do the hit buffer feedback
	/*if(CGamePlayState::GetInstance()->GetHasHitBuffer())
		m_pTM->Draw(m_nHUDPictureID, 32, m_nHUDUpperBound, 7.5f, 1.25f);
	else
		m_pTM->Draw(m_nHUDPictureID, 32, m_nHUDUpperBound, 7.5f, 1.25f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));*/


	//////////////////////////////////////////////////////////////////////////
	m_pFont->SetCurFont(FONT_B);

	int nIconRightOffset = 200;
	int nTextRightOffset = 160;
	int nLifeYOffset = 70;
	int nSprYOffset = 30;
	//LIFE
	m_pTM->Draw(CAssetState::GetInstance()->GetImageID("LifeID"), CGame::GetInstance()->GetWindowWidth() - nIconRightOffset, m_nHUDLowerBound - nLifeYOffset);
	
	char buffer[32];
	sprintf_s(buffer, "x %d", CGamePlayState::GetInstance()->GetLives());
	m_pFont->Print(buffer, CGame::GetInstance()->GetWindowWidth() - nTextRightOffset, m_nHUDLowerBound - nLifeYOffset);


	//SPROKETS
	m_pTM->Draw(CAssetState::GetInstance()->GetImageID("CurrencyImageID"), CGame::GetInstance()->GetWindowWidth() - nIconRightOffset, m_nHUDLowerBound - nSprYOffset);

	sprintf_s(buffer, "x %i", m_nUnspentCurrency);
	m_pFont->Print(buffer, CGame::GetInstance()->GetWindowWidth() - nTextRightOffset , m_nHUDLowerBound - nSprYOffset);

	m_pFont->SetCurFont(FONT_A);

	// CUR STAGE
	int nCurStage = CGame::GetInstance()->GetLoadLevel();
	if(CGame::GetInstance()->GetLoadLevel() < 30)
		sprintf_s(buffer, "Stage : %i / 20", nCurStage);
	else
		sprintf_s(buffer, "Tutorial Level");

	m_pFont->Print(buffer, (CGame::GetInstance()->GetWindowWidth() / 2) - 10, m_nHUDLowerBound - 70, true, false, .50f );

	// TIMER
	m_pTM->Draw(CAssetState::GetInstance()->GetImageID("HourglassID"), (CGame::GetInstance()->GetWindowWidth() / 2) - 100, m_nHUDLowerBound - 40);

	m_nTimer = (int)CGamePlayState::GetInstance()->GetCurrentLevelTime();
	sprintf_s(buffer, "%d", m_nTimer);
	m_pFont->Print(buffer, (CGame::GetInstance()->GetWindowWidth() / 2), m_nHUDLowerBound - 40, true, false, 1.5f, m_lTimerColor );


	//////////////////////////////////////////////////////////////////////////
	// BARS
	//////////////////////////////////////////////////////////////////////////
	int nBarOffset = 100;
	int nFillerOffset = 154;
	int nIconOffset = 58;
	// ENERGY
	m_pTM->Draw(CAssetState::GetInstance()->GetImageID("EnergyHUDID"), nIconOffset, (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) - 10 ));
	m_pTM->Draw(m_nBarImageID, nBarOffset, (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) - 10 ), 1.0f, 1.0f);
	m_pTM->Draw(m_nFillerImageID, nFillerOffset, (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) - 3 ), 1.0f, 1.0f, &m_tBarFillRect, 0.0f, 0.0f, 0.0f, m_lBarColor);

	// POWERUPS
	if(m_bPowerUpOn)
	{
		m_pTM->Draw(m_nBarImageID, nBarOffset, (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) + 30 ), 1.0f, 1.0f);

		m_pTM->Draw(m_nFillerImageID, nFillerOffset, (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) + 37 ), 1.0f, 1.0f, &m_tPowerUpRect, 0.0f, 0.0f, 0.0f, m_lPowerUpColor);

		if(CGamePlayState::GetInstance()->GetIsBounce() == true)
			m_pTM->Draw(CAssetState::GetInstance()->GetImageID("BounceHUDID"), nIconOffset, (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) + 30 ));
		if(CGamePlayState::GetInstance()->GetIsWeighted() == true)
			m_pTM->Draw(CAssetState::GetInstance()->GetImageID("WeightedHUDID"), nIconOffset, (m_nHUDUpperBound + ((m_nHUDLowerBound - m_nHUDUpperBound) / 4) + 30 ));
	}

	//////////////////////////////////////////////////////////////////////////
}

void CHUD::Update(float fElapsedTime)
{
	m_fElapsedTime += fElapsedTime;

	//Every time the bucket gets to 10, I subtract a second off the timer.
	//Why 10? I have no idea.  Somewhere Justin already did some sort of conversion on the
	//time stamp.
	if(m_fElapsedTime >= 1)
	{
		--m_nTimer;
		m_fElapsedTime = 0.0f;
	}
	if(m_nTimer > 150)
	{
		m_lTimerColor = D3DCOLOR_XRGB(0, 255, 0);
	}
	else if(m_nTimer > 60)
	{
		m_lTimerColor = D3DCOLOR_XRGB(255, 255, 0);
	}
	else
	{
		m_lTimerColor = D3DCOLOR_XRGB(255, 0, 0);
	}

	m_tBarFillRect.right = (int)( 192.0f * (CGamePlayState::GetInstance()->GetCurrentEnergy() / 300.0f));

	if( m_tBarFillRect.right >= 128)
	{
		m_lBarColor = D3DCOLOR_XRGB(0, 255, 0);
	}
	else if(m_tBarFillRect.right >= 64)
	{
		m_lBarColor = D3DCOLOR_XRGB(255, 255, 0);
	}
	else
	{
		m_lBarColor = D3DCOLOR_XRGB(255, 0, 0);
	}
	if(CGamePlayState::GetInstance()->GetIsBounce() == true)
	{
		m_lPowerUpColor = D3DCOLOR_XRGB(255, 123, 0);
	}
	if(CGamePlayState::GetInstance()->GetIsWeighted() == true)
	{
		m_lPowerUpColor = D3DCOLOR_XRGB(123, 0, 123);
	}

	m_fPowerUpTime = CGamePlayState::GetInstance()->GetPowerUpTimer();
	m_tPowerUpRect.right = (int)(192.0f * (m_fPowerUpTime / 10.0f));
	if(m_fPowerUpTime > 0.0f)
		m_bPowerUpOn = true;
	else
		m_bPowerUpOn = false;
	m_nUnspentCurrency = CGame::GetInstance()->GetTotalCurrency();

	if(CGamePlayState::GetInstance()->GetCurrentLevelTime() <= 30 && CGamePlayState::GetInstance()->GetCurrentLevelTime() >= 28 )
	{
		if(CAssetState::GetInstance()->IsSoundPlaying("WarningSoundID") == false) 
			CAssetState::GetInstance()->PlaySoundA("WarningSoundID"); 
	}
	else if(CGamePlayState::GetInstance()->GetCurrentLevelTime() <= 10 && CGamePlayState::GetInstance()->GetCurrentLevelTime() >= 8)
	{
		if(CAssetState::GetInstance()->IsSoundPlaying("WarningSoundID") == false) 
			CAssetState::GetInstance()->PlaySoundA("WarningSoundID"); 
	}
}

void CHUD::StopWarning(void)
{
		if(CAssetState::GetInstance()->IsSoundPlaying("WarningSoundID"))
			CAssetState::GetInstance()->StopSound("WarningSoundID"); 
			
}