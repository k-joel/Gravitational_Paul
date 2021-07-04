/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"SaveSystem.cpp"
//
// Author: Justin Dumas (JuD)
// 
// Purpose: Declares the structures and information declared for the save system. This will act as a save and load system 
//			automatically at level end. This will load at game launch, and save each time a significant change happens.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "SaveSystem.h"
#include "CGame.h"
#include <fstream>
#include "CUpgradeState.h"
using namespace std;


CGameSaveFile::CGameSaveFile()
{
	szFileName = "\0";
	SaveData.bIsWindowed = true;
	SaveData.fMusicVol = 1.0f;
	SaveData.fSFXVol = 1.0f;
	SaveData.nPanning = 0;
	SaveData.szPlayerName[0] = '\0';
	for(unsigned int i = 0; i < 10 ; ++i)
	{
		SaveData.bAchievmentList[i] = false;
	}
	
	ClearSaveSlot(0);
	ClearSaveSlot(1);
	ClearSaveSlot(2);
	SaveData.nControlSchemes[0] = 0;
	SaveData.nControlSchemes[1] = 0;

	
}

CGameSaveFile::~CGameSaveFile()
{

}

CGameSaveFile::CGameSaveFile(const CGameSaveFile &newGame)
{

}

CGameSaveFile& CGameSaveFile::operator =(const CGameSaveFile &newGame)
{
	return *this;
}

void CGameSaveFile::LevelComplete(unsigned int fileNum, unsigned int lvlNum, float time)
{
	if( false == SaveData.tAllSaves[fileNum].tAllLevels[lvlNum - 1].bComplete || time < SaveData.tAllSaves[fileNum].tAllLevels[lvlNum - 1].fTimeRecord)
	{
		SaveData.tAllSaves[fileNum].tAllLevels[lvlNum - 1].bComplete = true;
		SaveData.tAllSaves[fileNum].tAllLevels[lvlNum - 1].fTimeRecord = time;
		SaveData.tAllSaves[fileNum].bFreshSlot = false;
		Save();
		CGame::GetInstance()->SyncWithFile(CGame::GetInstance()->GetCurrentSaveSlot());
		
	}
}

bool CGameSaveFile::Upgrade(unsigned int fileNum, unsigned int Stat)
{	
	int currLevel;
	currLevel = SaveData.tAllSaves[fileNum].nUpgrades[Stat];
	if( 5 > currLevel)
	{
		int cost;
		switch(currLevel + 1)
		{
		case 1:
			{
				cost = COST_1;
				break;
			}
		case 2:
			{
				cost = COST_2;
				break;
			}
		case 3:
			{
				cost = COST_3;
				break;
			}
		case 4:
			{
				cost = COST_4;
				break;
			}
		case 5:
			{
				cost = COST_5;
				break;
			}
		default:
			{
				return false;
			}
		}
		if(cost <= SaveData.tAllSaves[fileNum].nUnspentCurrency)
		{
			SaveData.tAllSaves[fileNum].nUnspentCurrency -= cost;
			SaveData.tAllSaves[fileNum].nUpgrades[Stat]++; //should only happen until 5
			SaveData.tAllSaves[fileNum].bFreshSlot = false;
			//check for one trick pony achiev
			if(cost == COST_5)
			{
				if(CheckAchievment(ACHIEV_1MAX))
				{
					CGame::GetInstance()->AchievmentUnlocked(ACHIEV_1MAX);
				}
				
				if(SaveData.tAllSaves[fileNum].nUpgrades[0] == 5 &&
					SaveData.tAllSaves[fileNum].nUpgrades[1] == 5 &&
					SaveData.tAllSaves[fileNum].nUpgrades[2] == 5)
				{
					if(CheckAchievment(ACHIEV_ALLMAX))
					{
						CGame::GetInstance()->AchievmentUnlocked(ACHIEV_ALLMAX);
					}
				}
			}

			
			
			Save();
			CGame::GetInstance()->SyncWithFile(CGame::GetInstance()->GetCurrentSaveSlot());
			


			return true;
		}

	}
	return false;
}

void CGameSaveFile::GemCollected(unsigned int fileNum, unsigned int levelNum,  int gemNum)
{
	if(gemNum > -1 && gemNum < 15)
	{
		if(false == SaveData.tAllSaves[fileNum].tAllLevels[levelNum - 1].bGemCollected[gemNum])
		{
			SaveData.tAllSaves[fileNum].nUnspentCurrency += (3 - SaveData.tAllSaves[fileNum].nDifficulty);
			SaveData.tAllSaves[fileNum].tAllLevels[levelNum - 1].bGemCollected[gemNum] = true;
			SaveData.tAllSaves[fileNum].bFreshSlot = false;
			
			//if all currency has been found
			if(CheckAllCurrency(fileNum))
			{
				if(CheckAchievment(ACHIEV_MCDUCK))
					CGame::GetInstance()->AchievmentUnlocked(ACHIEV_MCDUCK); //chekc for the achieve
			}


			Save();
			CGame::GetInstance()->SyncWithFile(CGame::GetInstance()->GetCurrentSaveSlot());

		}
	}
}
bool CGameSaveFile::Save()
{
	fstream myFile(this->szFileName.c_str(), ios_base::out | ios_base::binary);
	if(myFile.is_open())
	{
		myFile.write((char*)(&SaveData), sizeof(tFileData));
		myFile.close();
		return true;
	}
	else
		return false;
	




	
}



bool CGameSaveFile::Load()
{
	fstream myFile(this->szFileName.c_str(), ios_base::in | ios_base::binary);
	if(myFile.is_open())
	{
		myFile.read((char*)(&SaveData), sizeof(tFileData));
		myFile.close();
		return true;
	}
	return true;
}
void CGameSaveFile::ClearSaveSlot(unsigned int slotNum)
{
	tSaveSlot* empty = &(SaveData.tAllSaves[slotNum]);
	empty->nUnspentCurrency = 0;
	empty->nDifficulty = DIFF_EASY;
	empty->nUpgrades[0] = 0;
	empty->nUpgrades[1] = 0;
	empty->nUpgrades[2] = 0;
	for(unsigned int i = 0; i < 20; ++i)
	{
		empty->tAllLevels[i].bComplete = false;
		empty->tAllLevels[i].fTimeRecord = 300.0f;
		for(unsigned int j = 0; j < 15; ++j)
		{
			empty->tAllLevels[i].bGemCollected[j] = false;
		}
	}
	empty->bFreshSlot = true;
	empty->bEverDied = false;
}
void CGameSaveFile::ClearEntireFile()
{
	szFileName = "\0";
	SaveData.bIsWindowed = true;
	SaveData.fMusicVol = 1.0f;
	SaveData.fSFXVol = 1.0f;
	SaveData.nPanning = 0;
	SaveData.szPlayerName[0] = '\0';
	for(unsigned int i = 0; i < 10 ; ++i)
	{
		SaveData.bAchievmentList[i] = false;
	}
	ClearSaveSlot(0);
	ClearSaveSlot(1);
	ClearSaveSlot(2);
}

bool CGameSaveFile::CheckAchievment(unsigned int whichOne)
{
	if(whichOne > 9)
		return false;

	if(SaveData.bAchievmentList[whichOne] == false)
	{
		SaveData.bAchievmentList[whichOne] = true;
		Save();
		return true;
	}
	else
		return false;


}
bool CGameSaveFile::CheckLevelCurrency(unsigned int nLevel, unsigned int nSlot)
{
	
	//for each currecy in the level
	for(unsigned int i = 0; i < 15; ++i)
	{
		//if any gem in the level has not been found, return false
		if( SaveData.tAllSaves[nSlot].tAllLevels[nLevel].bGemCollected[i] == false)
			return false;
	}
	//otherwise return true
	return true;



}

bool CGameSaveFile::CheckAllCurrency(unsigned int nSlot)
{
	
	//for every leevl in the game
	for(unsigned int i = 0; i < 20; ++i)
	{
		//if any level has currency still to collect return false
		if( CheckLevelCurrency(i, nSlot) == false)
			return false;
	}
	return true;

}
int CGameSaveFile::CountLevelCurrency(unsigned int nLevel, unsigned int nSlot)
{
	int count = 0;

	//for each gem in the level
	for(unsigned int i = 0; i < 15; ++i)
	{
		if(SaveData.tAllSaves[nSlot].tAllLevels[nLevel].bGemCollected[i] == true)
			++count;
	}
	return count;
}

