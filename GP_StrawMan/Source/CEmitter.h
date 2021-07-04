#pragma once
#include "CParticle.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include <string>
#include <list>
using namespace std;

class CEmitter
{
	string m_szName;

	list<CParticle> m_lLiveParticleList;

	//Emitter info
	D3DXVECTOR2 m_v2Position;
	int m_nMaxParticles;
	float m_fSpawnRateMin;
	float m_fSpawnRateMax;
	float m_fSpawnRate;
	float m_fTimeWaited;
	bool m_bIsContinuous;
	bool m_bHitLimit;
	bool m_bFinished;
	bool m_bIsAffectedByGrav;
	bool m_bBlendEnabled;
	bool m_bIsLockedToPos;
	int m_nImgID;
	int m_nWidthCenter;
	int m_nHeightCenter;

	//Particle Info
	D3DXVECTOR2 m_v2PositionMin;
	D3DXVECTOR2 m_v2PositionMax;
	D3DXVECTOR2 m_v2VelocityMin;
	D3DXVECTOR2 m_v2VelocityMax;
	D3DXVECTOR2 m_v2AccelerationMin;
	D3DXVECTOR2 m_v2AccelerationMax;
	D3DXVECTOR2 m_v2RotationPosMin;
	D3DXVECTOR2 m_v2RotationPosMax;
	float m_fScaleStartMin;
	float m_fScaleStartMax;
	float m_fScaleEndMin;
	float m_fScaleEndMax;
	float m_fLifeMin;
	float m_fLifeMax;
	float m_fRotationMin;
	float m_fRotationMax;
	float m_fRotationSpeedMin;
	float m_fRotationSpeedMax;
	float m_fFadeMin;
	float m_fFadeMax;
	Color m_tStartColor;
	Color m_tEndColor;
	int m_nCount;

	// blends
	int m_nSourceBlend;
	int m_nDestBlend;

	CBaseObject* m_pObjAttached;

public:
	CEmitter();
	~CEmitter();
	CEmitter& operator=(const CEmitter& e);

	CEmitter(string szName, int nImgID, int nWidthCenter, int nHeightCenter, int nMaxParticles, float fSpawnRateMin, float fSpawnRateMax,
		D3DXVECTOR2 v2PositionMin, D3DXVECTOR2 v2PositionMax, D3DXVECTOR2 v2VelocityMin, D3DXVECTOR2 v2VelocityMax,
		D3DXVECTOR2 v2AccelerationMin, D3DXVECTOR2 v2AccelerationMax, D3DXVECTOR2 v2RotationPosMin, D3DXVECTOR2 v2RotationPosMax,
		float fScaleStartMin, float fScaleStartMax, float fScaleEndMin, float fScaleEndMax, float fLifeMin, float fLifeMax, float fRotationMin,
		float fRotationMax, float fRotationSpeedMin, float fRotationSpeedMax, float fFadeMin, float fFadeMax,
		Color tStartColor, Color tEndColor, int nSourceBlend, int nDestBlend, bool bIsCont, bool bIsAffByGrav, bool bBlendEnabled, bool bIsLockedToPos);

	void Update(float fDT);
	void Render();

	void ClearParticleList() { m_lLiveParticleList.clear(); }

	float RandomFloat(float a, float b);
	D3DXVECTOR2 RandomVec(D3DXVECTOR2 a, D3DXVECTOR2 b);

	//Accessors
	string GetName() const { return m_szName; }
	CBaseObject* GetObject() const { return m_pObjAttached; }
	bool GetFinished() const { return m_bFinished; }

	//Mutators
	void SetImageID(int nImgID) { m_nImgID = nImgID; }
	void SetPosition(float x, float y) { m_v2Position.x = x; m_v2Position.y = y; }
	void SetObj(CBaseObject* pObj) { m_pObjAttached = pObj; }
};