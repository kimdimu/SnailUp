#pragma once
#include "NewSnail.h"

class BigStone : public NewSnail
{
protected:
	BOOL			m_isInverseMove;
	float				m_fDir;
	int				    m_jumpInterval;

public:
	BigStone();
	~BigStone();

	int OnUpdate(DWORD tick, int x, int y);
	void OnMoveY(float offset);
};

