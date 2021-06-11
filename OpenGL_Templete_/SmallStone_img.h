#pragma once
#include "BigStone.h"

class SmallStone_img : public BigStone
{
public:
	bool Create(char *charname, char *pngname);
	void OnMoveY(float offset);
	int OnUpdate(DWORD tick, int x, int y);
};

