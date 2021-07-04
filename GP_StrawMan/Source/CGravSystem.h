/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CGravSystem.h"
//
// Author: Justin Dumas (JuD)
//
// Purpose: This simple system stores pointer to active wells and allows gravity objects to
//			Check their current gravity
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DEF_GRAV_SYSTEM
#define DEF_GRAV_SYSTEM

#include <vector> // uses vectors to store the pointer to the wells
using std::vector;
#include <d3dx9math.h> //includes math for vectors, does need rest of d3dx
class CGravWell; //forward declares gravwell
class CTileManager; //forward declares tile manager system
#include "IListener.h"

class CGravSystem : public IListener
{
private:

	vector<CGravWell*>		m_vAllWells; //vector of gravwell pointers
	CTileManager*	m_pTileMan; //pointer to tile system

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function:	"Constructor"
//
// Purpose: Sets initial values for the system. Declared private for singleton use
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGravSystem();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Destructor�
//
// Purpose: Used to clean up data before deleting. Made private for singleton use.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
~CGravSystem();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Copy COnstructor�
//
// Purpose: Made to create a new system off of an old. Delacred provate for singleton use.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGravSystem(const CGravSystem&);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �assignment operator�
//
// Purpose: Copies one instance of the class to another. Overloaded for singleton use.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGravSystem& operator = (const CGravSystem&);

public:

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �GetInstance�
//
// Purpose: Returns the current instance of the class. Declares static so it can be called even without an object.
//			If no instance exists, it creates one.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static CGravSystem* GetInstance();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �GetGravity�
//
// Purpose: Returns te Gravity Vector for the location passed in. this will check the gravity wells, and if none are in range,
//			return the tile's natural gravity.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
D3DXVECTOR2  GetGravity(D3DXVECTOR2 currLoc, int nType = 0) ;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �GetTileOnlyGrav�
//
// Purpose: This function will return JUST the tile gravity. This will make sure "Gravity Immune" enemies still react to world
//			gravity, just not to wells in the world.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
D3DXVECTOR2	 GetTileOnlyGrav(D3DXVECTOR2 currLoc);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �ClearGravField�
//
// Purpose: This Loops through the Gravity field releasing all of the wells. Because they are no longer in the system, they will
//			no longer affect the world. This is mainly made for the destruction.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ClearGravField();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �AddWell�
//
// Purpose: Adds a newWell to the system and then calls add ref on it. This well will now be taken into account for all gravity
//			objects.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AddWell(CGravWell* newWell);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �RemoveWell�
//
// Purpose: Loops through the system, finds the well passed into the function, and removes it. This well will be released and
//			no longer affect the world.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RemoveWell(CGravWell* deadWell);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �ClosestWell�
//
// Purpose: This function returns the location of the closest well to a passed in location. This will check for just player wells
//			of the passed in bool is left to defaulr
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
D3DXVECTOR2 ClosestWell(D3DXVECTOR2 snipeLoc, bool PlayerOnly = true);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �LockWell�
//
// Purpose: This function return the Pointer to the closest well to a point passed in. This is for closing a well directly from
//			another object. Snipers and zones will be able to do this.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGravWell* LockWell(D3DXVECTOR2 snipeLoc, bool PlayerOnly = true);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �ZoneToggle�
//
// Purpose: Loops through the Gravwells in the system and flip them accrdingly with the reversal zones.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZoneToggle(D3DXVECTOR2 zoneLoc, float zoneRange, bool zoneOnOff);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �ZoneKill�
//
// Purpose: Lopps through the gravwells in the system and kills them if they are within range
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ZoneKill(D3DXVECTOR2 zoneLoc, float zoneRange);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �HandleEvent�
//
// Purpose: This function reacts to in game events. This will let the player know when their gravity well changes state.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HandleEvent(CEvent* pEvent);
};

#endif