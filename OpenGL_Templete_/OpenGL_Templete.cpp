// OpenGL_Templete.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "global.h"
#include "dsTexture.h"
#include <time.h>
#include "dsOpenALSoundManager.h"
#include "MyCharacter.h"
#include "MapObject.h"
//#include "BigStone.h"
#include "SmallStone_img.h"
#include "Endline.h"
#include "SnailAI.h"

#include <stdlib.h>

CFrameOpenGL  g_OpenGL;
HDC						g_hDC;

CMapObject    g_MapObj;
DWORD         g_tick;
NewSnail	s_snail;
NewSnail		s_snailblue; //ai blue
NewSnail		s_snailyellow; // ai yellow
NewSnail	s_snailred; // ai red
BigStone		s_bigstone; // 큰돌1
BigStone		s_bigstone2; // 큰돌2
SmallStone_img      s_smallstone; // 작은돌1
SmallStone_img      s_smallstone2; // 작은돌2
SmallStone_img      s_snailloseImg; // 패 이미지
SmallStone_img      s_snailwinImg; // 승 이미지
SmallStone_img      s_titleImg; // 타이틀 이미지
SmallStone_img      s_snailBwinImg; // b승 이미지
SmallStone_img      s_snailYwinImg; // y승 이미지
SmallStone_img      s_snailRwinImg; // r승 이미지
SnailAI      s_snailBai; // 1p blue
SnailAI      s_snailYai; // 2p yellow
SnailAI      s_snailRai; // 3p red
Endline      s_endline; // 결승점 이미지

void OnIdle();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS   wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = NULL;
	wndclass.hCursor = NULL;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "OpenGL_Templete";

	if (RegisterClass(&wndclass) == 0)
	{
		return 0;
	}

	RECT WindowRect;                            // 사각형의 좌상단과 / 우하단 값을 저장함
	WindowRect.left = (long)0;                    // 왼쪽 값을 0으로 지정
	WindowRect.right = (long)WINDOW_SIZE_WIDTH;               // 오른쪽 값을 요청된 너비로 지정
	WindowRect.top = (long)0;                     // 위쪽 값을 0으로 지정
	WindowRect.bottom = (long)WINDOW_SIZE_HEIGHT;             // 아래쪽 값을 요청된 높이로 지정

	DWORD        dwExStyle;                        // 윈도우 확장 스타일
	DWORD        dwStyle;                          // 윈도우 스타일

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // 윈도우 확장 스타일
	dwStyle = WS_OVERLAPPEDWINDOW;                    // 윈도우즈 스타일

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);        // 실제 요청된 크기로 윈도우 크기를 조정한다

	HWND hwnd = CreateWindowEx(dwExStyle,                // 본 윈도우용 확장스타일
		"OpenGL_Templete",							    // 클래스 이름
		"달팽이 응원하기",									   // 윈도우 타이틀
		WS_CLIPSIBLINGS |							    // 필수 윈도우 스타일
		WS_CLIPCHILDREN |						        // 필수 윈도우 스타일
		dwStyle,								    // 선택된 윈도우 스타일
		0, 0,									       // 창 위치
		WindowRect.right - WindowRect.left,			  // 조정된 창 너비를 계산함
		WindowRect.bottom - WindowRect.top,			   // 조정된 창 높이를 계산함
		NULL,												     // 부모 윈도우 없음
		NULL,									     // 메뉴 없음
		hInstance,										// 인스턴스
		NULL);										 // WM_CREATE에 아무것도 전달하지 않음


	if (hwnd == NULL)
	{
		return 0;
	}
	

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	RECT rc;
	GetClientRect(hwnd, &rc);

	srand(time(NULL));

	//
	s_snail.Create("Character");
	s_snail.SetLayer(10);

	s_snailblue.Create("Character");
	s_snailblue.SetLayer(10);

	s_snailyellow.Create("Character"); 
	s_snailyellow.SetLayer(10);

	s_snailred.Create("Character");
	s_snailred.SetLayer(10);
	//
	s_snailBai.Create("CharacterB");
	s_snailBai.SetLayer(10);

	s_snailYai.Create("CharacterY");
	s_snailYai.SetLayer(10);

	s_snailRai.Create("CharacterR");
	s_snailRai.SetLayer(10);
	//
	s_bigstone.Create("S_StoneB");
	s_bigstone.SetLayer(10);

	s_bigstone2.Create("S_StoneB");
	s_bigstone2.SetLayer(10);
	//
	s_smallstone.Create("S_StoneS", "S_StoneB");
	s_smallstone.SetLayer(10);

	s_smallstone2.Create("S_StoneS", "S_StoneB");
	s_smallstone2.SetLayer(10);
	//
	s_snailloseImg.Create("S_SnailLose", "S_StoneB");
	s_snailloseImg.SetPosition(0, -900);
	s_snailloseImg.SetLayer(10);

	s_snailwinImg.Create("S_SnailWin", "S_StoneB");
	s_snailwinImg.SetPosition(0, -900);
	s_snailwinImg.SetLayer(10);

	s_titleImg.Create("Title", "S_StoneB");
	s_titleImg.SetPosition(0, -900);
	s_titleImg.SetLayer(10);

	s_snailBwinImg.Create("S_BlueWin", "S_StoneB");
	s_snailBwinImg.SetPosition(0, -900);
	s_snailBwinImg.SetLayer(10);

	s_snailYwinImg.Create("S_YellowWin", "S_StoneB");
	s_snailYwinImg.SetPosition(0, -900);
	s_snailYwinImg.SetLayer(10);

	s_snailRwinImg.Create("S_RedWin", "S_StoneB");
	s_snailRwinImg.SetPosition(0, -900);
	s_snailRwinImg.SetLayer(10);
	//
	s_endline.Create("S_Endline");
	s_endline.SetPosition(250 + rand() % 150, -900);
	s_endline.SetLayer(10);

	g_MapObj.Create("mapdata.txt");

	dsOpenALSoundManager* pSoundManger = GetOpenALSoundManager();
	dsSound* pSound = pSoundManger->LoadSound("./Resources/Sounds/William Tell Overture Finale.wav", true);
	if (pSound)
	{
		pSound->Play();
	}

	g_tick = GetTickCount();

	MSG msg;
	while (1)
	{
		//윈도우 메세지가 있을경우 메세지를 처리한다.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //메세지가 없을 경우 게임 루프를 실행한다.
		{
			OnIdle();
		}
	}

	ReleaseDC(hwnd, g_hDC);
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//DWORD	tickCount = GetTickCount();

	switch (msg)
	{
	case WM_KEYUP:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			exit(0x1B);
			break;
		}
	}
	break;
	case WM_CREATE:
	{
		g_hDC = GetDC(hWnd);

		g_OpenGL.Create(g_hDC);
	}
	break;
	case WM_SIZE:
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);

		g_OpenGL.SetSize(width, height);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


void OnIdle()
{
	DWORD	tickCount = GetTickCount();
	DWORD tick = tickCount - g_tick;
	
	g_OpenGL.BeginRender();	
	
	g_MapObj.OnUpdate(tick);

	g_OpenGL.EndRender(g_hDC);

	g_tick = tickCount;
}


