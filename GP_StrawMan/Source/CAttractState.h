#ifndef CATTRACTSTATE_H_
#define CATTRACTSTATE_H_
#include "IGameState.h"



class CGame;
class CAttractState : public IGameState
{
private:
	float		m_fAttractTimer; // This is the super awesome timer that makes everything fun and fantastic
	int			m_nImageID; // This is the brother of the super awesome timer but he took a different direction in life and displays images
	CGame*		m_pGame;

	CAttractState();
	~CAttractState();
	CAttractState(const CAttractState&);
	CAttractState& operator=(const CAttractState&);

public:
	static CAttractState* GetInstance();
	void Enter();
	bool Input();
	void Update(float fDT);
	void Render();
	void Exit();
	
	


};

#endif