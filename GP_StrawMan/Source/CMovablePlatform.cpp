#include "CMovablePlatform.h"
#include "CTileManager.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include <Windows.h>

CMovablePlatform::CMovablePlatform() : CGravObject(TYPE_HOVERPLAT)
{
	m_bInWell = false;
	m_bGravImmune = false;
	m_fGravResist = 0.0f;
}

CMovablePlatform::~CMovablePlatform()
{
}
CMovablePlatform::CMovablePlatform(int nType) : CGravObject(nType)
{
		m_bInWell = false;
	m_bGravImmune = false;
	m_fGravResist = 0.0f;
}

void CMovablePlatform::Update(float fDT)
{
	if(m_pGS == NULL) // if it hasnt checked the grav system yet, check it
	{
		m_pGS = CGravSystem::GetInstance();
	}

	if((m_pGS->GetGravity( GetPosVec() ) != m_pGS->GetTileOnlyGrav( GetPosVec() ) )) //if gravity is the tile gravity, then its not in a well
		m_bInWell = true;
	else
		m_bInWell = false;

	//update velocity direction and speed
	if(!m_bInWell) //if not in a well
	{
		SetVelVec( GetVelVec() * 0.99f); // slowmovement
		if( abs(GetVelVec().x) <= 0.001f)
			SetVelX(0.0f); 
		if(abs(GetVelVec().y) <= 0.001f)
			SetVelY(0.0f); // stop movment all together
	}
	else //if not immune and in a well
		SetVelVec( GetVelVec() + ( CheckGravity() * fDT) ); //use normal grav object calculations * its resistance

	CBaseObject::Update(fDT);

	CTileManager::GetInstance()->SetObjectPseudoWalls(this);
	if(GetPosVec().y + (GetSizeVec().y / 2.0f) > m_fFloorValue && GetVelVec().y > 0.0f)
		{
			SetPosY(m_fFloorValue -  (GetSizeVec().y / 2.0f)); // set to floor
			
			SetVelY(0.0f); // stop vert movement
			
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
		/*
		SetCeiling(WorldConstants::fDefaultCeiling);
		SetFloor(WorldConstants::fDefaultFloor);
		SetLWall(WorldConstants::fDefaultLWall);
		SetRWall(WorldConstants::fDefaultRWall);
		*/

	
}

void CMovablePlatform::Render()
{
	CBaseObject::Render();

	/*CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	RECT r;
	r.left = GetPosVec().x - GetSizeVec().x/2.0f;
	r.top = GetPosVec().y - GetSizeVec().y/2.0f;
	r.right = r.left + GetSizeVec().x;
	r.bottom = r.top + GetSizeVec().y;
	CSGD_Direct3D::GetInstance()->DrawRect(r, 255, 0, 0);*/
}

void CMovablePlatform::CollisionReact(int CollideObjType, int nAxis, float fSpeed)
{
	
}