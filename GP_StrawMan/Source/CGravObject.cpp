/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CGravObject.cpp"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Defines the GravObject class, derived from BaseObjects to be affected by gravity
//			This class will ask the gravity system for tis current and dynamic gravity.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CGravObject.h"

//other included files
#include "CGravSystem.h"
#include "CAnimationManager.h"

CGravObject::CGravObject() : CBaseObject(TYPE_GRAVOBJ) //sets the type of object in the constructor
{
	m_pGS = NULL;
	m_bInWell = false;
	SetIsSolid(true); // GravObjects default to solid, as they are most often in game objects
	m_bOnGround = false;
	m_pGS = CGravSystem::GetInstance();
}

CGravObject::~CGravObject()
{
}
CGravObject::CGravObject(int nType) : CBaseObject(nType)
{
	m_pGS = NULL;
	m_bInWell = false;
	SetIsSolid(true); // GravObjects default to solid, as they are most often in game objects

	m_pGS = CGravSystem::GetInstance();
}

void CGravObject::Update(float fDT)
{
	//current updates (based on tech demo needs)

	if((m_pGS->GetGravity( GetPosVec() ) != m_pGS->GetTileOnlyGrav( GetPosVec() ) )) //if gravity is the tile gravity, then its not in a well
	{
		m_bInWell = true;
		m_bOnGround = false;
		if(GetIsAnimated())
			CAnimationManager::GetInstance()->PerformAction(m_nAnimID, IN_GRAV_WELL);
	}
	else
	{
		m_bInWell = false;

		if(GetVelVec().y > WorldConstants::fGravity + 0.5f && GetIsAnimated())
			CAnimationManager::GetInstance()->PerformAction(m_nAnimID, POS_VELY_AND_OUT_OF_GRAV_WELL);
	}

	if(GetObjState() != OBJ_DEAD)
	{
		//state swicth to go here

		//update position based on time (calls parent update)
		CBaseObject::Update(fDT);

		//Update Velocity based on current gravity (return of CheckGravity funct * fDT)
		SetVelVec( GetVelVec() + ( CheckGravity() * fDT));

		if(GetPosVec().y + (GetSizeVec().y / 2.0f) > m_fFloorValue && GetVelVec().y > 0.0f)
		{
			SetPosY(m_fFloorValue -  (GetSizeVec().y / 2.0f)); // set to floor

			SetVelY(0.0f); // stop vert movement
			m_bOnGround = true;
			if(GetIsAnimated())
				CAnimationManager::GetInstance()->PerformAction(m_nAnimID, VELY_ZERO);
		}
		if(GetPosVec().y - (GetSizeVec().y / 2.0f) < m_fCeilingValue)
		{
			SetPosY(m_fCeilingValue + (GetSizeVec().y / 2.0f));
			SetVelY(0.0f);
		}

		if(GetPosVec().x + (GetSizeVec().x / 2.0f) > m_fRWallValue && GetVelVec().x > 0.0f)
		{
			SetPosX(m_fRWallValue -  (GetSizeVec().x / 2.0f)); // set to floor
			SetVelX(0.0f); // stop horz movement
		}
		if(GetPosVec().x - (GetSizeVec().x / 2.0f) < m_fLWallValue && GetVelVec().x < 0.0f)
		{
			SetPosX(m_fLWallValue + (GetSizeVec().x / 2.0f)); // set to floor
			SetVelX(0.0f); // stop horz movement
		}

			//Keep in main window checks

		if(GetPosVec().x > WorldConstants::fDefaultRWall - (GetSizeVec().x / 2.0f))
		{
			SetPosX(WorldConstants::fDefaultRWall - (GetSizeVec().x / 2.0f)); // set to floor
			SetVelX( -GetVelVec().x ); // stop horz movement
		}
		else if (GetPosVec().x < WorldConstants::fDefaultLWall + (GetSizeVec().x / 2.0f))
		{
			SetPosX(WorldConstants::fDefaultLWall +  (GetSizeVec().x / 2.0f)); // set to floor
			SetVelX( -GetVelVec().x ); // stop horz movement
		}

		//this will reset the ceiling and floor values every frame (as collision is called after this
		//the values for object will be used on the next pass if they are changes by a collision
		//this will be changed to proc the tile manager for the solid tiles next to the player, and set that
		//as the new wall value
		SetCeiling(WorldConstants::fDefaultCeiling);
		SetFloor(WorldConstants::fDefaultFloor);
		SetLWall(WorldConstants::fDefaultLWall);
		SetRWall(WorldConstants::fDefaultRWall);
	}
}

void CGravObject::CollisionReact(int CollideObjType, int nAxis, float fSpeed)
{
	//updated collision reactions to go here

	if(m_bInWell && GetType() != TYPE_HEAVY) //if in a well the object will react to collisions, otherwwise its too heavy'
		BounceBack(this, nAxis);
}

D3DXVECTOR2 CGravObject::CheckGravity()
{
	//check gravity function to go here when needed (will set pointer if NULL)
	//check if grav system has been loaded into object yet
	if(m_pGS == NULL)
	{
		//if not set it
		m_pGS = CGravSystem::GetInstance();
	}
	//call GetGravity passing obj position, and return this value. no need to store in default obj
	return m_pGS->GetGravity(GetPosVec(), GetType());
}