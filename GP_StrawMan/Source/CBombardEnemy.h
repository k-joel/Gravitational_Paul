/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CBombardEnemy.h"
//
// Author: Justin Dumas (JuD)
// 
// Purpose: Declares the bombardier class of enemy, which will shott grav wells at the player. 
//			These will derive from the sniper class and will follow a similar ai.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DEF_BOMBARD_ENEMY
#define DEF_BOMBARD_ENEMY

#include "CSniperEnemy.h" //derives from sniper directly

//additional #includes ehre


//forward declarations here
class CGravWell;
class CPlayer;


class CBombardier : public CSniper
{

private:
	
	CPlayer*		m_pPlayer;
	CGravWell*		m_pMyWell;
	bool			m_bPushPull;
	float			m_fWellTime;
	int				m_nWellImage;
	float			m_fTargetDist;
	float			m_fWellRange;



public:

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function:	"Constructor"
//
// Purpose: This function initlaizes the values for the class and passes the type back to all the parent constructors
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CBombardier();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �destructor�
//
// Purpose: Cleans up virtual memory for the class. In this case will terminate the gravity wells if it is still open
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
~CBombardier();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Aim�
//
// Purpose: Overloaded aim function from the sniper class. This will check for the player and check the aim timer before it fires
//			Overloaded to look for players instead of wells
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Aim(float fDT);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Fire�
//
// Purpose: Fires a grav well if there is no well out. This will fire the enemy well type at the players current location, 
//			setting a distance to wait before it opens.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Fire(D3DXVECTOR2 targetLoc);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �BulletCheck�
//
// Purpose: This checks whether the bullet has travelled far enough yet, and if so, opens it.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BulletCheck(float fDT);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �DropWell�
//
// Purpose: This will be called by the grav well if it terminates with a bombard as its owner. This will simply release the well 
//			and set the pointer to null.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DropWell();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �SpawnWell�
//
// Purpose: This function will be called if the shot is fired, creating a well and sending it to the OM to be handled.
//			The bombard will keep a pointer to it to open it when needed.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SpawnWell(D3DXVECTOR2 shotVec);
	

void CollisionReact(int CollideObjType, int nAxis, float fSpeed);

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: Modifiers
//
//  Purpose: Modifies the specified type.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetPlayer(CPlayer* newPlayer){m_pPlayer = newPlayer;}
void SetWellTime(float newTime){m_fWellTime = newTime;}
void SetWellImage(int newID){m_nWellImage = newID;}
void SetPushPull(bool pushPull){m_bPushPull = pushPull;}

};
#endif