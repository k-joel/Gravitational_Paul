#pragma once
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "CGravObject.h"

struct Color
{
	int A,R,G,B;
	DWORD GetDWORD() { return D3DCOLOR_ARGB(A,R,G,B); }
	Color& operator=(const Color& c)
	{
		if(&c == this) return *this;
		A = c.A; R = c.R; G = c.G; B = c.B;
		return *this;
	}
};

class CBaseObject;
class CParticle : public CGravObject
{
	//D3DXVECTOR2 m_v2Position;
	//D3DXVECTOR2 m_v2Velocity;
	D3DXVECTOR2 m_v2Acceleration;
	D3DXVECTOR2 m_v2RotationPosition;
	float m_fScaleStart;
	float m_fScaleEnd;
	float m_fCurScale;
	float m_fRotation;
	float m_fRotationSpeed;
	float m_fLife;
	float m_fTimeLived;
	float m_fFade;
	float m_fFadeTimer;
	Color m_tStartColor;
	Color m_tEndColor;
	Color m_tCurColor;
	bool m_bIsAlive;
	bool m_bIsAffectedByGrav;

	// rates
	float m_fdFA;
	float m_fdA;
	float m_fdR;
	float m_fdG;
	float m_fdB;
	float m_fdScale;

public:
	CParticle();
	~CParticle();

	CParticle(D3DXVECTOR2 v2Pos, D3DXVECTOR2 v2Vel, D3DXVECTOR2 v2Acc, D3DXVECTOR2 v2RotPos, float fScaleStart,
		float fScaleEnd, float fLife, float fRot, float fRotSpd, float fFade, Color tStart, Color tEnd, bool bIsAffectedByGrav);

	void Update(float fDT);
	void Render(int nImgID, int nWidthCenter, int nHeightCenter, bool bIsLockedToPos, D3DXVECTOR2 v2EmitterPos);

	float Lerp(float v1, float v2, float perc);

	// Accessors
	bool GetIsAlive() const { return m_bIsAlive; }
};