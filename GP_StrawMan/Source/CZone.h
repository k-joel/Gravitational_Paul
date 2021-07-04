/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CZone.h"
//
// Author: Justin Dumas (JuD)
// 
// Purpose: This declares the class used to make the zones that influence other gravity wells.
//			They will dynamically change gravity as they open and close.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CBaseObject.h"

//additional #includes here

//forward declarations here
class CEventSystem;

enum {ZONE_REVERSE, ZONE_NEGATE};

class CZone : public CBaseObject
{

private:
	
	float			m_fCurrentTime;
	float			m_fToggleTime;
	bool			m_bOnOff;
	bool			m_bPermanent;

	float			m_fRange;
	int				m_nZoneType;

	CEventSystem*	m_pES;



public:

	CZone();
	~CZone();
	void Update(float fDT);
	void Render();
	void CollisionReact(int CollideObjType, int nAxis, float fSpeed);


	void SetToggleTime(float newTime){m_fToggleTime = newTime;}
	void SetPerm(bool newPerm){m_bPermanent = newPerm;}
	void SetRange(float newRange){m_fRange = newRange;}
	void SetZoneType(int newType) { m_nZoneType = newType;}

	
	bool  IsItPerm() {return m_bPermanent;}
	float GetRange() {return m_fRange;}
	int	  GetZoneType() {return m_nZoneType;}








};