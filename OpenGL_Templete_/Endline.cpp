#include "Endline.h"


Endline::Endline()
{
}

Endline::~Endline()
{
}

bool Endline::OnUpdate(DWORD tick, int x, int y)
{
	m_deltaTime += tick;

	if (m_vecPosition.x <= x + 25 && m_vecPosition.x + 128 >= x - 25
		&& m_vecPosition.y <= y + 50 && m_vecPosition.y + 24 >= y - 50)//3½Ã°£
	{
		return true;
	}
	if (m_deltaTime >= m_interval)
	{
		m_deltaTime = 0;
		m_aniIndex++;
		if (m_aniIndex >= m_nFrame[m_idxMotion])
			m_aniIndex = 0;
	}
	return false;
}

void Endline::OnMoveY(float offset)
{
	m_vecPosition.y += (offset * 1);

	if ((m_vecPosition.y >= 512))
	{
		m_vecPosition.y = -(500.0f + rand() % 100);
		m_vecPosition.x = (rand() % 400);

		m_vecDirection.x = 1.0f;
	}
}