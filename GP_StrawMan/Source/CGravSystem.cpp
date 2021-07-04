/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CGravSystem.cpp"
//
// Author: Justin Dumas (JuD)
//
// Purpose: This simple system stores pointer to active wells and allows gravity objects to
//			Check their current gravity
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CGravSystem.h"

//additional includes
#include "CGravWell.h"
//#include "CTileManager.h"
#include "CEventSystem.h"
#include "CZone.h"

CGravSystem::CGravSystem()
{
	m_pTileMan = NULL;

	//register for events
	CEventSystem::GetInstance()->RegisterClient("GravWell Open", this);

	CEventSystem::GetInstance()->RegisterClient("Reversal Zone Open", this);

	CEventSystem::GetInstance()->RegisterClient("Negate Open", this);
}

CGravSystem::~CGravSystem()
{
	ClearGravField();
	CEventSystem::GetInstance()->UnregisterClient("GravWell Open", this);

	CEventSystem::GetInstance()->UnregisterClient("Reversal Zone Open", this);

	CEventSystem::GetInstance()->UnregisterClient("Negate Open", this);
}

CGravSystem* CGravSystem::GetInstance()
{
	static CGravSystem pInstance;
	return &pInstance;
}

D3DXVECTOR2 CGravSystem::GetGravity(D3DXVECTOR2 currLoc, int nType)
{
	bool bFound = false; //bool for if a well was found
	D3DXVECTOR2 returnVector;
	returnVector.x = 0.0f;
	returnVector.y = 0.0f;

	//loop through each well if not empty
	if(false == m_vAllWells.empty())
	{
		for(unsigned int i = 0; i < m_vAllWells.size(); ++i)
		{
			//dist to well = obj pos  - wel pos (length of vector)
			D3DXVECTOR2 objToWell = m_vAllWells[i]->GetPosVec() - currLoc;
			float wellDist = D3DXVec2Length( &objToWell );


			//check if this is a heavy
			if(nType == TYPE_HEAVY)
			{
				
				//if so check if the well is owned by a heavy
				if(m_vAllWells[i]->GetOwner() != NULL)
				{
					if(m_vAllWells[i]->GetOwner()->GetType() == TYPE_HEAVY)
					{
						//if its a heavy's well, ignore it
						continue;
					}
				}

			}

			//if dist is <= range then return grav vector
			if(wellDist <= m_vAllWells[i]->GetRange() && currLoc != m_vAllWells[i]->GetPosVec() )
			{
				//normalize objtowell vec
				D3DXVec2Normalize(&objToWell, &objToWell);

				//multiply by force of grav well
				objToWell *= m_vAllWells[i]->GetForce();

				//check direction
				//if it is a push well
				if( (m_vAllWells[i]->GetPushPull() == true && m_vAllWells[i]->HasItFlipped() == false )
					|| (m_vAllWells[i]->GetPushPull() == false && m_vAllWells[i]->HasItFlipped() == true) )
				{
					//reverse vector direction
					objToWell = -objToWell;
				}

				//return dynamic gravity vector
				returnVector += objToWell;
				bFound = true;
			}
		}
	}

	//if it breaks out of for loop w/o finding a well in range
	//check tile manager for default gravity

	//to be implemented

	//return normal down gravity for now
	if(bFound == false)
	{
		returnVector.x = 0.0f;
		returnVector.y = WorldConstants::fGravity; //worldConstants can be accessed bc of GravWell.h - no need for the whole namespace here incase it gets large
	}
	return returnVector;
}

D3DXVECTOR2 CGravSystem::GetTileOnlyGrav(D3DXVECTOR2 currLoc)
{
	//this will eventually return tile gravity, once the tile manager is inplemented, for now it returns down.

	D3DXVECTOR2 returnVector;
	returnVector.x = 0.0f;
	returnVector.y = WorldConstants::fGravity; //worldConstants can be accessed bc of GravWell.h - no need for the whole namespace here incase it gets large
	return returnVector;
}

void CGravSystem::AddWell(CGravWell *newWell)
{
	if(NULL != newWell)//being careful
	{
		m_vAllWells.push_back(newWell);
		newWell->AddRef();
	}
}

void CGravSystem::RemoveWell(CGravWell *deadWell)
{
	if(m_vAllWells.empty() != true)
	{
		unsigned int i = 0; //declares iterator variable

		//loop through vector until it gets through them all, or i is the index of the deadWell
		for(; i < m_vAllWells.size() && deadWell != m_vAllWells[i] ; ++i){};

		if(i < m_vAllWells.size())
		{
			if(m_vAllWells[i] == deadWell) //if it exited the loop bc it found the dead well
			{							   //this prevents it from calling relase or earasing a well not in the system, as it is not its job
				//deadWell->Terminate();
				m_vAllWells.erase(m_vAllWells.begin() + i);
				deadWell->Release();
			}
		}
	}
}
void CGravSystem::ClearGravField()
{
	//loops through well releasing wells (they will delet themselves if no one else has refs)
	for(unsigned int i = 0; i < m_vAllWells.size(); ++i)
	{
		
		m_vAllWells[i]->Terminate();
	}

	//clears the vector of the un needed pointers
	m_vAllWells.clear();
}

void CGravSystem::HandleEvent(CEvent* pEvent)
{
	if(pEvent->GetEventID() == "GravWell Open") //if it is an open event
	{
		CGravWell* openWell = (CGravWell*)(pEvent->GetParam()); // make it a grav well *
		AddWell(openWell); // and add this well
		openWell->Release(); // released to prevent mem errors
	}

	if(pEvent->GetEventID() == "Reversal Zone Open")
	{
		CZone* pZone = (CZone*)pEvent->GetParam(); //get the zone *
		float zoneRange = pZone->GetRange(); //get the range stored inside
		D3DXVECTOR2 zoneLoc = pZone->GetPosVec(); //get the zone position
		ZoneToggle(zoneLoc, zoneRange, true); //call the toggle function with this info to toggle on
	}

	if(pEvent->GetEventID() == "Negate Open")
	{
		CZone* pZone = (CZone*)pEvent->GetParam(); //get the zone *
		float zoneRange = pZone->GetRange(); //get the range stored inside
		D3DXVECTOR2 zoneLoc = pZone->GetPosVec(); //get the zone position
		ZoneKill(zoneLoc, zoneRange); //kill wells in that area
	}
}

D3DXVECTOR2 CGravSystem::ClosestWell(D3DXVECTOR2 snipeLoc, bool PlayerOnly)
{
	D3DXVECTOR2 aimPt;
	unsigned int i = 0; //files used to find closest
	bool bWellFound = false; //bool to determine if it has been found at all
	float	fCloseRange = 10000.0f; // closest range so far (well out of range)
	float   fCheckRange; //range to be returned
	if(m_vAllWells.empty() != TRUE) //dont do this if empty, just fail
	{
		for(;i < m_vAllWells.size(); ++i) //loop through wells
		{
			fCheckRange = D3DXVec2Length( &(snipeLoc - m_vAllWells[i]->GetPosVec())); //get the range to current well

			if( fCheckRange < fCloseRange) // check it against the closest so far
			{
				if( PlayerOnly ) //if only looking for player wells (snipers)
				{
					if(m_vAllWells[i]->GetOwner() != NULL)
					{
						if(m_vAllWells[i]->GetOwner()->GetType() != TYPE_PLAYER) //make sure its a player's
						{
							continue; //or else contine
						}
					}
				}
				aimPt = m_vAllWells[i]->GetPosVec(); //if it hasnt continued, the target is the wells location
				fCloseRange = fCheckRange; //this range becomes the new range
				bWellFound = true; //we found at least 1
			}
		}
	}

	if(bWellFound) //if we found on in our loop
	{
		return aimPt; //return the aim pt
	}
	else
	{
		return snipeLoc; // otherwise retrun the fail coondition of the sniper location
	}
}
CGravWell* CGravSystem::LockWell(D3DXVECTOR2 snipeLoc, bool PlayerOnly)
{
	if(m_vAllWells.empty() != TRUE) //dont do this if empty, just fail
	{
		float fCloseRange = 10000.0f; //arbitrary starting close

		for(unsigned int i = 0;i < m_vAllWells.size(); ++i) //loop through wells
		{
			float fCheckRange = D3DXVec2Length( &(snipeLoc - m_vAllWells[i]->GetPosVec())); //get the range to current well

			if( fCheckRange < fCloseRange) // check it against the closest so far
			{
				if( PlayerOnly ) //if only looking for player wells (snipers)
				{
					if(m_vAllWells[i]->GetOwner() != NULL)
					{
						if(m_vAllWells[i]->GetOwner()->GetType() != TYPE_PLAYER) //make sure its a player's
						{
							continue; //or else contine
						}
					}
				}
				return m_vAllWells[i];
			}
		}
	}
	//returns null if not found (should never reach this as this is only called once a well is found)
	return NULL;
}

void CGravSystem::ZoneToggle(D3DXVECTOR2 zoneLoc, float zoneRange, bool zoneOnOff)
{
	if(m_vAllWells.empty() != TRUE) //as long as there are wells
	{
		for( unsigned int i = 0; i < m_vAllWells.size(); ++i) //loop through the wells
		{
			if(m_vAllWells[i]->HasItFlipped() != zoneOnOff) //if its already been flipped this way, ignore it
			{
				float dist = D3DXVec2Length( &(m_vAllWells[i]->GetPosVec() - zoneLoc) ); //get the dist to the zone
				if(dist <= zoneRange) //if its less than the range of the zone
				{
					m_vAllWells[i]->FlipZone(); //flipp it (this flips the zone pull AND the fact that it has been flipped, preventing it from flipping till the off comes
				}
			}
		}
	}
}

void CGravSystem::ZoneKill(D3DXVECTOR2 zoneLoc, float zoneRange)
{
	if(m_vAllWells.empty() != TRUE) //as long as there are wells
	{
		for( unsigned int i = 0; i < m_vAllWells.size(); ++i) //loop through the wells
		{
			float dist = D3DXVec2Length( &(m_vAllWells[i]->GetPosVec() - zoneLoc) ); //get the dist to the zone
			if(dist <= zoneRange) //if its less than the range of the zone
			{
				m_vAllWells[i]->Terminate(); //kill it
			}
		}
	}
}