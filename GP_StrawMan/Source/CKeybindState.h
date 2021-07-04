#ifndef CKEYBIND_H_
#define CKEYBIND_H_

#include "IGameState.h"

class CGame;

class CKeybindState : public IGameState
{
private:
	CGame*		m_pGame;
	int			m_nImageID;
	int			m_nCurrSelection;


	CKeybindState();
	CKeybindState(const CKeybindState&);
	CKeybindState& operator=(const CKeybindState&);


public:

	void Enter();
	void Exit();
	void Update(float fDT);
	bool Input();
	void Render();

	static CKeybindState* GetInstance();
	~CKeybindState();
	
	//These render functions will display the gameplay controls based on the current keybinding
	void RenderKControls(int controlscheme);
	void RenderCControls(int controlscheme);
	void SwitchKControls(int controlscheme);
	void SwitchCControls(int controlscheme);


};


#endif