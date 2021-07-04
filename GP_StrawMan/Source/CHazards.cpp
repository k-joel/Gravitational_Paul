#include "CHazards.h"
#include "CGravSystem.h"
#include <d3dx9math.h>
#include "CMessageSystem.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include "CParticleManager.h"
#define EASY_TIMER 2.5f
#define MEDIUM_TIMER 1.5f
#define HARD_TIMER 1.0f

CHazards::CHazards() : CGravObject(TYPE_HAZARD)
{
	SetVelX(0);
	SetVelY(0);
	SetPosX(0);
	SetPosY(0);
	SetObjState(OBJ_NORMAL);
	SetIsSolid(true);
	hazardType = -1;
	originalPos = GetPosVec();
	SetMoving(false);
	m_bOnce = true;
}

CHazards::CHazards(int b_nType, int h_nType) : CGravObject(b_nType)
{
	SetHazType(h_nType);
	SetVelX(0);
	SetVelY(0);
	SetPosX(0);
	SetPosY(0);
	SetObjState(OBJ_NORMAL);
	SetIsSolid(true);
	originalPos = GetPosVec();
	SetMoving(false);
	m_bOnce = true;
}

CHazards::~CHazards()
{
	if(GetHazType() == HAZ_LAVA)
		CParticleManager::GetInstance()->KillParticle("Particle");
}

void CHazards::Update(float fDT)
{
	SetMyRect(); 
	if(GetHazType() == HAZ_SPIKES || GetHazType() == HAZ_LAVA)
	{
		if(GetHazType() == HAZ_LAVA)
		{
		}
		return;
	}

	CGravObject::Update(fDT);
	if(GetMoving() == false)
	{
		if(m_bInWell == true)
		{
			SetVelVec(CGravSystem::GetInstance()->GetGravity(GetVelVec()));
			SetMoving(true);
		}
		else
		{
			SetVelX(0);
			SetVelY(0);
			SetMoving(false);
			SetPosVec(originalPos);
		}
	}
	else
	{
		D3DXVECTOR2 tempVec = GetVelVec();
		/*
		if(tempVec.y > WorldConstants::fTermVel)
		{
			SetVelX(0); 
			SetVelY(0); 
			SetPosVec(originalPos);
			SetMoving(false); 
		}*/
	}
}

void CHazards::Render()
{
	/*D3DXVECTOR2 tempVec = GetPosVec() - (GetSizeVec() / 2);
	CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), tempVec.x, tempVec.y, 1, -1); */

	float drawX = GetPosVec().x - (GetSizeVec().x / 2.0f);
	float drawY = GetPosVec().y + (GetSizeVec().y / 2.0f);
	RECT rec = this->GetCollideRect();
	if(GetHazType() == HAZ_SPIKES || GetHazType() == HAZ_FALLING_SPIKES)
	{
		if(GetHazPlace() == HAZ_TOP)
		{
		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)drawX, (int)( drawY + ( GetSizeY() / 2) ), 1, -1);

		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX + GetSizeX()), (int)( drawY + ( GetSizeY() / 2) ), 1, -1);

		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX + (GetSizeX() * 2)), (int)( drawY + ( GetSizeY() / 2) ), 1, -1);

		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX + GetSizeX() * 3), (int)( drawY + ( GetSizeY() / 2) ), 1, -1);
		}
		else if(GetHazPlace() == HAZ_BOTTOM)
		{
		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)drawX, (int)( drawY -  ( GetSizeY() / 2) ), 1, 1);

		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX + GetSizeX()), (int)( drawY -  (GetSizeY() / 2) ), 1, 1);

		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX + (GetSizeX() * 2)), (int)( drawY - (GetSizeY() / 2) ), 1, 1);

		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX + GetSizeX() * 3), (int)( drawY -  (GetSizeY() / 2) ), 1, 1);
		}
		else if(GetHazPlace() == HAZ_RIGHT)
		{
		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX + (GetSizeX() / 2)), (int)drawY, 1, 1, 0, (float)(GetSizeX() / 2), (float)(GetSizeY() / 2), D3DXToRadian(90));

		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX + (GetSizeX() / 2)), (int)( drawY + GetSizeY() ), 1, 1, 0, (float)(GetSizeX() / 2), (float)(GetSizeY() / 2), D3DXToRadian(90));

		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX + (GetSizeX() / 2)), (int)( drawY + ( GetSizeY() * 2) ), 1, 1, 0, (float)(GetSizeX() / 2), (float)(GetSizeY() / 2), D3DXToRadian(90));

		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX + (GetSizeX() / 2)), (int)( drawY + ( GetSizeY() * 3) ), 1, 1, 0, (float)(GetSizeX() / 2), (float)(GetSizeY() / 2), D3DXToRadian(90));
		}
		else if(GetHazPlace() == HAZ_LEFT)
		{
		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX - (GetSizeX() / 2)), (int)drawY, 1, 1, 0, (float)(GetSizeX() / 2), (float)(GetSizeY() / 2), D3DXToRadian(270));

		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX - (GetSizeX() / 2)), (int)(drawY +  GetSizeY()), 1, 1, 0, (float)(GetSizeX() / 2), (float)(GetSizeY() / 2), D3DXToRadian(270));

		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX - (GetSizeX() / 2)), (int)( drawY + ( GetSizeY() * 2) ), 1, 1, 0, (float)(GetSizeX() / 2), (float)(GetSizeY() / 2), D3DXToRadian(270));

		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX - (GetSizeX() / 2)), (int)( drawY + ( GetSizeY() * 3) ), 1, 1, 0, (float)(GetSizeX() / 2), (float)(GetSizeY() / 2), D3DXToRadian(270));
		}
	}
	else if(GetHazType() == HAZ_LAVA)
	{
		///DRAW THE MOTHER F'N LAVA DUMB ASS
		if(GetHazPlace() == HAZ_TOP)
		{
			CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)drawX, (int)( drawY + ( GetSizeY() / 2) ), 1, -1);
		}
		else if(GetHazPlace() == HAZ_BOTTOM)
		{
			CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)drawX, (int)( drawY - ( GetSizeY() / 2) ), 1, 1);
		}
		else if(GetHazPlace() == HAZ_LEFT)
		{
		CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX - (GetSizeX() / 2)), (int)drawY, 1, 1, 0, (float)(GetSizeX() / 2), (float)(GetSizeY() / 2), D3DXToRadian(270));
		}
		else if(GetHazPlace() == HAZ_RIGHT)
		{
			CSGD_TextureManager::GetInstance()->Draw(this->GetAnimationID(), (int)(drawX + (GetSizeX() / 2)), (int)drawY, 1, 1, 0, (float)(GetSizeX() / 2), (float)(GetSizeY() / 2), D3DXToRadian(90));
		}

		MakeFire(drawX, (float)( drawY - ( GetSizeY() / 2) ), m_bOnce);
	}

	//CGravObject::Render();
}

void CHazards::SetMyRect()
{
	rec = CBaseObject::GetCollideRect();

		if(GetHazPlace() == HAZ_TOP)
		{
			rec.right += (int)GetSizeX() * 3;
		rec.top += (int)GetSizeY();
		rec.bottom += (int)GetSizeY() / 2;
		}
		else if(GetHazPlace() == HAZ_BOTTOM)
		{
			rec.right += (int)GetSizeX() * 3;
		rec.top += (int)GetSizeY() / 2;
		}
		else if(GetHazPlace() == HAZ_LEFT)
		{
			rec.right -= (int)GetSizeX();
		rec.left -= (int)GetSizeX() / 2;
		rec.bottom += (int)GetSizeY() * 4;
		rec.top += (int)GetSizeY();
		}
		else if(GetHazPlace() == HAZ_RIGHT)
		{
			rec.right += (int)GetSizeX() / 2;
		rec.left += (int)GetSizeX();
		rec.bottom += (int)GetSizeY() * 4;
		rec.top += (int)GetSizeY();
		}
}

RECT CHazards::GetCollideRect()
{
	return rec;
}

void CHazards::MakeFire(float x, float y, bool oneTime)
{
	if(oneTime == false)
		return;
	else
		CParticleManager::GetInstance()->SpawnParticle("Particled",x,y,this);

	m_bOnce = false;
}

bool CHazards::CheckCollision(CBaseObject* pbj)
{
	if(pbj->GetType() == TYPE_WALKER || pbj->GetType() == TYPE_HEAVY 
		|| pbj->GetType() == TYPE_SNIPER || pbj->GetType() == TYPE_BOMBARD 
		|| pbj->GetType() == TYPE_PLAYER)
	{
			SetVelX(0); 
			SetVelY(0); 
			SetPosVec(originalPos);
			SetMoving(false); 
	}

	return false; 
}