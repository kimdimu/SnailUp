#include "MyCharacter.h"


CMyCharacter::CMyCharacter()
{
	m_aniType = CHARMOTION::DOWN;

	m_aniIndex = 0;
	m_interval = 0;

	m_vecDirection = { 0.0f, 0.0f };
	m_vecPosition = { 300.0f, 500.0f };

}


CMyCharacter::~CMyCharacter()
{

}

bool CMyCharacter::LoadCharacterInfo(char* filename)
{
	int tDir, nDir, nFrame, type, left, top, width, height;

	FILE* fp = fopen(filename, "rt");
	fscanf(fp, "%d %d", &tDir, &nFrame);

	for (int t = 0; t < tDir; t++)
	{
		fscanf(fp, "%d %d", &nDir, &nFrame);

		m_nFrame[nDir] = nFrame;
		for (int idx = 0; idx < nFrame; idx++)
		{
			fscanf(fp, "%d %d %d %d", &left, &top, &width, &height);
			m_rcFrame[nDir][idx].left = left;
			m_rcFrame[nDir][idx].top = top;
			m_rcFrame[nDir][idx].right = width;
			m_rcFrame[nDir][idx].bottom = height;
		}
	}

	fclose(fp);

	return true;
}

bool CMyCharacter::Create(char* charname)
{
	if (m_pSprite != NULL)
		delete m_pSprite;

	char filename[MAX_PATH];

	wsprintf(filename, "./Resources/Character/%s.txt", charname);
	LoadCharacterInfo(filename);


	wsprintf(filename, "./Resources/Character/%s.png", charname);
	m_pSprite = new dsTexture(filename);
	return m_pSprite->LoadFromFilename(filename);
}

void CMyCharacter::OnDraw(int x, int y)
{
	m_pSprite->Draw(m_vecPosition.x, m_vecPosition.y,
		m_rcFrame[0][m_aniIndex].left,
		m_rcFrame[0][m_aniIndex].top,
		m_rcFrame[0][m_aniIndex].right,
		m_rcFrame[0][m_aniIndex].bottom, 0);
}

void CMyCharacter::OnUpdate(DWORD tick)
{
	m_interval += tick;

	if (m_interval > 100)
	{
		m_interval = 0;
		m_aniIndex++;
		if (m_aniIndex >= m_nFrame[m_aniType])
			m_aniIndex = 0;
	}

	m_vecPosition.x += (m_vecDirection.x * 200.0f * tick / 1000.0f);
	m_vecPosition.y += (m_vecDirection.y * 200.0f * tick / 1000.0f);

}

void CMyCharacter::SetAnimationChange(CHARMOTION ani)
{
	if (m_aniType == ani)
		return;

	m_aniType = ani;

	switch (m_aniType)
	{
	case CHARMOTION::LEFT: m_vecDirection = { -1.0f, 0.0f };
						   break;
	case CHARMOTION::RIGHT: m_vecDirection = { 1.0f, 0.0f };
							break;
	case CHARMOTION::UP: m_vecDirection = { 0.0f, -1.0f };
						 break;
	case CHARMOTION::DOWN: m_vecDirection = { 0.0f, 1.0f };
						   break;
	default: m_vecDirection = { 0.0f, 0.0f };
	}
}