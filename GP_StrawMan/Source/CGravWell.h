/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CGravWell.h"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Declares the Gravity well objects that will dynamically alter gravity.
//			These will derive from Cbase object, and will add themselves to the gravity system on opening
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DEF_GRAV_WELL
#define DEF_GRAV_WELL

#include "CBaseObject.h" //derives from cbase object
#include "IListener.h" //derived from I listener for event systems

//forward declare gravsystem here as only a pointer is needed.
class CGravSystem;

class CGravWell : public CBaseObject , public IListener
{
private:

	float			m_fForce; //the constant used for gravity magnitude
	float			m_fRange;//the gravitywell's area of effect
	float			m_fDuration; // the time the well lasts once open
	bool			m_bIsOpen; //is the well open
	bool			m_bPushPull;//whether the gravwell pushes orpulls true = push, pull = false
	bool			m_bTemporary;//is the well a permanant or temorary well
	bool			m_bTerminated;
	bool			m_bZoneFlipped; //tells if it has been flipped already
	CGravSystem*	m_pGS; //pointer to gravity system
	int				m_nClosedImage;

	CBaseObject*	m_pOwner; // potential owner

	int				m_nShotDissipateSoundID;
	int				m_nShotBounceSoundID;

	//Temp Powerup bools Acessors and mutators for both GetIsBounce() GetIsWeighted() blah blah
	bool			m_bBounce;
	bool			m_bWeighted;
	int				m_nBounceCounter;

	bool			m_bRebounded;

	int				m_nGravWellParticleID;
	int				m_nClosedWellAnimID;

	int				m_nShotParticles;
	bool			m_bSparkSet;

public:

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “constructor”
//
// Purpose: Similar to the base object constructor this will initialize values to usuable memory.
//			this will also call base object's constructor with a type appropriate for the child
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGravWell();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “destructor”
//
// Purpose: Destroys and cleans up after the object, like in the parent
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
~CGravWell();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Open”
//
// Purpose: This function changes the gravity well from shot to portal form.
//			Velocity will be set to 0, and the bIsOpen Bool will be seel to true.
//			Here the grav well will add itsself to the system and begin counting down.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Open();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Close”
//
// Purpose: This Function closes the gravity well but does not kill it. This will be used for permanent wells and the grav wells
//          inside heavies.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Close();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Terminate”
//
// Purpose: This function sets the grav well to dead, removes it from the grav system, and stops effecting the world.
//			Thsi will then leave it to be cleaned up and deleted
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Terminate();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Update”
//
// Purpose: This fucntion is overloaded to move the well when it is closed and to tick down its timer when it is open.
//			Once the timer reaches 0, it will terminate its self , unless it recieves the call before hand
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Update(float fDT);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CollisionReact”
//
// Purpose: Function is overloaded to only react to certain collisions. Grav wells are not affected by
//			much in the world, but are closed by some enemies and collisions.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CollisionReact(int CollideObjType, int nAxis, float fSpeed);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Render”
//
// Purpose: Currently overloaded for simple drawing when closed, may change based on animation implementation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Render();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “HandleEvent”
//
// Purpose: This function causes the grav well to react to events. The function will read the events for negation and / or
//			reversal zones.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HandleEvent(CEvent* pEvent);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function: Accessors
//
//  Purpose : Returns the specified type.
///////////////////////////////////////////////////////////////////////////////////////////////////////
	float GetForce(){return m_fForce;}
	float GetRange(){return m_fRange;}
	float GetTimeLeft(){return m_fDuration;}
	bool  GetPushPull(){return m_bPushPull;}
	bool  IsItOpen(){return m_bIsOpen;}
	CBaseObject* GetOwner() {return m_pOwner;}
	bool  HasItFlipped() {return m_bZoneFlipped;}

	//Powerup shit
	bool GetIsBounce() {return m_bBounce;}
	bool GetIsWeighted() {return m_bWeighted;}
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: Modifiers
//
//  Purpose: Modifies the specified type.
////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetForce(float newForce){m_fForce = newForce;}
	void SetRange(float newRange){m_fRange = newRange;}
	void SetDuration(float newTime) {m_fDuration = newTime;}
	void SetPushPull(bool newPP){m_bPushPull = newPP;}
	void SetIsTemporary(bool newTemp){m_bTemporary = newTemp;}
	void SetOwner(CBaseObject* newOwner){m_pOwner = newOwner; /*m_pOwner->AddRef();*/}
	void SetClosedImage(int newId){m_nClosedImage = newId;}
	void FlipZone(){ m_bZoneFlipped = !m_bZoneFlipped;}
	void SetAnimID(int nID) { m_nClosedWellAnimID = nID; }
	//The Sounds need to be set in gameplay when you make a shot
	void SetShotBounce(int nShotBounce) {m_nShotBounceSoundID = nShotBounce;}
	void SetShotDissipate(int nShotDissipate) {m_nShotDissipateSoundID = nShotDissipate;}

	//Powerup shit
	void SetIsBounce(bool bBounce) {m_bBounce = bBounce;}
	void SetIsWeighted(bool bWeighted) {m_bWeighted = bWeighted;}
};

#endif