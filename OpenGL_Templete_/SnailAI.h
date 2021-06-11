#pragma once
#include "MyCharacter.h"



enum WIZARDMOTION1
{
	WIZARD_MOVE1 = 0
};

class SnailAI : public CMyCharacter
{
private:
	float slowdown = 5.0f;
	float AIx;
	float AIy;
	int firstx;
	int secondx;
	int thirdx;
protected:
	Vector2		m_vecCenter[6][6];
	DWORD	    m_deltaTime;
	DWORD	    m_deltaTime2;

	WIZARDMOTION1	m_idxMotion;

protected:
	bool LoadCharacterInfo(char* filename);


public:
	void firstxGet(int x) { firstx = x; }
	void secondxGet(int x) { secondx = x; }
	void thirdxGet(int x) { thirdx = x; }
	bool firstxReturn() { return firstx; }
	bool secondxReturn() { return secondx; }
	bool thirdxReturn() { return thirdx; }

	SnailAI();
	virtual ~SnailAI();

	void OnDraw(int x, int y);
	void OnUpdate(DWORD tick);
	void OnMove(float movex, float movey);

	//void DieChange()
	//{

	//	if (diecheck == true)
	//		diecheck = false;
	//	else
	//		diecheck = true;
	//}
	//bool ReturnD()
	//{
	//	return diecheck;
	//}
};

