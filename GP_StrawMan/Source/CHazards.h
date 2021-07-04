#pragma once 
#include "CGravObject.h"

enum hazardTypes {HAZ_LAVA = 0, HAZ_SPIKES, HAZ_FALLING_SPIKES}; 

enum hazPlacers  {HAZ_TOP = 0, HAZ_BOTTOM, HAZ_LEFT, HAZ_RIGHT}; 

class CHazards : public CGravObject
{

private:
	int hazardType; 
	int m_nPlace; 
	bool isMoving; 
	D3DXVECTOR2		originalPos; 
	RECT rec; 
	bool		m_bOnce; 

public:

//Constructor 
	CHazards();

//Destructor
	~CHazards(); 

//Overloaded Constructor to set type of object and type of hazard 
	CHazards(int b_nType, int h_nType); 

//Update 
	void Update(float fDT); 

//Render
	void Render(); 

//Accessor
	int GetHazType() {return hazardType;}
	bool GetMoving() {return isMoving;}
	D3DXVECTOR2 GetOrgPos() {return originalPos;}
	float GetSizeX() {return GetSizeVec().x;}
	float GetSizeY() {return GetSizeVec().y;}
	int GetHazPlace() {return m_nPlace;}

//Mutators
	void SetHazType(int hType) {hazardType = hType;}
	void SetMoving(bool moving) {isMoving = moving;}
	void SetOrgPos(void) {originalPos = GetPosVec();}
	void SetHazPlace(int place) {m_nPlace = place;}

	//Use this check to see if there is a gravity well affecting the 
	//if( m_pGS->GetGravity( GetPosVec() ) != m_pGS->GetTileOnlyGrav( GetPosVec() ) )

	//Special RECT functions for collision with the hazards  
	void SetMyRect(); 
	RECT GetCollideRect(void); 

	//Makes Fire for the lava hazard
	void MakeFire(float x, float y, bool oneTime); 
	void KillFire(void); 

	bool CheckCollision(CBaseObject* pbj); 
}; 