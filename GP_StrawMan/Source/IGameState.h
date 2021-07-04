/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"IGameState.h"
//
// Author: Justin Dumas (JuD)
// 
// Purpose: Declares the abstract base class for all gamestates. This will be derived into multiple gamestates for the logic
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DEF_IGAME_STATE
#define DEF_IGAME_STATE

class IGameState
{

public:	
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Constructor�
//
// Purpose: Sets inital values for the game state, establishes usable memory
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IGameState();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Destructor�
//
// Purpose: Destroys the Gamestate, cleaning up memory where needed
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
~IGameState();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Enter�
//
// Purpose: Starts up the GameState to be ready for the next Game loop update. This Function will load all resources needed for 
//			game state as well as set any enter values.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
virtual void Enter() = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Input�
//
// Purpose: Gathers input data for the Game state using Direct Input wrappers. returns false if the game is to shut down
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
virtual bool Input() = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Update�
//
// Purpose: Updates the gamestate, and all relevant members by the increment of time, and execute any logic for the state
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
virtual void Update(float fDT) = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Render�
//
// Purpose: Draws all active state objects to screen, as dictated by state logic.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
virtual void Render() = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: �Exit�
//
// Purpose: Exits the state, clearning memory and shutting down any state exclusive systems. This is called when states are 
//			popped off of the stack
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
virtual void Exit() = 0;
	

};


#endif