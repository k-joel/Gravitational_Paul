/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CGravWell.cpp"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Defines the Gravity well objects that will dynamically alter gravity.
//			These will derive from Cbase object, and will add themselves to the gravity system on opening
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CGravWell.h"

//additional includes
#include "CGravSystem.h"
#include "CPlayer.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include "CEventSystem.h"
#include "CZone.h"
#include "SGD Wrappers/CSGD_FModManager.h"
#include "CTileManager.h"
#include "CParticleManager.h"
#include "CAssetState.h"
#include "CAnimationManager.h"
#include "CBombardEnemy.h"

using namespace WorldConstants; //using worldconstants namespace to have easy access to these variables
								//and them not be global

CGravWell::CGravWell() : CBaseObject(TYPE_GRAVWELL)
{
	m_fForce = fGravity;
	m_fRange = 5.0f * fTileSize; // based on tilesize defined in world constants namespace
	m_fDuration = 3.0f; //3 second default time
	m_bIsOpen = false; //defaults to closed
	m_bPushPull = false; //defaults to a pull
	m_bTemporary = true; //default wells are temporary
	m_pGS = NULL;
	m_pOwner = NULL; //set pointers to null. Owner will Set ownership, GravSystem set when opened
	m_bTerminated = false;
	m_nClosedImage = -1;
	m_bZoneFlipped = false;
	m_bBounce = false;
	m_bWeighted = false;
	m_bRebounded = false;

	m_bSparkSet = false;

	m_nShotDissipateSoundID = CAssetState::GetInstance()->GetSoundID("ShotDissipateSoundID");
	m_nShotBounceSoundID = CAssetState::GetInstance()->GetSoundID("ShotBounceSoundID");

	//event registration
	CEventSystem::GetInstance()->RegisterClient("Kill Well", this);
	m_nBounceCounter = 0;
	m_nShotParticles = -1;


	
}

CGravWell::~CGravWell()
{
	//CParticleManager::GetInstance()->KillParticle("GravWell");

	CEventSystem::GetInstance()->UnregisterClient("Kill Well", this);
}

void CGravWell::Open()
{
	//Get grav well system if it is NULL

	if(m_bIsOpen == false)
	{
		//Event for Open
		CGravSystem::GetInstance()->AddWell(this);
		if(m_pOwner != NULL)
		{
			if(m_pOwner->GetType() == TYPE_PLAYER)
			{
				((CPlayer*)(m_pOwner))->SetWell(GetPushPull(), this);
				CAnimationManager::GetInstance()->EndAnimation(m_nClosedWellAnimID);
			}
		}

		SetSize(4.0f * WorldConstants::fTileSize, 4.0f * WorldConstants::fTileSize);
		SetObjState(OBJ_NORMAL);

	//sets well to open
		m_bIsOpen = true;

	//sets velocity to 0
		SetVelX(0.0f);
		SetVelY(0.0f);
		float drawX = GetPosVec().x - (GetSizeVec().x / 2.0f);
		float drawY = GetPosVec().y - (GetSizeVec().y / 2.0f);
		
		CParticleManager::GetInstance()->KillParticle(m_nShotParticles);

		if(m_bPushPull)
			m_nGravWellParticleID = CParticleManager::GetInstance()->SpawnParticle("PushWell", 0.0f , 0.0f , this);
		else
			m_nGravWellParticleID = CParticleManager::GetInstance()->SpawnParticle("PullWell", 0.0f , 0.0f , this);
	}
}

void CGravWell::Terminate()
{
	if(!m_bTerminated)
	{
		m_bTerminated = true;
		//tell GravSystem to remove the well

		//Event for termination
		if(m_pGS == NULL)
		{
			m_pGS = CGravSystem::GetInstance();
		}
		m_pGS->RemoveWell(this);
		if(m_pOwner != NULL)
		{
			if(m_pOwner->GetType() == TYPE_PLAYER)
			{
				((CPlayer*)(m_pOwner))->TerminateWell(this);
				CAnimationManager::GetInstance()->EndAnimation(m_nClosedWellAnimID);
			}
			else if(m_pOwner->GetType() == TYPE_BOMBARD)
			{
				((CBombardier*)(m_pOwner))->DropWell();
			}
		}

		SetObjState(OBJ_DEAD);

		if(m_bIsOpen)
			CParticleManager::GetInstance()->KillParticle(m_nGravWellParticleID);
		else
			CParticleManager::GetInstance()->KillParticle(m_nShotParticles);
	}
}

void CGravWell::Update(float fDT)
{
	if(m_pGS == NULL)
	{
		m_pGS = CGravSystem::GetInstance();
	}
	if(GetObjState() != OBJ_DEAD)
	{
		if(m_bIsOpen) //if the well is open
		{
			m_fDuration -= fDT; //decrement duration

			if(m_fDuration <= 0.0f && m_bTemporary) // if no time is left in life
			{
				Terminate(); //terminate self
			}
		}
		else //if the well is not open
		{
			if(m_bSparkSet == false)
			{
				if(m_bPushPull)
				{
					m_nShotParticles = CParticleManager::GetInstance()->SpawnParticle("PushShot", 0.0f , 0.0f , this);
				}
				else
				{
					m_nShotParticles = CParticleManager::GetInstance()->SpawnParticle("PullShot", 0.0f , 0.0f , this);
					
				}
				m_bSparkSet = true;

			}
			CTileManager::GetInstance()->SetObjectPseudoWalls(this);

			if(m_bWeighted)
				SetVelVec(GetVelVec() + m_pGS->GetGravity(GetPosVec()) * fDT * .50f);

			CBaseObject::Update(fDT); //update movement based on time

			if(GetPosVec().y  - (GetSizeVec().y / 2.0f) < m_fCeilingValue || GetPosVec().y + (GetSizeVec().y / 2.0f) > m_fFloorValue) //outside of window or collided
			{
				if(m_bBounce && m_nBounceCounter < 3)
				{
					SetPosVec( GetPosVec() - (GetVelVec() * fDT));
						BounceBack(this, AXIS_YONLY, false);
						m_nBounceCounter++;
						//CSGD_FModManager::GetInstance()->PlaySoundA(m_nShotBounceSoundID);
						CAssetState::GetInstance()->PlaySound(m_nShotBounceSoundID);
				}
				else
				{
					Terminate(); //then terminate
					m_nBounceCounter = 0;
					//CSGD_FModManager::GetInstance()->PlaySoundA(m_nShotDissipateSoundID);
					CAssetState::GetInstance()->PlaySound(m_nShotDissipateSoundID);
				}
			}
			else if(GetPosVec().x + (GetSizeVec().x / 2.0f) > m_fRWallValue || GetPosVec().x  - (GetSizeVec().x / 2.0f) < m_fLWallValue)
			{
				if(m_bBounce && m_nBounceCounter < 3)
				{
						SetPosVec( GetPosVec() - (GetVelVec() * fDT));
						BounceBack(this,AXIS_XONLY, false);
						m_nBounceCounter++;
						//CSGD_FModManager::GetInstance()->PlaySoundA(m_nShotBounceSoundID);
						CAssetState::GetInstance()->PlaySound(m_nShotBounceSoundID);
				}
				else
				{
					Terminate();
					m_nBounceCounter = 0;
					//CSGD_FModManager::GetInstance()->PlaySoundA(m_nShotDissipateSoundID);
					CAssetState::GetInstance()->PlaySound(m_nShotDissipateSoundID);
				}
			}

		
		}
	}
}

void CGravWell::CollisionReact(int CollideObjType, int nAxis, float fSpeed)
{
}

void CGravWell::Render()
{
	
}
void CGravWell::HandleEvent(CEvent* pEvent)
{
		if(pEvent->GetEventID() == "Kill Well")
		{
			CGravWell* deadWell = (CGravWell*)(pEvent->GetParam());
			if(deadWell)
			{
				if(deadWell == this && GetObjState() != OBJ_DEAD)
				{
					Terminate();
				}
			}
		}
}

void CGravWell::Close()
{
	if(m_bIsOpen)
	{
		CParticleManager::GetInstance()->KillParticle(m_nGravWellParticleID);
		m_bIsOpen = false;
		SetObjState(OBJ_NORMAL);
		if(m_pGS == NULL)
		{
			m_pGS = CGravSystem::GetInstance();
		}

		m_pGS->RemoveWell(this);
	}
}