#include "CTurret.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include "CPlayer.h"


CTurret::CTurret() : CBaseObject(TYPE_TURRET)
{
	m_bCollided = false;
}

CTurret::~CTurret()
{
}

void CTurret::Update(float fDT)
{
	CBaseObject::Update(fDT);

	float x1 = GetPosVec().x;
	float y1 = GetPosVec().y;
	float x2 = m_pPlayer->GetPosVec().x;
	float y2 = m_pPlayer->GetPosVec().y;

	float distance = (x2-x1)*(x2-x1) - (y2-y1)*(y2-y1);
	distance = sqrt(distance);

	if(distance > 300.0f)
		m_bCollided = false;
}

void CTurret::Render()
{
	CBaseObject::Render();

	if(HasCollided())
	{
		CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
		CSGD_Direct3D::GetInstance()->DrawLine((int)GetPosVec().x, (int)GetPosVec().y,
			(int)m_pPlayer->GetPosVec().x, (int)m_pPlayer->GetPosVec().y, 255, 0, 0);
	}
}