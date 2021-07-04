#pragma once

#include <vector>
using std::vector;

class CBaseObject;

class CObjectManager
{
private:
	vector<CBaseObject*> m_vObjectList;

	CObjectManager(void);
	CObjectManager(const CObjectManager&);
	CObjectManager operator=(const CObjectManager&);
	~CObjectManager(void);

	//helper functions for collisions to keep code clean

public:
	static CObjectManager* GetInstance(void);
	void	DeleteInstance(void);

	void	UpdateObjects(float fElapsedTime);
	void    RenderObjects(void);

	void    AddObject(CBaseObject* pObject);
	void    RemoveObject(CBaseObject* pObject);

	void    RemoveAllObjects(void);

	void    CheckCollisions();

	void	RemoveDead();
}; 