#include "CLevelSelect.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_FModManager.h"
#include "CGame.h"
#include "CGamePlayState.h"
#include "CMainMenuState.h"
#include "CTileManager.h"
#include "CBitMapFont.h"
#include "CCustomLevelState.h"
#include "CUpgradeState.h"
#include "CAssetState.h"

CLevelSelect::CLevelSelect()
{
	m_pD3D = NULL;
	m_pTM = NULL;
	m_pGame = NULL;
	m_pDI = NULL;
	m_pCustom = NULL;
	m_nSelection = 0;
	m_nAlpha = 0;
	m_nBeta = 0;
	//Stages = NULL;
	m_nPcount = 0;
	m_nBGid = 0;
	m_pTimes = NULL;
	m_pUnlocks = NULL;
	m_nScreenShots[0] = 0;
	m_nScreenShots[1] = 0;
	m_nScreenShots[2] = 0;
	m_nScreenShots[3] = 0;
	m_nScreenShots[4] = 0;
}

CLevelSelect::~CLevelSelect()
{
}

CLevelSelect* CLevelSelect::GetInstance()
{
	static CLevelSelect instance;
	return &instance;
}

CLevelSelect::CLevelSelect(const CLevelSelect &)
{
}

void CLevelSelect::Enter()
{
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pGame = CGame::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pCustom = CCustomLevelState::GetInstance();
	m_nBeta = 100;
	m_nAlpha = 255;
	m_nSelection = 0;
	//Stages = new int[10];
	m_pTimes = CGame::GetInstance()->GetLevelTimes();
	m_pUnlocks = CGame::GetInstance()->GetCurrStageCompletes();

	Stages[0] = 1;
	Stages[1] = 2;
	Stages[2] = 3;
	Stages[3] = 4;
	Stages[4] = 5;
	Stages[5] = 6;
	Stages[6] = 7;
	Stages[7] = 8;
	Stages[8] = 9;
	Stages[9] = 10;

	m_nPcount = 0;
	m_nBGid = CAssetState::GetInstance()->GetImageID("LevelSelectBG");

	if(!CSGD_FModManager::GetInstance()->IsSoundPlaying(CMainMenuState::GetInstance()->GetTitleMusicID()))
	{
		//CSGD_FModManager::GetInstance()->PlaySoundA(CMainMenuState::GetInstance()->GetTitleMusicID());
		CAssetState::GetInstance()->PlaySound(CMainMenuState::GetInstance()->GetTitleMusicID(), SND_MUSIC);
	}
	CGame* pGame = CGame::GetInstance();
	pGame->SyncWithFile( pGame->GetCurrentSaveSlot() );
	char buffer[64];
	for(int i = 0; i < 20; i++)
	{
		sprintf_s(buffer,"Resource/Graphics/CbB_level%ddemo.png",i+1);
		m_nScreenShots[i] = m_pTM->LoadTexture(buffer);
	}


	if(CAssetState::GetInstance()->IsSoundPlaying("WarningSoundID"))
			CAssetState::GetInstance()->StopSound("WarningSoundID"); 

	for(int i = 0; i < 20; i++)
		m_sGemCount[i] = pGame->GetGemsPerLevel(i);

	for(int i = 0; i < 20; i++)
	{
			m_sPlayer[i].gemCount = 0;
		for(int j = 0; j < 15; j++)
		{
			m_sPlayer[i].gemCollected[j] = false;
			if(m_sGemCount[i].gems[j])
			{
				m_sPlayer[i].gemCollected[j] = true;
				m_sPlayer[i].gemCount++;
			}
		}
	}
}
/*
	Comment Block for TJ and anyone else messing around in this here Render() function

*/
void CLevelSelect::Render()
{
	m_pTM->Draw(m_nBGid, 0, 0);

	CBitMapFont::GetInstance()->Print("Level Selection", 20, 20, false, false, 1);

		//CBitMapFont::GetInstance()->Print("Level Selector", m_pGame->GetWindowWidth() / 3 * 1, (m_pGame->GetWindowHeight() / 5 * 1) - 200);

		//Color for alpha value there is an m_nAlpha variable set to 255, and m_nBeta set to like 100 or lower
		int color = m_nAlpha;

		//Big char buffer to handle all the sprintf_s calls
		char buffer[100];

		//Stages[] is the array for each stage number You dont need this check for yours TJ
		//Becuase nothing should ever be alphed out because everything should be able to be accessed
		//m_pUnlocks is my array to see if a level has been completed
		//and the m_nPcount is the page count to see if you're on page one or page two
		//and the minus one gets you the proper index since my Stage[0] is set to one
		if( Stages[0] + m_nPcount != 1 && m_pUnlocks[Stages[0] + m_nPcount - 1] == false)
		{
			color = m_nBeta;
			//Times is my obviously my times [] and the algorithm inside it is to get the correct stage time
			//to correspond with its stage
			//and the Player [] is a struct that holds and 15 bools and a gemcount
			//the 15 bools determines the value of gemcount depending if true or not
			sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[0] + m_nPcount - 1]);
			CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2) - 25, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
			sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[0] + m_nPcount - 1].gemCount, 15);
			CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2), false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		}
		else
		{
			sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[0] + m_nPcount - 1]);
			CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2) - 25, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
			sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[0] + m_nPcount - 1].gemCount, 15);
			CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2), false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		}

		sprintf_s(buffer, "Stage %d", Stages[0] + m_nPcount);
		//This code above just gets the level and the page count and adds it to give
		//me the correct stage number since i only show 10 stages per page
		//Pcount is only going to be 0 or 10
		//You shouldnt need this if check below

	if(m_nSelection == 0)
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2) - 200, false, false, 1, D3DCOLOR_ARGB(color,255,0,0));
	else
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2) - 200, false, false, 1, D3DCOLOR_ARGB(color,255,255,255));

	 //ScreenShots is my [] that holds all my level ids and the check beneath makes sure that the
	//index for the ScreenShots doesnt go over 5 because we only have five levels at the time
	//to be taken out once we get more levels
		m_pTM->Draw(m_nScreenShots[Stages[0] + m_nPcount - 1], (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2) - 170, 0.25, 0.25);
	 
		if(m_pUnlocks[Stages[1] + m_nPcount - 1] == false)
	{
		color = m_nBeta;
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[1] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2) - 25, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[1] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2), false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }
	else
	{
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[1] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2) - 25, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[1] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2), false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }

		sprintf_s(buffer, "Stage %d", Stages[1] + m_nPcount);
	if(m_nSelection == 1)
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2) - 200, false, false, 1, D3DCOLOR_ARGB(color,255,0,0));
	else
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2) - 200, false, false, 1, D3DCOLOR_ARGB(color,255,255,255));

		m_pTM->Draw(m_nScreenShots[Stages[1] + m_nPcount - 1], (m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2) - 170, 0.25, 0.25);

		m_pTM->Draw(m_nScreenShots[Stages[2] + m_nPcount - 1], (m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2) - 170, 0.25, 0.25);
	
	 if(m_pUnlocks[Stages[2] + m_nPcount - 1] == false)
	{
		color = m_nBeta;
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[2] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2) - 25, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[2] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2), false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }
	else
	{
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[2] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2) - 25, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[2] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2), false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }

		sprintf_s(buffer, "Stage %d", Stages[2] + m_nPcount);

	if(m_nSelection == 2)
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2) - 200, false, false, 1, D3DCOLOR_ARGB(color,255,0,0));
	else
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2) - 200, false, false, 1, D3DCOLOR_ARGB(color,255,255,255));

		m_pTM->Draw(m_nScreenShots[Stages[3] + m_nPcount - 1], (m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2) - 170, 0.25, 0.25);
	
	 if(m_pUnlocks[Stages[3] + m_nPcount - 1] == false)
	{
		color = m_nBeta;
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[3] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2) - 25, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[3] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2), false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }
	else
	{
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[3] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2) - 25, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[3] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2), false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }

	 		sprintf_s(buffer, "Stage %d", Stages[3] + m_nPcount);
	
			if(m_nSelection == 3)
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2) - 200, false, false, 1, D3DCOLOR_ARGB(color,255,0,0));
	else
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2) - 200, false, false, 1, D3DCOLOR_ARGB(color,255,255,255));

		m_pTM->Draw(m_nScreenShots[Stages[4] + m_nPcount - 1], (m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2) - 170, 0.25, 0.25);
	
	 if(m_pUnlocks[Stages[4] + m_nPcount - 1] == false)
	{
		color = m_nBeta;
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[4] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2) - 25, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[4] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2), false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }
	else
	{
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[4] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2) - 25, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[4] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2), false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }

	  	sprintf_s(buffer, "Stage %d", Stages[4] + m_nPcount);
	
		if(m_nSelection == 4)
			CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2) - 200, false, false, 1, D3DCOLOR_ARGB(color,255,0,0));
		else
			CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2) - 200, false, false, 1, D3DCOLOR_ARGB(color,255,255,255));

	 	if( Stages[5] + m_nPcount != 1 && m_pUnlocks[Stages[5] + m_nPcount - 1] == false)
		{
			color = m_nBeta;
			sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[5] + m_nPcount - 1]);
			CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2) + 225, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
			sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[5] + m_nPcount - 1].gemCount, 15);
			CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2) + 250, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		}
		else
		{
			sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[5] + m_nPcount - 1]);
			CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2) + 225, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
			sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[5] + m_nPcount - 1].gemCount, 15);
			CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2) + 250, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		}

		sprintf_s(buffer, "Stage %d", Stages[5] + m_nPcount);

	if(m_nSelection == 5)
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2) + 50, false, false, 1, D3DCOLOR_ARGB(color,255,0,0));
	else
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2) + 50, false, false, 1, D3DCOLOR_ARGB(color,255,255,255));


		m_pTM->Draw(m_nScreenShots[Stages[5] + m_nPcount - 1], (m_pGame->GetWindowWidth() / 4 * 1) - 220, (m_pGame->GetWindowHeight() / 5 * 2) + 80, 0.25, 0.25);
	 
	 if(m_pUnlocks[Stages[6] + m_nPcount - 1] == false)
	{
		color = m_nBeta;
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[6] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2) + 225, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[6] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2) + 250, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }
	else
	{
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[6] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2) + 225, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[6] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2) + 250, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }

		sprintf_s(buffer, "Stage %d", Stages[6] + m_nPcount);
	if(m_nSelection == 6)
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2) + 50, false, false, 1, D3DCOLOR_ARGB(color,255,0,0));
	else
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2) + 50, false, false, 1, D3DCOLOR_ARGB(color,255,255,255));

	
		m_pTM->Draw(m_nScreenShots[Stages[6] + m_nPcount - 1], (m_pGame->GetWindowWidth() / 4 * 1) - 20, (m_pGame->GetWindowHeight() / 5 * 2) + 80, 0.25, 0.25);
	
	
		m_pTM->Draw(m_nScreenShots[Stages[7] + m_nPcount - 1], (m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2) + 80, 0.25, 0.25);
	
	 if(m_pUnlocks[Stages[7] + m_nPcount - 1] == false)
	{
		color = m_nBeta;
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[7] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2) + 225, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[7] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2) + 250, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }
	else
	{
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[7] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2) + 225, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[7] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2) + 250, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }

		sprintf_s(buffer, "Stage %d", Stages[7] + m_nPcount);
	if(m_nSelection == 7)
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2) + 50, false, false, 1, D3DCOLOR_ARGB(color,255,0,0));
	else
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 180, (m_pGame->GetWindowHeight() / 5 * 2) + 50, false, false, 1, D3DCOLOR_ARGB(color,255,255,255));

	
		m_pTM->Draw(m_nScreenShots[Stages[8] + m_nPcount - 1], (m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2) + 80, 0.25, 0.25);
	
	 if(m_pUnlocks[Stages[8] + m_nPcount - 1] == false)
	{
		color = m_nBeta;
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[8] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2) + 225, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[8] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2) + 250, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }
	else
	{
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[8] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2) + 225, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[8] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2) + 250, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }

	  		sprintf_s(buffer, "Stage %d", Stages[8] + m_nPcount);
	if(m_nSelection == 8)
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2) + 50, false, false, 1, D3DCOLOR_ARGB(color,255,0,0));
	else
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 380, (m_pGame->GetWindowHeight() / 5 * 2) + 50, false, false, 1, D3DCOLOR_ARGB(color,255,255,255));


	m_pTM->Draw(m_nScreenShots[Stages[9] + m_nPcount - 1], (m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2) + 80, 0.25, 0.25);
	
	 if(m_pUnlocks[Stages[9] + m_nPcount - 1] == false)
	{
		color = m_nBeta;
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[9] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2) + 225, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[9] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2) + 250, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }
	else
	{
		sprintf_s(buffer, "Time %.2f", m_pTimes[Stages[9] + m_nPcount - 1]);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2) + 225, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
		sprintf_s(buffer, "Sprockets %d / %d",m_sPlayer[Stages[9] + m_nPcount - 1].gemCount, 15);
		CBitMapFont::GetInstance()->Print(buffer,(m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2) + 250, false, false, 0.5, D3DCOLOR_ARGB(color,0,255,0));
	 }

	  	sprintf_s(buffer, "Stage %d", Stages[9] + m_nPcount);
	if(m_nSelection == 9)
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2) + 50, false, false, 1, D3DCOLOR_ARGB(color,255,0,0));
	else
		CBitMapFont::GetInstance()->Print(buffer, (m_pGame->GetWindowWidth() / 4 * 1) + 580, (m_pGame->GetWindowHeight() / 5 * 2) + 50, false, false, 1, D3DCOLOR_ARGB(color,255,255,255));

	if(m_pGame->GetDifficulty() == 0)
		CBitMapFont::GetInstance()->Print("Difficulty - Easy", m_pGame->GetWindowWidth() - 600, 20);
	else if(m_pGame->GetDifficulty() == 1)
		CBitMapFont::GetInstance()->Print("Difficulty - Normal", m_pGame->GetWindowWidth() - 600, 20);
	else
		CBitMapFont::GetInstance()->Print("Difficulty - Hard", m_pGame->GetWindowWidth() - 600, 20);

		CBitMapFont::GetInstance()->Print("Press Down for more Levels Up for Previous Ones", m_pGame->GetWindowWidth() / 2, (m_pGame->GetWindowWidth() / 5 * 2) + 200, true, false, 0.6, D3DCOLOR_ARGB(255,255,0,0));

		CBitMapFont::GetInstance()->Print("Press U to access Upgrade Screen - Press X on the Xbox Controller", m_pGame->GetWindowWidth() / 2, (m_pGame->GetWindowWidth() / 5 * 2) + 250, true, false, 0.6, D3DCOLOR_ARGB(255,255,0,0));

		CBitMapFont::GetInstance()->Print("Press C to access Custom Level Screen - Press Y on the Xbox Controller", m_pGame->GetWindowWidth() / 2, (m_pGame->GetWindowWidth() / 5 * 2) + 300, true, false, 0.6, D3DCOLOR_ARGB(255,255,0,0));
}

void CLevelSelect::Update(float fDT)
{
	m_pTimes = CGame::GetInstance()->GetLevelTimes();
	if(CGame::GetInstance()->GetLevelUnlock() == false)
		m_pUnlocks = CGame::GetInstance()->GetCurrStageCompletes();
	else 
	{
		for(int i = 0; i < 20; i++)
			m_pUnlocks[i] = true; 
	}

	for(int i = 0; i < 20; i++)
		m_sGemCount[i] = m_pGame->GetGemsPerLevel(i);

	for(int i = 0; i < 20; i++)
	{
		for(int j = 0; j < 15; j++)
		{
			if(m_sGemCount[i].gems[j])
			{
				if(m_sPlayer[i].gemCollected[j] == true)
				{
				}
				else
				{
					m_sPlayer[i].gemCollected[j] = m_sGemCount[i].gems[j];
					m_sPlayer[i].gemCount++;
				}
			}
		}
	}
}

bool CLevelSelect::Input()
{
	DWORD dwResult;         // Used to store if a controller is connected
	DWORD dwStateResult;
	XINPUT_STATE state;     // Data type that stores all the current states
	XINPUT_KEYSTROKE key;
	// of a controller.

	ZeroMemory( &state, sizeof(XINPUT_STATE) ); // Zeros out the states of
	ZeroMemory( &key, sizeof(XINPUT_KEYSTROKE));
	// the controller.

	// Get all the current states of controller 1
	dwResult = XInputGetState( 0, &state );
	dwStateResult = XInputGetKeystroke(XUSER_INDEX_ANY,XINPUT_FLAG_GAMEPAD,&key);

	if(dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS)
	{
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_LTHUMB_UP || key.VirtualKey == VK_PAD_DPAD_UP))
		{
			if(m_nPcount == 8)
				m_nPcount = 8;
			else
				m_nPcount += 2;
		}
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_LTHUMB_DOWN || key.VirtualKey == VK_PAD_DPAD_DOWN))
		{
			if(m_nPcount == 0)
				m_nPcount = 0;
			else
				m_nPcount -= 2;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
		{
			int numPos = Stages[m_nSelection] + m_nPcount;

		switch(numPos)
		{
		case 1:
			{
			m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 2:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
				m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 3:
			{
				if(m_pUnlocks[numPos -1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 4:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
			m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 5:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 6:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 7:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 8:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 9:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 10:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		}
			Sleep(150);
		}
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_LTHUMB_LEFT || key.VirtualKey == VK_PAD_DPAD_LEFT))
		{
			--m_nSelection;
			if(m_nSelection < 0)
				m_nSelection = 9;
		}
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_LTHUMB_RIGHT || key.VirtualKey == VK_PAD_DPAD_RIGHT))
		{
			++m_nSelection;
			if(m_nSelection > 9)
				m_nSelection = 0;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_B)
		{
			m_pGame->PopState();
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_X)
		{
			m_pGame->PushState(CUpgradeState::GetInstance());
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_Y)
		{
			m_pGame->PushState(CCustomLevelState::GetInstance());
		}
	}

		if(m_pDI->KeyPressedEx(DIK_DOWN))
	{
		if(m_nPcount == 10)
			m_nPcount = 10;
		else
			m_nPcount += 10;

		m_nSelection = 0; 
	}

	if(m_pDI->KeyPressedEx(DIK_UP))
	{
		if(m_nPcount == 0)
			m_nPcount = 0;
		else
			m_nPcount -= 10;

		m_nSelection = 0; 
	}

	if(m_pDI->KeyPressed(DIK_U))
	{
		m_pGame->PushState(CUpgradeState::GetInstance());
			return true;
	}

	if(m_pDI->KeyPressed(DIK_C))
	{
			m_pGame->PushState(CCustomLevelState::GetInstance());
			return true;
	}

	if(m_pDI->KeyPressedEx(DIK_RETURN))
	{
		int numPos = Stages[m_nSelection] + m_nPcount;

		switch(Stages[m_nSelection])
		{
		case 1:
			{
			m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 2:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
				m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 3:
			{
				if(m_pUnlocks[numPos -1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 4:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
			m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 5:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 6:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 7:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 8:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 9:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		case 10:
			{
				if(m_pUnlocks[numPos - 1] == false)
					break;
				m_pGame->SetLoadLevel(numPos);
			m_pGame->PushState(CGamePlayState::GetInstance());
			}
			break;
		}
	}

	if(m_pDI->KeyPressed(DIK_LEFT))
	{
			--m_nSelection;
		if(m_nSelection < 0)
			m_nSelection = 9;
	}

	if(m_pDI->KeyPressed(DIK_RIGHT))
	{
		++m_nSelection;
		if(m_nSelection > 9)
			m_nSelection = 0;
	}

	if(m_pDI->KeyPressed(DIK_ESCAPE))
		m_pGame->PopState();

	return true;
}

void CLevelSelect::Exit()
{
	//CSGD_FModManager::GetInstance()->StopSound(CMainMenuState::GetInstance()->GetTitleMusicID());
	m_nPcount = 0;
	m_nSelection = 0;
	/*if(Stages != NULL)
	{
		for(int i = 0; i < 10; i++)
		Stages[i] = 0;

		delete [] Stages;
		Stages = NULL;
	}*/

	m_pTimes = NULL;
	m_pUnlocks = NULL;
}