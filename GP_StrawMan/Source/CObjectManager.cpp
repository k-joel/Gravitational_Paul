#include "CObjectManager.h"
#include "CBaseObject.h"
#include "CGravWell.h"
#include "CTileManager.h"
#include "CAbsorbPlatform.h"

CObjectManager::CObjectManager(void)
{
}

CObjectManager::~CObjectManager(void)
{
}

CObjectManager* CObjectManager::GetInstance(void)
{
	static CObjectManager pInstance;
	return &pInstance;
}

void CObjectManager::UpdateObjects(float fElapsedTime)
{
	vector<CBaseObject*>::iterator iter = m_vObjectList.begin();

	for(unsigned int i = 0; i < m_vObjectList.size(); ++i)
	{
		(m_vObjectList[i]->Update(fElapsedTime));
	}
}

void CObjectManager::AddObject(CBaseObject *pObject)
{
	if(pObject == NULL)
		return;

	m_vObjectList.push_back(pObject);

	pObject->AddRef();
}

void CObjectManager::RemoveObject(CBaseObject *pObject)
{
	if(pObject == NULL)
		return;

	for(vector<CBaseObject*>::iterator iter = m_vObjectList.begin();
		iter != m_vObjectList.end(); iter++)
	{
		if((*iter) == pObject)
		{
			(*iter)->Release();

			iter = m_vObjectList.erase(iter);
			break;
		}
	}
}

void CObjectManager::RemoveAllObjects(void)
{
	for (unsigned int i = 0; i < m_vObjectList.size(); i++)
	{
		m_vObjectList[i]->Release();
	}

	m_vObjectList.clear();
}

void CObjectManager::RenderObjects(void)
{
	vector<CBaseObject*>::iterator iter = m_vObjectList.begin();

	while(iter != m_vObjectList.end())
	{
		(*iter)->Render();
		iter++;
	}
}

void CObjectManager::RemoveDead()
{
	vector<CBaseObject*>::iterator iter = m_vObjectList.begin();

	for (unsigned int i = 0; i < m_vObjectList.size(); i++)
	{
		if(m_vObjectList[i]->GetObjState() == OBJ_DEAD)
		{
			RemoveObject(m_vObjectList[i]);
		}
	}
}

void CObjectManager::CheckCollisions()
{
	if(m_vObjectList.size() != 0) //if theres no objects, then theres no need for this loop
	{
		for (unsigned int i = 0; i < m_vObjectList.size(); ++i)
		{
			for (unsigned int j = 0; j < m_vObjectList.size(); ++j)
			{
				if(m_vObjectList[i] != m_vObjectList[j]) //if theyre not the same object
				{
					if(m_vObjectList[i]->GetObjState() != OBJ_DEAD && m_vObjectList[j]->GetObjState() != OBJ_DEAD) //and neither are dead
					{
						(CTileManager::GetInstance()->SetObjectPseudoWalls(m_vObjectList[i]));

						if( (m_vObjectList[i]->GetType() == TYPE_POWERUP && m_vObjectList[j]->GetType() == TYPE_PLAYER)
							|| (m_vObjectList[i]->GetType() == TYPE_EXIT && m_vObjectList[j]->GetType() == TYPE_PLAYER))
						{
							m_vObjectList[i]->CheckCollision(m_vObjectList[j]);
						}
						else if(m_vObjectList[i]->GetType() != TYPE_GRAVWELL && m_vObjectList[j]->GetType() != TYPE_GRAVWELL
							    && m_vObjectList[i]->GetType() != TYPE_ZONE && m_vObjectList[j]->GetType() != TYPE_ZONE
								&& m_vObjectList[i]->GetType() != TYPE_HOVERPLAT && m_vObjectList[j]->GetType() != TYPE_HOVERPLAT
								&& m_vObjectList[i]->GetType() != TYPE_ABSORBPLAT && m_vObjectList[j]->GetType() != TYPE_ABSORBPLAT //as long as neither is a grav well or a zone
								&& m_vObjectList[i]->GetType() != TYPE_PARTICLE && m_vObjectList[j]->GetType() != TYPE_PARTICLE
								&& m_vObjectList[i]->GetType() != TYPE_POWERUP && m_vObjectList[j]->GetType() != TYPE_POWERUP //as long as neither is a grav well or a zone
								&& m_vObjectList[i]->GetType() != TYPE_EXIT && m_vObjectList[j]->GetType() != TYPE_EXIT //as long as neither is a grav well or a zone
								&& m_vObjectList[i]->GetType() != TYPE_ZONE && m_vObjectList[j]->GetType() != TYPE_ZONE) //as long as neither is a grav well or a zone
						{
							//collision check functions here
							m_vObjectList[i]->CheckCollision(m_vObjectList[j]);
						}
						else if( (m_vObjectList[i]->GetType() == TYPE_HOVERPLAT || m_vObjectList[i]->GetType() == TYPE_ABSORBPLAT) && m_vObjectList[j]->GetType() == TYPE_PLAYER)
						{
							m_vObjectList[j]->CheckCollision(m_vObjectList[i]); // call j's collison react on i will not react bc of new check
						}
						else if(m_vObjectList[i]->GetType() == TYPE_GRAVWELL && m_vObjectList[j]->GetType() == TYPE_ABSORBPLAT) //if it is an absorb play / grav well collision
						{
							CGravWell* shot = (CGravWell* )m_vObjectList[i]; //type cast the well since we KNOW its a well
							CAbsorbPlatform* plat = (CAbsorbPlatform*)m_vObjectList[j]; //type cast the platform as well

							if( shot->IsItOpen() == false && shot->GetOwner() != NULL ) // now that we KNOW its a well, ttype cast it, and make sure it's closed.
							{
								if(shot->GetOwner()->GetType() == TYPE_PLAYER) //if the closed well if the players
								{
									//checks collision between grav wells and absorb plats
									if (shot->CheckCollision( plat ))
									{
										plat->SetPushPull( shot->GetPushPull() ); //set the plat type to the shot type
										plat->SetForce( shot->GetForce() ); //same with force
										plat->ResetTime(); //reset the timer

										plat->SetUpWell();
										plat->CollisionReact(TYPE_GRAVWELL, AXIS_BOTH, 1.0f);
										shot->Terminate();
									}
								}
								else //if its anyone elses
								{
									shot->Terminate();
								}
							}
						}
					}
				}
			}
		}
	}

	//loop for checking tile collision.
	//This will be done every frame to every object so this will be a seperate loop
}