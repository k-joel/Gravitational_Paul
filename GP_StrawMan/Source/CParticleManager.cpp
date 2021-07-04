#include "CParticleManager.h"
#include "CEmitter.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include <time.h>
#include <fstream>
using std::fstream;

CParticleManager::CParticleManager()
{
	srand(unsigned int(time(0)));
}

CParticleManager::~CParticleManager()
{
}

CParticleManager* CParticleManager::GetInstance()
{
	static CParticleManager instance;
	return &instance;
}

int CParticleManager::GetUniqueKey()
{
	for (;;)
	{
		int nID = rand() % INT_MAX;
		if(m_mActiveEmitters.count(nID) == 0)
			return nID;
	}
	return -1;
}

int CParticleManager::SpawnParticle(string szType, float fPosX, float fPosY, CBaseObject* pObj)
{
	CEmitter tempEmitter = m_mEmitterList[szType];
	tempEmitter.SetPosition(fPosX, fPosY);
	tempEmitter.SetObj(pObj);
	int nID = GetUniqueKey();
	m_mActiveEmitters.insert(pair<int, CEmitter>(nID, tempEmitter));
	return nID;
}

void CParticleManager::KillParticle(int nID)
{
	if(m_mActiveEmitters.count(nID) == 1 )
		m_mActiveEmitters.erase(m_mActiveEmitters.find(nID));
}

void CParticleManager::KillParticle(string szType)
{
	for(map<int, CEmitter>::iterator iter = m_mActiveEmitters.begin(); iter != m_mActiveEmitters.end(); )
	{
		if((*iter).second.GetName() == szType)
			m_mActiveEmitters.erase(iter++);
		else
			iter++;
	}
}

void CParticleManager::KillAllParticles()
{
	m_mActiveEmitters.clear();
}

void CParticleManager::LoadEmitter(string szFileName)
{
	ifstream fin;
	fin.open(szFileName.c_str(), ios_base::in | ios_base::binary);
	fin.seekg(0);
	if(fin.is_open())
	{
		int nNumEmitters;
		fin.read((char*)&nNumEmitters, sizeof(int));

		for(int i = 0; i < nNumEmitters; i++)
		{
			string szName,szImgFileName;
			for(int i = 0; i < 64; i++)
			{
				char c;
				fin.read((char*)&c, sizeof(char));
				if(c == '\0')
					break;

				szName+= c;
			}

			for(int i = 0; i < 64; i++)
			{
				char c;
				fin.read((char*)&c, sizeof(char));
				if(c == '\0')
					break;

				szImgFileName+= c;
			}

			string fullpath = "Resource/Graphics/" + szImgFileName;
			int nImgID = CSGD_TextureManager::GetInstance()->LoadTexture(fullpath.c_str());

			int nIsCont,nIsGravAffec, nIsBlendEnabled, nIsLockedToPos, nMaxParticles;

			fin.read((char*)&nIsCont, sizeof(int));
			fin.read((char*)&nIsGravAffec, sizeof(int));
			fin.read((char*)&nIsBlendEnabled, sizeof(int));
			fin.read((char*)&nIsLockedToPos, sizeof(int));

			fin.read((char*)&nMaxParticles, sizeof(int));

			D3DXVECTOR2 v2PositionMin, v2PositionMax, v2VelocityMin, v2VelocityMax, v2ScaleMin, v2ScaleMax, v2AccelerationMin,
								v2AccelerationMax, v2RotationPosMin, v2RotationPosMax;

			float fSpawnRateMin, fSpawnRateMax, fScaleStartMin, fScaleStartMax, fScaleEndMin, fScaleEndMax,
				fLifeMin, fLifeMax, fFadeMin, fFadeMax, fRotMin, fRotMax, fRotSpdMin, fRotSpdMax;

			fin.read((char*)&fSpawnRateMin, sizeof(float));
			fin.read((char*)&fSpawnRateMax, sizeof(float));
			fin.read((char*)&v2PositionMin.x, sizeof(float));
			fin.read((char*)&v2PositionMin.y, sizeof(float));
			fin.read((char*)&v2PositionMax.x, sizeof(float));
			fin.read((char*)&v2PositionMax.y, sizeof(float));
			fin.read((char*)&v2VelocityMin.x, sizeof(float));
			fin.read((char*)&v2VelocityMin.y, sizeof(float));
			fin.read((char*)&v2VelocityMax.x, sizeof(float));
			fin.read((char*)&v2VelocityMax.y, sizeof(float));
			fin.read((char*)&v2AccelerationMin.x, sizeof(float));
			fin.read((char*)&v2AccelerationMin.y, sizeof(float));
			fin.read((char*)&v2AccelerationMax.x, sizeof(float));
			fin.read((char*)&v2AccelerationMax.y, sizeof(float));
			fin.read((char*)&fScaleStartMin, sizeof(float));
			fin.read((char*)&fScaleStartMax, sizeof(float));
			fin.read((char*)&fScaleEndMin, sizeof(float));
			fin.read((char*)&fScaleEndMax, sizeof(float));
			fin.read((char*)&fLifeMin, sizeof(float));
			fin.read((char*)&fLifeMax, sizeof(float));
			fin.read((char*)&fFadeMin, sizeof(float));
			fin.read((char*)&fFadeMax, sizeof(float));
			fin.read((char*)&fRotMin, sizeof(float));
			fin.read((char*)&fRotMax, sizeof(float));
			fin.read((char*)&fRotSpdMin, sizeof(float));
			fin.read((char*)&fRotSpdMax, sizeof(float));
			fin.read((char*)&v2RotationPosMin.x, sizeof(float));
			fin.read((char*)&v2RotationPosMin.y, sizeof(float));
			fin.read((char*)&v2RotationPosMax.x, sizeof(float));
			fin.read((char*)&v2RotationPosMax.y, sizeof(float));

			Color tStart,tEnd;
			fin.read((char*)&tStart.A, sizeof(int));
			fin.read((char*)&tStart.R, sizeof(int));
			fin.read((char*)&tStart.G, sizeof(int));
			fin.read((char*)&tStart.B, sizeof(int));

			fin.read((char*)&tEnd.A, sizeof(int));
			fin.read((char*)&tEnd.R, sizeof(int));
			fin.read((char*)&tEnd.G, sizeof(int));
			fin.read((char*)&tEnd.B, sizeof(int));

			int nWidthCenter, nHeightCenter, nSource, nDest;
			fin.read((char*)&nWidthCenter, sizeof(int));
			fin.read((char*)&nHeightCenter, sizeof(int));

			fin.read((char*)&nSource, sizeof(int));
			fin.read((char*)&nDest, sizeof(int));

			CEmitter tempEmitter(szName, nImgID, nWidthCenter,nHeightCenter, nMaxParticles, fSpawnRateMin, fSpawnRateMax, v2PositionMin, v2PositionMax,
				 v2VelocityMin, v2VelocityMax, v2AccelerationMin, v2AccelerationMax, v2RotationPosMin, v2RotationPosMax, fScaleStartMin, fScaleStartMax,
				 fScaleEndMin, fScaleEndMax, fLifeMin,fLifeMax, fRotMin, fRotMax, fRotSpdMin, fRotSpdMax, fFadeMin, fFadeMax, tStart, tEnd, nSource, nDest,
				  (bool)nIsCont, (bool)nIsGravAffec, (bool)nIsBlendEnabled, (bool)nIsLockedToPos);

			m_mEmitterList.insert(pair<string, CEmitter>(szName, tempEmitter));
		}
	}
	fin.close();
}

void CParticleManager::UpdateParticles(float fDT)
{
	for(map<int, CEmitter>::iterator iter = m_mActiveEmitters.begin(); iter != m_mActiveEmitters.end(); )
	{
		(*iter).second.Update(fDT);
		if((*iter).second.GetFinished())
			m_mActiveEmitters.erase(iter++);
		else
			iter++;
	}
}

void CParticleManager::RenderParticles()
{
	for(map<int, CEmitter>::iterator iter = m_mActiveEmitters.begin(); iter != m_mActiveEmitters.end(); iter++)
		(*iter).second.Render();
}