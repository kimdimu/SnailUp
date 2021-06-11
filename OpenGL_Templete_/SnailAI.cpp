#include "SnailAI.h"
#include "MapObject.h"


SnailAI::SnailAI()
{
	m_idxMotion = WIZARDMOTION1::WIZARD_MOVE1;
	m_deltaTime = 0;
	m_deltaTime2 = 0;
	slowdown = 3.0f;//�ӵ�
	AIx = 0.0f; //�ӵ� ����
	AIy = -0.02f * slowdown; //�ӵ� ����
}

SnailAI::~SnailAI()
{

}



bool SnailAI::LoadCharacterInfo(char* filename)
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

void SnailAI::OnDraw(int x, int y)
{
	slowdown = 3.0f;
	float xPos, yPos;

	xPos = m_vecPosition.x - m_vecCenter[m_idxMotion][m_aniIndex].x;
	yPos = m_vecPosition.y - m_vecCenter[m_idxMotion][m_aniIndex].y;

	m_pSprite->Draw(xPos, yPos,
		m_rcFrame[0][m_aniIndex].left,
		m_rcFrame[0][m_aniIndex].top,
		m_rcFrame[0][m_aniIndex].right,
		m_rcFrame[0][m_aniIndex].bottom, 0);
}

void SnailAI::OnUpdate(DWORD tick)
{
	m_deltaTime += tick;

	OnMove(AIx, AIy);

	//������ x~x+n���� ���̿� �������� x-k x+k �� 1���� �����ϸ� n �� ���ݺ��� x�� �۴ٸ� ����, ũ�ٸ� ���������� �̵��Ѵ�.
	//������ ��ģ�ٸ��..? �����椾�R�R�� �׻����� ���߾�. . . �׷� ���⼭ �����ϰ����ϴ� . , ,

	if (slowdown <= 0)
		OnMove(0.0f, 0.02f * slowdown);

	m_vecPosition.x += (m_vecDirection.x * 200.0f * tick / 1000.0f);
	m_vecPosition.y += (m_vecDirection.y * 200.0f * tick / 1000.0f);


	if (m_deltaTime >= m_interval)
	{
		m_deltaTime = 0;
		m_aniIndex++;

		if (firstxReturn() == 2 || secondxReturn() == 2 || thirdxReturn() == 2)// 2 ��������
		{
			AIx = -0.02f * slowdown;
			AIy = 0.02f;
		}
		else if (firstxReturn() == 3 || secondxReturn() == 3 || thirdxReturn() == 3) // 3 ����������
		{
			AIx = 0.02f * slowdown;
			AIy = 0.02f;
		}

		if (rand() % 50 >= 0 && rand() % 50 < 10) // go left
		{
			if (m_vecPosition.x >= 0)
			{
				AIx = -0.02f * slowdown;
				AIy = 0.02f;
			}
			else
			{
				AIx = 0.0f;
				AIy = 0.02f;
			}
			//OnMove(-0.02f * slowdown, 0.02f);
		}
		else if (rand() % 50 >= 10 && rand() % 50 < 20) //go right
		{
			if (m_vecPosition.x <= 512)
			{
				AIx = 0.02f * slowdown;
				AIy = 0.02f;
			}
			else
			{
				AIx = 0.0f;
				AIy = 0.02f;
			}
		}
		else if (rand() % 50 >= 20 && rand() % 50 < 45) // go front
		{
			if (m_vecPosition.y > 0)
			{
				AIx = 0.0f;
				AIy = -0.04f * slowdown;
			}
			else
			{
				AIx = 0.0f;
				AIy = 0.0f;
			}
		}
		else if (rand() % 50 >= 45 && rand() % 50 < 49)
		{
			AIx = 0.0f;
			AIy = 0.02f * slowdown;
		}
		else
		{
			AIx = 0.0f;
			AIy = 0.02f;
		}

		if (rand() % 2 == 1)
		{
			if (slowdown >= 0.0f)
				slowdown -= 0.3f;
		}
		else
		{
			slowdown += 0.1f;
		}

		if (m_aniIndex >= m_nFrame[m_idxMotion])
		{
			m_aniIndex = 0;
		}
	}
}

void SnailAI::OnMove(float movex, float movey)
{
	m_vecDirection.x = movex;
	m_vecDirection.y = movey;
}