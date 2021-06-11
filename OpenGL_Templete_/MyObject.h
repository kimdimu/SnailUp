#pragma once

#include "dsTexture.h"

struct Vector2
{
	float x;
	float y;
};

class CMyObject
{
protected:
	dsTexture		*m_pSprite;

public:
	CMyObject();
	virtual ~CMyObject();

	virtual bool Create(char *filename) = 0;
	virtual void OnDraw(int x, int y) = 0;
	virtual void OnUpdate(DWORD tick) = 0;
};

