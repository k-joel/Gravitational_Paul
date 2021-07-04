#pragma once
#include "CBaseObject.h"

class CTurret;
class CTripwire : public CBaseObject
{
	bool m_bCollided;
	CTurret *m_pTurret;

public:
	CTripwire();
	~CTripwire();

	void Update(float fDT);
	void Render();
	void CollisionReact(int CollideObjType, int nAxis, float fSpeed);

	void SetTurret(CTurret *pTurret) { m_pTurret = pTurret; }
};