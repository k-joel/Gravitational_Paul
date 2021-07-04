/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CFileSelectionState"
//
// Author: Justin Dumas (JuD)
// 
// Purpose: This State will allow for the selection of a file by the user or a new file. This will make sleection of saves 
//			possible even with the 360 controller.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef DEF_FILE_SELECT
#define DEF_FILE_SELECT
#include "IGameState.h" //directly derived from ibasestate

//additional #includes
#include <windows.h> //needed for the file list object
#include <string>
using namespace std;
//forward declarations
class CGame;
class CSGD_TextureManager;
class CSGD_FModManager;
class CSGD_DirectInput;
class CBitMapFont;


#define DEF_FILEPATH "Resource/SaveData/*.gps"  //yes .Gravitational Paul Save
#define DEF_PATHADDNAME "Resource/SaveData/" //used to add and set it as the player file path in cgame

//switches for the file state
//yay even MORE enums!
enum {FILE_ENTERED, FILE_NEW, FILE_LOAD, FILE_SLOT};

class CFileSelectState : public IGameState
{

private:
	CGame*					m_pGame;
	CSGD_TextureManager*	m_pTM;
	CSGD_FModManager*		m_pFM;
	CSGD_DirectInput*		m_pDI;
	CBitMapFont*			m_pFont;


	bool					m_bFilesFound; //did we find files in the directory (can we load anything?)
	bool					m_bGameHasFile; //does CGame have a save file yet (allows to skip right to the level select if so)
	bool					m_bConfirming; //are we confirming a choice?
	int						m_nCurrState; //state of file load menu

	int						m_nCurrSelection; //currselection 
	int						m_nConfirmationSelection; //current selection when confirming so prev isnt lost)
	unsigned int			m_nListTop; //top file shown
	unsigned int			m_nListBot; //bottom file shown
	unsigned int			m_nLastFileIndex; //index of last file found

	string					m_szAllFiles[200]; // capped at 200 for now
	char					m_szPlayerName[32]; //32 limit for player name
	int						m_nNameSize;
	bool					m_bNameFilled;
	int						m_nChosenSlot;
	int						m_nMenuMax;
	char					m_nStartingChar;
	bool					m_bFileExists;

	int						m_nBGImageID;
	int						m_nBoxImageID;

	int						m_nPreviewPics[21];
	bool					m_bDeletingSlot;
	bool					m_bKillIt;
	bool					m_bDeleteMode;

	bool					m_bSettingDiff;
	int						m_nDiffSelection;
	bool					m_bEmpty;

	
	
//silngeton use - methodes for creation and destruction made private
	CFileSelectState();
	~CFileSelectState();
	CFileSelectState( const CFileSelectState&);
	CFileSelectState& operator = (const CFileSelectState&);

	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “PopulateList”
//
// Purpose: This function will check the def file path and fill the list with the extensions to these files.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void PopulateList();

public:

//all interface functions described in I Base State. these are needed to be a basestate
static CFileSelectState* GetInstance();
void Enter();
void Exit();
bool Input();
void Update(float fDT);
void Render();

void LoadFileIntoGame(string fileName);
void DrawFileList();
void DrawKeyboard();
void AddLetter();
void BackspaceLetter();
void CreateSave();
bool CheckForSave(string checkName);
void DrawGameSlots();
void SelectSlot();








};

#endif