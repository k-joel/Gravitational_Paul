/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CSniperEnemy.h"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Declares a stationary sniper enemy class that will shoot player grav wells out of the sky. This will then be derived
//			into the bombardier.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CSniperEnemy.h"

//additional #includes here
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "CGravSystem.h"
#include "CGravWell.h"
#include "CAnimationManager.h"
#include "CAssetState.h"
#include "CParticleManager.h"
using namespace WorldConstants;

CSniper::CSniper() : CGravObject(TYPE_SNIPER)
{
	m_fAttackRange = DEFAULT_SNIPER_RANGE;
	m_fGravResist = 0.0f;
	m_bGravImmune = false;
	m_bInWell = false;

	m_bShotOut = false;
	m_tBulletLoc = GetPosVec();
	m_nBulletImageID = -1;
	m_fShotDelay = DEFAULT_SNIPER_SHOTDELAY;
	m_fCurrentTime = 0.0f;

	m_bAuraSwitch = false;
	m_nAuraID = -1;

	//m_nAnimID = CAnimationManager::GetInstance()->StartAnimation("Sniper", this);
}

CSniper::~CSniper()
{
	if(m_bAuraSwitch)
	{
		CParticleManager::GetInstance()->KillParticle(m_nAuraID);
	}
}

CSniper::CSniper(int nType) : CGravObject(nType)
{
	m_fAttackRange = DEFAULT_SNIPER_RANGE;
	m_fGravResist = 0.0f;
	m_bGravImmune = false;
	m_bInWell = false;

	m_bShotOut = false;
	m_tBulletLoc = GetPosVec();
	m_nBulletImageID = -1;
	m_fShotDelay = DEFAULT_SNIPER_SHOTDELAY;
	m_fCurrentTime = 0.0f;
}

void CSniper::Aim(float fDT)
{
	if(m_pGS == NULL) //if the grav pointer is null
	{
		m_pGS = CGravSystem::GetInstance(); // get our system
	}

	D3DXVECTOR2 fireLoc = m_pGS->ClosestWell( GetPosVec() ); //get the location of the nearest well

	if( fireLoc != GetPosVec() && m_fAttackRange >= D3DXVec2Length( &(fireLoc - GetPosVec()) ) ) //if the fire loc returned is not ourlocation (the fail of finding a near well) and is within range
	{
		if(m_fCurrentTime >= m_fShotDelay) //if we've had time to aim
		{
			m_pTargetWell = m_pGS->LockWell( GetPosVec() ); // lock onto the well for tracking collision
			if(m_pTargetWell != NULL)
			{
				m_tTargetRect = m_pTargetWell->GetCollideRect(); // store the collide rect so that the bullet can track, and yet not call a dead well

				m_pTargetWell->AddRef(); //add a ref for the well

				Fire( fireLoc ); //fires a shot at that location
				m_fCurrentTime -= m_fShotDelay; //reset the shot time for when the shot is up again
			}
		}
		else
		{
			m_fCurrentTime += fDT; //continue aiming increment
		}
	}
}

void CSniper::Fire(D3DXVECTOR2 targetLoc)
{
	D3DXVECTOR2 shotLine; //declare a vec
	shotLine = targetLoc - GetPosVec(); // make this vec the line between me and my target
	D3DXVec2Normalize(&shotLine, &shotLine); //normalize the line for a unit vec
	shotLine *= SNIPE_SHOT_SPEED; //multiply by my default shot velocity (mag of vec)

	m_tBulletVel = shotLine; // set my shotVel to this
	m_bShotOut = true; // say I have a shot out
}

void CSniper::BulletCheck(float fDT)
{
	m_tBulletLoc += (m_tBulletVel * fDT);

	POINT bullet;
	bullet.x = (long)m_tBulletLoc.x;
	bullet.y = (long)m_tBulletLoc.y;
	//Check Bullet Collision Here
	if( PtInRect(&m_tTargetRect, bullet) == TRUE && m_bShotOut == true)
	{
			//change well out to false
		m_pTargetWell->Terminate(); //tell the grav system to kill the well, passing our stored point
										   //this prevents a pointer to a dead well from being called as the grav system
										   //wont call any functions till it has made sure the well still is open

		//play forced termination sfx

		m_pTargetWell->Release(); // release the target well
		m_pTargetWell = NULL;

		m_tBulletLoc = GetPosVec(); //reset bullet loc to sniper
		m_bShotOut = false; //no shot out now
	}
	else
	{
		//check tile system will go here until then

		static RECT playField;
		playField.top = 0;
		playField.left = 0;
		playField.bottom = WorldConstants::fDefaultFloor;
		playField.right = WorldConstants::fDefaultRWall;

		POINT bullet;
		bullet.x = (long) m_tBulletLoc.x;
		bullet.y = (long) m_tBulletLoc.y;

		if(FALSE == PtInRect(&playField, bullet) )
		{
			m_tBulletLoc = GetPosVec();
			m_bShotOut = false;
			m_pTargetWell->Release();
			m_pTargetWell = NULL;
		}
	}
}

void CSniper::Update(float fDT)
{
	if(m_bGravImmune && !m_bAuraSwitch)
	{
		m_nAuraID = CParticleManager::GetInstance()->SpawnParticle("ImmuneAura", 0.0f, 0.0f, this);
		m_bAuraSwitch = true;
	}

	if(GetObjState() != OBJ_DEAD) // if it is not dead
	{
		CGravObject::Update(fDT);
		if(m_bOnGround && !m_bInWell)
		{
			SetVelX(0.0f);
			CAnimationManager::GetInstance()->PerformAction(m_nAnimID, LEFT_RIGHT_RELEASED);
		}

		if(!m_bShotOut && !IsInAWell()) //if theres no shot out
		{
			//set bullet to player loc
			m_tBulletLoc = GetPosVec();
			Aim(fDT); //start firing circle
		}
		else if(m_bShotOut)//if theres a shot out
		{
				BulletCheck(fDT); //check and update our bullet
		}
	}
}

void CSniper::Render()
{
	if(GetObjState()  != OBJ_DEAD)
	{
		//CBaseObject::Render();

		if(m_bShotOut)
		{
			RECT drawRect;
			drawRect.top = 0;
			drawRect.left = 0;
			drawRect.bottom = drawRect.top + 8;
			drawRect.right = drawRect.left + 8;
			CSGD_TextureManager::GetInstance()->Draw(m_nBulletImageID, (int)m_tBulletLoc.x - 4, (int)m_tBulletLoc.y - 4,1.0f, 1.0f, &drawRect, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(240, 30, 30));
		}
	}
}

void CSniper::CollisionReact(int CollideObjType, int nAxis, float fSpeed)
{
	if(CollideObjType == TYPE_HAZARD)
	{
		//Do some collision crap here.
		//Create an explosion particle effect
		CAssetState::GetInstance()->PlaySound("KillSound", rand() % 3 + 1);
		CParticleManager::GetInstance()->SpawnParticle("Explosion", GetPosVec().x, GetPosVec().y);

		SetObjState(OBJ_DEAD);
		CAnimationManager::GetInstance()->EndAnimation(m_nAnimID);

	}
	else
		CGravObject::CollisionReact(CollideObjType, nAxis, fSpeed);
}
bool CSniper::CheckCollision(CBaseObject* pbj)
{
	if(pbj->GetType() != TYPE_EXIT && pbj->GetType() != TYPE_POWERUP && pbj->GetType() != TYPE_WALKER
	&& pbj->GetType() != TYPE_HEAVY && pbj->GetType() != TYPE_SNIPER && pbj->GetType() != TYPE_BOMBARD
	&& pbj->GetType() != TYPE_PARTICLE && pbj->GetType() != TYPE_ZONE)
		return CBaseObject::CheckCollision(pbj);

	return false;

}