#include "CBitMapFont.h"
#include "CAssetState.h"
#include <fstream>
using namespace std;

CBitMapFont::CBitMapFont()
{
	LoadKerningInfo("Resource/Scripts/CbB_FontA.bin", FONT_A);
	LoadKerningInfo("Resource/Scripts/CbB_FontB.bin", FONT_B);

	m_nCurrentFont = FONT_A;
	//m_nImageID =
}

CBitMapFont::~CBitMapFont()
{
}

CBitMapFont* CBitMapFont::GetInstance()
{
	static CBitMapFont instance;
	return &instance;
}

RECT CBitMapFont::CellAlgorithm(int nID)
{
	RECT drawRect;
	drawRect.top = (nID / m_tFonts[m_nCurrentFont].nNumCols) * m_tFonts[m_nCurrentFont].nCellHeight;
	drawRect.left = (nID % m_tFonts[m_nCurrentFont].nNumCols) * m_tFonts[m_nCurrentFont].nCellWidth;
	drawRect.bottom = drawRect.top + m_tFonts[m_nCurrentFont].nCellHeight;
	drawRect.right = drawRect.left + m_tFonts[m_nCurrentFont].nCellWidth;

	return drawRect;
}
void CBitMapFont::Print(const char* szText, int nPosX, int nPosY, bool bIsCentered, bool bIsSelected, float fScale, DWORD dwColor)
{
	int nLen = strlen(szText);
	int nOrigX = nPosX;
	m_tFonts[m_nCurrentFont].fScale = fScale;
	int nImgID = m_tFonts[m_nCurrentFont].nImageID;

	if(dwColor == D3DCOLOR_XRGB(255, 255, 255) && m_tFonts[m_nCurrentFont].nImageIDSelected == -1)
		dwColor = D3DCOLOR_XRGB(0, 175, 255);

	if(bIsSelected)
	{
		fScale += 0.1*fScale;
		if(m_tFonts[m_nCurrentFont].nImageIDSelected != -1)
			nImgID = m_tFonts[m_nCurrentFont].nImageIDSelected;
		else
			dwColor = D3DCOLOR_XRGB(255, 0, 0);
	}

	if(bIsCentered)
	{
		int nTextLen = 0;
		for(int i = 0; i < nLen; i++)
			nTextLen += int(fScale*(float)m_tFonts[m_nCurrentFont].vCharWidths[int(szText[i] - m_tFonts[m_nCurrentFont].cStartChar)]);

		nPosX -= nTextLen/2 ;
	}

	for(int i = 0; i < nLen; i++)
	{
		char ch = szText[i];

		if(ch == ' ')
		{
			/*nPosX += (int)(( m_tFonts[m_nCurrentFont].nCellWidth * fScale)/2);
			continue;*/
		}
		else if (ch == '\n')
		{
			nPosX = nOrigX;
			nPosY += int(m_tFonts[m_nCurrentFont].nCellHeight * fScale);
			continue;
		}

		int nID = int(szText[i] - m_tFonts[m_nCurrentFont].cStartChar);
		RECT rLetter = CellAlgorithm(nID);

		CSGD_TextureManager::GetInstance()->Draw(nImgID, nPosX, nPosY, fScale, fScale, &rLetter, 0.0f, 0.0f, 0.0f, dwColor);

		// set kerning
		nPosX += int(fScale*(float)m_tFonts[m_nCurrentFont].vCharWidths[nID]);
	}
}

void CBitMapFont::LoadKerningInfo(string szFileName, int nType)
{
	/*ifstream fin;

	fin.open(szFileName.c_str(), ios_base::in);
	int count = 0;
	if(fin.is_open())
	{
		while(!fin.eof())
		{
			fin >> m_nCharWidths[count];
			fin.ignore(INT_MAX, '\n');
			if(!fin.good())
				break;

			count++;
		}
		fin.close();
	}*/

	ifstream fin;
	fin.open(szFileName.c_str(), ios_base::in | ios_base::binary);
	fin.seekg(0);
	if(fin.is_open())
	{
		fin.read((char*)&m_tFonts[nType].nCellWidth, sizeof(int));
		fin.read((char*)&m_tFonts[nType].nCellHeight, sizeof(int));
		fin.read((char*)&m_tFonts[nType].nNumCols, sizeof(int));
		int nNumChars;
		fin.read((char*)&nNumChars, sizeof(int));
		for(int i = 0; i < nNumChars; i++)
		{
			int nWidth;
			fin.read((char*)&nWidth, sizeof(int));
			m_tFonts[nType].vCharWidths.push_back(nWidth);
		}

		fin.close();
	}
}