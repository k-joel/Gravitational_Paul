/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CWalkerEnemy.h"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Declares the CWalker enemy which will be the basic enemy. This will establish base values for it and then from here
//			update will handle the rest. Only position should need to be set and then the game engine will handle the rest.
//			The heavy will be derived from this.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DEF_WALKER_ENEMY
#define DEF_WALKER_ENEMY

#include "CGravObject.h" //directly derives from GravObject

//additional #includs here

//forward declarations here
class CPlayer;

class CWalker : public CGravObject
{
private:

	float			m_fAttackRange;
	float			m_fGravResist; // float for the percent of grav reduction (this will be multiplied by the grav return value)
	bool			m_bGravImmune; //bool for if it is immune to grav wells
	bool			m_bFaceDir; //bool for face dir - true = left, fasle = right
	CPlayer*		m_pPlayer;
	bool			m_bGrounded;
	bool			m_bAuraSwitch;
	int				m_nAuraID;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �CheckAI�
//
// Purpose: This function will change variables about the enemy based on AI. This will cause the eenemy action patters,
//			but only if the enemy is not in a grav well.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
virtual void CheckAI();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �CheckPlayerLoc�
//
// Purpose: This will check the distance between the player and the enemy, and react accordingly. This calculaition is based on
//			the enemy attack range.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CheckPlayerLoc();

public:

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Constructor�
//
// Purpose: Creates the object with all of its main variables ready to be used.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWalker();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �destructor�
//
// Purpose: Cleans up memory for the object before deletion.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
virtual ~CWalker();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Overloaded constructor�
//
// Purpose: This over load of the contructor takes a passed in type, and sets the parent class information accordingly.
//			This is used for children.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWalker(int nType);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Update�
//
// Purpose: Updates the object based on time. This will call check AI, resist gravity, move with gravoty, and even only react to
//			default tile gravity, if the enemy is immune.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Update(float fDT);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Render�
//
// Purpose: Renders the animation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Render();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Collision react�
//
// Purpose: Reacts to collisions for the enemy. This will cause it to die when needed.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CollisionReact(int CollideObjType, int nAxis, float fSpeed);
bool CheckCollision(CBaseObject* pbj);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function: Accessors
//
//  Purpose : Returns the specified type.
///////////////////////////////////////////////////////////////////////////////////////////////////////
bool IsInAWell() {return m_bInWell;}
bool IsGravityImmune() {return m_bGravImmune;}
CPlayer* GetPlayer() {return m_pPlayer;}
float	GetAttackRange() {return m_fAttackRange;}
float	GetResistance() {return m_fGravResist;}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: Modifiers
//
//  Purpose: Modifies the specified type.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetInWell(bool yesNo){m_bInWell = yesNo;}
void SetImmune(bool yesNo){m_bGravImmune = yesNo;}
void SetPlayer(CPlayer* newPlayer){m_pPlayer = newPlayer;}
void SetRange(float newRange){m_fAttackRange = newRange;}
void SetGravResist(float newResist){m_fGravResist = newResist;}
};

#endif