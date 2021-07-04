/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CHeavyEnemy.h"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Declares the class for the heavy enemy. This enemy is a large walker who contains his own grvaity well. This well can
//			be either type, have a variable range, and be permanent, or toggle on and off on a timer.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DEF_HEAVY_ENEMY
#define DEF_HEAVY_ENEMY
#include "CWalkerEnemy.h" //derived directly from walker (same AI)

#define DEFAULT_TOGGLE  3.0f
//additional #includes here

//foreward declarations here
class CGravWell;

class CHeavy : public CWalker
{
private:

	CGravWell*		m_pMyWell; //pointer to the contained grav well (this will NOT be added to the OM, the HEAVY will be responsible for this memory)
	bool			m_bTimed; //bool whether the well opens / closes with time
	float			m_fWellTimer; //if it opens this is the increment
	float			m_fCurrentTime; //the current time in its current state
	bool			m_bPushPull; //type of well the heavy contains
	float			m_fRange; // range of the well
	int				m_nWellImageID; //image to render when the well is open
	bool			m_bChangeDirect; //bool determining if the heavy ahs consistant gravoty direction or if that changes randomly as well
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �SetUpWell�
//
// Purpose: This function established the well for the heavy on its first update. This will only be called if the well * is null,
//			making it set up the well on run, based on how the heavy was designed.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void			SetUpWell();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �GravWellToggle�
//
// Purpose: This function is called whenever the timer is up. At this point, the well will open / close, and possibly
//			change direction.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void			GravWellToggle();

public:

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �constructor�
//
// Purpose: Thsi function will establish all of the base data for the class, as well as calling its parent constructor and set up
//			memory for use.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHeavy();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �destructor�
//
// Purpose: This cleans up the memory for the object before it is destroyed. This will destroy the grav well contained in the heavy.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
~CHeavy();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Update�
//
// Purpose: Calls it's parent's update fucntion, and also runs the checks for the gravity well change. This will happen
//			either at a set increment, or never if the well is permanent.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Update(float fDT);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Render�
//
// Purpose: This function simply calls the render for the gravity well and then the render for the heavy.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Render();

bool CheckCollision(CBaseObject* pbj);

//////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function: Accessors
//
//  Purpose : Returns the specified type.
///////////////////////////////////////////////////////////////////////////////////////////////////////
CGravWell* GetWell() {return m_pMyWell;}
bool IsItTimed(){return m_bTimed;}
bool GetPushPull(){return m_bPushPull;}
bool DoesItChangeDirect(){return m_bChangeDirect;}
int GetWellImageID(){return m_nWellImageID;}
float GetToggleTime(){return m_fWellTimer;}
float GetRange() {return m_fRange;}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: Modifiers
//
//  Purpose: Modifies the specified type.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetIsTimed(bool yesNo){m_bTimed = yesNo;}
void SetPushPull(bool pushPull){m_bPushPull = pushPull;}
void SetChangeDirect(bool yesNo){m_bChangeDirect = yesNo;}
void SetDuration(float newDuration){m_fWellTimer = newDuration;}
void SetRange(float newRange){m_fRange = newRange;}
void SetWellImage(int newID){m_nWellImageID = newID;}
};

#endif