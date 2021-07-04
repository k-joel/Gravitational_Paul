#include "CEmitter.h"
#include "SGD Wrappers/CSGD_Direct3D.h"
#include <cmath>

CEmitter::CEmitter()
{
}

CEmitter::~CEmitter()
{
}

CEmitter::CEmitter(string szName, int nImgID, int nWidthCenter, int nHeightCenter, int nMaxParticles, float fSpawnRateMin, float fSpawnRateMax,
				   D3DXVECTOR2 v2PositionMin, D3DXVECTOR2 v2PositionMax, D3DXVECTOR2 v2VelocityMin, D3DXVECTOR2 v2VelocityMax,
				   D3DXVECTOR2 v2AccelerationMin, D3DXVECTOR2 v2AccelerationMax, D3DXVECTOR2 v2RotationPosMin, D3DXVECTOR2 v2RotationPosMax,
				   float fScaleStartMin, float fScaleStartMax, float fScaleEndMin, float fScaleEndMax, float fLifeMin, float fLifeMax, float fRotationMin,
				   float fRotationMax, float fRotationSpeedMin, float fRotationSpeedMax, float fFadeMin, float fFadeMax,
				   Color tStartColor, Color tEndColor, int nSourceBlend, int nDestBlend, bool bIsCont, bool bIsAffByGrav, bool bBlendEnabled, bool bIsLockedToPos )
{
	m_szName = szName;
	m_nImgID = nImgID;
	m_nWidthCenter = nWidthCenter;
	m_nHeightCenter = nHeightCenter;
	m_nMaxParticles =   nMaxParticles;
	m_fSpawnRateMin =   fSpawnRateMin;
	m_fSpawnRateMax =   fSpawnRateMax;

	m_v2PositionMin =   v2PositionMin;
	m_v2PositionMax =   v2PositionMax;
	m_v2VelocityMin =   v2VelocityMin;
	m_v2VelocityMax =   v2VelocityMax;
	m_v2AccelerationMin = v2AccelerationMin;
	m_v2AccelerationMax = v2AccelerationMax;;
	m_v2RotationPosMin = v2RotationPosMin;
	m_v2RotationPosMax = v2RotationPosMax;
	m_fScaleStartMin = fScaleStartMin;
	m_fScaleStartMax = fScaleStartMax;
	m_fScaleEndMin = fScaleEndMin;
	m_fScaleEndMax = fScaleEndMax;
	m_fLifeMin =		   fLifeMin;
	m_fLifeMax =		   fLifeMax;
	m_fRotationMin =	   fRotationMin;
	m_fRotationMax =	   fRotationMax;
	m_fRotationSpeedMin = fRotationSpeedMin;
	m_fRotationSpeedMax = fRotationSpeedMax;
	m_fFadeMin =		fFadeMin;
	m_fFadeMax =		fFadeMax;
	m_tStartColor =		tStartColor;
	m_tEndColor =		tEndColor;

	m_bIsContinuous = bIsCont;
	m_bIsAffectedByGrav = bIsAffByGrav;
	m_bBlendEnabled = bBlendEnabled;
	m_bIsLockedToPos = bIsLockedToPos;

	m_nSourceBlend = nSourceBlend;
	m_nDestBlend = nDestBlend;

	m_pObjAttached = NULL;

	m_nCount = 0;
	m_fTimeWaited = 0.0f;
	m_bHitLimit = false;
	m_bFinished = false;
}

void CEmitter::Update(float fDT)
{
	m_fTimeWaited += fDT;

	m_fSpawnRate = RandomFloat(m_fSpawnRateMin, m_fSpawnRateMax);

	if(m_pObjAttached)
		m_v2Position = m_pObjAttached->GetPosVec();

	if (!m_bHitLimit && m_lLiveParticleList.size() < (unsigned int)m_nMaxParticles && m_fTimeWaited >= m_fSpawnRate)
	{
		CParticle newParticle(m_v2Position + RandomVec(m_v2PositionMin, m_v2PositionMax),
								RandomVec(m_v2VelocityMin, m_v2VelocityMax),
								RandomVec(m_v2AccelerationMin, m_v2AccelerationMax),
								RandomVec(m_v2RotationPosMin, m_v2RotationPosMax),
								RandomFloat(m_fScaleStartMin, m_fScaleStartMax),
								RandomFloat(m_fScaleEndMin, m_fScaleEndMax),
								RandomFloat(m_fLifeMin, m_fLifeMax),
								RandomFloat(m_fRotationMin, m_fRotationMax),
								RandomFloat(m_fRotationSpeedMin, m_fRotationSpeedMax),
								RandomFloat(m_fFadeMin, m_fFadeMax),
								m_tStartColor, m_tEndColor, m_bIsAffectedByGrav);

		m_lLiveParticleList.push_back(newParticle);

		if(!m_bIsContinuous)
		{
			m_nCount++;
			if(m_nCount >= m_nMaxParticles) m_bHitLimit = true;
		}

		m_fTimeWaited = 0.0f;
	}

	for(list<CParticle>::iterator iter = m_lLiveParticleList.begin(); iter != m_lLiveParticleList.end(); )
	{
		(*iter).Update(fDT);
		if(!(*iter).GetIsAlive())
			iter = m_lLiveParticleList.erase(iter);
		else
			iter++;
	}

	if(m_bHitLimit && m_lLiveParticleList.empty())
		m_bFinished = true;
}

void CEmitter::Render()
{
	DWORD dwSrc;
	DWORD dwDest;

	if(m_bBlendEnabled)
	{
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND, &dwSrc);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND, &dwDest);

		CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, (DWORD)(m_nSourceBlend+1));
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, (DWORD)(m_nDestBlend+1));
	}

	for(list<CParticle>::iterator iter = m_lLiveParticleList.begin(); iter != m_lLiveParticleList.end(); iter++)
	{
		(*iter).Render(m_nImgID, m_nWidthCenter, m_nHeightCenter, m_bIsLockedToPos, m_v2Position);
	}

	if(m_bBlendEnabled)
	{
		CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, dwSrc);
		CSGD_Direct3D::GetInstance()->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, dwDest);
	}
}

float CEmitter::RandomFloat(float a, float b)
{
	if(a == b || a > b) return a;
	return ((b-a)*((float)rand()/RAND_MAX))+a;
}

D3DXVECTOR2 CEmitter::RandomVec(D3DXVECTOR2 a, D3DXVECTOR2 b)
{
	if(a == b || a > b) return a;
	D3DXVECTOR2 tempVec;
	tempVec.x = ((b.x-a.x)*((float)rand()/RAND_MAX))+a.x;
	tempVec.y = ((b.y-a.y)*((float)rand()/RAND_MAX))+a.y;
	return tempVec;
}