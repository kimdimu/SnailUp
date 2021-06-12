#pragma once

#include "MyObject.h"


enum CHARMOTION
{
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3,
	HIT = 4,
	WIN = 5
};






class CMyCharacter : public CMyObject
{
private:


protected:
	RECT			 m_rcFrame[6][6];
	int  				 m_nFrame[6]; //방향별 애니메이션 프레임 수
	unsigned char    m_aniIndex;
	DWORD			 m_interval;

	CHARMOTION       m_aniType;

	Vector2			 m_vecDirection;
	Vector2             m_vecPosition;

	int						 m_layer;

protected:
	virtual bool LoadCharacterInfo(char* filename);

public:
	CMyCharacter();
	virtual ~CMyCharacter();

	bool Create(char* charname);
	void OnDraw(int x, int y);
	void OnUpdate(DWORD tick);

	void SetAnimationChange(CHARMOTION ani);
	void SetPosition(float x, float y) { m_vecPosition.x = x; m_vecPosition.y = y; }

	void SetLayer(int layer) { m_layer = layer; }
	int GetLayer() { return m_layer; }
	int ReturnX()
	{

		return m_vecPosition.x;
	}
	int ReturnY()
	{
		return m_vecPosition.y;
	}
};

