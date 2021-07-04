#ifndef CPICKUPS_H_
#define CPICKUPS_H_

#include "CBaseObject.h"
#include "CEventSystem.h"
#include "IListener.h"


enum {PICKUP_CURRENCY,PICKUP_BOUNCE,PICKUP_WEIGHTED };

class CPickups : public CBaseObject
{
private:
	int m_nPickUpType; //ID for type of pickup
	int m_nPickupCollideID; //ID for Sound Effect
	int m_nCurrencyIndexNum; //index number for currency in the level array
	bool m_bSparkleStarted;
	int	m_nSparkleID;

public:
	CPickups();
	//CPickups(int nPickUpType);
	~CPickups();

	void Update(float fDT);
	void Render();
	void CollisionReact(int nCollideObjType, int nAxis, float fSpeed);
	bool CheckCollision(CBaseObject* pbj);

	 int GetPickupType() const {return m_nPickUpType;}
	 int GetCurrencyIndex(){return m_nCurrencyIndexNum;}
	 

	 void SetPickupType(int nPickUpType) { m_nPickUpType = nPickUpType;}
	 void SetCurrencyIndex(int newIndex){m_nCurrencyIndexNum = newIndex;}


};

#endif
