/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CWalkerEnemy.h"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Declares the CWalker enemy which will be the basic enemy. This will establish base values for it and then from here
//			update will handle the rest. Only position should need to be set and then the game engine will handle the rest.
//			The heavy will be derived from this.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CWalkerEnemy.h"

//other #includes
#include "CPlayer.h"
#include <d3dx9math.h>
#include "CGravSystem.h"
#include "CTileManager.h"
#include "CGame.h"
#include "CParticleManager.h"
#include "CAnimationManager.h"
#include "CAssetState.h"
#include "SGD Wrappers/SGD_Math.h"

//Speeds pending difficulty
#define WALKER_SPEED (20.0f * (CGame::GetInstance()->GetDifficulty() + 1))

CWalker::CWalker() : CGravObject(TYPE_WALKER)
{
	m_fAttackRange = 0.0f;
	m_bInWell = false;
	m_bGravImmune = false;
	m_pPlayer = NULL;
	SetVelX( -WALKER_SPEED);
	m_fGravResist = 0.0f;
	m_bFaceDir = true;
	m_bGrounded = false;
	m_bAuraSwitch = false;
	m_nAuraID = -1;
}

CWalker::~CWalker()
{
	if(m_bAuraSwitch)
	{
		CParticleManager::GetInstance()->KillParticle(m_nAuraID);
	}
}

CWalker::CWalker(int nType) : CGravObject(nType)
{
	m_fAttackRange = 0.0f;
	m_bInWell = false;
	m_bGravImmune = false;
	m_pPlayer = NULL;
	SetVelX( -WALKER_SPEED);
}

void CWalker::CheckAI()
{
	if(CheckPlayerLoc())
	{
		//check will need to be refined based on default gravity direction
		if(GetPosVec().x > m_pPlayer->GetPosVec().x)
		{
			SetVelX( - (2.0f * WALKER_SPEED));
		}
		else if(GetPosVec().x < m_pPlayer->GetPosVec().x)
		{
			SetVelX((2.0f * WALKER_SPEED));
		}
	}
	else
	{
		if(m_bFaceDir) // if facing left vel is - in the x
		{
			SetVelX( - WALKER_SPEED );
		}
		else //other wise its positive
		{
			SetVelX( WALKER_SPEED );
		}
		//finds the direction of the walker (direction / abs(direction) ), and then multiplies it by walker speed
	}
	//Checking to see if we're about to fall off the platform.  If so, reverse direction.
}

bool CWalker::CheckPlayerLoc()
{
	if(m_pPlayer != NULL)
	{
		D3DXVECTOR2 meToPlayer = m_pPlayer->GetPosVec() - GetPosVec();

		if( abs( D3DXVec2Length(&meToPlayer) ) <= abs(m_fAttackRange) )
		{
			return true;
		}
	}

		return false;
}

void CWalker::Update(float fDT)
{

	if(m_bGravImmune && !m_bAuraSwitch)
	{
		m_nAuraID = CParticleManager::GetInstance()->SpawnParticle("ImmuneAura", 0.0f, 0.0f, this);
		m_bAuraSwitch = true;
	}

	if(m_pGS == NULL) // if it hasnt checked the grav system yet, check it
	{
		m_pGS = CGravSystem::GetInstance();
	}

	if(GetObjState() != OBJ_DEAD)
	{

		if(GetVelVec().x <= -20.0f)
		{
			CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_PRESSED);
			CAnimationManager::GetInstance()->SetCurrentDir(m_nAnimID, DIR_LEFT);
		}
		else if(GetVelVec().x >= 20.0f)
		{
			CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_PRESSED);
			CAnimationManager::GetInstance()->SetCurrentDir(m_nAnimID, DIR_RIGHT);
		}

		//check if is in a well
		if( m_pGS->GetGravity( GetPosVec(), this->GetType() ) == m_pGS->GetTileOnlyGrav( GetPosVec() ) )
		{
			m_bInWell = false;
		}
		else
		{
			if(m_bInWell == false) // is it just getting into the well?
			{
				//stop the enemey lol
				//CAnimationManager::GetInstance()->PerformAction(m_nAnimID, VELY_ZERO);
			}
			m_bInWell = true; //now we're in the well
			m_bGrounded = false;
		}

		//first check AI to update velocity direction and speed based on player location
		if(!m_bInWell && m_bGrounded) //if not in a well
		{
			CheckAI();
			SetVelVec( GetVelVec() + ( m_pGS->GetTileOnlyGrav( GetPosVec() ) * fDT)); //only use tile gravity
			if(CTileManager::GetInstance()->CheckPit(this))
			{
				SetVelX(-GetVelVec().x);
				m_bFaceDir = !m_bFaceDir;
			}
			/*
			else if(CTileManager::GetInstance()->CheckSpikes(this))
			{
				SetVelX(-GetVelVec().x);
				m_bFaceDir = !m_bFaceDir;
			}
			*/

			if( GetVelVec().y > WorldConstants::fGravity + 0.5f)
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, POS_VELY_AND_OUT_OF_GRAV_WELL);
		}
			//check immune
		else if(m_bGravImmune)//if immune to gravity but in a well
		{
			CheckAI();
			SetVelVec( GetVelVec() + ( m_pGS->GetTileOnlyGrav( GetPosVec() ) * fDT)); //only use tile gravity
			if(CTileManager::GetInstance()->CheckPit(this))
			{
				SetVelX(-GetVelVec().x);
				m_bFaceDir = !m_bFaceDir;
			}
		}
		else //if not immune and in a well
		{
			SetVelVec( GetVelVec() + ( CheckGravity() * fDT * (1.0f - m_fGravResist) ) ); //use normal grav object calculations * its resistance
			CAnimationManager::GetInstance()->PerformAction(m_nAnimID, IN_GRAV_WELL);
		}

		//usual motion update based on CBase object
		CBaseObject::Update(fDT);

		if(GetPosVec().y + (GetSizeVec().y / 2.0f) > m_fFloorValue)
		{
			SetPosY(m_fFloorValue -  (GetSizeVec().y / 2.0f)); // set to floor
			SetVelY(0.0f); // stop vert movement
			CAnimationManager::GetInstance()->PerformAction(m_nAnimID, VELY_ZERO);
			m_bGrounded = true;
		}
		if(GetPosVec().y - (GetSizeVec().y / 2.0f) < m_fCeilingValue)
		{
			SetPosY(m_fCeilingValue + (GetSizeVec().y / 2.0f));
			SetVelY(0.0f);
		}

		if(GetPosVec().x  > m_fRWallValue - (GetSizeVec().x / 2.0f))
		{
			SetPosX(m_fRWallValue -  (GetSizeVec().x / 2.0f)); // set to floor
			m_bFaceDir = true;
		}
		if(GetPosVec().x <  m_fLWallValue + (GetSizeVec().x / 2.0f))
		{
			SetPosX(m_fLWallValue + (GetSizeVec().x / 2.0f)); // set to floor
			SetVelX(WALKER_SPEED); // stop horz movement
			m_bFaceDir = false;
		}

		//Keep in main window checks

		if(GetPosVec().x > WorldConstants::fDefaultRWall - (GetSizeVec().x / 2.0f) && GetVelVec().x > 0.0f)
		{
			SetPosX(WorldConstants::fDefaultRWall - (GetSizeVec().x / 2.0f)); // set to floor
			SetVelX( -WALKER_SPEED  ); // stop horz movement
		}
		else if (GetPosVec().x < WorldConstants::fDefaultLWall + (GetSizeVec().x / 2.0f) && GetVelVec().x < 0.0f)
		{
			SetPosX(WorldConstants::fDefaultLWall +  (GetSizeVec().x / 2.0f)); // set to floor
			SetVelX( WALKER_SPEED ); // stop horz movement
		}
		if(GetPosVec().y + (GetSizeVec().y / 2.0f) > WorldConstants::fDefaultFloor - 96.0f)
			SetPosY( WorldConstants::fDefaultFloor - 97.0f - (GetSizeVec().y / 2.0f));
	}
}

void CWalker::Render()
{
}

void CWalker::CollisionReact(int CollideObjType, int nAxis, float fSpeed)
{
	if(CollideObjType == TYPE_HAZARD)
	{
		//Do some collision crap here.
		//Create an explosion particle effect
		if(!m_bInWell && m_bGrounded)
		{	

			SetVelVec( - GetVelVec() );
			m_bFaceDir = !m_bFaceDir;
			SetPosX( GetPosVec().x + (GetVelVec().x * .25f));
		}
		else
		{
			//CAssetState::GetInstance()->PlaySound("KillSound", 

			CAssetState::GetInstance()->PlaySound("KillSound", rand() % 3 + 1);
			CParticleManager::GetInstance()->SpawnParticle("Explosion", GetPosVec().x, GetPosVec().y);

			SetObjState(OBJ_DEAD);
			CAnimationManager::GetInstance()->EndAnimation(m_nAnimID);
		}
	}
	else if(CollideObjType == TYPE_HOVERPLAT || CollideObjType == TYPE_ABSORBPLAT)
	{
			if(FloatEquals(GetVelVec().y, 0.0f, 0.01f)) //if we're clliding with a gravobj and not moving up or down
			{
				m_bGrounded = true; //we've landed
				//CAnimationManager::GetInstance()->PerformAction(m_nAnimID, VELY_ZERO);
			}

			if(m_bInWell == true ||  (m_bGrounded == true && nAxis == AXIS_XONLY)) //if im in a well bounce off
				BounceBack(this, nAxis, false);

			if(GetVelVec().y > 0.0f) //if i am moving downward(falling) //will need to be changed to reflect local gravity
			{
				SetVelY(0.0f); // stop vert movement
				//set Landed to true
				if(!m_bGrounded && GetVelVec().y > 0.0f)
				{
					SetVelX(0.0f);
				}
				m_bGrounded = true;
				//CAnimationManager::GetInstance()->PerformAction(m_nAnimID, VELY_ZERO);
			}
			
			
	}
		
}

bool CWalker::CheckCollision(CBaseObject * pbj)
{
	if(pbj->GetType() != TYPE_EXIT && pbj->GetType() != TYPE_POWERUP && pbj->GetType() != TYPE_WALKER
		&& pbj->GetType() != TYPE_HEAVY && pbj->GetType() != TYPE_SNIPER && pbj->GetType() != TYPE_BOMBARD
		&& pbj->GetType() != TYPE_PARTICLE)
		return CBaseObject::CheckCollision(pbj);

	return false;
}