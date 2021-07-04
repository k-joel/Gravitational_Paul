/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CParticleManager"
//
// Author: Kahran Ghosalkar
//
// Purpose: This manages all the emitters.
//
// Tutorial: Use SpawnParticle() and pass in the name of the particle effect you want to use (e.g. SpawnParticle("GravWell"))
//		Call KillParticle() when you want to destroy the particle (only call it when the effect is continous (e.g. GravWell) )
//		If the particle isn't continuous it will be destroyed automatically when it dies ( e.g. a blood splatter effect )
//		If you are attaching an object(pObj), the posX and posY is relative to the object's position
//
// Current List of Particle Names: "GravWell",
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

class CEmitter;
class CBaseObject;
class CParticleManager
{
private:
	map<string, CEmitter> m_mEmitterList;
	map<int, CEmitter> m_mActiveEmitters;

	int GetUniqueKey();

	CParticleManager();
	~CParticleManager();
	CParticleManager(const CParticleManager&);
	CParticleManager& operator=(const CParticleManager&);
public:
	static CParticleManager* GetInstance();

	void LoadEmitter(string szFileName);
	int SpawnParticle(string szType, float fPosX, float fPosY, CBaseObject* pObj=NULL);
	void KillParticle(int nID);
	void KillParticle(string szType);
	void KillAllParticles();

	void UpdateParticles(float fDT);
	void RenderParticles();

	//Accessors
	//int GetPartType(void) {return pType;}
}; 