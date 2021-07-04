#include "CPickups.h"
#include "CGamePlayState.h"
#include "CMessageSystem.h"
#include "SGD Wrappers/CSGD_FModManager.h"
#include "CAssetState.h"
#include "CParticleManager.h"

//This will never be used
CPickups::CPickups() : CBaseObject(TYPE_POWERUP)
{
	//Will call the setup for a power up object everything else must be set in create function.
	
	m_nPickupCollideID = CAssetState::GetInstance()->GetSoundID("PickupCollideID");
	m_bSparkleStarted = false;
	m_nSparkleID = -1;
}

CPickups::~CPickups()
{
}

void CPickups::Update(float fDT)
{
	if(m_bSparkleStarted == false)
	{
		m_bSparkleStarted = true;
		m_nSparkleID = CParticleManager::GetInstance()->SpawnParticle("cog sparkle", 0.0f, -16.0f, this);
	}
	CBaseObject::Update(fDT);
}

void CPickups::Render()
{
	CBaseObject::Render();
}

void CPickups::CollisionReact(int nCollideObjType, int nAxis, float fSpeed)
{
	if(nCollideObjType == TYPE_PLAYER)
	{
		if( GetPickupType() == PICKUP_CURRENCY)
		{
			CMessageSystem::GetInstance()->SendMsg( new CCurrencyCollectedMessage( GetCurrencyIndex() ));
		}
		else
		{
			CGamePlayState::GetInstance()->CollisionPickup(GetPickupType());
		}

		//CSGD_FModManager::GetInstance()->PlaySoundA(m_nPickupCollideID);
		CAssetState::GetInstance()->PlaySound(m_nPickupCollideID);
		CParticleManager::GetInstance()->KillParticle(m_nSparkleID);
		SetObjState(OBJ_DEAD);
	}
}

bool CPickups::CheckCollision(CBaseObject* pbj)
{
	if(pbj->GetType() == TYPE_PLAYER)
		return CBaseObject::CheckCollision(pbj);

	return false;
}