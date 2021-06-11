#include "BigStone.h"


BigStone::BigStone()
{
}

BigStone::~BigStone()
{
}

int BigStone::OnUpdate(DWORD tick, int x, int y)
{
	m_deltaTime += tick;

	//if (m_vecPosition.x >=x && m_vecPosition.x + 178 <=x)
	if (m_vecPosition.x <= x + 25 && m_vecPosition.x + 178 >= x - 25
		&& m_vecPosition.y + 40 <= y + 50 && m_vecPosition.y + 70 >= y - 50)//3�ð�
		return 1;

	if (m_vecPosition.x - 25 <= x && m_vecPosition.y > y)
		return 2; //�̴�� �� ���� ���� ���ʿ� �´´�. �������� ����
	if (m_vecPosition.x + 178 + 25 >= x && m_vecPosition.y > y)
		return 3; // �̴�� �� ���� ���� �����ʿ� �´´�. ���������� ����

	if (m_deltaTime >= m_interval)
	{
		m_deltaTime = 0;
		m_aniIndex++;
		if (m_aniIndex >= m_nFrame[m_idxMotion])
			m_aniIndex = 0;
	}
	return false;
}

void BigStone::OnMoveY(float offset)
{
	m_vecPosition.y += (offset * 1);

	if ((m_vecPosition.y >= 512))
	{
		m_vecDirection.x = 1.0f;
		m_vecPosition.y = -(300.0f + rand() % 50);
		m_vecPosition.x = (rand() % 400);
	}
}