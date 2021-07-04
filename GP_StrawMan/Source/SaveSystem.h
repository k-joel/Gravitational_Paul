/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"SaveSystem.h"
//
// Author: Justin Dumas (JuD)
// 
// Purpose: Declares the structures and information declared for the save system. This will act as a save and load system 
//			automatically at level end. This will load at game launch, and save each time a significant change happens.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SAVESYSTEM_H_
#define SAVESYSTEM_H_

#include <string>
using std::string;
//enum for the stat values when passed in
enum {STAT_RECHARGE, STAT_RANGE, STAT_DURATION};
enum {COST_1 = 5, COST_2 = 10, COST_3 = 15, COST_4 = 20, COST_5 = 50};
enum {DIFF_EASY, DIFF_NORMAL, DIFF_HARD};
enum {ACHIEV_EASYBEAT, ACHIEV_NORMBEAT, ACHIEV_HARDBEAT, ACHIEV_CHAMP, ACHIEV_CHUMP, ACHIEV_1MAX, ACHIEV_ALLMAX, ACHIEV_MCDUCK, ACHIEV_CUSTOMLVL, ACHIEV_NOUPGRADE};

struct tLevelInfo //data about each level in the possible main game
{				  //this stores all of the data a level needs to be reloaded correctly
	
	bool	bComplete; //has the player completed this level
	float	fTimeRecord; //the fastest time they have completeed it in
	bool	bGemCollected[15]; //which gems have been collected in the level

};


struct tSaveSlot //this holds the info for the player's profile. this will hold player name, as well as info on 20 levels
{					  // and the player's personal upgrades
	bool			bFreshSlot; //is this slot empty?
	tLevelInfo		tAllLevels[20];
	
	//gems spent out of those earned will be determined by the upgrade levels, as each upgrade will have a set value.
	int				nDifficulty; //diffculty for the save slot
	int				nUpgrades[3]; //int 0 - 5 for upgrades, used in formulas, indeces are matched with enum recharge = 0, range = 1, duration = 2
	int				nUnspentCurrency;
	bool			bEverDied;
};

struct tFileData
{		
	char					szPlayerName[32];
	tSaveSlot				tAllSaves[3];
	bool					bIsWindowed;
	float					fSFXVol;
	float					fMusicVol;
	int						nPanning;
	bool					bAchievmentList[10];
	int						nControlSchemes[2];//0 is controller, 1 is keyboard
};


class CGameSaveFile //this self managing save file for the game will load based on
{

private:
	string					szFileName;
	tFileData				SaveData;
	
	


public:

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “constructor”
//
// Purpose: This function will declare the variables needed for the file. This will set the save file to null, making sure that
//			A file is chosen before it is messed with.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGameSaveFile();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “destructor”
//
// Purpose: This function cleans up all dynamic memory for the class before deletino. In this case, this is the char* that leads to 
//			the filename.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
~CGameSaveFile();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “copy constructor”
//
// Purpose: This function declares the variables of a new save file to copy an existing one.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGameSaveFile(const CGameSaveFile& newGame);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “assignment operator”
//
// Purpose: This function will set the values of an existing save file to that of  another existing one.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGameSaveFile& operator = (const CGameSaveFile& newGame);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Save”
//
// Purpose: This Will save the save file out as a binary file. This will simply vomit out the struct inside. If the file name 
//			has not been set yet, this will return a false, as well as if the saving fails.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Save();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Load”
//
// Purpose: This will load the save file set as the char*. Should this 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Load();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Level Complete”
//
// Purpose: This function takes information from teh game and gameplay state to store the information. A save filenumber 
//			( the current save file num from CGame), as well as a level number, the level complete time, and a count for total 
//			gems collected. If these are new records, they will be changed in the players save, and if there were more gems found, 
//			these will be added to the players currency.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LevelComplete(unsigned int fileNum, unsigned int lvlNum, float time);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Upgrade”
//
// Purpose: This function upgrades a stat on the passed in file. This will only succeed if the stat is not maxed, and the player 
//			has enough unspent gems to purchase it. Otherwise, it will return false.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Upgrade(unsigned int fileNum, unsigned int Stat);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “GemCollected”
//
// Purpose: This function checks if the current save file has already collected that gem. If not, it sets that gem bool to true, 
//			and increments the unspent currency for the file by the amout correct for the difficulty.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GemCollected(unsigned int fileNum, unsigned int levelNum,  int gemNum);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “ClearSaveSlot”
//
// Purpose: This will clear a specific save slot and can be used for a player to start a new game in a slot. This will  only be 
//			called if the player chooses a new game on a filled slot, as the slots will start cleared;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ClearSaveSlot(unsigned int slotNum);

void ClearEntireFile();


///////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: Modifiers
//
//  Purpose: Modifies the specified type.
////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetFileName(const char* newName){szFileName = newName;}
void SetPlayerName(const char* newName){strcpy_s(SaveData.szPlayerName, newName);} 


//////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function: Accessors
//
//  Purpose : Returns the specified type.
///////////////////////////////////////////////////////////////////////////////////////////////////////
tFileData* GetSaveData(){ return &SaveData;}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CheckAchievment”
//
// Purpose: This function will check the player's save file to see if they have aquired the achievment.
//			If so it will return true, and the achievement announcer will show the achievment on screen
//			It will also save the data if the achievment is new collection.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CheckAchievment(unsigned int whichOne);

bool CheckLevelCurrency(unsigned int nLevel, unsigned int nSlot);

bool CheckAllCurrency(unsigned int nSlot);

int CountLevelCurrency(unsigned int nLevel, unsigned int nSlot);












};

#endif