#include "CTileManager.h"
#include "CObjectManager.h"
#include "CGamePlayState.h"
#include "CBaseObject.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include <fstream>
#include <iostream>
#include "CParticleManager.h"

#include "CHazards.h"
#include "CPickups.h"
using std::fstream;
using std::iostream;
using std::ios_base;

//Constructicon
CTileManager::CTileManager()
{
	//Setting up the pointers.
	m_pOM = CObjectManager::GetInstance();
	m_pOF = CObjectFactory<string, CBaseObject>::GetInstance();
	m_pGS = CGamePlayState::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_nTileHeight = 0;
	m_nTileWidth = 0;
	m_nMapRows = 0;
	m_nMapColumns = 0;
	m_nImageID = -1;
}

CTileManager::~CTileManager()
{
	if(m_nMap)
	{
		delete[] m_nMap;
		m_nMap = NULL;
	}
}

//Loader - eventually put in code so that we can specify
//the level to load.  WORD.  HXC!
void CTileManager::LoadMap()
{
	char fileName[64];
	sprintf_s(fileName, "Resource/Levels/level%i.bin",CGame::GetInstance()->GetLoadLevel());
	fstream fs(fileName, ios_base::in | ios_base::binary);
	if(fs.is_open())
	{
		//Heeeey buddy, reading this crap in.
		fs.read(m_nBitmapName, sizeof(m_nBitmapName));
		fs.read((char *)&m_nTileHeight, sizeof(int));
		fs.read((char *)&m_nTileWidth, sizeof(int));
		fs.read((char *)&m_nMapRows, sizeof(int));
		fs.read((char *)&m_nMapColumns, sizeof(int));

		m_nMap = new tTile[m_nMapRows * m_nMapColumns];

		//Hey man, I'm done readin in the entire map now what what?
		for(int x = 0; x < m_nMapRows * m_nMapColumns; ++x)
		{
			fs.read((char *)&m_nMap[x].nID, sizeof(int));
			fs.read((char *)&m_nMap[x].nSolid, sizeof(int));
			fs.read((char *)&m_nMap[x].nEnemySpawn, sizeof(int));
			m_nMap[x].nTileWidth = m_nTileWidth;
			m_nMap[x].nTileHeight = m_nTileHeight;
		}

		fs.close();
	}

	//Now we loop through the entire map, and if there is an enemy being spawned on a tile, we make one.  Hurray!
	int nCurrencyIndex = 0;
	for(int x = 0; x < m_nMapRows * m_nMapColumns; ++x)
	{
		switch(m_nMap[x].nEnemySpawn)
		{
		case 0:
			break;
		case 1:
			{
				m_pGS->CreateWalker((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight) + 8.0f);
				break;
			}
		case 2:
			{
				m_pGS->CreateHeavy((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight));
				break;
			}
		case 3:
			{
				m_pGS->CreateBombardier((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight));
				break;
			}
		case 4:
			{
				m_pGS->CreateSniper((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight));
				break;
			}
		case 5:
			{
				CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_exitsparkle.bin");
				CParticleManager::GetInstance()->SpawnParticle("exit", (float)(x % m_nMapColumns * m_nTileWidth) + 8.0f, (float)(x / m_nMapColumns * m_nTileHeight) + 8.0f);
				m_pGS->CreateExitDoor((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight) + 16.0f);
				break;
			}
		case 6:
			{
				CGamePlayState::GetInstance()->SetPlayerSpawn((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight) + 16.0f);
				break;
			}
		case 7:
			{
				m_pGS->CreateHazard((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), true, HAZ_SPIKES, HAZ_BOTTOM);
				break;
			}
		case 8:
			{
				m_pGS->CreateHazard((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), true, HAZ_LAVA, HAZ_BOTTOM);
				break;
			}
		case 9:
			{
				m_pGS->CreateAbsorbPlatform((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight));
				break;
			}
		case 10:
			{
				m_pGS->CreateMovablePlatform((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight));
				break;
			}
		case 11:
			{
				if(false == CGame::GetInstance()->GetSaveFile()->GetSaveData()->tAllSaves[CGame::GetInstance()->GetCurrentSaveSlot()].tAllLevels[CGame::GetInstance()->GetLoadLevel() - 1].bGemCollected[nCurrencyIndex])
				{
					m_pGS->CreatePickUp((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight) + 16.0f, PICKUP_CURRENCY, nCurrencyIndex);
				}
				++nCurrencyIndex;
				break;
			}
		case 12:
			{
				m_pGS->CreatePickUp((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), PICKUP_WEIGHTED);
				break;
			}
		case 13:
			{
				m_pGS->CreatePickUp((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), PICKUP_BOUNCE);
				break;
			}
		case 14:
			{
				//Falling spikes
				m_pGS->CreateHazard((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), true, HAZ_FALLING_SPIKES, HAZ_TOP);
				break;
			}
		case 15:
			{
				//Right wall spikes
				m_pGS->CreateHazard((float)((x % m_nMapColumns * m_nTileWidth)) + 32.0f, (float)(x / m_nMapColumns * m_nTileHeight) - 16.0f, true, HAZ_SPIKES, HAZ_LEFT);
				break;
			}
		case 16:
			{
				//Left wall spikes
				m_pGS->CreateHazard((float)((x % m_nMapColumns * m_nTileWidth)), (float)(x / m_nMapColumns * m_nTileHeight) - 16.0f, true, HAZ_SPIKES, HAZ_RIGHT);				
				break;
			}
		case 17:
			{
				//Negative zone
				m_pGS->CreateModZone((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), false);
				break;
			}
		case 18:
			{
				//Reverse zone
				m_pGS->CreateModZone((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), true);
				break;
			}
		case 19:
			{
				//Immune walker
				m_pGS->CreateWalker((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight), true);
				break;
			}
		case 20:
			{
				//Immune heavy
				m_pGS->CreateHeavy((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight), true);
				break;
			}
		case 21:
			{
				//Immune sniper
				m_pGS->CreateSniper((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight), true);
				break;
			}
		case 22:
			{
				//Immune bombardier
				m_pGS->CreateBombardier((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight), true);
				break;
			}
		}
	}

	//Now loading in the actual tile map thing to render from
	string textureName = "Resource/Graphics/";
	textureName = textureName + m_nBitmapName;
	m_nImageID = m_pTM->LoadTexture(textureName.c_str());
}

//Unlaoder
void CTileManager::UnloadMap()
{
	if(m_nMap)
	{
		delete[] m_nMap;
		m_nMap = NULL;
	}
}

void CTileManager::Render()
{
	int nTileRows = m_pTM->GetTextureHeight(m_nImageID) / m_nTileHeight;
	int nTileColumns = m_pTM->GetTextureWidth(m_nImageID) / m_nTileWidth;

	//Looping through the entire map and drawing crap.  Yeah-yuh!
	for (int x = 0; x < m_nMapRows * m_nMapColumns; ++x)
	{
		if(m_nMap[x].nID == -1)
			continue;

		RECT srcRect; //= new Rectangle(m_tMap[i].nID % (int)(m_dTileRows) * (int)(m_dTileWidth), m_tMap[i].nID / (int)(m_dTileRows) * (int)(m_dTileHeight), (int)(m_dTileWidth), (int)(m_dTileHeight));
		srcRect.left = m_nMap[x].nID % nTileRows * m_nTileWidth;
		srcRect.top = m_nMap[x].nID / nTileRows * m_nTileHeight;
		srcRect.bottom = srcRect.top + m_nTileHeight;
		srcRect.right = srcRect.left + m_nTileWidth;

		m_pTM->Draw(m_nImageID, x % m_nMapColumns * m_nTileWidth, x / m_nMapColumns * m_nTileHeight, 1.0f, 1.0f, &srcRect);
	}
}

CTileManager* CTileManager::GetInstance()
{
	static CTileManager pInstance;
	return &pInstance;
}

void CTileManager::SetObjectPseudoWalls(CBaseObject *pObj)
{
	RECT collideRect = pObj->GetCollideRect();

	int XModifier;
	int YModifier;

	YModifier = ((int)pObj->GetSizeVec().y / 16);
	if(YModifier < 1)
		YModifier = 1;

	XModifier = ((int)pObj->GetSizeVec().x / 16);
	if(XModifier < 1)
		XModifier = 1;

	 int topTile = collideRect.top / m_nTileHeight;
	 if(topTile < 0)
		 topTile = 0;

	 int botTile = (collideRect.bottom / m_nTileHeight);
	 if(collideRect.bottom % m_nTileHeight == 0)
		 botTile--;
	 if(botTile < 0)
		 botTile = 0;

	 int leftTile = collideRect.left / m_nTileWidth;
	 if(leftTile < 0)
		 leftTile = 0;

	 int rightTile = (collideRect.right / m_nTileWidth);
	 if(collideRect.right % m_nTileWidth == 0)
		 rightTile--;
	 if(rightTile < 0)
		 rightTile = 0;

	//set ceiling loop
	 if(pObj->GetVelVec().y < 0.0f)
	 {
			for( int i = leftTile; i <= rightTile; ++i)
			{
				float newCeil = (float)(topTile * m_nTileHeight);
				if (topTile > 0)
				{
					if(m_nMap[ ((topTile - 1) * m_nMapColumns) + i].nSolid == 1 )
					{
						pObj->SetCeiling(newCeil);
						break;
					}
				}

				newCeil -= (float)m_nTileHeight * YModifier;
				if(newCeil <= WorldConstants::fDefaultCeiling)
					newCeil = WorldConstants::fDefaultCeiling;
				pObj->SetCeiling(newCeil);
			}
	 }

	//set floor loop

	 if(pObj->GetVelVec().y > 0.0f || pObj->GetType() == TYPE_PLAYER)
	 {
		for(int i = leftTile; i <= rightTile; ++i)
		{
			float newFloor = (float)( ((botTile + 1)  * m_nTileHeight));
			if(botTile > 0)
			{
				if(m_nMap[ ((botTile + 1) * m_nMapColumns) + i].nSolid == 1  )
				{
					pObj->SetFloor(newFloor);
					break;
				}
			}
			newFloor += (float)m_nTileHeight * YModifier;
			if(newFloor >= WorldConstants::fDefaultFloor)
				newFloor = WorldConstants::fDefaultFloor;
			pObj->SetFloor(newFloor);
		}
	 }
	//set lWall loop

	 if(pObj->GetVelVec().x < 0.0f)
	 {
		for( int i = topTile; i <= botTile; ++i)
		{
			float newLWall = (float)(leftTile * m_nTileWidth);
			if ( leftTile > 0 && m_nMap[ (i * m_nMapColumns) + (leftTile - 1)].nSolid == 1)
			{
				pObj->SetLWall(newLWall);
				break;
			}
			else
			{
				newLWall -= (float)m_nTileWidth * XModifier;
				if(newLWall <= WorldConstants::fDefaultLWall)
					newLWall = WorldConstants::fDefaultLWall;
				pObj->SetLWall(newLWall);
			}
		}
	 }

	//set rWall loop
	 if(pObj->GetVelVec().x > 0.0f)
	 {
		for( int i = topTile; i <= botTile; ++i)
		{
			float newRWall = (float)( ((rightTile + 1)  * m_nTileHeight));
			if(rightTile > 0 && m_nMap[ ( i * m_nMapColumns) + (rightTile + 1)].nSolid == 1)
			{
				pObj->SetRWall(newRWall);
				break;
			}

			else
			{
				newRWall += (float)m_nTileWidth * XModifier;
				if(newRWall >= WorldConstants::fDefaultRWall)
					newRWall = WorldConstants::fDefaultRWall;
				pObj->SetRWall(newRWall);
			}
		}
	 }
}

bool CTileManager::CheckPit(CWalker* walker)
{
	D3DXVECTOR2 pos = walker->GetPosVec();
	D3DXVECTOR2 vel = walker->GetVelVec();

	bool bLeft = false;

	if(vel.x < 0)
		bLeft = true;

	if(!bLeft)
	{
		int index = (int)(((int)(pos.y / m_nTileHeight) * m_nMapColumns) + (int)(pos.x / m_nTileWidth));

		if(m_nMap[index + 1].nSolid != 1)
		{
			if(m_nMap[index + 1 + m_nMapColumns].nSolid != 1)
			{
				return true;
			}
		}
	}
	else
	{
		int index = (int)(((int)(pos.y / m_nTileHeight) * m_nMapColumns) + (int)(pos.x / m_nTileWidth));

		if(m_nMap[index - 1].nSolid != 1)
		{
			if(m_nMap[index - 1 + m_nMapColumns].nSolid != 1)
			{
				return true;
			}
		}
	}

	return false;
}

bool CTileManager::CheckSpikes(CWalker* walker)
{
	D3DXVECTOR2 pos = walker->GetPosVec();
	D3DXVECTOR2 vel = walker->GetVelVec();

	bool bLeft = false;

	if(vel.x < 0)
		bLeft = true;

	if(!bLeft)
	{
		//FIND ME
		int index = (int)(((int)(pos.y / m_nTileHeight) * m_nMapColumns) + (int)(pos.x / m_nTileWidth));

		if(m_nMap[index + 1].nEnemySpawn == 16)
		{
			return true;
		}
	}
	else
	{
		int index = (int)(((int)(pos.y / m_nTileHeight) * m_nMapColumns) + (int)(pos.x / m_nTileWidth));

		if(m_nMap[index - 1].nEnemySpawn == 15)
		{
			return true;
		}
	}

	return false;
}

void CTileManager::LoadCustomLevel(std::string szFileName)
{
	char fileName[64];
	sprintf_s(fileName, "Resource/Custom Levels/%s", szFileName.c_str());
	fstream fs(fileName, ios_base::in | ios_base::binary);
	if(fs.is_open())
	{
		//Heeeey buddy, reading this crap in.
		fs.read(m_nBitmapName, sizeof(m_nBitmapName));
		fs.read((char *)&m_nTileHeight, sizeof(int));
		fs.read((char *)&m_nTileWidth, sizeof(int));
		fs.read((char *)&m_nMapRows, sizeof(int));
		fs.read((char *)&m_nMapColumns, sizeof(int));

		m_nMap = new tTile[m_nMapRows * m_nMapColumns];

		//Hey man, I'm done readin in the entire map now what what?
		for(int x = 0; x < m_nMapRows * m_nMapColumns; ++x)
		{
			fs.read((char *)&m_nMap[x].nID, sizeof(int));
			fs.read((char *)&m_nMap[x].nSolid, sizeof(int));
			fs.read((char *)&m_nMap[x].nEnemySpawn, sizeof(int));
			m_nMap[x].nTileWidth = m_nTileWidth;
			m_nMap[x].nTileHeight = m_nTileHeight;
		}

		fs.close();
	}

	//Now we loop through the entire map, and if there is an enemy being spawned on a tile, we make one.  Hurray!
	for(int x = 0; x < m_nMapRows * m_nMapColumns; ++x)
	{
		switch(m_nMap[x].nEnemySpawn)
		{
		case 0:
			break;
		case 1:
			{
				m_pGS->CreateWalker((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight));
				break;
			}
		case 2:
			{
				m_pGS->CreateHeavy((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight));
				break;
			}
		case 3:
			{
				m_pGS->CreateBombardier((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight));
				break;
			}
		case 4:
			{
				m_pGS->CreateSniper((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight));
				break;
			}
		case 5:
			{
				CParticleManager::GetInstance()->LoadEmitter("Resource/Scripts/CbB_exitsparkle.bin");
				CParticleManager::GetInstance()->SpawnParticle("exit", (float)(x % m_nMapColumns * m_nTileWidth) + 8.0f, (float)(x / m_nMapColumns * m_nTileHeight) + 8.0f);
				m_pGS->CreateExitDoor((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight) + 16.0f);
				break;
			}
		case 6:
			{
				CGamePlayState::GetInstance()->SetPlayerSpawn((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight) + 16.0f);
				break;
			}
		case 7:
			{
				m_pGS->CreateHazard((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), true, HAZ_SPIKES, HAZ_BOTTOM);
				break;
			}
		case 8:
			{
				m_pGS->CreateHazard((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), true, HAZ_LAVA, HAZ_BOTTOM);
				break;
			}
		case 9:
			{
				m_pGS->CreateAbsorbPlatform((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight));
				break;
			}
		case 10:
			{
				m_pGS->CreateMovablePlatform((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight));
				break;
			}
		case 11:
			{
				break;
			}
		case 12:
			{
				m_pGS->CreatePickUp((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), PICKUP_WEIGHTED);
				break;
			}
		case 13:
			{
				m_pGS->CreatePickUp((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), PICKUP_BOUNCE);
				break;
			}
		case 14:
			{
				//Falling spikes
				m_pGS->CreateHazard((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), true, HAZ_FALLING_SPIKES, HAZ_TOP);
				break;
			}
		case 15:
			{
				//Right wall spikes
				m_pGS->CreateHazard((float)((x % m_nMapColumns * m_nTileWidth)) + 32.0f, (float)(x / m_nMapColumns * m_nTileHeight) - 16.0f, true, HAZ_SPIKES, HAZ_LEFT);
				break;
			}
		case 16:
			{
				//Left wall spikes
				m_pGS->CreateHazard((float)((x % m_nMapColumns * m_nTileWidth)), (float)(x / m_nMapColumns * m_nTileHeight) - 16.0f, true, HAZ_SPIKES, HAZ_RIGHT);				
				break;
			}
		case 17:
			{
				//Negative zone
				m_pGS->CreateModZone((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), false);
				break;
			}
		case 18:
			{
				//Reverse zone
				m_pGS->CreateModZone((float)((x % m_nMapColumns * m_nTileWidth) + 16.0f), (float)(x / m_nMapColumns * m_nTileHeight), true);
				break;
			}
		case 19:
			{
				//Immune walker
				m_pGS->CreateWalker((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight), true);
				break;
			}
		case 20:
			{
				//Immune heavy
				m_pGS->CreateHeavy((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight), true);
				break;
			}
		case 21:
			{
				//Immune sniper
				m_pGS->CreateSniper((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight), true);
				break;
			}
		case 22:
			{
				//Immune bombardier
				m_pGS->CreateBombardier((float)((x % m_nMapColumns * m_nTileWidth) + 8.0f), (float)(x / m_nMapColumns * m_nTileHeight), true);
				break;
			}
		}
	}

	//Now loading in the actual tile map thing to render from
	string textureName = "Resource/Graphics/";
	textureName = textureName + m_nBitmapName;
	m_nImageID = m_pTM->LoadTexture(textureName.c_str());
}
bool CTileManager::StillOnFloor(CBaseObject* pObj)
{
	RECT collideRect = pObj->GetCollideRect();

	int YModifier;

	YModifier = ((int)pObj->GetSizeVec().y / 16);
	if(YModifier < 1)
		YModifier = 1;

	 int topTile = collideRect.top / m_nTileHeight;
	 if(topTile < 0)
		 topTile = 0;

	 int botTile = (collideRect.bottom / m_nTileHeight);
	 if(collideRect.bottom % m_nTileHeight == 0)
		 botTile--;
	 if(botTile < 0)
		 botTile = 0;

	 int leftTile = collideRect.left / m_nTileWidth;
	 if(leftTile < 0)
		 leftTile = 0;

	 int rightTile = (collideRect.right / m_nTileWidth);
	 if(collideRect.right % m_nTileWidth == 0)
		 rightTile--;
	 if(rightTile < 0)
		 rightTile = 0;

		for(int i = leftTile; i <= rightTile; ++i)
		{
			if(botTile > 0)
			{
				if(m_nMap[ ((botTile + 1) * m_nMapColumns) + i].nSolid == 1  )
				{
					return true;
				}
			}
		}
		return false;
}
void CTileManager::HardResetPseudoWalls(CBaseObject* pObj)
{
	pObj->SetCeiling(WorldConstants::fDefaultCeiling);
	pObj->SetFloor(WorldConstants::fDefaultFloor);
	pObj->SetLWall(WorldConstants::fDefaultLWall);
	pObj->SetRWall(WorldConstants::fDefaultRWall);
}