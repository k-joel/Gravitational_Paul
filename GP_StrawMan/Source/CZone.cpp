/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CZone.cpp"
//
// Author: Justin Dumas (JuD)
// 
// Purpose: This defines the class used to make the zones that influence other gravity wells.
//			They will dynamically change gravity as they open and close.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CZone.h"


#include "CEventSystem.h"


CZone::CZone() : CBaseObject(TYPE_ZONE)
{
	m_fCurrentTime = 0.0f;
	m_fToggleTime = 10.0f;
	m_bOnOff = true;
	m_bPermanent = false;

	m_fRange = 160.0f;
	m_nZoneType = ZONE_REVERSE;

	m_pES = NULL;
	
}

CZone::~CZone()
{

}

void CZone::Update(float fDT)
{	
	if(m_pES == NULL)
	{
		m_pES = CEventSystem::GetInstance();
	}


		m_fCurrentTime += fDT;
		if(m_fCurrentTime >= m_fToggleTime && m_bPermanent == false)
		{
			m_bOnOff = !m_bOnOff;
			m_fCurrentTime -= m_fToggleTime;
		}
		
		if(m_bOnOff)
		{
			if(m_nZoneType == ZONE_REVERSE)
				m_pES->SendEvent("Reversal Zone Open", this);	
			else if(m_nZoneType == ZONE_NEGATE)
				m_pES->SendEvent("Negate Open", this);

		}
		

}

void CZone::Render()
{
	if(m_bOnOff)
	{

		//float drawX = GetPosVec().x - (GetSizeVec().x);
		//float drawY = GetPosVec().y - (GetSizeVec().y);

		if(GetObjState() != OBJ_DEAD)
		{
			if(m_nZoneType == ZONE_REVERSE)
			{
// 				DWORD dwSrc;
// 				DWORD dwDest;
// 
// 				CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND, &dwSrc);
// 				CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND, &dwDest);
// 				
// 				CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
// 
// 				CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
// 				CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

				CSGD_TextureManager::GetInstance()->Draw(GetAnimationID(), int(GetPosVec().x - m_fRange/2.0f), int(GetPosVec().y - m_fRange/2.0f)/*, 1.0f, 1.0f, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255, 255, 255, 255)*/ );

// 				CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
// 
// 				CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, dwSrc);
// 				CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, dwDest);


			}
			else
			{
				CSGD_TextureManager::GetInstance()->Draw(GetAnimationID(), int(GetPosVec().x - m_fRange/2.0f), int(GetPosVec().y - m_fRange/2.0f));
			}
			
		}
	}
}

void CZone::CollisionReact(int CollideObjType, int nAxis, float fSpeed)
{
	//nothing at all.
	//no collision so we overload it empty.

}