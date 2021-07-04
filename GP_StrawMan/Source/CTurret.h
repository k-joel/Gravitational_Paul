#pragma once
#include "CBaseObject.h"

class CPlayer;
class CTurret : public CBaseObject
{
	bool m_bCollided;
	CPlayer* m_pPlayer;

public:
	CTurret();
	~CTurret();

	void Update(float fDT);
	void Render();

	// mutators
	bool HasCollided() const { return m_bCollided; }

	// accessors
	void HasCollidedWithTripwire(bool bCollide=true) { m_bCollided = bCollide; }
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
};