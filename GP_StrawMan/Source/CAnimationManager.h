/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CAnimationManager.h"
//
// Author: Kahran Ghosalkar (KaG)
//
// Purpose: Loads and renders animations. Has a state machine to control when the animations are triggered.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <map>
using namespace std;

#include "IListener.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_TextureManager.h"

enum eActions {JUMP_BTN_PRESSED, LEFT_RIGHT_PRESSED, LEFT_RIGHT_RELEASED, IN_GRAV_WELL, POS_VELY_AND_OUT_OF_GRAV_WELL, VELY_ZERO, ACTIONS_MAX};

// Objects
// "Player"

class CBaseObject;
class CAnimationManager : public IListener
{
	struct tFrame
	{
		int nFrameNum;
		float fFrameDur;
		string szFrameEvent;
		RECT rFrame;
		RECT rCollision;
		POINT ptAnchor;
		POINT ptWeapon;
	};

	struct tAnimation
	{
		vector<tFrame> vFrameList;
		string szName;
		bool bIsLooping;
	};

	struct tObject
	{
		map<string, tAnimation> mAnimationList;
		string szName;

		int nCurrentFrame;
		string szCurrentStateName;
		int nCurrentDir;
		bool bIsPlaying;
		int nImgID;
		CBaseObject* pObj;
		float fTimeWaited;
		DWORD dwTransparency;
		float fScale; // will never change

		tObject()
		{
			nCurrentFrame = 0;
			szCurrentStateName = "Standing";
			nCurrentDir = DIR_RIGHT;
			bIsPlaying = true;
			nImgID = -1;
			pObj = NULL;
			fTimeWaited = 0.0f;
			fScale = 1.0f;
			dwTransparency = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
	};

	map<string, tObject> m_mObjectList;
	map<int, tObject> m_mActiveAnimations;

	bool m_bIsPlayerInvisible;

	// 3d jagged map, fuck yeah!
	map<string, map<string, map<int, string>>> m_mStateMachine;

	// get a unique random key
	int GetUniqueKey();

	CAnimationManager(void);
	~CAnimationManager(void);
	CAnimationManager(const CAnimationManager&);
	CAnimationManager& operator=(const CAnimationManager&);
public:
	static CAnimationManager* GetInstance();

	// handle animations
	void LoadAnimations(const char* szFileName, bool bXML=true);
	int StartAnimation(string szObject, CBaseObject* pObj, float fPosX=0.0f, float fPosY=0.0f);
	void EndAnimation(int nID);
	void UpdateAnimations(float fDT);
	void RenderAnimations();
	void Reset(int nID);
	void Stop(int nID);
	void Play(int nID);
	void SetAnimInfo(void);
	void ClearAllAnimations() { m_mActiveAnimations.clear(); }

	// handle states
	void SetRule(string szObject, string szStartState, int nAction, string szResultState);
	void PerformAction(int nID, int nAction);

	// handle events
	void HandleEvent(CEvent* pEvent);

	// Accessors
	float GetScale(int nID) { return m_mActiveAnimations[nID].fScale; }
	int GetCurrentDir(int nID) { return m_mActiveAnimations[nID].nCurrentDir; }
	bool GetIsPlaying(int nID) { return m_mActiveAnimations[nID].bIsPlaying; }
	RECT GetCurrentFrameRect(int nID) { return m_mActiveAnimations[nID].mAnimationList[m_mActiveAnimations[nID].szCurrentStateName].vFrameList[m_mActiveAnimations[nID].nCurrentFrame].rFrame; }
	RECT GetCurrentCollRect(int nID) { return m_mActiveAnimations[nID].mAnimationList[m_mActiveAnimations[nID].szCurrentStateName].vFrameList[m_mActiveAnimations[nID].nCurrentFrame].rCollision; }
	POINT GetCurrentAnchorPt(int nID) { return m_mActiveAnimations[nID].mAnimationList[m_mActiveAnimations[nID].szCurrentStateName].vFrameList[m_mActiveAnimations[nID].nCurrentFrame].ptAnchor; }
	POINT GetCurrentWeaponPt(int nID) { return m_mActiveAnimations[nID].mAnimationList[m_mActiveAnimations[nID].szCurrentStateName].vFrameList[m_mActiveAnimations[nID].nCurrentFrame].ptWeapon; }

	// Mutators
	//void SetScale(float val) { m_fScale = val; }
	void SetCurrentDir(int nID, int nDir) { m_mActiveAnimations[nID].nCurrentDir = nDir; }
	void SetPlayerInvisible(bool bVal) { m_bIsPlayerInvisible = bVal; }
};