#include "CTripwire.h"
#include "CTurret.h"

CTripwire::CTripwire() : CBaseObject(TYPE_TRIPWIRE)
{
}

CTripwire::~CTripwire()
{
}

void CTripwire::Update(float fDT)
{
	CBaseObject::Update(fDT);
}

void CTripwire::Render()
{
	CBaseObject::Render();
}

void CTripwire::CollisionReact(int CollideObjType, int nAxis, float fSpeed)
{
	CBaseObject::CollisionReact(CollideObjType, nAxis, fSpeed);

	switch(CollideObjType)
	{
	case TYPE_PLAYER:
		m_pTurret->HasCollidedWithTripwire();
		break;
	}
}