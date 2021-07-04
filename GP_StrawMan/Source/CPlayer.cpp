/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CPlayer.cpp"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Defines the CPlayer class. This class will act as the in game avatar
//			for the player. This will get input to influence its motion. This will be derived from CGrav Object.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CPlayer.h"

//additional #includes here

#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_WaveManager.h"
#include "CGamePlayState.h"
#include "CEventSystem.h"
#include "CAnimationManager.h"
#include "CGravSystem.h"
#include "CGame.h"
#include "CGamePlayState.h"
#include "CLevelSelect.h"
#include "CMainMenuState.h"
#include "CMessageSystem.h"
#include "CMessage.h"
#include "CParticleManager.h"
#include "CTileManager.h"
#include "SGD Wrappers/CSGD_FModManager.h"
#include "CBitMapFont.h"
#include "CAssetState.h"
#include "SGD Wrappers/SGD_Math.h"

#define PLAYER_MOVE 100.0f
#define PLAYER_JUMP 160.0f
#define MOVEMENT_DELAY .005f

CPlayer::CPlayer()
{
	SetType(TYPE_PLAYER);
	m_pDI = NULL;
	m_pPushWell = NULL;
	m_pPullWell = NULL;
	m_pShot = NULL;
	m_bShotOut = false;
	m_tShotVec.x = 0.0f;
	m_tShotVec.y = 0.0f;
	m_tRelativeMousePt.x = WorldConstants::fDefaultRWall / 2.0f;
	m_tRelativeMousePt.y = WorldConstants::fDefaultFloor / 2.0f;
	m_bLeftHeld = false;
	m_bRightHeld = false;
	m_bHasLanded = false;
	CEventSystem::GetInstance()->RegisterClient("GravWell Open", this);
	CEventSystem::GetInstance()->RegisterClient("GravWell Terminate", this);
	m_nCollideSound = -1;
	m_fSoundTimer = 0.0f;
	m_bJustInWell = false;
	m_fMovementTimer = 0.0f;
	m_bControllerConnected = false;
	m_bKill = false;
	m_fKillTimer = 0.0f;
	m_bDying = false;
	m_fFlashTimer = 0.0f;
	m_bFlashing = false;
	m_fTrailTimer = 0.0f;
	m_fFlashTick = 0.0f;
	m_nShieldID = -1;
	m_bInvisible = false;

	m_pPlatform = NULL;

	m_bRTriggerHeld = false;
	m_bLTriggerHeld = false;

	m_nAnimID = CAnimationManager::GetInstance()->StartAnimation("Player", this);
}

CPlayer::~CPlayer()
{
	if(m_pPushWell != NULL) //if well is not null
	{
		m_pPushWell->Release(); //release
		CEventSystem::GetInstance()->SendEvent("Kill Well", m_pPushWell); //and terminate (to close off in grav system)
	}
	if(m_pPullWell != NULL)
	{
		m_pPullWell->Release();
		CEventSystem::GetInstance()->SendEvent("Kill Well", m_pPullWell);
	}
		if(m_pShot != NULL)
	{
		m_pShot->Release();
		CEventSystem::GetInstance()->SendEvent("Kill Well", m_pShot);
	}
	CEventSystem::GetInstance()->UnregisterClient("GravWell Open", this);
	CEventSystem::GetInstance()->UnregisterClient("GravWell Terminate", this);
	CParticleManager::GetInstance()->KillParticle("Grav_Trail");
	CAnimationManager::GetInstance()->EndAnimation(m_nAnimID);
}

void CPlayer::Update(float fDT)
{
	
	//flashing effect timer
	if(m_fFlashTimer < 3.0f)
	{
		m_fFlashTimer += fDT;
		m_fFlashTick += fDT;

	}
	else
	{
		m_bFlashing = false;
	}

	if(GetObjState() != OBJ_DEAD)
	{
		m_fMovementTimer += fDT;
		Input();


		if(m_pPlatform != NULL)
		{
			float fLeft = m_pPlatform->GetPosVec().x - m_pPlatform->GetSizeVec().x/2.0f;
			float fRight = fLeft + m_pPlatform->GetSizeVec().x;

			if( GetPosVec().x + (GetSizeVec().x/2.0f) > fLeft && GetPosVec().x - (GetSizeVec().x/2.0f) < fRight )
				m_fFloorValue = m_pPlatform->GetPosVec().y - m_pPlatform->GetSizeVec().y/2.0f ;
			else
				m_pPlatform = NULL;
		}

		float fError = 4.0f;
		if(GetPosVec().y < m_fFloorValue - (GetSizeVec().y / 2.0f) - fError)
			m_bHasLanded = false;

		if(GetPosVec().x + (GetSizeVec().x / 2.0f) > m_fRWallValue && GetVelVec().x > 0.0f)
		{
			m_bRightHeld = false;
			SetPosX(m_fRWallValue -  (GetSizeVec().x / 2.0f));
			SetVelX(0.0f); // stop horz movement
		}
		if(GetPosVec().x - (GetSizeVec().x / 2.0f) < m_fLWallValue && GetVelVec().x < 0.0f)
		{
			m_bLeftHeld = false;
			SetPosX( m_fLWallValue + (GetSizeVec().x / 2.0f));
			SetVelX(0.0f); // stop horz movement
		}

		//update position based on time (calls parent update)
		CBaseObject::Update(fDT);
		if(this->GetVelVec().y + 30.5f > WorldConstants::fTermVel)
		{
			if(CSGD_FModManager::GetInstance()->IsSoundPlaying(CAssetState::GetInstance()->GetSoundID("TermEnterID")) == false)
				CAssetState::GetInstance()->PlaySound("TermEnterID");
				//CSGD_FModManager::GetInstance()->PlaySoundA(CAssetState::GetInstance()->GetSoundID("TermEnterID"));

			m_bDying = true;
			m_fTrailTimer = 0.0f;
		}
		else
		{
			m_bDying = false;
			m_fTrailTimer = 1.0f;
		}

		if(m_bKill == true)
		{
			m_fKillTimer += fDT;
			if(m_fKillTimer > 0.3f)
			{
				m_fKillTimer = 0.0f;
				m_bKill = false;
				CParticleManager::GetInstance()->KillParticle("Blood");
			}
		}

		//Update Velocity based on current gravity (return of CheckGravity funct * fDT)
		if(m_bHasLanded == false || m_bInWell == true)
			SetVelVec( GetVelVec() + ( CheckGravity() * fDT));

		if( CGravSystem::GetInstance()->GetGravity(GetPosVec()) != CGravSystem::GetInstance()->GetTileOnlyGrav(GetPosVec()) )
		{
			m_bInWell = true;
		}
		else
		{
			m_bInWell = false;
		}

		if(m_bInWell && !m_bJustInWell)
		{
			m_bJustInWell = true;
			CParticleManager::GetInstance()->SpawnParticle("Grav_Trail", 0, 0, this);
		}
		else if(!m_bInWell && m_bJustInWell)
		{
			m_bJustInWell = false;
			CParticleManager::GetInstance()->KillParticle("Grav_Trail");
		}

		//**Tech Demo Floor Check**
		//to be removed once tiles are implemented.
		if(GetPosVec().y + (GetSizeVec().y / 2.0f) > m_fFloorValue)
		{
			SetPosY(m_fFloorValue  - (GetSizeVec().y / 2.0f)); // set to floor
			if(this->GetTermVelIsTrue())
			{
				CParticleManager::GetInstance()->SpawnParticle("Blood",GetPosVec().x - (GetSizeVec().x / 2), GetPosVec().y - (GetSizeVec().y / 2));
				CGamePlayState::GetInstance()->PlayerHit();
				m_bKill = true;
				return;
			}

			SetVelY(0.0f); // stop vert movement
			//set Landed to true
			if( false == (m_bInWell || m_bLeftHeld || m_bRightHeld)) //if landing, not in a well and not moving then stop motion
				SetVelX(0.0f);

			if(!m_bHasLanded)
			{
				m_bLeftHeld = false;
				m_bRightHeld = false; //set the key held bools to false so that the input doesnt correct the velocity again
			}
			m_bHasLanded = true;
			CAnimationManager::GetInstance()->PerformAction(m_nAnimID, VELY_ZERO);
		}
		if(GetPosVec().y - (GetSizeVec().y / 2.0f) < m_fCeilingValue)
		{
			SetPosY(m_fCeilingValue + (GetSizeVec().y / 2.0f));
			SetVelY(0.0f);
		}
		if(GetPosVec().x + (GetSizeVec().x / 2.0f) > m_fRWallValue)
		{
			m_bRightHeld = false;
			SetPosX(m_fRWallValue -  (GetSizeVec().x / 2.0f));
			SetVelX(0.0f); // stop horz movement
		}
		if(GetPosVec().x - (GetSizeVec().x / 2.0f) < m_fLWallValue)
		{
			m_bLeftHeld = false;
			SetPosX( m_fLWallValue + (GetSizeVec().x / 2.0f));
			SetVelX(0.0f); // stop horz movement
		}

		// in a well?
		if( m_bInWell )
			CAnimationManager::GetInstance()->PerformAction(m_nAnimID, IN_GRAV_WELL);

		// are we falling?
		if( GetVelVec().y > 0.5f && !m_bInWell && m_bHasLanded == false)
			CAnimationManager::GetInstance()->PerformAction(m_nAnimID, POS_VELY_AND_OUT_OF_GRAV_WELL);
	}
			//this will reset the ceiling and floor values every frame (as collision is called after this
		//the values for object will be used on the next pass if they are changes by a collision
		//this will be changed to proc the tile manager for the solid tiles next to the player, and set that
		//as the new wall value

	if(m_bHasLanded == true && GetVelVec().y == 0 && CTileManager::GetInstance()->StillOnFloor(this))
	{
		SetPosY(m_fFloorValue  - (GetSizeVec().y / 2.0f)); // set to floor
	}

	
/*
		SetCeiling(WorldConstants::fDefaultCeiling);
		if(m_bHasLanded == false) //only change the floor back if they have left the ground
			SetFloor(WorldConstants::fDefaultFloor);
		SetLWall(WorldConstants::fDefaultLWall);
		SetRWall(WorldConstants::fDefaultRWall);
		*/

	//sound update timer for collide
	m_fSoundTimer += fDT;
}
void CPlayer::Input()
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

		//input check for left and right (must go here as position update happens afer input)
		if(m_bLeftHeld)
		{
			m_bLeftHeld = false;
			SetVelX( GetVelVec().x + PLAYER_MOVE);
		}
		if(m_bRightHeld)
		{
			m_bRightHeld = false;
			SetVelX( GetVelVec().x - PLAYER_MOVE);
		}

		if(state.Gamepad.bRightTrigger < 45 && m_bRTriggerHeld)
		{
			m_bRTriggerHeld = false;
		}

		if(state.Gamepad.bLeftTrigger < 45 && m_bLTriggerHeld)
		{
			m_bLTriggerHeld = false;
		}

	if(m_pDI == NULL)
		m_pDI = CSGD_DirectInput::GetInstance();

	if(dwResult == ERROR_SUCCESS || dwStateResult == ERROR_SUCCESS)
	{
		m_bControllerConnected = true;

		if( (state.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
            (state.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
            state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) )
            {
                  state.Gamepad.sThumbLX = 0;
                  state.Gamepad.sThumbLY = 0;
            }

      if( (state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
            state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
            (state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
            state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) )
            {
                  state.Gamepad.sThumbRX = 0;
                  state.Gamepad.sThumbRY = 0;
            }

	  if(CGame::GetInstance()->GetControllerLayout() == 0 || CGame::GetInstance()->GetControllerLayout() == 2)
	  {
		  if(state.Gamepad.bRightTrigger && state.Gamepad.bRightTrigger > 150 && state.Gamepad.bRightTrigger < 255 && m_bRTriggerHeld == false)
		  {
			  m_bRTriggerHeld = true;
			  FirePullWell();
		  }
	  }
	  else
	  {
		  if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == CGame::GetInstance()->GetCShootPull())
		  {
			  FirePullWell();
		  }
	  }

	  if(CGame::GetInstance()->GetControllerLayout() == 0 || CGame::GetInstance()->GetControllerLayout() == 2)
	  {
		  if(state.Gamepad.bLeftTrigger && state.Gamepad.bLeftTrigger > 150 && state.Gamepad.bLeftTrigger < 255 && m_bLTriggerHeld == false &&
			  CGame::GetInstance()->GetLoadLevel() > 5)
		  {
			  m_bLTriggerHeld = true;
			  FirePushWell();
		  }
	  }
	  else
	  {
		  if(key.Flags == XINPUT_KEYSTROKE_KEYDOWN && key.VirtualKey == CGame::GetInstance()->GetCShootPush() && CGame::GetInstance()->GetLoadLevel() > 5)
		  {
			  FirePushWell();
		  }

	  }

	  if(CGame::GetInstance()->GetControllerLayout() == 0)
	  {
		  if(state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		  {
			  //close all logic here
			  if(m_pPushWell != NULL)
				  m_pPushWell->Terminate();
			  if(m_pPullWell != NULL)
				  m_pPullWell->Terminate();
		  }
	  }
	  else if(CGame::GetInstance()->GetControllerLayout() == 1)
	  {
		  if(state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
		  {
			  //close all logic here
			  if(m_pPushWell != NULL)
				  m_pPushWell->Terminate();
			  if(m_pPullWell != NULL)
				  m_pPullWell->Terminate();
		  }
	  }
	  else
	  {
		  if(state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		  {
			  //close all logic here
			  if(m_pPushWell != NULL)
				  m_pPushWell->Terminate();
			  if(m_pPullWell != NULL)
				  m_pPullWell->Terminate();
		  }
	  }

	

		if(CGame::GetInstance()->GetControllerLayout() == 0 || CGame::GetInstance()->GetControllerLayout() == 2)
		{
			if(state.Gamepad.sThumbLX < -(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*2) && m_fMovementTimer > MOVEMENT_DELAY)
			{
				SetVelX( GetVelVec().x - PLAYER_MOVE); // change vel vector
				m_bLeftHeld = true; //set that it was changed
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_PRESSED);
				CAnimationManager::GetInstance()->SetCurrentDir(m_nAnimID, DIR_LEFT);
				m_fMovementTimer = 0.0f;
			}
			else if(state.Gamepad.sThumbLX > -(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*2) && state.Gamepad.sThumbLX < (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*2) && m_bInWell == false )
			{
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_RELEASED);
				if(!m_bInWell)
					this->SetVelX(0);
			}
		}
		else if(CGame::GetInstance()->GetControllerLayout() == 1)
		{
			if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT && m_fMovementTimer > MOVEMENT_DELAY)
			{
				SetVelX( GetVelVec().x - PLAYER_MOVE); // change vel vector
				m_bLeftHeld = true; //set that it was changed
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_PRESSED);
				CAnimationManager::GetInstance()->SetCurrentDir(m_nAnimID, DIR_LEFT);
				m_fMovementTimer = 0.0f;
			}
			else if(!(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) && !(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))
			{
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_RELEASED);
				if(!m_bInWell)
					this->SetVelX(0);
			}
		}

		if(CGame::GetInstance()->GetControllerLayout() == 0 || CGame::GetInstance()->GetControllerLayout() == 2)
		{
			//if((key.Flags == XINPUT_KEYSTROKE_KEYDOWN || key.Flags == XINPUT_KEYSTROKE_REPEAT) && key.VirtualKey == VK_PAD_LTHUMB_RIGHT)
			if(state.Gamepad.sThumbLX > (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*2) && m_fMovementTimer > MOVEMENT_DELAY)
			{
				SetVelX( GetVelVec().x + PLAYER_MOVE);
				m_bRightHeld = true;
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_PRESSED);
				CAnimationManager::GetInstance()->SetCurrentDir(m_nAnimID, DIR_RIGHT);
				m_fMovementTimer = 0.0f;
			}
			else if(state.Gamepad.sThumbLX < (XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*2) && state.Gamepad.sThumbLX > -(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*2) && m_bInWell == false )
			{
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_RELEASED);
				if(!m_bInWell)
					this->SetVelX(0);
			}
		}
		else if(CGame::GetInstance()->GetControllerLayout() == 1)
		{
			if(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT && m_fMovementTimer > MOVEMENT_DELAY)
			{
				SetVelX( GetVelVec().x + PLAYER_MOVE);
				m_bRightHeld = true;
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_PRESSED);
				CAnimationManager::GetInstance()->SetCurrentDir(m_nAnimID, DIR_RIGHT);
				m_fMovementTimer = 0.0f;
			}
			else if(!(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) && !(state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT))
			{
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_RELEASED);
				if(!m_bInWell)
					this->SetVelX(0);
			}


		}

		if(CGame::GetInstance()->GetControllerLayout() == 0)
		{
			if((state.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || state.Gamepad.wButtons & XINPUT_GAMEPAD_A) && m_bHasLanded)
			{
				m_bHasLanded = false; //not landed any more
				SetPosY( GetPosVec().y - 4.0f);
				SetVelY( GetVelVec().y - PLAYER_JUMP); //launch up
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, JUMP_BTN_PRESSED);
			}
		}
		else if(CGame::GetInstance()->GetControllerLayout() == 1)
		{
			if((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP || state.Gamepad.wButtons & XINPUT_GAMEPAD_A) && m_bHasLanded)
			{
				m_bHasLanded = false; //not landed any more
				SetPosY( GetPosVec().y - 4.0f);
				SetVelY( GetVelVec().y - PLAYER_JUMP); //launch up
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, JUMP_BTN_PRESSED);
			}
		}
		else
		{
			if((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER || state.Gamepad.wButtons & XINPUT_GAMEPAD_A) && m_bHasLanded)
			{
				m_bHasLanded = false; //not landed any more
				SetPosY( GetPosVec().y - 4.0f);
				SetVelY( GetVelVec().y - PLAYER_JUMP); //launch up
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, JUMP_BTN_PRESSED);
			}
		}

		if(state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_tRelativeMousePt.x -= 2;
			if(m_tRelativeMousePt.x < 0)
				m_tRelativeMousePt.x = 0;
		}
		if(state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_tRelativeMousePt.x += 2;
			if(m_tRelativeMousePt.x > WorldConstants::fDefaultRWall)
				m_tRelativeMousePt.x = WorldConstants::fDefaultRWall;
		}
		if(state.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_tRelativeMousePt.y -= 2;
			if(m_tRelativeMousePt.y < 0)
				m_tRelativeMousePt.y = 0;
		}
		if(state.Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			m_tRelativeMousePt.y += 2;
			if(m_tRelativeMousePt.y > WorldConstants::fDefaultFloor - 96)
				m_tRelativeMousePt.y = WorldConstants::fDefaultFloor - 96;
		}
	}
	else
		m_bControllerConnected = false;

		if(m_bControllerConnected == false)
		{
			if(m_pDI->KeyDown(CGame::GetInstance()->GetKMoveLeft()))//if key was held for left
			{
				SetVelX( GetVelVec().x - PLAYER_MOVE); // change vel vector
				m_bLeftHeld = true; //set that it was changed
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_PRESSED);
				CAnimationManager::GetInstance()->SetCurrentDir(m_nAnimID, DIR_LEFT);
			}

			if(m_pDI->KeyDown(CGame::GetInstance()->GetKMoveRight())) //same chack as above
			{
				SetVelX( GetVelVec().x + PLAYER_MOVE);
				m_bRightHeld = true;
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_PRESSED);
				CAnimationManager::GetInstance()->SetCurrentDir(m_nAnimID, DIR_RIGHT);
			}

			if(!m_pDI->KeyDown(CGame::GetInstance()->GetKMoveLeft()) && !m_pDI->KeyDown(CGame::GetInstance()->GetKMoveRight()))
			{
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_RELEASED);
				if(!m_bInWell)
					this->SetVelX(0);
			}

			if(m_pDI->KeyPressed(CGame::GetInstance()->GetKJump()) && m_bHasLanded) //if they have landed and press space
			{
				m_bHasLanded = false; //not landed any more
				SetPosY( GetPosVec().y - 4.0f);
				SetVelY( GetVelVec().y - PLAYER_JUMP); //launch up
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, JUMP_BTN_PRESSED);
			}
		}

		// mouse movementx and y is the mouse velocity that returns a value based on how fast you moved the mouse
	if(m_bControllerConnected == false)
		m_tRelativeMousePt.x += (float)(m_pDI->MouseMovementX());

	if(m_tRelativeMousePt.x > WorldConstants::fDefaultRWall)
		m_tRelativeMousePt.x = WorldConstants::fDefaultRWall;
	if(m_tRelativeMousePt.x < WorldConstants::fDefaultLWall)
		m_tRelativeMousePt.x = WorldConstants::fDefaultLWall;

	if(m_bControllerConnected == false)
		m_tRelativeMousePt.y += (float)(m_pDI->MouseMovementY());

	if(m_tRelativeMousePt.y > WorldConstants::fDefaultFloor - 96)
		m_tRelativeMousePt.y = WorldConstants::fDefaultFloor - 96;
	if(m_tRelativeMousePt.y < WorldConstants::fDefaultCeiling)
		m_tRelativeMousePt.y = WorldConstants::fDefaultCeiling;

	CalcShotVec();

	//Checking to see if the shooting is done by mouse or not
	if(CGame::GetInstance()->GetKShootPull() >= 0 && CGame::GetInstance()->GetKShootPull() <= 2)
	{
		if(m_pDI->MouseButtonPressed(CGame::GetInstance()->GetKShootPull()))
		{
			FirePullWell();
		}
	}
	else
	{
		if(m_pDI->KeyPressedEx(CGame::GetInstance()->GetKShootPull()))
		{
			FirePullWell();
		}
	}
	//Same thing but for push well
	if(CGame::GetInstance()->GetKShootPush() >= 0 && CGame::GetInstance()->GetKShootPush() <= 2)
	{
		if(m_pDI->MouseButtonPressed(CGame::GetInstance()->GetKShootPush()) && (CGame::GetInstance()->GetLoadLevel() > 5 || CGame::GetInstance()->GetIsCustom()))
		{
			FirePushWell();
		}
	}
	else
	{
		if(m_pDI->KeyPressedEx(CGame::GetInstance()->GetKShootPush())&& (CGame::GetInstance()->GetLoadLevel() > 5  || CGame::GetInstance()->GetIsCustom()))
		{
			FirePushWell();
		}
	}

	//Same thing but for close all
	if(CGame::GetInstance()->GetKCloseAll() >= 0 && CGame::GetInstance()->GetKCloseAll() <= 2)
	{
		if(m_pDI->MouseButtonPressed(CGame::GetInstance()->GetKCloseAll()))
		{
			//close all logic here
			if(m_pPushWell != NULL)
				m_pPushWell->Terminate();
			if(m_pPullWell != NULL)
				m_pPullWell->Terminate();
		}
	}
	else
	{
		if(m_pDI->KeyPressedEx(CGame::GetInstance()->GetKCloseAll()))
		{
			//close all logic here
			if(m_pPushWell != NULL)
				m_pPushWell->Terminate();
			if(m_pPullWell != NULL)
				m_pPullWell->Terminate();
		}
	}
}

void CPlayer::Render()
{
	/*RECT drawRect = GetDrawRect();

	float drawX = GetPosVec().x - (GetSizeVec().x / 2.0f);
	float drawY = GetPosVec().y - (GetSizeVec().y / 2.0f);*/

	RECT targetDrawRect;
	targetDrawRect.top = 0;
	targetDrawRect.left = 0;
	targetDrawRect.bottom = 8;
	targetDrawRect.right = 8;

	if(GetObjState() != OBJ_DEAD)
	{
		

		if(m_bFlashing == false)
		{
			CAnimationManager::GetInstance()->SetPlayerInvisible(false);
			m_bInvisible = false;
			m_fFlashTick = 0.0f;
		}
		else if(m_fFlashTick > .15f)
		{
			m_fFlashTick = 0.0f;
			m_bInvisible = !m_bInvisible;
			CAnimationManager::GetInstance()->SetPlayerInvisible(m_bInvisible);
			
		}
		
		
		CSGD_TextureManager::GetInstance()->Draw(m_nTargetImageID, (int)m_tRelativeMousePt.x - 4, (int)m_tRelativeMousePt.y - 4, 1.0f, 1.0f, &targetDrawRect);
		
		
		if(m_bDying)
		{
			float drawX = GetPosVec().x - (GetSizeVec().x / 2.0f);
			float drawY = GetPosVec().y - (GetSizeVec().y / 2.0f);
			if(m_fTrailTimer < 1.0f)
				CParticleManager::GetInstance()->SpawnParticle("Grav_Trail",drawX,drawY);
			else
			{
			}
		}
		else
		{
		   CParticleManager::GetInstance()->KillParticle("Grav_Trail");
		}
	}
}
void CPlayer::CollisionReact(int CollideObjType, int nAxis, float fSpeed)
{
	if(fSpeed >= WorldConstants::fTermVel)
	{
		//kill player on highspeed collisions here then return
	}

	

	switch(CollideObjType) //swtich based on type of object. this will allow for specific reactions
	{
	case TYPE_ABSORBPLAT:
		{
			if(this->GetTermVelIsTrue())
			{
				CParticleManager::GetInstance()->SpawnParticle("Blood",GetPosVec().x - (GetSizeVec().x / 2), GetPosVec().y - (GetSizeVec().y / 2));
				CGamePlayState::GetInstance()->PlayerHit();
				m_bKill = true;
			}
		}
		
	case TYPE_HOVERPLAT:
		{
			if(this->GetTermVelIsTrue())
			{
				CParticleManager::GetInstance()->SpawnParticle("Blood",GetPosVec().x - (GetSizeVec().x / 2), GetPosVec().y - (GetSizeVec().y / 2));
				CGamePlayState::GetInstance()->PlayerHit();
				m_bKill = true;
			}
		}
	case TYPE_GRAVOBJ:
		{
			if(FloatEquals(GetVelVec().y, 0.0f, 0.01f)) //if we're clliding with a gravobj and not moving up or down
			{
				m_bHasLanded = true; //we've landed
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, VELY_ZERO);
			}

			if(m_bInWell == true ||  (m_bHasLanded == true && nAxis == AXIS_XONLY)) //if im in a well bounce off
				BounceBack(this, nAxis, false);

			if(GetVelVec().y > 0.0f) //if i am moving downward(falling) //will need to be changed to reflect local gravity
			{
				SetVelY(0.0f); // stop vert movement
				//set Landed to true
				if(!m_bHasLanded && GetVelVec().y > 0.0f)
				{
					m_bLeftHeld = false;
					m_bRightHeld = false; //set the key held bols to false so that the input doesnt correct the velocity again
					SetVelX(0.0f);
				}
				m_bHasLanded = true;
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, VELY_ZERO);
			}
			if( (m_bLeftHeld == true || m_bRightHeld == true) && nAxis == AXIS_XONLY) //if the player was moving and it was only a sidee collision
			{
				SetVelX(0.0f); //set vel to 0 after the bounce back
				m_bLeftHeld = false;
				m_bRightHeld = false; //set the key held bols to false so that the input doesnt correct the velocity again
			}
			break;
		}
	case TYPE_WALKER:
	case TYPE_HEAVY:
	case TYPE_SNIPER:
	case TYPE_BOMBARD:
	case TYPE_HAZARD:
		{
			if(m_bFlashing == false)
			{
				CParticleManager::GetInstance()->SpawnParticle("Blood",GetPosVec().x - (GetSizeVec().x / 2), GetPosVec().y - (GetSizeVec().y / 2));
				
				m_bKill = true;
				CGamePlayState::GetInstance()->PlayerHit();
			}
			if(m_bFlashing == true && CollideObjType == TYPE_HAZARD)
			{
				BounceBack(this, AXIS_BOTH, false);
			}
			break;
		}
	case TYPE_EXIT:
		{
			//CGame::GetInstance()->PopState();
			//CGame::GetInstance()->PushState(CLevelSelect::GetInstance());
			//CGamePlayState::GetInstance()->
			//CGame::GetInstance()->PopState();
			//CGame::GetInstance()->PushState(CMainMenuState::GetInstance());
			//check for achievments for beating the game

			if(CGame::GetInstance()->GetLoadLevel() == 20)
			{
				switch(CGame::GetInstance()->GetDifficulty())
				{
				case DIFF_EASY:
					{
						CGame::GetInstance()->CheckSaveAchieves(ACHIEV_EASYBEAT);
						break;
					}
				case DIFF_NORMAL:
					{
						CGame::GetInstance()->CheckSaveAchieves(ACHIEV_NORMBEAT);
						break;
					}
				case DIFF_HARD:
					{
						CGame::GetInstance()->CheckSaveAchieves(ACHIEV_HARDBEAT);
						break;
					}
				}

				//check fror waranty achieve
				if(CGame::GetInstance()->GetDurationLevel() == 0 &&
					CGame::GetInstance()->GetRangeLevel() == 0 &&
					CGame::GetInstance()->GetRechargeLevel() == 0 &&
					CGame::GetInstance()->GetDifficulty() > 0 )
				{
					CGame::GetInstance()->CheckSaveAchieves(ACHIEV_NOUPGRADE);
				}

				

				//check for gravitational champ achieve
				if(CGame::GetInstance()->GetDifficulty() == DIFF_HARD && CGame::GetInstance()->GetSaveFile()->GetSaveData()->tAllSaves[CGame::GetInstance()->GetCurrentSaveSlot()].bEverDied == false)
				{
					CGame::GetInstance()->CheckSaveAchieves(ACHIEV_CHAMP);
				}
			}
			
			if(CGame::GetInstance()->GetIsCustom() == true)
				{
					CGame::GetInstance()->CheckSaveAchieves(ACHIEV_CUSTOMLVL);
				}
			CMessageSystem::GetInstance()->SendMsg(new CExitLevelMessage(true, CGamePlayState::GetInstance()->GetRecordTime()));
			break;
		}
	}
}
RECT CPlayer::GetCollideRect()
{
		return CBaseObject::GetCollideRect();
}
void CPlayer::CalcShotVec()
{
	//set shot angle to the normalized vector between player and the relative mouse
	D3DXVec2Normalize(&m_tShotVec, &(m_tRelativeMousePt - GetPosVec()));
}
void CPlayer::NullWell(bool PushPull)
{
	if(PushPull && m_pPushWell !=  NULL) // if the well is push (pushpull = true)
	{
		//terminate the well and set to null
		m_pPushWell->Release();

		m_pPushWell = NULL;
	}
	else if (!PushPull && m_pPullWell != NULL)
	{
		m_pPullWell->Release();

		m_pPullWell = NULL;
	}
}
void CPlayer::SetWell(bool PushPull, CGravWell* newWell)
{
	//if the well is push (push pull = true)
	if(PushPull)
	{
		//if the well isnt null and is not the new well, null it
		if(m_pPushWell != NULL && m_pPushWell != newWell)
		{
			m_pPushWell->Terminate();
		}
		m_pPushWell = newWell;
	}
	else //same check other well
	{
		if(m_pPullWell != NULL && m_pPullWell != newWell)
		{
			m_pPullWell->Terminate();
		}

		m_pPullWell = newWell;
	}
	m_pShot = NULL;
}
CGravWell* CPlayer::GetWell(bool PushPull)
{
	if(PushPull) //same bool as above
		return m_pPushWell;
	else
		return m_pPullWell;
}

void CPlayer::FirePullWell()
{
//pull well logic here
		if(m_bShotOut && m_pShot != NULL) //if theres a shot out
		{
			m_pShot->Open();
			m_bShotOut = false;
		}
		else //if no shot out
		{
			//call CGame spawn shot here
			//change to message later
			CGamePlayState::GetInstance()->CreatePlayerShot(false);
			m_bShotOut = true;
		}
}
void CPlayer::FirePushWell()
{
	//push well logic here

		if(m_bShotOut && m_pShot != NULL) //if theres a shot out
		{
			m_pShot->Open();

			m_bShotOut = false;
		}
		else //if no shot out
		{
			//call CGame spawn shot here
			//change to message later
			CGamePlayState::GetInstance()->CreatePlayerShot(true);
			m_bShotOut = true;
		}
}

void CPlayer::TerminateWell(CGravWell* deadWell)
{
	if(deadWell == m_pShot) //if it was the players shot
		{
			ShotDied(); // tell them it died
		}
		else if(deadWell->IsItOpen()) //if not is it open?
		{
			m_pGS->RemoveWell(deadWell);
			if(deadWell == m_pPullWell || deadWell == m_pPushWell) // is it one of our wells?
			{
					NullWell(deadWell->GetPushPull()); // null that well
			}
		}
}

void CPlayer::HandleEvent(CEvent* pEvent)
{
	if(pEvent->GetEventID() == "GravWell Open")
	{
		CGravWell* openWell = (CGravWell*)(pEvent->GetParam());

		if(openWell == m_pShot)
		{
			SetWell(openWell->GetPushPull(), openWell);
			m_pShot = NULL;
			m_bShotOut = false;
		}
	}
}
void CPlayer::ShieldToggle(bool onOff)
{
	if(onOff && m_nShieldID == -1)
	{
		m_nShieldID = CParticleManager::GetInstance()->SpawnParticle("PlayerShield", 0.0f, 0.0f,this); 
	}
	else if( !onOff && m_nShieldID != -1)
	{
		CParticleManager::GetInstance()->KillParticle(m_nShieldID);
		m_nShieldID = -1;
	}
}