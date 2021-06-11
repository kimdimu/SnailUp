
#pragma once
#include "NewSnail.h"

class Endline : public NewSnail
{
protected:

public:
	Endline();
	~Endline();

	bool OnUpdate(DWORD tick, int x, int y);
	void OnMoveY(float offset);
};

