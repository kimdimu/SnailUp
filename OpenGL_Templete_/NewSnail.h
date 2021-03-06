#pragma once
#include "MyCharacter.h"


//Snail에서 스킨과 조작만 다르다.
enum WIZARDMOTION
{
	WIZARD_MOVE = 0,
	WIZARD_ATTACK = 1,
	WIZARD_HIT = 2,
	WIZARD_WIN = 3,
	WIZARD_DEATH = 4
};

class NewSnail : public CMyCharacter
{
private:
	float slowdown = 5.0f;
	int skin;
	int playId=0;
protected:
	Vector2		m_vecCenter[6][6];
	DWORD	    m_deltaTime;
	DWORD	    m_deltaTime2;

	WIZARDMOTION	m_idxMotion;

protected:
	bool LoadCharacterInfo(char* filename);


public:

	NewSnail();
	virtual ~NewSnail();
	void SetSkin(int number) { skin = number; }
	void SetPlayer(int number) { playId= number; }
	int GetPlayerID() { return playId; }
	int Skinreturn() { return skin; }
	void OnDraw(int z);
	void OnUpdate(DWORD tick);
	void OnDraw(int x, int y, int z);
	void OnUpdate(DWORD tick, int x);
	void OnMove(float movex, float movey);
	//void SetPosition(float x, float y) { m_vecPosition.x = x; m_vecPosition.y = y; }
};

