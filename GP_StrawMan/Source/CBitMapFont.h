/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	"CBitMapFont.h"
//
// Author: Kahran Ghosalkar
//
// Purpose: Singleton that draws a bitmap font to the screen
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DEF_BITMAP_FONT
#define DEF_BITMAP_FONT
#include <windows.h>
#include "SGD Wrappers/CSGD_TextureManager.h"

#include <vector>
#include <string>
using namespace std;

enum { FONT_A, FONT_B, FONT_MAX };

class CBitMapFont
{
private:

	struct tFont
	{
		int nCellWidth;
		int nCellHeight;
		int nNumCols;
		int nImageID;
		int nImageIDSelected;
		float fScale;
		char cStartChar;
		vector<int> vCharWidths;

		tFont()
		{
			nCellWidth = 32;
			nCellHeight = 32;
			nNumCols = 8;
			nImageID = -1;
			nImageIDSelected = -1;
			fScale = 1.0f;
			cStartChar = 32;
		}
	};

	int m_nCurrentFont;
	tFont m_tFonts[FONT_MAX];
	RECT CellAlgorithm(int nID);

	CBitMapFont();
	~CBitMapFont();
	CBitMapFont(const CBitMapFont&);
	CBitMapFont& operator=(const CBitMapFont&);
public:
	static CBitMapFont* GetInstance();

	void Print(const char* szText, int nPosX, int nPosY, bool bIsCentered=false, bool bIsSelected=false, float fScale=1.0f, DWORD dwColor=D3DCOLOR_XRGB(255, 255, 255));
	void LoadKerningInfo(string szFileName, int nType=FONT_A);

	//Accessors
	int GetCellHeight() const { return int( (float)m_tFonts[m_nCurrentFont].nCellHeight * m_tFonts[m_nCurrentFont].fScale ); }

	//Mutators
	void SetImageID(int newID, int nType=FONT_A) {m_tFonts[nType].nImageID = newID;}
	void SetSelectedID(int nID, int nType=FONT_A) {m_tFonts[nType].nImageIDSelected = nID;}
	void SetCurFont(int nType) {m_nCurrentFont = nType;}
};

#endif