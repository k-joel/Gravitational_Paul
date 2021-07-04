/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CBombardEnemy.cpp"
//
// Author: Justin Dumas (JuD)
// 
// Purpose: Defines the bombardier class of enemy, which will shott grav wells at the player. 
//			These will derive from the sniper class and will follow a similar ai.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CBombardEnemy.h"

//additional #includes here
#include "CObjectManager.h"
#include "CObjectFactory.h"
#include "CPlayer.h"
#include "CGame.h"

CBombardier::CBombardier() : CSniper(TYPE_BOMBARD)
{
	m_pPlayer = NULL;
	m_pMyWell = NULL;
	m_bPushPull = false;
	m_fWellTime = 3.0f;
	m_fWellRange = (5.0f + (2.0f * CGame::GetInstance()->GetDifficulty())) * WorldConstants::fTileSize;

}

CBombardier::~CBombardier()
{
	if(m_pMyWell != NULL)
	{
		DropWell();
	}
}

void CBombardier::Aim(float fDT)
{
	
	D3DXVECTOR2 fireLoc = m_pPlayer->GetPosVec(); //get the location of the player
	
	if(m_fAttackRange >= D3DXVec2Length( &(fireLoc - GetPosVec()) )  ) //if the fire loc returned  is within range
	{
		if(m_fCurrentTime >= m_fShotDelay && m_bShotOut == false) //if we've had time to aim
		{
			Fire( fireLoc ); //fires a shot at that location
			m_fCurrentTime -= m_fShotDelay; //reset the shot time for when the shot is up again
		}
		else 
		{
			m_fCurrentTime += fDT; //continue aiming increment
						
		}
	}
}

void CBombardier::Fire(D3DXVECTOR2 targetLoc)
{
	D3DXVECTOR2 shotLine; //declare a vec
	shotLine = targetLoc - GetPosVec(); // make this vec the line between me and my target
	D3DXVec2Normalize(&shotLine, &shotLine); //normalize the line for a unit vec
	
	SpawnWell(shotLine); // set my shotVel to this

	m_fTargetDist = D3DXVec2Length( &(GetPosVec() - targetLoc) ); // set distance till open
	m_bShotOut = true; // say I have a shot out
	

}

void CBombardier::BulletCheck(float fDT)
{
	if(m_pMyWell != NULL)
	{
		float currDist = D3DXVec2Length( &(GetPosVec() - m_pMyWell->GetPosVec()) ); // get dist to well (how far its traveled so far)
	
	
		if(currDist >= m_fTargetDist - m_fWellRange) //if this is greater or = to our target dist
		{
		
			m_pMyWell->Open(); //open the well
			
			m_fCurrentTime -= m_fShotDelay; //reset time delay
		}
	}
	else
	{
		m_bShotOut = false;
	}
}
	
void CBombardier::SpawnWell(D3DXVECTOR2 shotVec)
{
	static CObjectManager* pOM = CObjectManager::GetInstance();
	static CObjectFactory<string, CBaseObject>* pOF;
	pOF = CObjectFactory<string, CBaseObject>::GetInstance();
	
	
	CGravWell* pNewWell = (CGravWell*)pOF->CreateObject("GravWell");

	pNewWell->SetPosVec(GetPosVec());

	pNewWell->SetVelVec(WorldConstants::fGravShotSpeed * shotVec);

	pNewWell->SetSize(64.0f, 64.0f);

	pNewWell->SetDuration(m_fWellTime);
	pNewWell->SetForce(WorldConstants::fGravity);
	pNewWell->SetRange(m_fWellRange);

	if(CGame::GetInstance()->GetDifficulty() != DIFF_HARD)
		pNewWell->SetPushPull(m_bPushPull);
	else
	{
		if(rand() % 2 == 0)
			pNewWell->SetPushPull(true);
		else
			pNewWell->SetPushPull(false);
			
	}
	pNewWell->SetIsTemporary(true);

	pNewWell->SetAnimation(m_nWellImage);
	pNewWell->SetClosedImage(m_nBulletImageID);

	pNewWell->SetObjState(OBJ_NORMAL);
	pNewWell->SetOwner(this);
	m_pMyWell = pNewWell;
	
	pOM->AddObject(pNewWell);

	
}
void CBombardier::CollisionReact(int CollideObjType, int nAxis, float fSpeed)
{
	if(CollideObjType == TYPE_HAZARD)
	{
		if(m_pMyWell != NULL)
		{
			m_pMyWell->Terminate();
			m_pMyWell = NULL;

		}
		//Do some collision crap here.
		//Create an explosion particle effect
		
	}
	CSniper::CollisionReact(CollideObjType, nAxis, fSpeed);

}

void CBombardier::DropWell()
{
	m_pMyWell->Release();
	m_pMyWell = NULL;
	m_bShotOut = false;
	m_fCurrentTime = 0.0f;
}