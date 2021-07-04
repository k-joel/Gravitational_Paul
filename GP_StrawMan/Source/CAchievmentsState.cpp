#include "CAchievmentsState.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "CAssetState.h"
#include "CGame.h"
#include "CBitMapFont.h"
#include <string>
using std::string;



CAchievementsState::CAchievementsState()
{
	m_nCurrSelection = 0;
	m_fTickTime = 0.0f;

	m_nBoxImageID = -1;
	m_nBGImageID = -1;
	m_nSelectID = -1;
	for(unsigned int i = 0; i < 10; ++i)
	{
		m_nAchievIcons[i] = -1;
	}
	
	m_dwGreyScale = 0;

	m_pTM = NULL;
	m_pGame = NULL;
	m_pFont = NULL;

	m_dwGreyScale = D3DCOLOR_ARGB(100, 255, 255, 255);

	
}
CAchievementsState::~CAchievementsState()
{

}
void CAchievementsState::Enter()
{
	CAssetState* m_pAS = CAssetState::GetInstance();


	m_pGame = CGame::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pFont = CBitMapFont::GetInstance();

	m_nBGImageID = m_pAS->GetImageID("AchievScreenBG");
	m_nBoxImageID = m_pAS->GetImageID("SlotBoxID");
	m_nSelectID = m_pAS->GetImageID("AchievSelectBox");

	m_nAchievIcons[ACHIEV_EASYBEAT] = m_pAS->GetImageID("AchievBeatEasyIcon");
	m_nAchievIcons[ACHIEV_NORMBEAT] = m_pAS->GetImageID("AchievBeatNormIcon");
	m_nAchievIcons[ACHIEV_HARDBEAT] = m_pAS->GetImageID("AchievBeatHardIcon");
	m_nAchievIcons[ACHIEV_CHAMP] = m_pAS->GetImageID("AchievChampIcon");
	m_nAchievIcons[ACHIEV_CHUMP] = m_pAS->GetImageID("AchievChumpIcon");
	m_nAchievIcons[ACHIEV_1MAX] = m_pAS->GetImageID("AchievPonyIcon");
	m_nAchievIcons[ACHIEV_ALLMAX] = m_pAS->GetImageID("AchievBestIcon");
	m_nAchievIcons[ACHIEV_MCDUCK] = m_pAS->GetImageID("AchievDuckIcon");
	m_nAchievIcons[ACHIEV_CUSTOMLVL] = m_pAS->GetImageID("AchievResumeIcon");
	m_nAchievIcons[ACHIEV_NOUPGRADE] = m_pAS->GetImageID("AchievWarrantyIcon");

}

void CAchievementsState::Exit()
{

}

bool CAchievementsState::Input()
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

	if(dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS)
	{
		
		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == VK_PAD_B)
		{
			m_pGame->PopState();
			return true;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_UP || key.VirtualKey == VK_PAD_LTHUMB_UP))
		{
			if(m_nCurrSelection < 5)
				m_nCurrSelection += 5;
			else
				m_nCurrSelection -= 5;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_DOWN || key.VirtualKey == VK_PAD_LTHUMB_DOWN))
		{
			if(m_nCurrSelection > 4)
				m_nCurrSelection -= 5;
			else
				m_nCurrSelection += 5;
		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_LEFT || key.VirtualKey == VK_PAD_LTHUMB_LEFT))
		{
			m_nCurrSelection--;
			if(m_nCurrSelection < 0)
				m_nCurrSelection = 9;

		}

		if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && (key.VirtualKey == VK_PAD_DPAD_RIGHT || key.VirtualKey == VK_PAD_LTHUMB_RIGHT))
		{
			m_nCurrSelection++;
			if(m_nCurrSelection > 9)
				m_nCurrSelection = 0;

		}

	}

	CSGD_DirectInput* m_pDI = CSGD_DirectInput::GetInstance();
	
	if(m_pDI->KeyPressed(DIK_ESCAPE))
	{
		m_pGame->PopState();
		return true;
	}

	if(m_pDI->KeyPressed(DIK_UP))
	{
		if(m_nCurrSelection < 5)
			m_nCurrSelection += 5;
		else
			m_nCurrSelection -= 5;
	}
	
	if(m_pDI->KeyPressed(DIK_LEFT))
	{
		m_nCurrSelection--;
		if(m_nCurrSelection < 0)
			m_nCurrSelection = 9;
	}

	if(m_pDI->KeyPressed(DIK_DOWN))
	{
		if(m_nCurrSelection > 4)
			m_nCurrSelection -= 5;
		else
			m_nCurrSelection += 5;
	}

	if(m_pDI->KeyPressed(DIK_RIGHT))
	{
		m_nCurrSelection++;
		if(m_nCurrSelection > 9)
			m_nCurrSelection = 0;
	}
	return true;
}

void CAchievementsState::Update(float fDT)
{
	m_fTickTime += fDT;
	if(m_fTickTime > 1.0f)
		m_fTickTime = 0.0f;
}

void CAchievementsState::Render()
{

	
	//get ints for the incrementing for the render
	int xInc = 68;
	int yVal = 128;
	int xVal = 344;

	//draw bg
	if(m_nBGImageID != -1)
	m_pTM->Draw(m_nBGImageID, 0, 0);
	m_pFont->Print("Achievments", 512, 85, true, false, 1.0f, D3DCOLOR_XRGB(255, 255, 255));

	

	for(unsigned int i = 0; i < 10; ++i)
	{
		
		if(m_pGame->GetSaveFile()->GetSaveData()->bAchievmentList[i] == true)
			m_pTM->Draw(m_nAchievIcons[i], xVal, yVal, 1.0f, 1.0f);
		else
			m_pTM->Draw(m_nAchievIcons[i], xVal, yVal, 1.0f, 1.0f, 0, 0.0f, 0.0f, 0.0f, m_dwGreyScale);


		//move over an icon space
		xVal += xInc;
		//if we're on the fifth icon
		if(i == 4)
		{
			//add the size to the y val, and then reset the x val
			yVal += xInc;
			xVal = 344;
			
		}
	}

	
	//draw the text box
	RECT boxRect;
	boxRect.top = 0;
	boxRect.left = 0;
	boxRect.right = 768;
	boxRect.bottom = 154;

	m_pTM->Draw(m_nBoxImageID, 128, 400, 1.0f, 1.5f, &boxRect);
	
	//fill it wil text
	

	string AchievName;
	string AchievDesc;

	switch(m_nCurrSelection)
	{
	case ACHIEV_EASYBEAT:
		{
			AchievName = "Step Up Your Game";
			AchievDesc = "Completed All Standard Stages on Easy Mode";
			break;
		}
	case ACHIEV_NORMBEAT :
		{
			AchievName = "Just Like Everybody Else";
			AchievDesc = "Completed All Standard Stages on Normal Mode";
			
			break;
		}
	case ACHIEV_HARDBEAT :
		{
			AchievName = "It's Lonely at the Top";
			AchievDesc = "Completed All Standard Stages on Hard Mode";
			
			break;
		}
	case ACHIEV_CHAMP:
		{
			AchievName = "Gravitational Champ";
			AchievDesc = "Clear a Single Slot on Hard Mode Without\nDying EVEN ONCE EVER";
			
			break;
		}
	case ACHIEV_CHUMP:
		{
			AchievName = "Gravitational Chump";
			AchievDesc = "Get a Game Over While On Easy Mode in \nLevel 1";
			
			break;
		}
	case ACHIEV_1MAX:
		{
			AchievName = "One Trick Pony";
			AchievDesc = "Max One Stat on your Gravity Gun";
			
			break;
		}
	case ACHIEV_ALLMAX:
		{
			AchievName = "I'm the Best at What I Do!";
			AchievDesc = "Maxed all Gravity Gun Stats";
			
			break;
		}
	case ACHIEV_MCDUCK:
		{
			AchievName = "The McDuck";
			AchievDesc = "Collect All Currency In the Game's \nStandard Levels";
			
			break;
		}
	case ACHIEV_CUSTOMLVL:
		{
			AchievName = "Level Designer Resume";
			AchievDesc = "Complete A User Created Level";
			
			break;
		}
	case ACHIEV_NOUPGRADE:
		{
			AchievName = "Won't Void My Warranty";
			AchievDesc = "Completed the Game on Normal or \nHard mode without upgrading the Gravity Gun at all";
			
			break;
		}
		
	}
	
	

	//render the text to the box
	m_pFont->Print(AchievName.c_str(), 512, 430, true, true, 1.0f);
	m_pFont->SetCurFont(FONT_B);
	m_pFont->Print(AchievDesc.c_str(), 160, 500, false, false, .6f, D3DCOLOR_XRGB(255, 255, 255));
	m_pFont->SetCurFont(FONT_A);


	//figure out where the selection box goes

	if(m_nCurrSelection > 4)
	{
		xVal = 344 + ((m_nCurrSelection - 5) * xInc);
		yVal = 196;
	}
	else
	{
		xVal = 344 + (m_nCurrSelection * xInc);
		yVal = 128;
	}
	
	//scale the box by time 
	float fScale = (m_fTickTime / 4.0f);
	
	//draw the box
	m_pTM->Draw(m_nSelectID, xVal - (int)(32.0f * fScale), yVal - (int)(32.0f * fScale), 1.0f + fScale, 1.0f + fScale);
	
	m_pFont->SetCurFont(FONT_B);

	m_pFont->Print("Escape / B : Back", 800, 690, false, false, .5f);
	m_pFont->SetCurFont(FONT_A);
}
CAchievementsState* CAchievementsState::GetInstance()
{
	static CAchievementsState instance;
	return &instance;
}