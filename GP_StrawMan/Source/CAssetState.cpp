#include "CAssetState.h"

#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_FModManager.h"
#include "CMainMenuState.h"
#include "CBitMapFont.h"
#include "CIntroState.h"
#include "COutroState.h"

#include "CGame.h"

CAssetState::CAssetState()
{
	m_pTM = NULL;
	m_pFM = NULL;
	m_pFont = NULL;

	m_fSFXVolume = 1.0f;
	m_fMusicVolume = 1.0f;

	//////////////////////////////////////////////////////////////////////////
	// loading the loading assets for the loading screen
	// eggs bacon spam spam spam and spam
	//////////////////////////////////////////////////////////////////////////
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pFM = CSGD_FModManager::GetInstance();
	m_pFont = CBitMapFont::GetInstance();

	m_nCurrentStage = 0;

	m_rLoadbarBGRect.left = 0;
	m_rLoadbarBGRect.top = 0;
	m_rLoadbarBGRect.right = 506;
	m_rLoadbarBGRect.bottom = 84;

	m_rLoadbarRect.left = 0;
	m_rLoadbarRect.top = 0;
	m_rLoadbarRect.right = 468;
	m_rLoadbarRect.bottom = 48;

	m_mImageIDMap["LoadingBar"] = m_pTM->LoadTexture("Resource/Graphics/CbB_loadingbar.png");
	m_mImageIDMap["LoadingBarBG"] = m_pTM->LoadTexture("Resource/Graphics/CbB_loadingbarBG.png");
	m_mImageIDMap["IntroCBB"] = m_pTM->LoadTexture("Resource/Graphics/CbB_CardboardBox.png");
	m_mImageIDMap["FontA"] = m_pTM->LoadTexture("Resource/Graphics/CbB_FontA.png");
	m_mImageIDMap["FontB"] = m_pTM->LoadTexture("Resource/Graphics/CbB_FontB.png");
	m_mImageIDMap["FontASel"] = m_pTM->LoadTexture("Resource/Graphics/CbB_FontASelected.png");
	m_mImageIDMap["FontBSel"] = m_pTM->LoadTexture("Resource/Graphics/CbB_FontBSelected.png");

	m_pFont->SetImageID(m_mImageIDMap["FontA"], FONT_A);
	m_pFont->SetImageID(m_mImageIDMap["FontB"], FONT_B);
	m_pFont->SetSelectedID(m_mImageIDMap["FontASel"], FONT_A);
	m_pFont->SetSelectedID(m_mImageIDMap["FontBSel"], FONT_B);

	//m_mImageIDMap["Test"] = m_pTM->LoadTexture("Resource/Graphics/test.png");
}

CAssetState::~CAssetState()
{
}

CAssetState* CAssetState::GetInstance()
{
	static CAssetState instance;
	return &instance;
}

void CAssetState::Enter()
{
	//m_nLoadingScreenID = m_pTM->LoadTexture();
	SetAllMusicVolumes(CGame::GetInstance()->GetSaveFile()->GetSaveData()->fMusicVol);
	SetAllSFXVolumes(CGame::GetInstance()->GetSaveFile()->GetSaveData()->fSFXVol);
}

void CAssetState::Exit()
{
	m_nCurrentStage = 0;

	for(map<string, int>::iterator iter = m_mImageIDMap.begin(); iter != m_mImageIDMap.end(); iter++)
		m_pTM->UnloadTexture((*iter).second);

	for(map<string, int>::iterator iter = m_mSFXIDMap.begin(); iter != m_mSFXIDMap.end(); iter++)
		m_pFM->UnloadSound((*iter).second);

	for(map<string, int>::iterator iter = m_mMusicIDMap.begin(); iter != m_mMusicIDMap.end(); iter++)
		m_pFM->UnloadSound((*iter).second);
}

bool CAssetState::Input()
{
	return true;
}

void CAssetState::Update(float fDT)
{
	m_rLoadbarRect.right = int((float)m_rLoadbarRect.left + 468.0f*((float)m_nCurrentStage/(float)NUM_ASSETS));

	//////////////////////////////////////////////////////////////////////////
	// REMEMBER TO INCREMENT THE NUM_ASSETS DEFINE WHEN YOU ADD MORE STAGES
	//////////////////////////////////////////////////////////////////////////
	switch(m_nCurrentStage)
	{
	case 0:
		//keep this case empty, it just needs to set the name of the next stage
		//////////////////////////////////////////////////////////////////////////
		m_szCurAssetName = "Sound Effects";
		break;
	case 1:
		m_mSFXIDMap["StartSound1"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_start1.wav");
		m_mSFXIDMap["StartSound2"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_start2.wav");
		m_mSFXIDMap["StartSound3"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_start3.wav");
		m_mSFXIDMap["StartSound4"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_start4.wav");
		m_mSFXIDMap["StartSound5"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_start5.wav");
		m_mSFXIDMap["StartSound6"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_start6.wav");
		m_mSFXIDMap["StartSound7"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_start7.wav");
		m_mSFXIDMap["StartSound8"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_start8.wav");
		m_mSFXIDMap["StartSound9"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_start9.wav");
		m_mSFXIDMap["StartSound10"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_start10.wav");

		m_mSFXIDMap["EndSound1"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_end1.wav");
		m_mSFXIDMap["EndSound2"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_end2.wav");
		m_mSFXIDMap["EndSound3"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_end3.wav");
		m_mSFXIDMap["EndSound4"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_end4.wav");

		m_mSFXIDMap["DeathSound1"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_death1.wav");
		m_mSFXIDMap["DeathSound2"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_death2.wav");
		m_mSFXIDMap["DeathSound3"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_death3.wav");
		m_mSFXIDMap["DeathSound4"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_death4.wav");

		m_mSFXIDMap["KillSound1"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_kill1.wav");
		m_mSFXIDMap["KillSound2"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_kill2.wav");
		m_mSFXIDMap["KillSound3"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_kill3.wav");
		m_mSFXIDMap["KillSound4"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_kill4.wav");
		m_mSFXIDMap["KillSound5"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_kill5.wav");

		m_mSFXIDMap["LavaKill1"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_kill6.wav");
		m_mSFXIDMap["LavaKill2"] = m_pFM->LoadSound("Resource/Sound/Voice/CbB_kill7.wav");

		m_mSFXIDMap["FireShotSoundID"] = m_pFM->LoadSound("Resource/Sound/CbB_ShotSound.mp3");
		m_mSFXIDMap["ShotDissipateSoundID"] = m_pFM->LoadSound("Resource/Sound/CbB_ShotHitDissipate.mp3");
		m_mSFXIDMap["ShotBounceSoundID"] = m_pFM->LoadSound("Resource/Sound/CbB_ShotBounceSound.mp3");
		m_mSFXIDMap["PickupCollideID"] = m_pFM->LoadSound("Resource/Sound/CbB_PowerUpCollide.wav");
		m_mSFXIDMap["TerminalFallSoundID"]  = m_pFM->LoadSound("Resource/Sound/CbB_TerminalFall.wav");

		m_mSFXIDMap["UpgradeSoundID"] = m_pFM->LoadSound("Resource/Sound/CbB_UpgradeComplete.mp3");
		m_mSFXIDMap["FailedUpgradeSoundID"] = m_pFM->LoadSound("Resource/Sound/CbB_FailedUpgrade.wav");
		m_mSFXIDMap["AdjustFXID"] = m_pFM->LoadSound("Resource/Sound/CbB_testsound.wav", SGD_FMOD_LOOPING);
		m_mSFXIDMap["ScreamID"] = m_pFM->LoadSound("Resource/Sound/CbB_Scream.mp3");
		m_mSFXIDMap["TermEnterID"] = m_pFM->LoadSound("Resource/Sound/CbB_TermEnter2.mp3");

		m_mSFXIDMap["CollideSoundID"] = m_pFM->LoadSound("Resource/Sound/CbB_CollideTest.wav");
		m_mSFXIDMap["WarningSoundID"] = m_pFM->LoadSound("Resource/Sound/CbB_Warning.mp3"); 
		//////////////////////////////////////////////////////////////////////////
		m_szCurAssetName = "Music";
		break;
	case 2:
		m_mMusicIDMap["MainMenuMusic"] = m_pFM->LoadSound("Resource/Sound/CbB_TitleMusic.mp3", SGD_FMOD_LOOPING);

		m_mMusicIDMap["StageMusic1"] = m_pFM->LoadSound("Resource/Sound/CbB_stagemusic1.mp3");
		m_mMusicIDMap["StageMusic2"] = m_pFM->LoadSound("Resource/Sound/CbB_stagemusic2.mp3");
		m_mMusicIDMap["StageMusic3"] = m_pFM->LoadSound("Resource/Sound/CbB_stagemusic3.mp3");
		m_mMusicIDMap["StageMusic4"] = m_pFM->LoadSound("Resource/Sound/CbB_stagemusic4.mp3");
		m_mMusicIDMap["StageMusic5"] = m_pFM->LoadSound("Resource/Sound/CbB_stagemusic5.mp3");

		//////////////////////////////////////////////////////////////////////////
		m_szCurAssetName = "Images";
		break;
	case 3:
		m_mImageIDMap["MainMenuBG"] = m_pTM->LoadTexture("Resource/Graphics/CbB_mainmenubg.jpg");

		m_mImageIDMap["FileSelectBG"] = m_pTM->LoadTexture("Resource/Graphics/CbB_FileBG.png");
		m_mImageIDMap["SlotBoxID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_SlotBoxImage.png", D3DCOLOR_XRGB(255, 0, 255));

		m_mImageIDMap["StagePreview0"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level1demo.png");
		m_mImageIDMap["StagePreview1"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level2demo.png");
		m_mImageIDMap["StagePreview2"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level3demo.png");
		m_mImageIDMap["StagePreview3"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level4demo.png");
		m_mImageIDMap["StagePreview4"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level5demo.png");
		m_mImageIDMap["StagePreview5"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level6demo.png");
		m_mImageIDMap["StagePreview6"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level7demo.png");
		m_mImageIDMap["StagePreview7"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level8demo.png");
		m_mImageIDMap["StagePreview8"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level9demo.png");
		m_mImageIDMap["StagePreview9"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level10demo.png");
		m_mImageIDMap["StagePreview10"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level11demo.png");
		m_mImageIDMap["StagePreview11"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level12demo.png");
		m_mImageIDMap["StagePreview12"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level13demo.png");
		m_mImageIDMap["StagePreview13"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level14demo.png");
		m_mImageIDMap["StagePreview14"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level15demo.png");
		m_mImageIDMap["StagePreview15"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level16demo.png");
		m_mImageIDMap["StagePreview16"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level17demo.png");
		m_mImageIDMap["StagePreview17"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level18demo.png");
		m_mImageIDMap["StagePreview18"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level19demo.png");
		m_mImageIDMap["StagePreview19"] = m_pTM->LoadTexture("Resource/Graphics/CbB_level20demo.png");

		m_mImageIDMap["IntroGP"] = m_pTM->LoadTexture("Resource/Graphics/CbB_Intro.png");
		m_mImageIDMap["OutroGO"] = m_pTM->LoadTexture("Resource/Graphics/CbB_OutroGO.png");
		m_mImageIDMap["Outro"] = m_pTM->LoadTexture("Resource/Graphics/CbB_Outro.png");
		m_mImageIDMap["AttractCurrency"] = m_pTM->LoadTexture("Resource/Graphics/CbB_AttractCurrency.png");
		m_mImageIDMap["AttractEnemies"] = m_pTM->LoadTexture("Resource/Graphics/CbB_AttractEnemies.png");
		m_mImageIDMap["AttractObstacles"] = m_pTM->LoadTexture("Resource/Graphics/CbB_AttractObstacles.png");
		m_mImageIDMap["AttractFinish"] = m_pTM->LoadTexture("Resource/Graphics/CbB_AttractFinish.png");
		for(int i = 0; i < 9; i++)
		{
			char cbuf[64], cbuf2[64];
			sprintf_s(cbuf, _countof(cbuf), "Resource/Graphics/CbB_intro%d.png", i+1);
			sprintf_s(cbuf2, _countof(cbuf2), "IntroSS%d", i);
			string szIdx = cbuf2;
			m_mImageIDMap[szIdx] = m_pTM->LoadTexture(cbuf);
		}

		// images
		m_mImageIDMap["ObjectImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_Ball.png", D3DCOLOR_XRGB(255, 0, 255));
		//m_mImageIDMap["WellImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_TestWell.png", D3DCOLOR_XRGB(255, 0, 255));
		//m_mImageIDMap["WellClosedImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_TestShot.png", D3DCOLOR_XRGB(255, 0, 255));
		//m_mImageIDMap["PlayerImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_TestSprite.png", D3DCOLOR_XRGB(255, 0, 255));
		m_mImageIDMap["TargetImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_Target.png", D3DCOLOR_XRGB(255, 0, 255));
		m_mImageIDMap["BGImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_TestBG.png", D3DCOLOR_XRGB(255, 0, 255));
		//m_mImageIDMap["WalkerImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_TestWalker.png", D3DCOLOR_XRGB(255, 0, 255));
		//m_mImageIDMap["HeavyImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_TestHeavy.png", D3DCOLOR_XRGB(255, 0, 255));
		//m_mImageIDMap["SniperImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_TestSniper.png", D3DCOLOR_XRGB(255, 0, 255));
		//m_mImageIDMap["BombardImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_TestBombard.png", D3DCOLOR_XRGB(255, 0, 255));
		m_mImageIDMap["PlatformImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_Platform.png");
		//m_mImageIDMap["TurretImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_Turret.png");
		//m_mImageIDMap["TripwireImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_Tripwire.png");
		m_mImageIDMap["FallSpikeImageID"] = m_pTM->LoadTexture("Resource/Graphics/CBb_betaSpike.png");
		m_mImageIDMap["ExitImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_Exit.png", D3DCOLOR_XRGB(255, 0, 255));
		m_mImageIDMap["UpgradeBGImgID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_Upgradebg.jpg");
		m_mImageIDMap["OptionsBG"] = m_pTM->LoadTexture("Resource/Graphics/CbB_optionsbg.jpg");
		m_mImageIDMap["LevelSelectBG"] = m_pTM->LoadTexture("Resource/Graphics/CbB_space.jpg");
		//m_mImageIDMap["HUDImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_HUDBG.png");
		//m_mImageIDMap["LifeIconID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_LifeIcon.png", D3DCOLOR_XRGB(255, 0, 255));
		m_mImageIDMap["BarImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_EnergyBar.png", D3DCOLOR_XRGB(255, 0, 255));
		m_mImageIDMap["FillerImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_BarFiller.png", D3DCOLOR_XRGB(255, 0, 255));
		m_mImageIDMap["CustomLevelID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_customdemo.png");
		m_mImageIDMap["SpikeImageID"] = m_pTM->LoadTexture("Resource/Graphics/CBb_betaSpike.png");
		m_mImageIDMap["LavaImageID"] = m_pTM->LoadTexture("Resource/Graphics/lava.png");
		m_mImageIDMap["HowToPlayID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_HowToPlay.png");
		m_mImageIDMap["CreditsID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_CreditsBG.png");
		m_mImageIDMap["AlphaBlackID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_alphablack.png");
		m_mImageIDMap["AlarmID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_Alarm.png");
		m_mImageIDMap["HourglassID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_Hourglass.png");
		m_mImageIDMap["BounceHUDID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_bounceHUD.png");
		m_mImageIDMap["EnergyHUDID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_energyHUD.png");
		m_mImageIDMap["WeightedHUDID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_weightHUD.png");
		m_mImageIDMap["LifeID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_heart.png");
 		m_mImageIDMap["ReverseZoneEasy"] = m_pTM->LoadTexture("Resource/Graphics/CbB_ReverseEasy.png");
		m_mImageIDMap["ReverseZoneMedium"] = m_pTM->LoadTexture("Resource/Graphics/CbB_ReverseNormal.png");
 		m_mImageIDMap["ReverseZoneHard"] = m_pTM->LoadTexture("Resource/Graphics/CbB_ReverseHard.png");
 		m_mImageIDMap["NegateZoneEasy"] = m_pTM->LoadTexture("Resource/Graphics/CbB_NegateEasy.png");
 		m_mImageIDMap["NegateZoneMedium"] = m_pTM->LoadTexture("Resource/Graphics/CbB_NegateNormal.png");
 		m_mImageIDMap["NegateZoneHard"] = m_pTM->LoadTexture("Resource/Graphics/CbB_NegateHard.png");
		m_mImageIDMap["PauseBGID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_PauseBG.png");


		//Pickup image init
		m_mImageIDMap["CurrencyImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_CogImage.png", D3DCOLOR_XRGB(255, 0, 255));
		m_mImageIDMap["BounceImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_BouncePowerUp.png");
		m_mImageIDMap["WeightedImageID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_WeightedPowerUp.png");

		//achievments images
		m_mImageIDMap["AchievBoxID"] = m_pTM->LoadTexture("Resource/Graphics/CbB_AchievBox.png", D3DCOLOR_XRGB(255, 0, 255));
		m_mImageIDMap["AchievChumpIcon"] = m_pTM->LoadTexture("Resource/Graphics/CbB_AchievChumpIcon.png");
		m_mImageIDMap["AchievChampIcon"] = m_pTM->LoadTexture("Resource/Graphics/CbB_AchievChampIcon.png");
		m_mImageIDMap["AchievBeatHardIcon"] = m_pTM->LoadTexture("Resource/Graphics/CbB_BeatHardIcon.png");
		m_mImageIDMap["AchievBeatNormIcon"] = m_pTM->LoadTexture("Resource/Graphics/CbB_BeatNormalIcon.png");
		m_mImageIDMap["AchievBeatEasyIcon"] = m_pTM->LoadTexture("Resource/Graphics/CbB_BeatEasyIcon.png");
		m_mImageIDMap["AchievPonyIcon"] = m_pTM->LoadTexture("Resource/Graphics/CbB_OneTrickPonyIcon.png");
		m_mImageIDMap["AchievBestIcon"] = m_pTM->LoadTexture("Resource/Graphics/CbB_BestAtIcon.png");
		m_mImageIDMap["AchievDuckIcon"] = m_pTM->LoadTexture("Resource/Graphics/CbB_McDuckIcon.png");
		m_mImageIDMap["AchievWarrantyIcon"] = m_pTM->LoadTexture("Resource/Graphics/CbB_WarrantyIcon.png");
		m_mImageIDMap["AchievResumeIcon"] = m_pTM->LoadTexture("Resource/Graphics/CbB_ResumeIcon.png");
		m_mImageIDMap["AchievScreenBG"] = m_pTM->LoadTexture("Resource/Graphics/CbB_AchievmentsBG.png");
		m_mImageIDMap["AchievSelectBox"] = m_pTM->LoadTexture("Resource/Graphics/CbB_AchievSelectBox.png", D3DCOLOR_XRGB(255, 0, 255));

		break;
		//////////////////////////////////////////////////////////////////////////
	case NUM_ASSETS:
		m_szCurAssetName = "Done!";
		m_nCurrentStage--;
		static float fTimer = 0.0f;
		fTimer += fDT;
		if(fTimer >= 1.0f)
		{
			//CGame::GetInstance()->PushState(COutroState::GetInstance());
			CGame::GetInstance()->PushState(CIntroState::GetInstance());
			//CGame::GetInstance()->PushState(CMainMenuState::GetInstance());
		}
		break;
	}

	m_nCurrentStage++;
}

void CAssetState::Render()
{
	RECT bgRect;
	bgRect.top = 0;
	bgRect.left = 0;
	bgRect.right = 1024;
	bgRect.bottom = 768;
	m_pTM->Draw(m_mImageIDMap["IntroCBB"], 256,192,0.5f, 0.5f, &bgRect);

	int nX = 512 - 253;
	int nY = 384 + 200;

	m_pTM->Draw(m_mImageIDMap["LoadingBarBG"], nX, nY, 1.0f, 1.0f, &m_rLoadbarBGRect);
	m_pTM->Draw(m_mImageIDMap["LoadingBar"], nX + 19, nY + 18, 1.0f, 1.0f, &m_rLoadbarRect);

	string szText = "Loading... " + m_szCurAssetName;
	m_pFont->Print(szText.c_str(), 512, nY + 90, true );

	/*m_pFont->Print("Loading Assets . . .", 40, 40, false, false, .50f);

	if(bMainMenuLoaded)
	{
		m_pFont->Print("Repealing Laws of Gravitation . . .", 40, 60, false, false, .50f);
	}
	if(bFileSelectLoaded)
	{
		m_pFont->Print("Marinating in Awesomesauce . . .", 40, 80, false, false, .50f);
	}*/
}

int CAssetState::GetSoundID(string soundName, int nIndex, int nType)
{
	if(nIndex != -1)
	{
		char c[2];
		sprintf_s(c, _countof(c), "%d", nIndex);
		soundName += c;
	}

	if(nType == SND_SFX)
		return m_mSFXIDMap[soundName];
	else if(nType == SND_MUSIC)
		return m_mMusicIDMap[soundName];

	return -1;
}

int CAssetState::GetImageID(string imageName, int stageNum)
{
	if(stageNum != -1)
	{
		char* c;
		if(stageNum < 11)
		{
			c = new char[2];
			sprintf_s(c,2 * sizeof(char), "%d", stageNum-1);
		}
		else
		{
			c = new char[3];
			sprintf_s(c,3 * sizeof(char), "%d", stageNum-1);

		}

		
		imageName += c;
		
		delete[] c;
	}
	
	return m_mImageIDMap[imageName];
}

void CAssetState::SetAllMusicVolumes(float fVol)
{
	if(fVol > 1.0f)	fVol = 1.0f;
	if(fVol < 0.0f) fVol = 0.0f;

	m_fMusicVolume = fVol;
	for(map<string, int>::iterator iter = m_mMusicIDMap.begin(); iter != m_mMusicIDMap.end(); iter++)
		CSGD_FModManager::GetInstance()->SetVolume((*iter).second, m_fMusicVolume);
}

void CAssetState::SetAllSFXVolumes(float fVol)
{
	if(fVol > 1.0f)	fVol = 1.0f;
	if(fVol < 0.0f) fVol = 0.0f;

	m_fSFXVolume = fVol;
	for(map<string, int>::iterator iter = m_mSFXIDMap.begin(); iter != m_mSFXIDMap.end(); iter++)
		CSGD_FModManager::GetInstance()->SetVolume((*iter).second, m_fSFXVolume);
}

float CAssetState::GetAllMusicVolumes()
{
	return m_fMusicVolume;
}

float CAssetState::GetAllSFXVolumes()
{
	return m_fSFXVolume;
}

void CAssetState::PlaySound(string soundName, int nIndex, int nType)
{
	if(nIndex != -1)
	{
		char c[2];
		sprintf_s(c, _countof(c), "%d", nIndex);
		soundName += c;
	}

	if(nType == SND_SFX)
	{
		CSGD_FModManager::GetInstance()->PlaySound(m_mSFXIDMap[soundName]);
		CSGD_FModManager::GetInstance()->SetVolume(m_mSFXIDMap[soundName], m_fSFXVolume);
	}
	else if(nType == SND_MUSIC)
	{
		CSGD_FModManager::GetInstance()->PlaySound(m_mMusicIDMap[soundName]);
		CSGD_FModManager::GetInstance()->SetVolume(m_mMusicIDMap[soundName], m_fMusicVolume);
	}
}

void CAssetState::PlaySound(int nID, int nType)
{
	CSGD_FModManager::GetInstance()->PlaySound(nID);
	if(nType == SND_MUSIC)
		CSGD_FModManager::GetInstance()->SetVolume(nID, m_fMusicVolume);
	else if(nType == SND_SFX)
		CSGD_FModManager::GetInstance()->SetVolume(nID, m_fSFXVolume);
}

void CAssetState::StopSound(int nID)
{
	CSGD_FModManager::GetInstance()->StopSound(nID); 
}

void CAssetState::StopSound(string soundName)
{

	CSGD_FModManager::GetInstance()->StopSound(m_mSFXIDMap[soundName]); 
}

bool CAssetState::IsSoundPlaying(std::string soundName)
{

	return CSGD_FModManager::GetInstance()->IsSoundPlaying(m_mSFXIDMap[soundName]); 
}