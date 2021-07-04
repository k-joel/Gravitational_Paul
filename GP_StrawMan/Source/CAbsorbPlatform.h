#pragma once
#include "CMovablePlatform.h"
//#include "CBaseObject.h"

#define DEFAULT_ABSORB_TOGGLE  7.0f

class CGravWell;
class CAbsorbPlatform : public CMovablePlatform
{
	
	

	CGravWell*		m_pMyWell; //pointer to the contained grav well (this will NOT be added to the OM, the platform will be responsible for this memory)
	float			m_fWellTimer; //if it opens this is the increment
	float			m_fCurrentTime; //the current time in its current state
	bool			m_bPushPull; //type of well the platform contains
	float			m_fRange; // range of the well
	int				m_nWellImageID; //image to render when the well is open
	float			m_fForce; //force of the well. defaults to grav constant. well be set to the well that collides with the plats force, making it upgrade as the player does


public:
	CAbsorbPlatform();
	~CAbsorbPlatform();

	void Update(float fDT);
	void Render();
	void CollisionReact(int CollideObjType, int nAxis, float fSpeed);
	void SetUpWell();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Function: Accessors
	//
	//  Purpose : Returns the specified type.
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	CGravWell* GetWell() {return m_pMyWell;}
	bool GetPushPull(){return m_bPushPull;}
	int GetWellImageID(){return m_nWellImageID;}
	float GetToggleTime(){return m_fWellTimer;}
	float GetRange() {return m_fRange;}
	float GetForce(){return m_fForce;}


	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: Modifiers
	//
	//  Purpose: Modifies the specified type.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetPushPull(bool pushPull){m_bPushPull = pushPull;}
	void SetDuration(float newDuration){m_fWellTimer = newDuration;}
	void SetWellImage(int newID){m_nWellImageID = newID;}
	void SetRange(float newRange){m_fRange = newRange;}
	void SetForce(float newForce){m_fForce = newForce;}
	void ResetTime(){m_fCurrentTime = 0.0f;}


	
};