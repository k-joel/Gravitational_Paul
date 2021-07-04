/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CTileManager.h"
//
// Author: TJ "Lord of the Tiles" Farnie (TJF)
// 
// Purpose: Declares the tile manager, which will be responsible for loading up the level. It will also
//			render the tiles in the game
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CTILEMANAGER_H
#define CTILEMANAGER_H

struct tTile
{
	int		nID; //The ID of the 
	int		nSolid; //If the tile is solid 0 = no 1 = yes
	int		nTileHeight; //Height o' the tile
	int		nTileWidth; //Width o' the tile
	int		nEnemySpawn; //If an enemy should spawn on the tile or not.  WORD.

};

#include <string>
using std::string;
#include "CObjectFactory.h"
#include "CWalkerEnemy.h"

//Forward declarations
class CObjectManager;
class CGamePlayState;
class CBaseObject;
class CSGD_TextureManager;


class CTileManager
{
private:
	CObjectManager*								m_pOM; //Pointer to the object manager
	CObjectFactory<string, CBaseObject>*		m_pOF; //Pointer to the object factory
	CGamePlayState*								m_pGS; //Pointer to the gameplay state
	CSGD_TextureManager*						m_pTM; //Pointer to the texture manager.
	tTile*										m_nMap; //Pointer to the map of the level.  Will be a delicious array.  Dynamic.  Sorry, a bit hungry.
	char										m_nBitmapName[64]; //Name of the bitmap to load up.
	int											m_nTileHeight; //Height of the tiles
	int											m_nTileWidth; //Width of the tiles
	int											m_nMapRows; //Number of rows in the map
	int											m_nMapColumns; //Number of columns in the map
	int											m_nImageID; //ID of the tile image thang.
	//Private methos to make this thing a singleton.
	CTileManager();
	~CTileManager();
	CTileManager( const CTileManager&);
	CTileManager& operator = (const CTileManager&);
	
public:
	//Used to load a map
	void LoadMap();

	//Used to clean up dynamic memory and get rid of a map
	void UnloadMap();

	//Be renderin' up all the tiles and what have
	void Render();

	//Function to return the singleton pointer
	static CTileManager*  GetInstance();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “SetObjectPseudoWalls”
//
// Purpose: This Function is called by the object manager to set the tile based collision limits. This function will set the 
//			movement limits of the object if the next tile is solid, otherwise, it will set no limit to allow for easy movment.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetObjectPseudoWalls(CBaseObject* pObj);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CheckPit”
//
// Purpose: This function checks the tile FORWARD and then DOWN to see if they're both not solid.  If they are NOT solid, then
//			it's a pit, and it returns true.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CheckPit(CWalker* walker);

bool CheckSpikes(CWalker* walker);

bool StillOnFloor(CBaseObject* pObj);

void HardResetPseudoWalls(CBaseObject* pObj);

//Function to load in custom levels.  Takes in the custom level file name.  Word.
void LoadCustomLevel(string szFileName);
};


#endif