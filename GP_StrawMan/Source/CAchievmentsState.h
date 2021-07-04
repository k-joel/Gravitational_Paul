#ifndef DEF_ACHIEV_STATE
#define DEF_ACHIEV_STATE
#include "IGameState.h" //ninherits directly from i game state

///additional #includes

//forward declarations
class CSGD_TextureManager;
class CGame;
class CBitMapFont;


class CAchievementsState : public IGameState
{

	int				m_nCurrSelection;
	float			m_fTickTime;

	int				m_nBoxImageID;
	int				m_nBGImageID;
	int				m_nSelectID;
	int				m_nAchievIcons[10];
	unsigned long			m_dwGreyScale;


	CSGD_TextureManager*		m_pTM;
	CGame*						m_pGame;
	CBitMapFont*				m_pFont;






private:

	CAchievementsState();
	~CAchievementsState();
	CAchievementsState(const CAchievementsState&);
	CAchievementsState& operator = (const CAchievementsState&);

	

	


public:
	
	
	static CAchievementsState* GetInstance();
	void Enter();
	void Exit();
	bool Input();
	void Update(float fDT);
	void Render();

};
#endif