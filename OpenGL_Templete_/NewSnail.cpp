#include "NewSnail.h"


NewSnail::NewSnail()
{
	m_idxMotion = WIZARDMOTION::WIZARD_MOVE;
	m_deltaTime = 0;
	m_deltaTime2 = 0;
	slowdown = 2.5f;

}

NewSnail::~NewSnail()
{
}


bool NewSnail::LoadCharacterInfo(char* filename)
{
	int tDir, nDir, nFrame, type, left, top, width, height, xCenter, yCenter, interval;

	FILE* fp = fopen(filename, "rt");
	fscanf(fp, "%d %d", &tDir, &nFrame);

	for (int t = 0; t < tDir; t++)
	{
		fscanf(fp, "%d %d", &nDir, &nFrame);

		m_nFrame[nDir] = nFrame;
		for (int idx = 0; idx < nFrame; idx++)
		{
			fscanf(fp, "%d %d %d %d %d %d %d", &left, &top, &width, &height, &xCenter, &yCenter, &interval);
			m_rcFrame[nDir][idx].left = left;
			m_rcFrame[nDir][idx].top = top;
			m_rcFrame[nDir][idx].right = width;
			m_rcFrame[nDir][idx].bottom = height;
			m_interval = interval;
			m_vecCenter[nDir][idx].x = (float)xCenter;
			m_vecCenter[nDir][idx].y = (float)yCenter;
		}
	}

	fclose(fp);

	return true;
}

void NewSnail::OnDraw(int z)
{
	float xPos, yPos;

	xPos = m_vecPosition.x - m_vecCenter[m_idxMotion][m_aniIndex].x;
	yPos = m_vecPosition.y - m_vecCenter[m_idxMotion][m_aniIndex].y;

	m_pSprite->Draw(xPos, yPos,
		m_rcFrame[z][m_aniIndex].left,
		m_rcFrame[z][m_aniIndex].top,
		m_rcFrame[z][m_aniIndex].right,
		m_rcFrame[z][m_aniIndex].bottom, 0);
}

void NewSnail::OnUpdate(DWORD tick)
{
	m_deltaTime += tick;

	switch (playId)
	{
	case 0: //blue
	{
		BYTE key[256];
		::GetKeyboardState(key);
		if (key[VK_LSHIFT] & 0x80)
		{
			m_deltaTime2 += tick;
			if (m_deltaTime2 <= 10)
				slowdown += 0.21f;
		}
		else
		{
			slowdown += 0.0f;
			m_deltaTime2 = 0;
		}

		if (key[0x41] & 0x80)
		{
			if (m_vecPosition.x >= 0)
				OnMove(-0.02f * slowdown, 0.02f);
			else
				OnMove(0.0f, 0.02f);
		}
		else if (key[0x44] & 0x80)
		{
			if (m_vecPosition.x <= 512)
				OnMove(0.02f * slowdown, 0.02f);
			else
				OnMove(0.0f, 0.02f);
		}
		else if (key[0x57] & 0x80)
		{
			if (m_vecPosition.y > 0)
				OnMove(0.0f, -0.01f * slowdown);
			else
				OnMove(0.0f, 0.0f);
		}
		else
		{
			OnMove(0.0f, 0.02f);
		}

		if (slowdown <= 0)
		{
			OnMove(0.0f, 0.02f);
		}
	}
	break;
	case 1:
	{
		BYTE key[256];

		::GetKeyboardState(key);
		if (key[VK_SPACE] & 0x80)
		{
			m_deltaTime2 += tick;
			if (m_deltaTime2 <= 10)
				slowdown += 0.21f;
		}
		else
		{
			slowdown += 0.0f;
			m_deltaTime2 = 0;
		}

		if (key[0x48] & 0x80)
		{
			if (m_vecPosition.x >= 0)
				OnMove(-0.02f * slowdown, 0.02f);
			else
				OnMove(0.0f, 0.02f);
		}
		else if (key[0x4B] & 0x80)
		{
			if (m_vecPosition.x <= 512)
				OnMove(0.02f * slowdown, 0.02f);
			else
				OnMove(0.0f, 0.02f);
		}
		else if (key[0x55] & 0x80)
		{
			if (m_vecPosition.y > 0)
				OnMove(0.0f, -0.01f * slowdown);
			else
				OnMove(0.0f, 0.0f);
		}
		else
		{
			OnMove(0.0f, 0.02f);
		}

		if (slowdown <= 0)
		{
			OnMove(0.0f, 0.02f);
		}
	}
	break;
	case 2: //red
	{
		BYTE key[256];
		::GetKeyboardState(key);
		if (key[VK_RSHIFT] & 0x80)
		{
			m_deltaTime2 += tick;
			if (m_deltaTime2 <= 10)
				slowdown += 0.21f;
		}
		else
		{
			slowdown += 0.0f;
			m_deltaTime2 = 0;
		}

		if (key[VK_LEFT] & 0x80)
		{
			if (m_vecPosition.x >= 0)
				OnMove(-0.02f * slowdown, 0.02f);
			else
				OnMove(0.0f, 0.02f);
		}
		else if (key[VK_RIGHT] & 0x80)
		{
			if (m_vecPosition.x <= 512)
				OnMove(0.02f * slowdown, 0.02f);
			else
				OnMove(0.0f, 0.02f);
		}
		else if (key[VK_UP] & 0x80)
		{
			if (m_vecPosition.y > 0)
				OnMove(0.0f, -0.01f * slowdown);
			else
				OnMove(0.0f, 0.0f);
		}
		else
		{
			OnMove(0.0f, 0.02f);
		}

		if (slowdown <= 0)
		{
			OnMove(0.0f, 0.02f);
		}
	}
	break;
	}
		
	m_vecPosition.x += (m_vecDirection.x * 200.0f * tick / 1000.0f);
	m_vecPosition.y += (m_vecDirection.y * 200.0f * tick / 1000.0f);


	if (m_deltaTime >= m_interval)
	{
		if (slowdown >= 0.0f)
			slowdown -= 0.5f;
		m_deltaTime = 0;
		m_aniIndex++;
		if (m_aniIndex >= m_nFrame[m_idxMotion])
		{
			m_aniIndex = 0;
		}
	}
}

void NewSnail::OnDraw(int x, int y, int z)
{
	float xPos, yPos;

	xPos = m_vecPosition.x - m_vecCenter[m_idxMotion][m_aniIndex].x;
	yPos = m_vecPosition.y - m_vecCenter[m_idxMotion][m_aniIndex].y;

	m_pSprite->Draw(x, y,
		m_rcFrame[z][m_aniIndex].left,
		m_rcFrame[z][m_aniIndex].top,
		m_rcFrame[z][m_aniIndex].right,
		m_rcFrame[z][m_aniIndex].bottom, 0);
}

void NewSnail::OnUpdate(DWORD tick, int x)
{
	m_deltaTime += tick;

	if (m_deltaTime >= m_interval)
	{
		if (slowdown >= 0.0f)
			slowdown -= 0.5f;
		m_deltaTime = 0;
		m_aniIndex++;
		if (m_aniIndex >= m_nFrame[m_idxMotion])
		{
			m_aniIndex = 0;
		}
	}
}
void NewSnail::OnMove(float movex, float movey)
{
	m_vecDirection.x = movex;
	m_vecDirection.y = movey;
}