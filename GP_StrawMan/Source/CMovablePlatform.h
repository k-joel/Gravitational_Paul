#pragma once
#include "CGravObject.h"
#include "CGravSystem.h"
#include "CGravWell.h"

class CMovablePlatform : public CGravObject
{
	float m_fGravResist;
	bool m_bGravImmune;

public:
	CMovablePlatform();
	~CMovablePlatform();
	CMovablePlatform(int nType);

	void Update(float fDT);
	void Render();
	void CollisionReact(int CollideObjType, int nAxis, float fSpeed);

	// Accessors
	bool IsInAWell() {return m_bInWell;}
	bool IsGravityImmune() {return m_bGravImmune;}
	float	GetResistance() {return m_fGravResist;}

	// Mutators
	void SetInWell(bool yesNo){m_bInWell = yesNo;}
	void SetImmune(bool yesNo){m_bGravImmune = yesNo;}
	void SetGravResist(float newResist){m_fGravResist = newResist;}
};