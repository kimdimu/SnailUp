#include "SmallStone_img.h"


bool SmallStone_img::Create(char *charname, char *pngname)
{
	if (m_pSprite != NULL)
		delete m_pSprite;

	char filename[MAX_PATH];

	wsprintf(filename, "./Resources/Character/%s.txt", charname);
	LoadCharacterInfo(filename);


	wsprintf(filename, "./Resources/Character/%s.png", pngname);
	m_pSprite = new dsTexture(filename);
	return m_pSprite->LoadFromFilename(filename);
}


int SmallStone_img::OnUpdate(DWORD tick, int x, int y)
{
	m_deltaTime += tick;

	if (m_vecPosition.x <= x + 25 && m_vecPosition.x + 75 >= x - 25
		&& m_vecPosition.y + 50 <= y + 50 && m_vecPosition.y + 85 >= y - 50)
		return 1;

	if (m_vecPosition.x - 25 <= x && m_vecPosition.y > y)
		return 2; //이대로 쭉 가면 돌의 왼쪽에 맞는다.
	if (m_vecPosition.x + 178 + 25 >= x && m_vecPosition.y > y)
		return 3; // 이대로 쭉 가면 돌의 오른쪽에 맞는다.

	if (m_deltaTime >= m_interval)
	{
		m_deltaTime = 0;
		m_aniIndex++;
		if (m_aniIndex >= m_nFrame[m_idxMotion])
			m_aniIndex = 0;
	}
	m_vecPosition.x += (m_vecDirection.x * 50.0f * tick / 1000.0f);
	m_vecPosition.y += (m_vecDirection.y * 50.0f * tick / 1000.0f);
}

void SmallStone_img::OnMoveY(float offset)
{
	m_vecPosition.y += (offset * 1);

	if ((m_vecPosition.y >= 512))
	{
		m_vecPosition.y = -(300.0f + rand() % 50);
		m_vecPosition.x =(rand() % 400);

		m_vecDirection.x = 1.0f;
	}
}