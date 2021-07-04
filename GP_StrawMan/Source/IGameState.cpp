/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"IGameState.cpp"
//
// Author: Justin Dumas (JuD)
// 
// Purpose: Defines the abstract base class for all gamestates. This will be derived into multiple gamestates for the logic
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "IGameState.h"

IGameState::IGameState()
{

}

IGameState::~IGameState()
{

}

void IGameState::Enter()
{

}
bool IGameState::Input()
{
	return true;
}
void IGameState::Update(float fDT)
{

}
void IGameState::Render()
{

}
void IGameState::Exit()
{

}

