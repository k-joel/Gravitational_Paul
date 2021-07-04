/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CBaseObject.h"
//
// Author: Justin Dumas (JuD)
//
// Purpose: Declares the Baseobject Class. This will ahve all base functionaloty and interface for gameobjects.
//			These objects are currently unaffected by the gravity field. Also defined in this file is a namespace of constants
//			used by all objects in this hierarchy. This will stream line changes to the physics of the system
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DEF_BASE_OBJECT
#define DEF_BASE_OBJECT

//File inclusions
#include "SGD Wrappers/CSGD_TextureManager.h" //included to get D3DX Files, needed for vectors
#include "CGame.h" //So we can get the window height
//Enum for types
//sorted by the initial 3 values / types, allowing for < and > to be used in collision checks
// and other checks that only apply to specific objects
enum{TYPE_BASE, TYPE_GRAVOBJ, TYPE_PLAYER, TYPE_GRAVWELL, TYPE_ENEMYVAL, TYPE_WALKER, TYPE_HEAVY, TYPE_SNIPER,
	TYPE_BOMBARD, TYPE_HIGHSPEEDOBJ, TYPE_HOVERPLAT, TYPE_ABSORBPLAT, TYPE_HAZARD, TYPE_POWERUP, TYPE_TURRET,
	TYPE_TRIPWIRE, TYPE_ZONE, TYPE_EXIT, TYPE_PARTICLE};

//Enum for obj states. these are used to externaly watch the life of an object.
//these can be used for many purposes in different objects but spawn, default, and dead are universal
//spawn = obj created but not all set up values. Spawning objects will not update. (most objs will have SetState(OBJ_NORMAL) called right after creation and initialization, but should delay be needed the ability is there
//despawn = when an object is set to be destroyed by ingame events or reactions, it will set its state to despawn. this will then have the obj's death animation play in this state. once completely dead, it will change again. (collision can be turned off during death animations this way)
//dead = dead objects will not draw or update. they will be around for only 1 or 2 more ticks as the object manager loops through, but once it removes dead, they will be released and cleaned up. Yay polymorphism.
enum{OBJ_SPAWN, OBJ_NORMAL, OBJ_PAUSED, OBJ_HIT, OBJ_DESPAWN, OBJ_DEAD};

//enum for axis of reflection at collision
enum {AXIS_XONLY, AXIS_YONLY, AXIS_BOTH};

//namespace for defined constants.
//not global, but a specified set of values, that are constant amoung all objects derived from this
//changing these can easily redefine game vars
namespace WorldConstants
{
	const float fTileSize = 16.0f;

	const float fTermVel = fTileSize * 25.0f;

	const float fGravShotSpeed = fTileSize * 15.0f;
	const float fGravity = fTileSize * 15.0f; // time not yet a factor this is per sec^2
	const float fDefaultFloor = 768.0f;
	const float fDefaultCeiling = 0.0f;
	const float fDefaultLWall = 0.0f;
	const float fDefaultRWall = 1024.0f; //world box constants
	const float fPowerUpTimer = 10.0f;
}
class CBaseObject
{
private:

	//object type
	int					m_nType;
	//obj refs
	int					m_nRefCount;

	//private members for movement and collision
	D3DXVECTOR2			m_tPosition;
	D3DXVECTOR2			m_tVelocity;
	D3DXVECTOR2			m_tSize;
	bool				m_bIsSolid;

	//Animation / Rendering Variables
	int					m_nAnimationID;
	int					m_nLayer;

	//object State variable, for OF deletion and other effects
	int					m_nState;

protected:

	float				m_fCeilingValue;
	float				m_fLWallValue;
	float				m_fRWallValue;
	float				m_fFloorValue; //values used for universal collision detection
	bool				m_bFalling;
	float				m_fScreamTime;
	bool				m_bIsAnimated;

	int					m_nAnimID;
public:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “BounceBack”
//
// Purpose: This function is called within the same space function to bounce objects back if needed. This will take a pointer
//			to the object that will be bounced, and the axis to reverse velocity on. This axis int will be a defined by an enum
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	BounceBack(CBaseObject* bounceMe, int nAxis, bool killSpeed = true);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Constructor”
//
// Purpose: Creates and establishes the object at initlal values
//			These are usually 0 or non valid values, but clear and set the memory for use
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CBaseObject();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Destructor”
//
// Purpose: Destroys the object, cleaning up any memory of needed
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
virtual ~CBaseObject();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “overloaded constructor with type”
//
// Purpose: This function creates the object with initial base values like the constructor
//			But also takes in a Type at creation in order to set the object initially
//			This will be called in child constructors. This will be the only modifier there is to
//			object type, preventing objects from changing types during runtime.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CBaseObject(int newType);

///**********************************************
///
///		Main Object Interface Methods
///
///**********************************************

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Update”
//
// Purpose: This function will update the object based on the time increment passed in as a float.
//			This function is virtual so that all object children can overload this is nessescary to.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
virtual void Update(float fDT);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Render”
//
// Purpose: Renders the object to screen. This will draw either placeholder images or final animations depending on the game
//			settings. Currently Animations are ot yet implemented.
//			This function is virtual, should a child class need to overload it.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
virtual void Render();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CollisionReact”
//
// Purpose: This fucntion causes any reactions needed for a collision.
//			It takes in the other objects type to determine reactions.
//			Velocity could be a factor.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
virtual void CollisionReact(int CollideObjType, int nAxis, float fSpeed);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “GetDrawRect”
//
// Purpose: Returns the draw rect of the object based on size. This will be updated for the animation sizes.
//			once those are implemented
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RECT GetDrawRect();

//////////////////////////////////////////////////////////////////
//Purpose: To check collisions to against any other base object
//
/////////////////////////////////////////////////////////////////
virtual bool CheckCollision(CBaseObject *pbj);

////////////////////////////////////////////////
//Purpose: Get the Rect around the object
//
/////////////////////////////////////////////////
virtual RECT GetCollideRect(void);

///***************************
///
///	Object reference Managment
///
///***************************
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “ AddRef ”
//
// Purpose: Adds one to the ref counter for the object. This wll keep track of if anything is still using it
//			This does not need to be virtual as all children wil react the same way. (inlined)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AddRef(){++m_nRefCount;}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Release”
//
// Purpose: This function will decrease the ref count as an object or other part of the program no longer needs the object
//			Once the Ref count is 0, the object will delete its self, allowing the manager to simply release it.
//			This does not need to be virtual as all children will react the same way.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Release();

//////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function: Accessors
//
//  Purpose : Returns the specified type.
///////////////////////////////////////////////////////////////////////////////////////////////////////
	D3DXVECTOR2 GetPosVec(){return m_tPosition;}
	D3DXVECTOR2 GetVelVec(){return m_tVelocity;}
	D3DXVECTOR2 GetSizeVec(){return m_tSize;}
	int			GetType() {return m_nType;}
	int			GetLayer(){return m_nLayer;}
	int			GetAnimationID(){return m_nAnimationID;}
	int			GetObjState(){return m_nState;}
	bool		IsItSolid(){return m_bIsSolid;}
	bool		GetTermVelIsTrue() {return m_bFalling;}
	bool		GetIsAnimated(){return m_bIsAnimated;}; //use for collision rect
	float		GetCeilingValue(){return m_fCeilingValue;}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: Modifiers
//
//  Purpose: Modifies the specified type.
////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetPosVec(D3DXVECTOR2 newPos){m_tPosition = newPos;}
	void SetPosX(float newX){m_tPosition.x = newX;}
	void SetPosY(float newY){m_tPosition.y = newY;}
	void SetVelVec(D3DXVECTOR2 newVel){m_tVelocity = newVel;}
	void SetVelY(float newY){m_tVelocity.y = newY;}
	void SetVelX(float newX){m_tVelocity.x = newX;}
	void SetSize(float newWid, float newHei){m_tSize.x = newWid; m_tSize.y = newHei;}
	void SetLayer(int newLayer){m_nLayer = newLayer;}
	void SetObjState(int newState){m_nState = newState;}
	void SetIsSolid(bool yesNo){m_bIsSolid = yesNo;}
	void SetAnimation(int newID){m_nAnimationID = newID;}
	void SetType(int newType){m_nType = newType;}
	void SetLWall(float newLeft){m_fLWallValue = newLeft;}
	void SetRWall(float newRight){m_fRWallValue = newRight;}
	void SetFloor(float newFloor){m_fFloorValue = newFloor;}
	void SetCeiling(float newCeil){m_fCeilingValue = newCeil;}
	void SetTermVel(bool fall) {m_bFalling = fall;}
	void SetIsAnimated(bool yesNo){m_bIsAnimated = yesNo;}
};

#endif