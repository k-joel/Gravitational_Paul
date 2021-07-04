#include "CUpgradeState.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "CBitMapFont.h"
#include "SGD Wrappers/CSGD_FModManager.h"
#include "CLevelSelect.h"
#include "CGame.h"
#include "CGamePlayState.h"
#include "SaveSystem.h"
#include "CAssetState.h"

#define SAVEFILE m_pGame->GetSaveFile()

CUpgradeState::CUpgradeState()
{
	m_pD3D = NULL;
	m_pTM = NULL;
	m_pGame = NULL;
	m_pDI = NULL;

	m_nUpgradeSoundID = -1;
	m_nFailedUpgradeID = -1;
	m_bUpgradeConfirm = false;
	m_nConfirmSelection = 0;
}

CUpgradeState::~CUpgradeState()
{
}

void CUpgradeState::Enter()
{
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pGame = CGame::GetInstance();
	m_pDI = CSGD_DirectInput::GetInstance();
	m_nCurrSelection = MENU_RECHARGE;
	m_nConfirmSelection = 0;

	m_bUpgradeConfirm = false;

	m_szMenuItems[0] = "Upgrade Shot Recharge";
	m_szMenuItems[1] = "Upgrade Gravity Well Radius";
	m_szMenuItems[2] = "Upgrade Gravity Well Duration";
	m_szMenuItems[3] = "exit";

	m_nImgIDBG = CAssetState::GetInstance()->GetImageID("UpgradeBGImgID");
	m_nBoxImageID = CAssetState::GetInstance()->GetImageID("SlotBoxID");
	m_nUpgradeSoundID = CAssetState::GetInstance()->GetSoundID("UpgradeSoundID");
	m_nFailedUpgradeID = CAssetState::GetInstance()->GetSoundID("FailedUpgradeSoundID");
}

void CUpgradeState::Exit()
{
}

bool CUpgradeState::Input()
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
	if(m_bUpgradeConfirm == false)
	{
		if(dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS)
		{
			if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_UP || key.VirtualKey == VK_PAD_LTHUMB_UP))
			{
				--m_nCurrSelection;
				if(m_nCurrSelection < MENU_RECHARGE)
					m_nCurrSelection = MENU_LEAVE;
			}

			if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_DOWN || key.VirtualKey == VK_PAD_LTHUMB_DOWN))
			{
				++m_nCurrSelection;
				if(m_nCurrSelection > MENU_LEAVE)
					m_nCurrSelection = MENU_RECHARGE;
			}

			if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_A)
			{
				switch(m_nCurrSelection)
				{
				case MENU_RECHARGE:
					if(CheckUpgrade(STAT_RECHARGE) && m_pGame->GetGunCheat() == false)
					{
						m_bUpgradeConfirm = true;
					}
					else
					{
						if(m_nFailedUpgradeID != -1)
							CAssetState::GetInstance()->PlaySound(m_nFailedUpgradeID);
							//CSGD_FModManager::GetInstance()->PlaySoundA(m_nFailedUpgradeID);
					}
					break;
				case MENU_RANGE:
					if(CheckUpgrade(STAT_RANGE) && m_pGame->GetGunCheat() == false)
					{
						m_bUpgradeConfirm = true;
					}
					else
					{
						if(m_nFailedUpgradeID != -1)
							CAssetState::GetInstance()->PlaySound(m_nFailedUpgradeID);
							//CSGD_FModManager::GetInstance()->PlaySoundA(m_nFailedUpgradeID);
					}
					break;
				case MENU_DURATION:
					if(CheckUpgrade(STAT_DURATION) && m_pGame->GetGunCheat() == false )
					{
						m_bUpgradeConfirm = true;
					}
					else
					{
						if(m_nFailedUpgradeID != -1)
							CAssetState::GetInstance()->PlaySound(m_nFailedUpgradeID);
							//CSGD_FModManager::GetInstance()->PlaySoundA(m_nFailedUpgradeID);
					}
					break;
				case MENU_LEAVE:
					m_pGame->PopState();
					break;
				}
			}
		}

		if(m_pDI->KeyPressedEx(DIK_UP))
		{
			--m_nCurrSelection;
			if(m_nCurrSelection < MENU_RECHARGE)
				m_nCurrSelection = MENU_LEAVE;
		}

		if(m_pDI->KeyPressedEx(DIK_DOWN))
		{
			++m_nCurrSelection;
			if(m_nCurrSelection > MENU_LEAVE)
				m_nCurrSelection = MENU_RECHARGE;
		}

		if(m_pDI->KeyPressedEx(DIK_RETURN))
		{
			switch(m_nCurrSelection)
			{
			case MENU_RECHARGE:
				if(CheckUpgrade(STAT_RECHARGE)&& m_pGame->GetGunCheat() == false)
				{
					m_bUpgradeConfirm = true;
				}
				else
				{
					if(m_nFailedUpgradeID != -1)
						CAssetState::GetInstance()->PlaySound(m_nFailedUpgradeID);
						//CSGD_FModManager::GetInstance()->PlaySoundA(m_nFailedUpgradeID);
				}
				break;
			case MENU_RANGE:
				if(CheckUpgrade(STAT_RANGE)&& m_pGame->GetGunCheat() == false)
				{
					m_bUpgradeConfirm = true;
				}
				else
				{
					if(m_nFailedUpgradeID != -1)
						CAssetState::GetInstance()->PlaySound(m_nFailedUpgradeID);
						//CSGD_FModManager::GetInstance()->PlaySoundA(m_nFailedUpgradeID);
				}
				break;
			case MENU_DURATION:
				if(CheckUpgrade(STAT_DURATION)&& m_pGame->GetGunCheat() == false)
				{
					m_bUpgradeConfirm = true;
				}
				else
				{
					if(m_nFailedUpgradeID != -1)
						CAssetState::GetInstance()->PlaySound(m_nFailedUpgradeID);
						//CSGD_FModManager::GetInstance()->PlaySoundA(m_nFailedUpgradeID);
				}
				break;
			case MENU_LEAVE:
				m_pGame->PopState();
				break;
			}
		}
	}
	else
	{
		//Upgrade Confirm Input
		if(dwStateResult == ERROR_SUCCESS || dwResult == ERROR_SUCCESS)
		{
			if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_LTHUMB_UP || key.VirtualKey == VK_PAD_DPAD_UP))
			{
				--m_nConfirmSelection;
				if(m_nConfirmSelection < 0)
					m_nConfirmSelection = 1;
			}

			if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_LTHUMB_DOWN || key.VirtualKey == VK_PAD_DPAD_DOWN))
			{
				++m_nConfirmSelection;
				if(m_nConfirmSelection > 1)
					m_nConfirmSelection = 0;
			}

			if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_A))
			{
				switch(m_nConfirmSelection)
				{
				case 0:
					//This means no
					m_bUpgradeConfirm = false;
					break;
				case 1:
					//This means yes
					if(SAVEFILE->Upgrade(m_pGame->GetCurrentSaveSlot(),m_nCurrSelection))
					{
						if(m_nUpgradeSoundID != -1)
							CAssetState::GetInstance()->PlaySound(m_nUpgradeSoundID);
							//CSGD_FModManager::GetInstance()->PlaySoundA(m_nUpgradeSoundID);

						m_bUpgradeConfirm = false;
					}
				}
			}
		}

		if(m_pDI->KeyPressedEx(DIK_UP))
		{
			--m_nConfirmSelection;
			if(m_nConfirmSelection < 0)
				m_nConfirmSelection = 1;
		}

		if(m_pDI->KeyPressedEx(DIK_DOWN))
		{
			++m_nConfirmSelection;
			if(m_nConfirmSelection > 1)
				m_nConfirmSelection = 0;
		}

		if(m_pDI->KeyPressedEx(DIK_RETURN))
		{
			switch(m_nConfirmSelection)
			{
			case 0:
				//This means no
				m_bUpgradeConfirm = false;
				break;
			case 1:
				//This means yes
				if(SAVEFILE->Upgrade(m_pGame->GetCurrentSaveSlot(),m_nCurrSelection))
				{
					if(m_nUpgradeSoundID != -1)
						CAssetState::GetInstance()->PlaySound(m_nUpgradeSoundID);
							//CSGD_FModManager::GetInstance()->PlaySoundA(m_nUpgradeSoundID);

					m_bUpgradeConfirm = false;
				}
			}
		}
		}

	return true;
}

CUpgradeState* CUpgradeState::GetInstance()
{
	static CUpgradeState instance;
	return &instance;
}

void CUpgradeState::Update(float fDT)
{
}

void CUpgradeState::Render()
{
	m_pTM->Draw(m_nImgIDBG, 0, 0);

	if(m_bUpgradeConfirm == false)
	{
		CBitMapFont::GetInstance()->Print("Upgrades", 400, 100,true);

		char buffer[100];
		sprintf_s(buffer,"Sprockets: %i",SAVEFILE->GetSaveData()->tAllSaves[m_pGame->GetCurrentSaveSlot()].nUnspentCurrency);

		CBitMapFont::GetInstance()->Print(buffer,650,150);

		RECT boxRect;
		boxRect.top = 0;
		boxRect.left = 0;
		boxRect.right = 768;
		boxRect.bottom = 154;

		int nOffsetY = 250;
		int cost;
		int currentStat;
		int nextStat;

		for(int i = 0; i < MENU_MAXIMUM; i++)
		{
			bool bSelected = false;

			if( i == m_nCurrSelection )
				bSelected = true;

			switch(SAVEFILE->GetSaveData()->tAllSaves[m_pGame->GetCurrentSaveSlot()].nUpgrades[i])
			{
			case 0:
				cost = COST_1;
				break;
			case 1:
				cost = COST_2;
				break;
			case 2:
				cost = COST_3;
				break;
			case 3:
				cost = COST_4;
				break;
			case 4:
				cost = COST_5;
				break;
			default:
				cost = -1;
				break;
			};

			char text[500];
			char Upgrade[200];
			char NextUpgrade[200];

			switch(i)
			{
			case 0:
				if(m_pGame->GetGunCheat())
				{
					sprintf_s(text,"Stat Maxed");
				}
				else if(cost > -1)
				{
					sprintf_s(text," %s to Level %i:   cost: %i",m_szMenuItems[i].c_str(),SAVEFILE->GetSaveData()->tAllSaves[m_pGame->GetCurrentSaveSlot()].nUpgrades[i] + 1,cost);
					if(i == m_nCurrSelection)
					{
						currentStat = (int)(20.0f + (RECHARGE_INCREMENT * (float)(CGame::GetInstance()->GetRechargeLevel())));
						nextStat = (int)(20.0f + (RECHARGE_INCREMENT * (float)(CGame::GetInstance()->GetRechargeLevel() + 1)));
						sprintf_s(Upgrade,"Current Shot Recharge: %i energy/sec",currentStat);
						sprintf_s(NextUpgrade,"Next Shot Recharge: %i energy/sec",nextStat);
						m_pTM->Draw(m_nBoxImageID, 200, 580, .80f, .80f, &boxRect, 0.0f, 0.0f, 0.0f);
						CBitMapFont::GetInstance()->Print(Upgrade,230,600,false,false,.80f);
						CBitMapFont::GetInstance()->Print(NextUpgrade,250,650,false,false,.80f);
					}
				}
				else
				{
					sprintf_s(text,"Stat Maxed");
				}
				break;

			case 1:
				if(m_pGame->GetGunCheat())
				{
					sprintf_s(text,"Stat Maxed");
				}
				else if(cost > -1)
				{
					sprintf_s(text," %s to Level %i:   cost: %i",m_szMenuItems[i].c_str(),SAVEFILE->GetSaveData()->tAllSaves[m_pGame->GetCurrentSaveSlot()].nUpgrades[i] + 1,cost);
					if(i == m_nCurrSelection)
					{
						currentStat = (int)(10.0f + (RANGE_INCREMENT * (float)(CGame::GetInstance()->GetRangeLevel())));
						nextStat = (int)(10.0f + (RANGE_INCREMENT * (float)(CGame::GetInstance()->GetRangeLevel()+ 1)));
						sprintf_s(Upgrade,"Current Well Radius: %i feet",currentStat);
						sprintf_s(NextUpgrade,"Next Well Radius: %i feet",nextStat);
						m_pTM->Draw(m_nBoxImageID, 200, 580, .80f, .80f, &boxRect, 0.0f, 0.0f, 0.0f);
						CBitMapFont::GetInstance()->Print(Upgrade,250,600,false,false,.80f);
						CBitMapFont::GetInstance()->Print(NextUpgrade,250,650,false,false,.80f);
					}
				}
				else
				{
					sprintf_s(text,"Stat Maxed");
				}
				break;

			case 2:
				if(m_pGame->GetGunCheat())
				{
					sprintf_s(text,"Stat Maxed");
				}
				else if(cost > -1)
				{
					sprintf_s(text," %s to Level %i:   cost: %i",m_szMenuItems[i].c_str(),SAVEFILE->GetSaveData()->tAllSaves[m_pGame->GetCurrentSaveSlot()].nUpgrades[i] + 1,cost);
					if(i == m_nCurrSelection)
					{
						currentStat = (int)(3.0f + (DURATION_INCREMENT * (float)(CGame::GetInstance()->GetDurationLevel())));
						nextStat = (int)(3.0f + (DURATION_INCREMENT * (float)(CGame::GetInstance()->GetDurationLevel()+ 1)));
						sprintf_s(Upgrade,"Current Well Duration: %i seconds",currentStat);
						sprintf_s(NextUpgrade,"Next Well Duration: %i seconds",nextStat);
						m_pTM->Draw(m_nBoxImageID, 200, 580, .80f, .80f, &boxRect, 0.0f, 0.0f, 0.0f);
						CBitMapFont::GetInstance()->Print(Upgrade,250,600,false,false,.80f);
						CBitMapFont::GetInstance()->Print(NextUpgrade,250,650,false,false,.80f);
					}
				}
				else
				{
					sprintf_s(text,"Stat Maxed");
				}
				break;

			case 3:
				sprintf_s(text,"Back");
				break;
			};

			CBitMapFont::GetInstance()->Print(text, 480, nOffsetY + ( 50 * i ), true, bSelected,.8f);
		}
	}
	else
	{
		//Upgrade Confirm Render
		CBitMapFont::GetInstance()->Print("Are you sure you want to upgrade?",250,400,false);

		for(int i = 0; i < 2; ++i)
		{
			bool bSelected = false;

			if(i == m_nConfirmSelection)
				bSelected = true;

			switch(i)
			{
			case 0:
				CBitMapFont::GetInstance()->Print("No",300,500,false,bSelected);
				break;
			case 1:
				CBitMapFont::GetInstance()->Print("Yes",300,550,false,bSelected);
				break;
			}
		}
	}
}

bool CUpgradeState::CheckUpgrade(int nStat)
{
	int currLevel;
	currLevel = SAVEFILE->GetSaveData()->tAllSaves[m_pGame->GetCurrentSaveSlot()].nUpgrades[nStat];
	if( 5 > currLevel)
	{
		int cost;
		switch(currLevel + 1)
		{
		case 1:
			{
				cost = COST_1;
				break;
			}
		case 2:
			{
				cost = COST_2;
				break;
			}
		case 3:
			{
				cost = COST_3;
				break;
			}
		case 4:
			{
				cost = COST_4;
				break;
			}
		case 5:
			{
				cost = COST_5;
				break;
			}
		default:
			{
				return false;
			}
		}

		if(cost <= SAVEFILE->GetSaveData()->tAllSaves[m_pGame->GetCurrentSaveSlot()].nUnspentCurrency)
			{
				return true;
			}
			else
			{
				return false;
			}
	}
	else
	{
		return false;
	}

	return true;
}