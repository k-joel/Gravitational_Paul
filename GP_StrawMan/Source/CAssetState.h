#ifndef DEF_ASSET_STATE
#define DEF_ASSET_STATE

#include "IGameState.h"
#include <Windows.h>

//additional #includes
#include <string>
#include <map>
using std::string;
using std::map;

// is it music or a sound effect?
enum eType { SND_SFX, SND_MUSIC };

//forward declarations
class CSGD_TextureManager;
class CSGD_FModManager;
class CBitMapFont;

//IMPORTANT//
//This is used to define the number of "steps" that the loading bar must lerp thru to get to 100%
// i.e. Value of 5 will increase the bar by 20% each time, 5 times.
#define NUM_ASSETS 4

class CAssetState : public IGameState
{
private:

	//manager pointers needed
	CSGD_TextureManager*			m_pTM;
	CSGD_FModManager*				m_pFM;
	CBitMapFont*					m_pFont;

	// the current loading stage
	int								m_nCurrentStage;
	RECT							m_rLoadbarRect;
	RECT							m_rLoadbarBGRect;

	// name of the current stage
	string							m_szCurAssetName;

	// maps to store the IDs
	map<string, int>				m_mImageIDMap;
	map<string, int>				m_mSFXIDMap;
	map<string, int>				m_mMusicIDMap;

	// volumes
	float							m_fSFXVolume;
	float							m_fMusicVolume;

	CAssetState();
	~CAssetState();
	CAssetState( const CAssetState&);
	CAssetState& operator = (const CAssetState& );

public:

	static CAssetState* GetInstance();
	void Enter();
	void Exit();
	bool Input();
	void Update(float fDT);
	void Render();

	void SetAllMusicVolumes(float fVol);
	void SetAllSFXVolumes(float fVol);
	float GetAllMusicVolumes();
	float GetAllSFXVolumes();

	int GetSoundID(string soundName, int nIndex = -1, int nType = SND_SFX);
	int GetImageID(string imageName, int stageNum = -1);

	void PlaySound(string soundName, int nIndex = -1, int nType = SND_SFX);
	void PlaySound(int nID, int nType = SND_SFX);
	void StopSound(string soundName); 
	void StopSound(int nID); 
	bool IsSoundPlaying(string soundName); 
};
#endif