/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CAnimationManager.cpp"
//
// Author: Kahran Ghosalkar (KaG)
//
// Purpose: Loads and renders animations. Has a state machine to control when the animations are triggered.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "CAnimationManager.h"
#include "TinyXML/tinyxml.h"
#include "CEventSystem.h"
#include "CGamePlayState.h"
#include "CPlayer.h"
#include "CGravObject.h"
#include <fstream>
using namespace std;

CAnimationManager::CAnimationManager()
{
	m_bIsPlayerInvisible = false;
}

CAnimationManager::~CAnimationManager()
{
}

CAnimationManager* CAnimationManager::GetInstance()
{
	//return static instance
	static CAnimationManager instance;
	return &instance;
}

void CAnimationManager::LoadAnimations(const char* szFileName, bool bXML)
{
	if(bXML)
	{
		TiXmlDocument doc;

		if(!doc.LoadFile(szFileName)) return;

		TiXmlElement *pRoot = doc.RootElement();

		if(!pRoot) return;

		int nNumObjs;
		pRoot->Attribute("NumberOfObjects", &nNumObjs);

		TiXmlElement *pImage = pRoot->FirstChildElement("Image");
		string szImgFileName = (char*)pImage->GetText();

		TiXmlElement *pTransparency = pRoot->FirstChildElement("Transparency");

		int nRed,nGreen,nBlue,nAlpha;
		pTransparency->Attribute("Red", &nRed);
		pTransparency->Attribute("Green", &nGreen);
		pTransparency->Attribute("Blue", &nBlue);
		pTransparency->Attribute("Alpha", &nAlpha);

		DWORD dwTransparency = D3DCOLOR_ARGB(nAlpha,nRed,nGreen,nBlue);

		TiXmlElement *pObject = pRoot->FirstChildElement("Object");

		while(pObject)
		{
			tObject tempObject;

			int nNumAnims;
			pObject->Attribute("NumberOfAnimations", &nNumAnims);

			tempObject.szName = pObject->Attribute("Name");
			string szLoadFilePath = "Resource/Graphics/" + szImgFileName;
			tempObject.nImgID = CSGD_TextureManager::GetInstance()->LoadTexture(szLoadFilePath.c_str(), dwTransparency);

			TiXmlElement *pAnimation = pObject->FirstChildElement("Animation");

			while(pAnimation)
			{
				tAnimation tempAnim;

				int nNumFrames;
				pAnimation->Attribute("NumberOfFrames", &nNumFrames);

				string szLoop = pAnimation->Attribute("Looping");
				szLoop == "true" ? tempAnim.bIsLooping = true : tempAnim.bIsLooping = false;

				tempAnim.szName = pAnimation->Attribute("Name");

				TiXmlElement *pFrame = pAnimation->FirstChildElement("Frame");

				while(pFrame)
				{
					tFrame tempFrame;

					pFrame->Attribute("FrameNum", &tempFrame.nFrameNum);

					double dFrameDur;
					pFrame->Attribute("FrameDuration", &dFrameDur);
					tempFrame.fFrameDur = (float)dFrameDur;

					tempFrame.szFrameEvent = pFrame->Attribute("FrameEvent");

					TiXmlElement *pFrameRect = pFrame->FirstChildElement("FrameRect");

					int nFrX, nFrY, nFrWidth, nFrHeight;
					pFrameRect->Attribute("X", &nFrX);
					pFrameRect->Attribute("Y", &nFrY);
					pFrameRect->Attribute("Width", &nFrWidth);
					pFrameRect->Attribute("Height", &nFrHeight);

					tempFrame.rFrame.left = (LONG)nFrX;
					tempFrame.rFrame.top = (LONG)nFrY;
					tempFrame.rFrame.right = tempFrame.rFrame.left + (LONG)nFrWidth;
					tempFrame.rFrame.bottom = tempFrame.rFrame.top + (LONG)nFrHeight;

					TiXmlElement *pCollRect = pFrame->FirstChildElement("CollisionRect");

					int nClX,nClY,nClWidth,nClHeight;
					pCollRect->Attribute("X", &nClX);
					pCollRect->Attribute("Y", &nClY);
					pCollRect->Attribute("Width", &nClWidth);
					pCollRect->Attribute("Height", &nClHeight);

					tempFrame.rCollision.left = (LONG)nClX;
					tempFrame.rCollision.top = (LONG)nClY;
					tempFrame.rCollision.right = tempFrame.rCollision.left + (LONG)nClWidth;
					tempFrame.rCollision.bottom = tempFrame.rCollision.top + (LONG)nClHeight;

					TiXmlElement *pAnchor = pFrame->FirstChildElement("AnchorPoint");

					int nAnX,nAnY;
					pAnchor->Attribute("X", &nAnX);
					pAnchor->Attribute("Y", &nAnY);

					tempFrame.ptAnchor.x = (LONG)nAnX;
					tempFrame.ptAnchor.y = (LONG)nAnY;

					TiXmlElement *pWeapon = pFrame->FirstChildElement("WeaponPoint");

					int nWpX,nWpY;
					pWeapon->Attribute("X", &nWpX);
					pWeapon->Attribute("Y", &nWpY);

					tempFrame.ptWeapon.x = (LONG)nWpX;
					tempFrame.ptWeapon.y = (LONG)nWpY;

					tempAnim.vFrameList.push_back(tempFrame);
					pFrame = pFrame->NextSiblingElement("Frame");
				}

				tempObject.mAnimationList[tempAnim.szName] = tempAnim;
				pAnimation = pAnimation->NextSiblingElement("Animation");
			}
			m_mObjectList[tempObject.szName] = tempObject;
			pObject = pObject->NextSiblingElement("Object");
		}
	}
	else
	{
		ifstream fin;
		fin.open(szFileName, ios_base::in | ios_base::binary);
		fin.seekg(0);
		if(fin.is_open())
		{
			int nNumObjects;
			fin.read((char*)&nNumObjects, sizeof(int));

			string szImgFileName;
			for(int i = 0; i < 64; i++)
			{
				char c;
				fin.read((char*)&c, sizeof(char));
				if(c == '\0')
					break;

				szImgFileName += c;
			}

			int nRed,nGreen,nBlue,nAlpha;
			fin.read((char*)&nRed, sizeof(int));
			fin.read((char*)&nGreen, sizeof(int));
			fin.read((char*)&nBlue, sizeof(int));
			fin.read((char*)&nAlpha, sizeof(int));

			DWORD dwTransparency = D3DCOLOR_ARGB(nAlpha,nRed,nGreen,nBlue);

			for(int obj = 0; obj < nNumObjects; obj++)
			{
				tObject tempObject;

				for(int i = 0; i < 64; i++)
				{
					char c;
					fin.read((char*)&c, sizeof(char));
					if(c == '\0')
						break;

					tempObject.szName += c;
				}

				int nNumAnims;
				fin.read((char*)&nNumAnims, sizeof(int));

				string szLoadFilePath = "Resource/Graphics/" + szImgFileName;
				tempObject.nImgID = CSGD_TextureManager::GetInstance()->LoadTexture(szLoadFilePath.c_str(), dwTransparency);

				for(int anim = 0; anim < nNumAnims; anim++)
				{
					tAnimation tempAnim;

					for(int i = 0; i < 64; i++)
					{
						char c;
						fin.read((char*)&c, sizeof(char));
						if(c == '\0')
							break;

						tempAnim.szName += c;
					}

					int nNumFrames;
					fin.read((char*)&nNumFrames, sizeof(int));

					int nIsLooping;
					fin.read((char*)&nIsLooping, sizeof(int));
					tempAnim.bIsLooping = (bool)nIsLooping;

					for(int frame = 0; frame < nNumFrames; frame++)
					{
						tFrame tempFrame;

						fin.read((char*)&tempFrame.nFrameNum, sizeof(int));
						fin.read((char*)&tempFrame.fFrameDur, sizeof(float));
						for(int i = 0; i < 64; i++)
						{
							char c;
							fin.read((char*)&c, sizeof(char));
							if(c == '\0')
								break;

							tempFrame.szFrameEvent += c;
						}

						fin.read((char*)&tempFrame.rFrame.left, sizeof(int));
						fin.read((char*)&tempFrame.rFrame.top, sizeof(int));

						int nFrWidth, nFrHeight;
						fin.read((char*)&nFrWidth, sizeof(int));
						fin.read((char*)&nFrHeight, sizeof(int));

						tempFrame.rFrame.right = tempFrame.rFrame.left + nFrWidth;
						tempFrame.rFrame.bottom = tempFrame.rFrame.top + nFrHeight;

						fin.read((char*)&tempFrame.rCollision.left, sizeof(int));
						fin.read((char*)&tempFrame.rCollision.top, sizeof(int));

						int nColWidth, nColHeight;
						fin.read((char*)&nColWidth, sizeof(int));
						fin.read((char*)&nColHeight, sizeof(int));

						tempFrame.rCollision.right = tempFrame.rCollision.left + nColWidth;
						tempFrame.rCollision.bottom = tempFrame.rCollision.top + nColHeight;

						fin.read((char*)&tempFrame.ptAnchor.x, sizeof(int));
						fin.read((char*)&tempFrame.ptAnchor.y, sizeof(int));
						fin.read((char*)&tempFrame.ptWeapon.x, sizeof(int));
						fin.read((char*)&tempFrame.ptWeapon.y, sizeof(int));

						tempAnim.vFrameList.push_back(tempFrame);
					}
					tempObject.mAnimationList[tempAnim.szName] = tempAnim;
				}
				m_mObjectList[tempObject.szName] = tempObject;
			}
		}
		fin.close();
	}

	//SetAnimInfo();
}

int CAnimationManager::GetUniqueKey()
{
	for (;;)
	{
		int nID = rand() % INT_MAX;
		if(m_mActiveAnimations.count(nID) == 0)
			return nID;
	}
	return -1;
}
int CAnimationManager::StartAnimation(string szObject, CBaseObject* pObj, float fPosX, float fPosY)
{
	tObject tempObj = m_mObjectList[szObject];
	tempObj.pObj = pObj;
	int nID = GetUniqueKey();
	m_mActiveAnimations.insert(pair<int, tObject>(nID, tempObj));
	Reset(nID);
	Play(nID);
	return nID;
}

void CAnimationManager::EndAnimation(int nID)
{
	if(m_mActiveAnimations.count(nID) == 1)
		m_mActiveAnimations.erase(m_mActiveAnimations.find(nID));
}

void CAnimationManager::UpdateAnimations(float fDT)
{
	for(map<int, tObject>::iterator iter = m_mActiveAnimations.begin(); iter != m_mActiveAnimations.end(); iter++)
	{
		if(!(*iter).second.bIsPlaying) continue;

		(*iter).second.fTimeWaited += fDT;

		if((*iter).second.fTimeWaited < (*iter).second.mAnimationList[(*iter).second.szCurrentStateName].vFrameList[(*iter).second.nCurrentFrame].fFrameDur) continue;

		(*iter).second.fTimeWaited = 0.0f;

		if((*iter).second.mAnimationList[(*iter).second.szCurrentStateName].vFrameList[(*iter).second.nCurrentFrame].szFrameEvent != "")
			CEventSystem::GetInstance()->SendEvent((*iter).second.mAnimationList[(*iter).second.szCurrentStateName].vFrameList[(*iter).second.nCurrentFrame].szFrameEvent);

		if((*iter).second.nCurrentFrame == (*iter).second.mAnimationList[(*iter).second.szCurrentStateName].vFrameList.size()-1)
		{
			if((*iter).second.mAnimationList[(*iter).second.szCurrentStateName].bIsLooping)
				Reset((*iter).first);
			else
				Stop((*iter).first);
		}
		else
			(*iter).second.nCurrentFrame++;
	}
}

void CAnimationManager::RenderAnimations()
{
	for(map<int, tObject>::iterator iter = m_mActiveAnimations.begin(); iter != m_mActiveAnimations.end(); iter++)
	{
		int nX = (int)(*iter).second.pObj->GetPosVec().x - (*iter).second.mAnimationList[(*iter).second.szCurrentStateName].vFrameList[(*iter).second.nCurrentFrame].ptAnchor.x;
		int nY = (int)(*iter).second.pObj->GetPosVec().y - (*iter).second.mAnimationList[(*iter).second.szCurrentStateName].vFrameList[(*iter).second.nCurrentFrame].ptAnchor.y;

		DWORD dwColor = D3DCOLOR_ARGB(255, 255, 255, 255);
		if((*iter).second.szName == "Player" && m_bIsPlayerInvisible)
			dwColor = D3DCOLOR_ARGB(0, 255, 255, 255);

		if((*iter).second.nCurrentDir == DIR_RIGHT)
		{
			CSGD_TextureManager::GetInstance()->Draw((*iter).second.nImgID, nX, nY, (*iter).second.fScale, (*iter).second.fScale,
				&(*iter).second.mAnimationList[(*iter).second.szCurrentStateName].vFrameList[(*iter).second.nCurrentFrame].rFrame, 0.0f, 0.0f, 0.0f, dwColor );
		}
		else
		{
			nX += 2*(*iter).second.mAnimationList[(*iter).second.szCurrentStateName].vFrameList[(*iter).second.nCurrentFrame].ptAnchor.x;
			CSGD_TextureManager::GetInstance()->Draw((*iter).second.nImgID, nX, nY, -1.0f*(*iter).second.fScale, (*iter).second.fScale,
				&(*iter).second.mAnimationList[(*iter).second.szCurrentStateName].vFrameList[(*iter).second.nCurrentFrame].rFrame, 0.0f, 0.0f, 0.0f, dwColor );
		}
	}
}

void CAnimationManager::SetRule(string szObject, string szStartState, int nAction, string szResultState)
{
	m_mStateMachine[szObject][szStartState][nAction] = szResultState;
}

void CAnimationManager::PerformAction(int nID, int nAction)
{
	if(m_mStateMachine[m_mActiveAnimations[nID].szName][m_mActiveAnimations[nID].szCurrentStateName][nAction] == "") return;
	m_mActiveAnimations[nID].szCurrentStateName = m_mStateMachine[m_mActiveAnimations[nID].szName][m_mActiveAnimations[nID].szCurrentStateName][nAction];
	Reset(nID);
	Play(nID);
}

void CAnimationManager::Reset(int nID) { m_mActiveAnimations[nID].nCurrentFrame = 0; m_mActiveAnimations[nID].fTimeWaited = 0.0f; }
void CAnimationManager::Stop(int nID) { m_mActiveAnimations[nID].bIsPlaying = false; }
void CAnimationManager::Play(int nID) { Reset(nID); m_mActiveAnimations[nID].bIsPlaying = true; }

void CAnimationManager::HandleEvent(CEvent* pEvent)
{
	// handle frame events here
	/*if(pEvent->GetEventID() == "PlaySound")
	{
	}*/
}

void CAnimationManager::SetAnimInfo()
{
	// Set rules
	// states change depending on these rules
	// Object name, current state, action, resultant state
	SetRule("Player",	"Standing",	JUMP_BTN_PRESSED,				"Jumping");
	SetRule("Player",	"Standing",	POS_VELY_AND_OUT_OF_GRAV_WELL,	"Falling");
	SetRule("Player",	"Standing",	LEFT_RIGHT_PRESSED,				"Walking");
	SetRule("Player",	"Walking",	JUMP_BTN_PRESSED,				"Jumping");
	SetRule("Player",	"Walking",	POS_VELY_AND_OUT_OF_GRAV_WELL,	"Falling");
	SetRule("Player",	"Walking",	IN_GRAV_WELL,					"Standing");
	SetRule("Player",	"Walking",	LEFT_RIGHT_RELEASED,			"Standing");
	SetRule("Player",	"Jumping",	POS_VELY_AND_OUT_OF_GRAV_WELL,	"Falling");
	SetRule("Player",	"Jumping",	IN_GRAV_WELL,					"Standing");
	SetRule("Player",	"Falling",	VELY_ZERO,						"Standing");
	SetRule("Player",	"Falling",	IN_GRAV_WELL,					"Standing");
	//////////////////////////////////////////////////////////////////////////
	SetRule("Walker",	"Standing",	LEFT_RIGHT_PRESSED,				"Walking");
	SetRule("Walker",	"Standing",	POS_VELY_AND_OUT_OF_GRAV_WELL,	"Falling");
	SetRule("Walker",	"Walking",	LEFT_RIGHT_RELEASED,			"Standing");
	SetRule("Walker",	"Walking",	POS_VELY_AND_OUT_OF_GRAV_WELL,	"Falling");
	SetRule("Walker",	"Walking",	IN_GRAV_WELL,					"Standing");
	SetRule("Walker",	"Falling",	VELY_ZERO,						"Standing");
	SetRule("Walker",	"Falling",	IN_GRAV_WELL,					"Standing");
	//////////////////////////////////////////////////////////////////////////
	SetRule("Sniper",	"Standing",	LEFT_RIGHT_PRESSED,				"Walking");
	SetRule("Sniper",	"Standing",	POS_VELY_AND_OUT_OF_GRAV_WELL,	"Falling");
	SetRule("Sniper",	"Walking",	LEFT_RIGHT_RELEASED,			"Standing");
	SetRule("Sniper",	"Walking",	POS_VELY_AND_OUT_OF_GRAV_WELL,	"Falling");
	SetRule("Sniper",	"Walking",	IN_GRAV_WELL,					"Standing");
	SetRule("Sniper",	"Falling",	VELY_ZERO,						"Standing");
	SetRule("Sniper",	"Falling",	IN_GRAV_WELL,					"Standing");
	//////////////////////////////////////////////////////////////////////////
	SetRule("Heavy",	"Standing",	LEFT_RIGHT_PRESSED,				"Walking");
	SetRule("Heavy",	"Standing",	POS_VELY_AND_OUT_OF_GRAV_WELL,	"Falling");
	SetRule("Heavy",	"Walking",	LEFT_RIGHT_RELEASED,			"Standing");
	SetRule("Heavy",	"Walking",	POS_VELY_AND_OUT_OF_GRAV_WELL,	"Falling");
	SetRule("Heavy",	"Walking",	IN_GRAV_WELL,					"Standing");
	SetRule("Heavy",	"Falling",	VELY_ZERO,						"Standing");
	SetRule("Heavy",	"Falling",	IN_GRAV_WELL,					"Standing");
	//////////////////////////////////////////////////////////////////////////
	SetRule("Bombardier",	"Standing",	LEFT_RIGHT_PRESSED,				"Walking");
	SetRule("Bombardier",	"Standing",	POS_VELY_AND_OUT_OF_GRAV_WELL,	"Falling");
	SetRule("Bombardier",	"Walking",	LEFT_RIGHT_RELEASED,			"Standing");
	SetRule("Bombardier",	"Walking",	POS_VELY_AND_OUT_OF_GRAV_WELL,	"Falling");
	SetRule("Bombardier",	"Walking",	IN_GRAV_WELL,					"Standing");
	SetRule("Bombardier",	"Falling",	VELY_ZERO,						"Standing");
	SetRule("Bombardier",	"Falling",	IN_GRAV_WELL,					"Standing");
	//////////////////////////////////////////////////////////////////////////

	// Special cases
	m_mObjectList["ClosedShot"].szCurrentStateName = "WellClosed";
}