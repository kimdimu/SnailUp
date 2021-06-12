#include "MapObject.h"
#include "MapSubObject.h"
#include "BigStone.h"
#include "SmallStone_img.h"
#include "Endline.h"
#include "SnailAI.h"
#include <conio.h>

extern NewSnail      s_snail;
//extern NewSnail         s_snailblue;
//extern NewSnail         s_snailyellow;
//extern NewSnail      s_snailred;

extern NewSnail* snailarr[3];
extern int   g_totalUser;

extern BigStone		s_bigstone;
extern BigStone      s_bigstone2;

extern SmallStone_img      s_smallstone;
extern SmallStone_img      s_smallstone2;
extern SmallStone_img      s_snailloseImg;
extern SmallStone_img      s_snailwinImg;
extern SmallStone_img      s_titleImg;
extern SmallStone_img      s_snailBwinImg;
extern SmallStone_img      s_snailYwinImg;
extern SmallStone_img      s_snailRwinImg;
extern SnailAI      s_snailBai;
extern SnailAI      s_snailYai;
extern SnailAI      s_snailRai;
extern Endline      s_endline;


CMapObject::CMapObject()
{
	scean = 0;
	startx = 128;

	blueout = false;
	yellowout = false;
	redout = false;
}


CMapObject::~CMapObject()
{
	MAP_SUBOBJ::iterator it = m_mapSubObject.begin();
	for (; it != m_mapSubObject.end(); it++)
	{
		CMapSubObject* pSubObj = (CMapSubObject*)it->second;
		delete pSubObj;
	}
	m_mapSubObject.clear();

	delete m_pBackground;
}

bool CMapObject::Create(char* filename)
{
	if (m_pSprite != NULL)
		delete m_pSprite;

	char buff[MAX_PATH];
	char texture[MAX_PATH];
	wsprintf(buff, "./Resources/Maps/%s", filename);

	int count, type, left, top, width, height, interval, x, y, layer;
	FILE* fp = fopen(buff, "rt");

	fscanf(fp, "%s", buff);
	wsprintf(texture, "./Resources/%s", buff);
	m_pSprite = new dsTexture(texture);
	if (m_pSprite->LoadFromFilename(texture) == false)
	{
		fclose(fp);
		return false;
	}

	fscanf(fp, "%d %d", &type, &count);	//type : 0 - background, 1 - object

	fscanf(fp, "%d %d %d %d %d", &left, &top, &width, &height, &interval);
	m_pBackground = new CMapSubObject();
	m_pBackground->SetObectRect(left, top, width, height);
	m_pBackground->SetPosition((float)0, (float)0);
	m_pBackground->SetScrollSpeed((float)interval);
	m_pBackground->SetScrollOffset((float)0, (float)0);
	m_pBackground->SetLayer(0);

	fscanf(fp, "%d %d", &type, &count);
	for (int i = 0; i < count; i++)
	{
		fscanf(fp, "%d %d %d %d %d %d %d %d", &left, &top, &width, &height, &interval, &x, &y, &layer);
		CMapSubObject* pSubObj = new CMapSubObject();
		pSubObj->SetObectRect(left, top, width, height);
		pSubObj->SetPosition((float)x, (float)y);
		pSubObj->SetScrollSpeed((float)interval);
		pSubObj->SetScrollOffset((float)0, (float)0);
		pSubObj->SetLayer(layer);

		m_mapSubObject[i] = pSubObj;
	}

	fclose(fp);
}

void CMapObject::OnDraw(int x, int y)
{
	MAP_SUBOBJ::iterator it = m_mapSubObject.begin();
	for (; it != m_mapSubObject.end(); it++)
	{
		CMapSubObject* pSubObj = (CMapSubObject*)it->second;
		pSubObj->Draw(m_pSprite);
	}
}

void CMapObject::OnDrawBackground(int x, int y)
{
	m_pBackground->Draw(m_pSprite);
}

void CMapObject::OnUpdate(DWORD tick)
{
	if (scean == 0)//시작화면
	{
		//타이틀 이미지 그리기
		s_titleImg.OnUpdate(tick, 0, 0);
		s_titleImg.SetPosition(0, 0);
		s_titleImg.OnDraw(0);

		BYTE key[256];
		::GetKeyboardState(key);

		if (key[VK_RETURN] & 0x80)
			scean = 2;
		if (key[0x43] & 0x80)
			scean = 1;

		//응원할 달팽이 고르기
		if (key[0x31] & 0x80)
		{
			s_snail.SetSkin(0);
			startx = 128;
		}
		if (key[0x32] & 0x80)
		{
			s_snail.SetSkin(1);
			startx = 256;
		}
		if (key[0x33] & 0x80)
		{
			s_snail.SetSkin(2);
			startx = 384;
		}
		s_snail.OnUpdate(tick, 3);
		s_snail.OnDraw(375, 380, 33);

		int x;
		//위치 초기화하기
		//for (int i = 0 ; i < g_totalUser;++i)
		//{
		//	snailarr[i]->SetPosition(128 * (i + 1), 450);
		//}
		//s_snail.SetPosition(startx, 450);
		//s_snailblue.SetPosition(128, 450);
		//s_snailblue.SetPlayer(0);
		//s_snailyellow.SetPosition(256, 450);
		//s_snailyellow.SetPlayer(1);
		//s_snailred.SetPosition(384, 450);
		//s_snailred.SetPlayer(2);
		//
		s_snailBai.SetPosition(128, 450);
		s_snailYai.SetPosition(256, 450);
		s_snailRai.SetPosition(384, 450);
		//
		s_bigstone.SetPosition(rand() % 500 - 100, rand() % 300 - 400);
		s_bigstone2.SetPosition(rand() % 500, rand() % 300 - 300);
		s_smallstone.SetPosition(rand() % 500 - 100, rand() % 300 - 200);
		s_smallstone2.SetPosition(rand() % 500, rand() % 300 - 100);
		//
		s_snailloseImg.SetPosition(0, -900);
		s_snailwinImg.SetPosition(0, -900);
		s_titleImg.SetPosition(0, -900);
		s_snailBwinImg.SetPosition(0, -900);
		s_snailYwinImg.SetPosition(0, -900);
		s_snailRwinImg.SetPosition(0, -900);
		//
		s_endline.SetPosition(rand() % 400, -300);

		blueout = false;
		yellowout = false;
		redout = false;
	}
	if (scean == 1) //ai모드
	{
		s_snail.OnUpdate(tick);
		s_snailBai.OnUpdate(tick);
		s_snailYai.OnUpdate(tick);
		s_snailRai.OnUpdate(tick);

		float offset = m_pBackground->OnUpdate(m_pSprite, tick);


		int charLayer = s_snail.GetLayer();

		typedef map<int, CMapSubObject*>  MAP_PREDRAWOBJ;
		MAP_PREDRAWOBJ			m_mapPreDrawSubObject;

		typedef map<int, CMapSubObject*>  MAP_POSTDRAWOBJ;
		MAP_POSTDRAWOBJ			m_mapPostDrawSubObject;


		MAP_SUBOBJ::iterator it = m_mapSubObject.begin();
		for (; it != m_mapSubObject.end(); it++)
		{
			CMapSubObject* pSubObj = (CMapSubObject*)it->second;
			if (pSubObj->GetLayer() < charLayer)
			{
				m_mapPreDrawSubObject[it->first] = it->second;
			}
			else
			{
				m_mapPostDrawSubObject[it->first] = it->second;
			}
		}

		MAP_PREDRAWOBJ::iterator itPre = m_mapPreDrawSubObject.begin();
		for (; itPre != m_mapPreDrawSubObject.end(); itPre++)
		{
			CMapSubObject* pSubObj = (CMapSubObject*)itPre->second;
			pSubObj->OnUpdate(m_pSprite, tick, offset);
		}


		s_snail.OnDraw(0);

		//선택한 달팽이에 따라 등장하는 개체가 다른 ai 달팽이
		if (s_snail.Skinreturn() == 0)
		{
			if (yellowout == false)
				s_snailYai.OnDraw(0, 0);
			if (redout == false)
				s_snailRai.OnDraw(0, 0);
		}
		else if (s_snail.Skinreturn() == 1)
		{
			if (blueout == false)
				s_snailBai.OnDraw(0, 0);
			if (redout == false)
				s_snailRai.OnDraw(0, 0);
		}
		else
		{
			if (blueout == false)
				s_snailBai.OnDraw(0, 0);
			if (yellowout == false)
				s_snailYai.OnDraw(0, 0);
		}

		//드로우
		s_bigstone.OnMoveY(offset);
		s_bigstone.OnDraw(0);

		s_smallstone.OnMoveY(offset);
		s_smallstone.OnDraw(0);

		s_smallstone2.OnMoveY(offset);
		s_smallstone2.OnDraw(0);

		s_bigstone2.OnMoveY(offset);
		s_bigstone2.OnDraw(0);

		s_endline.OnMoveY(offset);
		s_endline.OnDraw(0);

		s_snailloseImg.OnDraw(0);
		s_snailwinImg.OnDraw(0);


		MAP_POSTDRAWOBJ::iterator itPost = m_mapPostDrawSubObject.begin();
		for (; itPost != m_mapPostDrawSubObject.end(); itPost++)
		{
			CMapSubObject* pSubObj = (CMapSubObject*)itPost->second;
			pSubObj->OnUpdate(m_pSprite, tick, offset);
		}

		m_mapPreDrawSubObject.clear();
		m_mapPostDrawSubObject.clear();

		//돌과 닿으면 플레이어 죽음
		if (s_bigstone.OnUpdate(tick, s_snail.ReturnX(), s_snail.ReturnY()) == true)
		{
			scean = 3;
		}
		if (s_bigstone2.OnUpdate(tick, s_snail.ReturnX(), s_snail.ReturnY()) == true)
		{
			scean = 3;
		}
		if (s_smallstone.OnUpdate(tick, s_snail.ReturnX(), s_snail.ReturnY()) == true)
		{
			scean = 3;
		}
		if (s_smallstone2.OnUpdate(tick, s_snail.ReturnX(), s_snail.ReturnY()) == true)
		{
			scean = 3;
		}

		//AI 의 시작
		//ai에게 바위의 x를 알려주는 코드 아 잠깐만 y도 검사해야함,아래 한계를 없애자
		if (s_bigstone.OnUpdate(tick, s_snailBai.ReturnX(), s_snailBai.ReturnY()) == 2 ||
			s_smallstone.OnUpdate(tick, s_snailBai.ReturnX(), s_snailBai.ReturnY()) == 2 ||
			s_smallstone2.OnUpdate(tick, s_snailBai.ReturnX(), s_snailBai.ReturnY()) == 2 ||
			s_bigstone2.OnUpdate(tick, s_snailBai.ReturnX(), s_snailBai.ReturnY()) == 2)
			s_snailBai.firstxGet(1);
		if (s_bigstone.OnUpdate(tick, s_snailBai.ReturnX(), s_snailBai.ReturnY()) == 3 ||
			s_smallstone.OnUpdate(tick, s_snailBai.ReturnX(), s_snailBai.ReturnY()) == 3 ||
			s_smallstone2.OnUpdate(tick, s_snailBai.ReturnX(), s_snailBai.ReturnY()) == 3 ||
			s_bigstone2.OnUpdate(tick, s_snailBai.ReturnX(), s_snailBai.ReturnY()) == 3)
			s_snailBai.firstxGet(2);
		if (s_bigstone.OnUpdate(tick, s_snailYai.ReturnX(), s_snailYai.ReturnY()) == 2 ||
			s_smallstone.OnUpdate(tick, s_snailYai.ReturnX(), s_snailYai.ReturnY()) == 2 ||
			s_smallstone2.OnUpdate(tick, s_snailYai.ReturnX(), s_snailYai.ReturnY()) == 2 ||
			s_bigstone2.OnUpdate(tick, s_snailYai.ReturnX(), s_snailYai.ReturnY()) == 2)
			s_snailYai.secondxGet(1);
		if (s_bigstone.OnUpdate(tick, s_snailYai.ReturnX(), s_snailYai.ReturnY()) == 3 ||
			s_smallstone.OnUpdate(tick, s_snailYai.ReturnX(), s_snailYai.ReturnY()) == 3 ||
			s_smallstone2.OnUpdate(tick, s_snailYai.ReturnX(), s_snailYai.ReturnY()) == 3 ||
			s_bigstone2.OnUpdate(tick, s_snailYai.ReturnX(), s_snailYai.ReturnY()) == 3)
			s_snailYai.secondxGet(2);
		if (s_bigstone.OnUpdate(tick, s_snailRai.ReturnX(), s_snailRai.ReturnY()) == 2 ||
			s_smallstone.OnUpdate(tick, s_snailRai.ReturnX(), s_snailRai.ReturnY()) == 2 ||
			s_smallstone2.OnUpdate(tick, s_snailRai.ReturnX(), s_snailRai.ReturnY()) == 2 ||
			s_bigstone2.OnUpdate(tick, s_snailRai.ReturnX(), s_snailRai.ReturnY()) == 2)
			s_snailRai.thirdxGet(1);
		if (s_bigstone.OnUpdate(tick, s_snailRai.ReturnX(), s_snailRai.ReturnY()) == 3 ||
			s_smallstone.OnUpdate(tick, s_snailRai.ReturnX(), s_snailRai.ReturnY()) == 3 ||
			s_smallstone2.OnUpdate(tick, s_snailRai.ReturnX(), s_snailRai.ReturnY()) == 3 ||
			s_bigstone2.OnUpdate(tick, s_snailRai.ReturnX(), s_snailRai.ReturnY()) == 3)
			s_snailRai.thirdxGet(2);


		//적 ai가 모두 죽어서 승리화면 뜨는것
		//pl blue
		if (s_bigstone.OnUpdate(tick, s_snailBai.ReturnX(), s_snailBai.ReturnY()) == true ||
			s_smallstone.OnUpdate(tick, s_snailBai.ReturnX(), s_snailBai.ReturnY()) == true ||
			s_smallstone2.OnUpdate(tick, s_snailBai.ReturnX(), s_snailBai.ReturnY()) == true ||
			s_bigstone2.OnUpdate(tick, s_snailBai.ReturnX(), s_snailBai.ReturnY()) == true)
			blueout = true;
		//pl yellow
		if (s_bigstone.OnUpdate(tick, s_snailYai.ReturnX(), s_snailYai.ReturnY()) == true ||
			s_smallstone.OnUpdate(tick, s_snailYai.ReturnX(), s_snailYai.ReturnY()) == true ||
			s_smallstone2.OnUpdate(tick, s_snailYai.ReturnX(), s_snailYai.ReturnY()) == true ||
			s_bigstone2.OnUpdate(tick, s_snailYai.ReturnX(), s_snailYai.ReturnY()) == true)
			yellowout = true;
		//pl red
		if (s_bigstone.OnUpdate(tick, s_snailRai.ReturnX(), s_snailRai.ReturnY()) == true ||
			s_smallstone.OnUpdate(tick, s_snailRai.ReturnX(), s_snailRai.ReturnY()) == true ||
			s_smallstone2.OnUpdate(tick, s_snailRai.ReturnX(), s_snailRai.ReturnY()) == true ||
			s_bigstone2.OnUpdate(tick, s_snailRai.ReturnX(), s_snailRai.ReturnY()) == true)
			redout = true;

		//pl blue win
		if (s_snail.Skinreturn() == 0)
		{
			if (yellowout == true && redout == true)
			{
				scean = 4;
			}
		}
		//pl yellow win
		else if (s_snail.Skinreturn() == 1)
		{
			if (blueout == true && redout == true)
			{
				scean = 4;
			}
		}
		//pl red win
		else
		{
			if (blueout == true && yellowout == true)
			{
				scean = 4;
			}
		}


		//pl가 결승점 도착하면 승리
		if (s_endline.OnUpdate(tick, s_snail.ReturnX(), s_snail.ReturnY()) == true)
		{
			scean = 4;
		}

		//ai가 결승점 도착하면 pl 패배
		if (s_endline.OnUpdate(tick, s_snailBai.ReturnX(), s_snailBai.ReturnY()) == true)
		{
			scean = 3;
		}
		if (s_endline.OnUpdate(tick, s_snailYai.ReturnX(), s_snailYai.ReturnY()) == true)
		{
			scean = 3;
		}
		if (s_endline.OnUpdate(tick, s_snailRai.ReturnX(), s_snailRai.ReturnY()) == true)
		{
			scean = 3;
		}
	}

	if (scean == 2)//3p모드
	{
		for (int i = 0; i < g_totalUser; ++i)
		{
			snailarr[i]->OnUpdate(tick);
		}
		s_snailBai.OnUpdate(tick);
		s_snailYai.OnUpdate(tick);
		s_snailRai.OnUpdate(tick);

		float offset = m_pBackground->OnUpdate(m_pSprite, tick);


		int charLayer = snailarr[0]->GetLayer();

		typedef map<int, CMapSubObject*>  MAP_PREDRAWOBJ;
		MAP_PREDRAWOBJ			m_mapPreDrawSubObject;

		typedef map<int, CMapSubObject*>  MAP_POSTDRAWOBJ;
		MAP_POSTDRAWOBJ			m_mapPostDrawSubObject;


		MAP_SUBOBJ::iterator it = m_mapSubObject.begin();
		for (; it != m_mapSubObject.end(); it++)
		{
			CMapSubObject* pSubObj = (CMapSubObject*)it->second;
			if (pSubObj->GetLayer() < charLayer)
			{
				m_mapPreDrawSubObject[it->first] = it->second;
			}
			else
			{
				m_mapPostDrawSubObject[it->first] = it->second;
			}
		}

		MAP_PREDRAWOBJ::iterator itPre = m_mapPreDrawSubObject.begin();
		for (; itPre != m_mapPreDrawSubObject.end(); itPre++)
		{
			CMapSubObject* pSubObj = (CMapSubObject*)itPre->second;
			pSubObj->OnUpdate(m_pSprite, tick, offset);
		}


		for (int i = 0; i < g_totalUser; ++i)
		{
			snailarr[i]->OnDraw(i);
		}

		s_bigstone.OnMoveY(offset);
		s_bigstone.OnDraw(0);

		s_smallstone.OnMoveY(offset);
		s_smallstone.OnDraw(0);

		s_smallstone2.OnMoveY(offset);
		s_smallstone2.OnDraw(0);

		s_bigstone2.OnMoveY(offset);
		s_bigstone2.OnDraw(0);

		s_endline.OnMoveY(offset);
		s_endline.OnDraw(0);

		s_snailloseImg.OnDraw(0);
		s_snailwinImg.OnDraw(0);


		MAP_POSTDRAWOBJ::iterator itPost = m_mapPostDrawSubObject.begin();
		for (; itPost != m_mapPostDrawSubObject.end(); itPost++)
		{
			CMapSubObject* pSubObj = (CMapSubObject*)itPost->second;
			pSubObj->OnUpdate(m_pSprite, tick, offset);
		}

		m_mapPreDrawSubObject.clear();
		m_mapPostDrawSubObject.clear();

		for (int i = 0; i < g_totalUser; ++i)
		{
			//게임 오버
			if (s_bigstone.OnUpdate(tick, snailarr[i]->ReturnX(), snailarr[i]->ReturnY()) == true
				|| s_bigstone2.OnUpdate(tick, snailarr[i]->ReturnX(), snailarr[i]->ReturnY()) == true
				|| s_smallstone.OnUpdate(tick, snailarr[i]->ReturnX(), snailarr[i]->ReturnY()) == true
				|| s_smallstone2.OnUpdate(tick, snailarr[i]->ReturnX(), snailarr[i]->ReturnY()) == true)
			{
				snailarr[i]->SetPosition(-128, 450);

				if (i == 0)
					blueout = true;
				else if (i == 1)
					yellowout = true;
				else if (i == 1)
					redout = true;
			}
		}


		if (redout && yellowout)
			scean = 5; //blue win

		if (blueout && redout)
			scean = 6; // yellow win

		if (blueout && yellowout)
			scean = 7; //red win


		//결승점

		for (int i = 0; i < g_totalUser; ++i)
		{
			if (s_endline.OnUpdate(tick, snailarr[i]->ReturnX(), snailarr[i]->ReturnY()) == true)
			{
				if (i == 0)
					scean = 5;
				else if (i == 1)
					scean = 6;
				else if (i == 1)
					scean = 7;
				scean = 5;
			}
		}
	}

	if (scean == 3)//pl의 패배
	{
		s_snailloseImg.OnUpdate(tick, 0, 0);
		s_snailloseImg.SetPosition(0, 0);
		s_snailloseImg.OnDraw(0);

		BYTE key[256];
		::GetKeyboardState(key);
		if (key[VK_BACK] & 0x80)
			scean = 0;
	}
	if (scean == 4)//pl으 승리
	{
		s_snailwinImg.OnUpdate(tick, 0, 0);
		s_snailwinImg.SetPosition(0, 0);
		s_snailwinImg.OnDraw(0);

		BYTE key[256];
		::GetKeyboardState(key);
		if (key[VK_BACK] & 0x80)
			scean = 0;
	}
	if (scean == 5)//파란달팽이 승리
	{
		s_snailBwinImg.OnUpdate(tick, 0, 0);
		s_snailBwinImg.SetPosition(0, 0);
		s_snailBwinImg.OnDraw(0);

		BYTE key[256];
		::GetKeyboardState(key);
		if (key[VK_BACK] & 0x80)
			scean = 0;
	}
	if (scean == 6)//노란달팽이 승리
	{
		s_snailYwinImg.OnUpdate(tick, 0, 0);
		s_snailYwinImg.SetPosition(0, 0);
		s_snailYwinImg.OnDraw(0);

		BYTE key[256];
		::GetKeyboardState(key);
		if (key[VK_BACK] & 0x80)
			scean = 0;
	}
	if (scean == 7)//빨간달팽이 승리
	{
		s_snailRwinImg.OnUpdate(tick, 0, 0);
		s_snailRwinImg.SetPosition(0, 0);
		s_snailRwinImg.OnDraw(0);

		BYTE key[256];
		::GetKeyboardState(key);
		if (key[VK_BACK] & 0x80)
			scean = 0;
	}
}

