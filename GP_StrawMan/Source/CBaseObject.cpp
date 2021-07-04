/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CBaseObject.cpp"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Defines the Baseobject Class. This will ahve all base functionaloty and interface for gameobjects.
//			These objects are currently unaffected by the gravity field. Also defined in this file is a namespace of constants
//			used by all objects in this hierarchy. This will stream line changes to the physics of the system
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CBaseObject.h"
#include "CTileManager.h"
#include "SGD Wrappers/CSGD_FModManager.h"
#include "CAnimationManager.h"
#include "CGame.h"
#include "CPlayer.h"
#include "CAssetState.h"
#include "CGamePlayState.h"

CBaseObject::CBaseObject()
{
	m_nType = TYPE_BASE;
	m_nRefCount = 1;
	m_tPosition.x = 0.0f;
	m_tPosition.y = 0.0f;
	m_tVelocity.x = 0.0f;
	m_tVelocity.y = 0.0f;
	m_tSize.x = 0.0f;
	m_tSize.y = 0.0f;

	m_bIsSolid = false; //objects not effected by gravity default to not solid (hud etc)
	m_nAnimationID = -1;
	m_nLayer = 0;
	m_nState = OBJ_SPAWN;
	m_fScreamTime = 0.0f;
	m_bIsAnimated = false;
}

CBaseObject::~CBaseObject()
{
}

CBaseObject::CBaseObject(int newType)
{
	m_nType = newType;
	m_nRefCount = 1;
	m_tPosition.x = 0.0f;
	m_tPosition.y = 0.0f;
	m_tVelocity.x = 0.0f;
	m_tVelocity.y = 0.0f;
	m_tSize.x = 0.0f;
	m_tSize.y = 0.0f;
	m_bIsAnimated = false;

	m_bIsSolid = false; //objects not effected by gravity default to not solid (hud etc)
	m_nAnimationID = -1;
	m_nLayer = 0;
	m_nState = OBJ_SPAWN;

	if(newType == TYPE_SNIPER)
		m_nAnimID = CAnimationManager::GetInstance()->StartAnimation("Sniper", this);
	else if(newType == TYPE_BOMBARD)
		m_nAnimID = CAnimationManager::GetInstance()->StartAnimation("Bombardier", this);
	else if(newType == TYPE_WALKER)
		m_nAnimID = CAnimationManager::GetInstance()->StartAnimation("Walker", this);
	else if(newType == TYPE_HEAVY)
		m_nAnimID = CAnimationManager::GetInstance()->StartAnimation("Heavy", this);

	m_fCeilingValue = WorldConstants::fDefaultCeiling;
	m_fLWallValue = WorldConstants::fDefaultLWall;
	m_fRWallValue = WorldConstants::fDefaultRWall;
	m_fFloorValue = WorldConstants::fDefaultFloor;
	m_fScreamTime = 0.0f;
}

void CBaseObject::Update(float fDT)
{
	

	if(m_nState!= OBJ_DEAD) //does not update dead objects
	{//state switch to go here
		if(GetIsAnimated())
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
		}

		CTileManager::GetInstance()->SetObjectPseudoWalls(this);
		//update position by velocity * time
		m_tPosition += (m_tVelocity * fDT);

		if(m_tVelocity.y > WorldConstants::fTermVel)
		{
			if(GetType() == TYPE_HAZARD)
				return; 
			
			m_bFalling = true;
			m_fScreamTime = 0.0f;
			if(CSGD_FModManager::GetInstance()->IsSoundPlaying(CAssetState::GetInstance()->GetSoundID("ScreamID")) == false)
			{
			//CSGD_FModManager::GetInstance()->PlaySoundA(CAssetState::GetInstance()->GetSoundID("ScreamID"));
			CAssetState::GetInstance()->PlaySound("ScreamID");
			//CSGD_FModManager::GetInstance()->Update();
			}
		}

		if(m_tVelocity.y < WorldConstants::fTermVel)
			m_bFalling = false;

		if(CSGD_FModManager::GetInstance()->IsSoundPlaying(CAssetState::GetInstance()->GetSoundID("ScreamID")))
		{
			m_fScreamTime += fDT;
			if(m_fScreamTime > 1.9f)
			{
				m_fScreamTime = 0.0f;
				CSGD_FModManager::GetInstance()->StopSound(CAssetState::GetInstance()->GetSoundID("ScreamID"));
			}
		}
	}
	else
	{
		if(GetIsAnimated())
			CAnimationManager::GetInstance()->EndAnimation(m_nAnimID);
	}
}

void CBaseObject::Render()
{
	RECT drawRect = GetDrawRect();

	float drawX = GetPosVec().x - (GetSizeVec().x / 2.0f);
	float drawY = GetPosVec().y - (GetSizeVec().y / 2.0f);

	if(m_nState != OBJ_DEAD)
	{
		CSGD_TextureManager::GetInstance()->Draw( m_nAnimationID, (int)drawX, (int)drawY, 1.0f, 1.0f, &drawRect);
	}
}

void CBaseObject::CollisionReact(int CollideObjType, int nAxis, float fSpeed)
{
}

RECT CBaseObject::GetDrawRect()
{
	//current draw rect is with object location at center of the image
	RECT drawRect;
	drawRect.left = 0;
	drawRect.top =  0;
	drawRect.right = drawRect.left + (long)(m_tSize.x);
	drawRect.bottom = drawRect.top + (long)(m_tSize.y);

	return drawRect;
}

void CBaseObject::Release()
{
	--m_nRefCount;
	if(0 == m_nRefCount)
	{
		delete this;
	}
}

bool CBaseObject::CheckCollision(CBaseObject *pbj)
{
	RECT Rec1 = GetCollideRect(); // get collision rects for both objects
	RECT Rec2 = pbj->GetCollideRect();

	RECT output; //a rect for the result
	if(!IntersectRect(&output, &Rec1, &Rec2)) return false;// if they collided


	if(GetType() == TYPE_POWERUP)
	{

		CollisionReact(TYPE_PLAYER, 0, 0.0);
		return true;

	}


	int vSide = output.bottom - output.top;
	int hSide = output.right - output.left;  //declares directional variables for the collision
	int nBounceAxis;

	float collSpeed = D3DXVec2Dot(&(GetVelVec()), &( pbj->GetVelVec() )); //calculate combined force of collision
	bool m_bCheckBoth = true;
	bool bIsPlayer = false;
	if(GetType() == TYPE_PLAYER)
	{
		bIsPlayer = true;
		if(((CPlayer*)(this))->GetIsFlashing())
		{
			return true;
		}
		
	}

	if(pbj->GetType() == TYPE_HOVERPLAT || pbj->GetType() == TYPE_ABSORBPLAT)
	{
		m_bCheckBoth = false;
		if(bIsPlayer)
			CGamePlayState::GetInstance()->GetPlayer()->SetPlatform(pbj);
	}
	if((bIsPlayer && (pbj->GetType() == TYPE_WALKER || pbj->GetType() == TYPE_HEAVY || pbj->GetType() == TYPE_SNIPER || pbj->GetType() == TYPE_BOMBARD)))
	{
		m_bCheckBoth = false;

	}
	else if(pbj->GetType() == TYPE_PLAYER && (GetType() == TYPE_WALKER || GetType() == TYPE_HEAVY || GetType() == TYPE_SNIPER || GetType() == TYPE_BOMBARD))
	{
		return false;
	}

	if( abs(hSide - vSide) == 0 ) //if it was withing 4/32 pixels from corner collision
	{
			if(GetPosVec().x > pbj->GetPosVec().x) // if im to the right
			{
				SetLWall((float)Rec2.right); //my left wall is his right side

				if(m_bCheckBoth)
				pbj->SetRWall((float)Rec1.left); //his right wall is my left side
				else
				{
					if( m_fRWallValue > GetPosVec().x + (GetSizeVec().x / 2.0f) )
					{
						SetVelX( pbj->GetVelVec().x);
						SetPosX(GetPosVec().x + hSide);
					}
				}
			}
			else//if theyre to the right
			{
				SetRWall((float)Rec2.left);

				if(m_bCheckBoth)
				pbj->SetLWall((float)Rec1.right); //do the opposite
				else
				{
					if( m_fLWallValue < GetPosVec().x - (GetSizeVec().x / 2.0f) )
					{
						SetVelX( pbj->GetVelVec().x);
						SetPosX(GetPosVec().x - hSide);
					}
				}
			}

			if(GetPosVec().y > pbj->GetPosVec().y) //if im below him
			{
				SetCeiling((float)Rec2.bottom); //he's my ceiling

				if(m_bCheckBoth)
				pbj->SetFloor((float)Rec1.top); // and im his floor
				else
				{
					if( m_fFloorValue > GetPosVec().y + (GetSizeVec().y / 2.0f) )
					{
						SetVelY( pbj->GetVelVec().y);
						SetPosY(GetPosVec().y + vSide);
					}
					((CPlayer*)this)->PlayerLanded();
				}
			}
			else //of im on top of him
			{
				SetFloor((float)Rec2.top);

				if(m_bCheckBoth)
				{
					pbj->SetCeiling((float)Rec1.bottom);
				}
				else
				{
					if( GetCeilingValue() < GetPosVec().y - (GetSizeVec().y / 2.0f) )
					{
						SetVelY( pbj->GetVelVec().y);
						SetPosY(GetPosVec().y - vSide);
					}
					((CPlayer*)this)->PlayerLanded();
				}
			}
			nBounceAxis = AXIS_BOTH; //reflect both axis
		}
		else if(hSide > vSide) //otherwise, if the hside of the collision rect was larger, reflect just vert vel
		{
			if(GetPosVec().y > pbj->GetPosVec().y) //if im below him
			{
				SetCeiling((float)Rec2.bottom);//he's my ceiling

				if(m_bCheckBoth)
				pbj->SetFloor((float)Rec1.top); // and im his floor
				else
				{
					if( m_fFloorValue > GetPosVec().y + (GetSizeVec().y / 2.0f) )
					{
						SetVelY( pbj->GetVelVec().y);
						SetPosY(GetPosVec().y + vSide);
					}
				}
			}
			else //of im on top of him
			{
				SetFloor((float)Rec2.top);

				if(m_bCheckBoth)
				{
					pbj->SetCeiling((float)Rec1.bottom);
				}
				else
				{
					if( GetCeilingValue() < GetPosVec().y - (GetSizeVec().y / 2.0f) )
					{
						SetVelY( pbj->GetVelVec().y);
						SetPosY(GetPosVec().y - vSide);
					}
					((CPlayer*)this)->PlayerLanded();
				}
			}

			nBounceAxis = AXIS_YONLY;
	}
	else if(vSide > hSide) //same check as above, to reflact on horz velocity
	{
		if(GetPosVec().x > pbj->GetPosVec().x) // if im to the right
		{
			SetLWall((float)Rec2.right); //my left wall is his right side
			if(m_bCheckBoth)
			pbj->SetRWall((float)Rec1.left); //his right wall is my left side
			else
			{
				if( m_fRWallValue > GetPosVec().x + (GetSizeVec().x / 2.0f) )
				{
					SetVelX( pbj->GetVelVec().x);
					SetPosX(GetPosVec().x + hSide);
				}
			}
		}

		else//if theyre to the right
		{
			SetRWall((float)Rec2.left);

			if(m_bCheckBoth)
				pbj->SetLWall((float)Rec1.right); //do the opposite
			else
			{
				if( m_fLWallValue < GetPosVec().x - (GetSizeVec().x / 2.0f) )
				{
					SetVelX( pbj->GetVelVec().x);
					SetPosX(GetPosVec().x - hSide);
				}
			}
		}

			nBounceAxis = AXIS_XONLY;
		}

	CollisionReact(pbj->GetType(), nBounceAxis, collSpeed); //pass the react functions these values and let the objects determine if needed

	if(m_bCheckBoth)
	pbj->CollisionReact(GetType(), nBounceAxis, collSpeed);

	return true;
}

RECT CBaseObject::GetCollideRect(void)
{
	if(GetIsAnimated() )
	{
		RECT collideRect = CAnimationManager::GetInstance()->GetCurrentCollRect(m_nAnimID);
		int xOffset =  collideRect.right - collideRect.left;
		int yOffset =  collideRect.bottom - collideRect.top;

		SetSize((float)xOffset, (float)yOffset);
	}

	RECT rect;
	D3DXVECTOR2 pos = this->GetPosVec();
	D3DXVECTOR2 siz = this->GetSizeVec();
	rect.left = (int)(pos.x - (siz.x / 2.0f));
	rect.top = (int)(pos.y - (siz.y / 2.0f));

	rect.right = (rect.left + (int)siz.x);
	rect.bottom = (rect.top + (int)siz.y);

	return rect;
}

void CBaseObject::BounceBack(CBaseObject *bounceMe, int nAxis, bool killSpeed)
{
	switch(nAxis)
	{
	case AXIS_XONLY:
		{
			//SetPosX( GetPosVec().x - (GetVelVec().x * .1f) ); //move is out of the object by velocity
			if(killSpeed)
				SetVelX( 0.0f ); //reverse velocity in x direction
			else
				SetVelX( - GetVelVec().x );
			break;
		}
	case AXIS_YONLY:
		{
			//SetPosY( GetPosVec().y - (GetVelVec().y * .1f) ); //move it out of object by velocity
			if(killSpeed)
				SetVelY( 0.0f );
			else
				SetVelY( -GetVelVec().y );
			break;
		}
	case AXIS_BOTH:
		{
			//SetPosX( GetPosVec().x - (GetVelVec().x *.1f)); //move is out of the object by velocity
			if(killSpeed)
				SetVelX( 0.0f ); //reverse velocity in x direction
			else
				SetVelX( - GetVelVec().x );

			SetPosY( GetPosVec().y - (GetVelVec().y *.1f)); //move it out of object by velocity
			if(killSpeed)
				SetVelY( 0.0f );
			else
				SetVelY( -GetVelVec().y );

			break;
		}
	}
}