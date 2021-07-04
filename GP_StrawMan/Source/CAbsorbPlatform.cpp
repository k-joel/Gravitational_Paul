#include "CAbsorbPlatform.h"
#include "CGamePlayState.h"
#include "CPlayer.h"
#include "CGravSystem.h"
#include "CGravWell.h"

CAbsorbPlatform::CAbsorbPlatform() : CMovablePlatform(TYPE_ABSORBPLAT)
{
	m_pMyWell = NULL; //null the gravwell pointer
	
	m_fWellTimer = DEFAULT_ABSORB_TOGGLE; //default 10 sec timer
	m_fCurrentTime = 0.0f; //0 current time
	m_bPushPull = false; //default to pull
	m_fRange = 96.0f; //defaults to 3 tiles (more room than the player wells)
	m_nWellImageID = -1;
	m_fForce = WorldConstants::fGravity;
	
}

CAbsorbPlatform::~CAbsorbPlatform()
{
	if(m_pMyWell != NULL)
	{
		m_pMyWell->Terminate();
		m_pMyWell->Release();

	}
	
}

void CAbsorbPlatform::Update(float fDT)
{
	

	if(m_pMyWell != NULL) //if i have a well
	{
		
		m_pMyWell->SetPosVec( GetPosVec() ); //always make my well my position
		if(m_pMyWell->IsItOpen() == true) //if my well is open
		{
			m_fCurrentTime += fDT; //increment the timer
			if(m_fCurrentTime >= m_fWellTimer) //if its been longer than my set time
			{
				m_pMyWell->Close(); //close the well
			}
		}
	}
	
	CMovablePlatform::Update(fDT); //otherwise update like a movable platform
}

void CAbsorbPlatform::Render()
{
	if(m_pMyWell && m_pMyWell->IsItOpen())
		m_pMyWell->Render(); //render the well only if there is one
	CGravObject::Render(); //now render me on top
}

void CAbsorbPlatform::CollisionReact(int CollideObjType, int nAxis, float fSpeed)
{
	

	switch(CollideObjType)
	{
	case TYPE_GRAVWELL:
		{
			if(m_pMyWell == NULL) //if i dont have a well
			{
				SetUpWell(); //set it up by my settings (changed in collision)
				m_pMyWell->Open(); //open it
			}
			else if(m_pMyWell->IsItOpen() == false)
			{
				m_pMyWell->Open();
			}
			m_pMyWell->SetPushPull(m_bPushPull);

			break;
		}	
	}
	
	CBaseObject::CollisionReact(CollideObjType, nAxis, fSpeed);
}

void CAbsorbPlatform::SetUpWell()
{
	if(m_pMyWell == NULL)
	{
		m_pMyWell = new CGravWell; //new grav well, deleted when this goes out of scope
	}
	
	m_pMyWell->SetPosVec( GetPosVec() ); //sets position to hevay's position
	m_pMyWell->SetIsTemporary(false); // well never dies until the end (terminate called by heavy)
	
	m_pMyWell->SetRange( m_fRange ); //sets the wells range to the established range
	m_pMyWell->SetForce( m_fForce ); //default power
	m_pMyWell->SetAnimation( m_nWellImageID);
	m_pMyWell->SetPushPull( m_bPushPull );
	m_pMyWell->SetOwner(this);
	m_pMyWell->SetSize(WorldConstants::fTileSize * 2.0f, WorldConstants::fTileSize * 2.0f);
	
	
}