#include "CParticle.h"
#include "CGravObject.h"

CParticle::CParticle() : CGravObject(TYPE_PARTICLE)
{
}

CParticle::~CParticle()
{
}

CParticle::CParticle(D3DXVECTOR2 v2Pos, D3DXVECTOR2 v2Vel, D3DXVECTOR2 v2Acc, D3DXVECTOR2 v2RotPos, float fScaleStart,
					 float fScaleEnd, float fLife, float fRot, float fRotSpd, float fFade, Color tStart, Color tEnd, bool bIsAffectedByGrav) : CGravObject(TYPE_PARTICLE)
{
	m_fTimeLived = 0.0f;
	m_fFadeTimer = 0.0f;

	SetPosVec(v2Pos);
	SetVelVec(v2Vel);
	//m_v2Position = v2Pos;
	//m_v2Velocity = v2Vel;
	m_v2Acceleration = v2Acc;
	m_v2RotationPosition = v2RotPos;
	m_fLife = fLife;
	m_fRotation = fRot;
	m_fRotationSpeed = fRotSpd;
	m_fFade = fFade;
	m_fScaleStart = fScaleStart;
	m_fScaleEnd = fScaleEnd;
	m_fCurScale = fScaleStart;
	m_fdScale = (m_fScaleEnd - m_fScaleStart) / (m_fLife + m_fFade);

	m_tCurColor = tStart;
	m_tStartColor = tStart;
	m_tEndColor = tEnd;
	m_fdFA = (0.0f - (float)m_tEndColor.A) / m_fFade;
	m_fdA = float(m_tEndColor.A - m_tStartColor.A) / m_fLife;
	m_fdR = float(m_tEndColor.R - m_tStartColor.R) / m_fLife;
	m_fdG = float(m_tEndColor.G - m_tStartColor.G) / m_fLife;
	m_fdB = float(m_tEndColor.B - m_tStartColor.B) / m_fLife;

	m_bIsAffectedByGrav = bIsAffectedByGrav;

	m_bIsAlive = true;
}

void CParticle::Update(float fDT)
{
	m_fTimeLived += fDT;

	//m_v2Position += m_v2Velocity * fDT + 0.5f * m_v2Acceleration * (fDT*fDT);
	//m_v2Velocity += m_v2Acceleration * fDT;
	//SetPosVec(GetPosVec() + GetVelVec()*fDT + 0.5f * m_v2Acceleration * (fDT*fDT));

	SetPosVec(GetPosVec() + GetVelVec()*fDT);
	if(m_bIsAffectedByGrav)
		SetVelVec(GetVelVec() + (m_v2Acceleration+CheckGravity()) * fDT);
	else
		SetVelVec(GetVelVec() + m_v2Acceleration * fDT);

	m_fRotation += m_fRotationSpeed * fDT;

	if(m_fScaleStart != m_fScaleEnd)
		m_fCurScale = m_fScaleStart + m_fdScale*m_fTimeLived;

	if(m_fTimeLived < m_fLife)
	{
		m_tCurColor.A = int((float)m_tStartColor.A + m_fdA*m_fTimeLived);
		m_tCurColor.R = int((float)m_tStartColor.R + m_fdR*m_fTimeLived);
		m_tCurColor.G = int((float)m_tStartColor.G + m_fdG*m_fTimeLived);
		m_tCurColor.B = int((float)m_tStartColor.B + m_fdB*m_fTimeLived);
	}
	else
	{
		m_fFadeTimer += fDT;
		if(m_fFadeTimer < m_fFade)
			m_tCurColor.A = int((float)m_tEndColor.A + m_fdFA*m_fFadeTimer);
		else
			m_bIsAlive = false;
	}
}

void CParticle::Render(int nImgID, int nWidthCenter, int nHeightCenter, bool bIsLockedToPos, D3DXVECTOR2 v2EmitterPos)
{
	if(bIsLockedToPos)
		SetPosVec(v2EmitterPos);

	CSGD_TextureManager::GetInstance()->Draw(nImgID, (int)(GetPosVec().x - nWidthCenter*m_fCurScale - m_v2RotationPosition.x*m_fCurScale), 
		(int)(GetPosVec().y - nHeightCenter*m_fCurScale - m_v2RotationPosition.y*m_fCurScale), m_fCurScale, m_fCurScale, NULL,
		(float)nWidthCenter + m_v2RotationPosition.x, (float)nHeightCenter + m_v2RotationPosition.y, m_fRotation, m_tCurColor.GetDWORD());
}

float CParticle::Lerp( float v1, float v2, float perc)
{
	if(v1 == v2) return v1;
	return v1 + ((v2 - v1) * perc);
}