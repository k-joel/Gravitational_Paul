/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CHeavyEnemy.cpp"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Defines the class for the heavy enemy. This enemy is a large walker who contains his own grvaity well. This well can
//			be either type, have a variable range, and be permanent, or toggle on and off on a timer.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CHeavyEnemy.h"

//additional #includes
#include "CGravWell.h"
#include "CAnimationManager.h"
#include "CParticleManager.h"

CHeavy::CHeavy() : CWalker(TYPE_HEAVY)
{
	m_pMyWell = NULL; //null the gravwell pointer
	m_bTimed = true; // is it timed
	m_fWellTimer = DEFAULT_TOGGLE; //default 3 sec timer
	m_fCurrentTime = 0.0f; //0 current time
	m_bPushPull = false; //default to pull
	m_fRange = 96.0f; //defaults to 3 tiles (more room than the player wells)
	m_nWellImageID = -1;
	m_bChangeDirect = false; //defaults to static direction
}

CHeavy::~CHeavy()
{
	if(m_pMyWell != NULL)
	{
		m_pMyWell->Terminate();
	}
	delete m_pMyWell;
}

void CHeavy::SetUpWell()
{
	m_pMyWell = new CGravWell; //new grav well, deleted when this goes out of scope
	m_pMyWell->SetPosVec( GetPosVec() ); //sets position to hevay's position
	m_pMyWell->SetIsTemporary(false); // well never dies until the end (terminate called by heavy)
	m_pMyWell->SetRange( m_fRange ); //sets the wells range to the established range
	m_pMyWell->SetForce( WorldConstants::fGravity ); //default power
	m_pMyWell->SetAnimation( m_nWellImageID );
	m_pMyWell->SetPushPull( m_bPushPull );
	m_pMyWell->SetOwner(this);
	m_pMyWell->SetSize(64.0f, 64.0f);
}

void CHeavy::GravWellToggle()
{
	if(m_bChangeDirect && !(m_pMyWell->IsItOpen()) ) //will only change direction when it has closed and then re opens
	{
		if(rand() % 2 == 0) //randomly swaps direction if its a toggling well
		{
			m_pMyWell->SetPushPull( true ); //makes it a push well
		}
		else
		{
			m_pMyWell->SetPushPull( false); //makes it pull
		}
	}

	if(m_pMyWell->IsItOpen()) //if its open,
	{
		m_pMyWell->Close(); //clsoe it
	}
	else //otherwise
	{
		m_pMyWell->Open(); //open it
	}
}

void CHeavy::Update(float fDT)
{
	if(m_pMyWell == NULL)
	{
		SetUpWell();
	}
	m_fCurrentTime += fDT; //update current time
	if(m_fCurrentTime >= m_fWellTimer) //if its been the increment
	{
		GravWellToggle(); //toggle my grvaity (On or off)
		//animation change goes here

		m_fCurrentTime -= m_fWellTimer; // then decrement the timer to time again
	}

	m_pMyWell->SetPosVec( GetPosVec() ); //sets position of well to heavy position each frame

	CWalker::Update(fDT); //otherwise use the walker AI
}

void CHeavy::Render()
{
	if(m_pMyWell->IsItOpen())
		m_pMyWell->Render(); //render the well
	//CBaseObject::Render(); //now render me on top
}
bool CHeavy::CheckCollision(CBaseObject *pbj)
{
	return CWalker::CheckCollision(pbj);
}