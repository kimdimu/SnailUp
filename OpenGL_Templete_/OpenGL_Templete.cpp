// OpenGL_Templete.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include "CirQueue.h"
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
//-------------------------------------
#define WM_SOCKET   WM_USER+1
#define BUFSIZE  1024

_tgSNAILMOVE	g_myImage;


SOCKET		g_sock = INVALID_SOCKET;

CCirQueue	g_cirQueue;


void OnUpdate(HWND hWnd, DWORD tick);
void CreateBuffer(HWND hWnd, HDC hDC);
void ReleaseBuffer(HWND hWnd, HDC hDC);

// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg);

BOOL            g_isLogin = FALSE;

char            g_myStrID[20];
DWORD			g_myID;

LONG			 g_OldEditFunc;
HINSTANCE		 g_hInst;


//user struct...
typedef struct _tgUser
{
	DWORD  dwUserID;
	char   strUserID[30];
	int    xPos;
	int    yPos;
}USER, * LPUSER;

#define MAX_USER	100
USER  g_arrayUser[MAX_USER];
int   g_totalUser = 0;

enum GAME_SCENE_STATE
{
	TITLE = 0,
	LOGIN = 1,
	GAME = 2,
};


GAME_SCENE_STATE g_gameState = GAME_SCENE_STATE::TITLE;
DWORD g_titleInterval = 0;
//-------------------------------------

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
LRESULT CALLBACK EditFunction(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int OnSendPacket(char* data, int size);
void OnPacketProcess(LPPACKETHEADER pHeader);
BOOL isConnect(char* strID);
LPUSER FindUser(char* strID);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	g_hInst = hInstance;

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
		// 윈속 종료
		WSACleanup();
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
		// 윈속 종료
		WSACleanup();
		return 0;
	}


	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	RECT rc;
	GetClientRect(hwnd, &rc);

	srand(time(NULL));

#pragma region Create
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

#pragma endregion

	dsOpenALSoundManager* pSoundManger = GetOpenALSoundManager();
	dsSound* pSound = pSoundManger->LoadSound("./Resources/Sounds/William Tell Overture Finale.wav", true);
	if (pSound)
	{
		pSound->Play();
	}

	g_tick = GetTickCount();

	MSG msg;
	DWORD tick = GetTickCount();
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
			DWORD curTick = GetTickCount();
			switch (g_gameState)
			{
			case GAME_SCENE_STATE::TITLE:
			{
				HDC hDC = GetDC(hwnd);

				g_titleInterval += (curTick - tick);
				if (g_titleInterval >= 5000)
				{
					g_gameState = GAME_SCENE_STATE::LOGIN;
					TextOut(hDC, 300, 300, "                              ",
						strlen("                              "));
				}
				else
				{
					TextOut(hDC, 300, 300, "GAME TITLE!!!",
						strlen("GAME TITLE!!!"));
				}
				ReleaseDC(hwnd, hDC);
			}
			break;
			case GAME_SCENE_STATE::LOGIN:
			{
				HDC hDC = GetDC(hwnd);
				int yID = 0;
				for (int i = 0; i < g_totalUser; i++)
				{
					if (g_arrayUser[i].dwUserID == 0)
						continue;

					TextOut(hDC, 10, yID, g_arrayUser[i].strUserID,
						strlen(g_arrayUser[i].strUserID));
					yID += 20;
				}
				ReleaseDC(hwnd, hDC);
			}
			break;
			case GAME_SCENE_STATE::GAME:
			{
				OnIdle();
				OnUpdate(hwnd, curTick - tick);
				tick = curTick;

				//g_MainBuffer->DrawImage(g_Bitmap, 0, 0);
			}
			break;
			}
		tick = curTick;
		}

	}

	closesocket(g_sock);
	// 윈속 종료
	WSACleanup();
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



LRESULT CALLBACK EditFunction(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_RETURN)
		{
			GetWindowText(hDlg, g_myStrID, 20);
			SetWindowLong(hDlg, GWL_WNDPROC, (LONG)g_OldEditFunc);
			DestroyWindow(hDlg);

			//SEND MY LOGIN DATA....
			MYLOGIN  myLogin;
			myLogin.id = PKT_MY_LOGIN;
			myLogin.size = sizeof(MYLOGIN);
			strcpy(myLogin.playerid, g_myStrID);

			OnSendPacket((char*)& myLogin, myLogin.size);
		}
	}
	return TRUE;
	}

	return ((WNDPROC)g_OldEditFunc)(hDlg, message, wParam, lParam);
}
void OnUpdate(HWND hWnd, DWORD tick)
{
	if (hWnd == NULL)
		return;

	//Color color(255, 255, 255);
	//g_BackBuffer->Clear(color);

	//g_myImage.img.Move(g_BackBuffer, g_myImage.xpos, g_myImage.ypos);

	//text out...
	//HDC hDC = g_BackBuffer->GetHDC();
	//TextOut(hDC, 300, 100, "ABCD", strlen("ABCD"));
	//g_BackBuffer->ReleaseHDC(hDC);
}
int OnSendPacket(char* data, int size)
{
	int retval = send(g_sock, data, size, 0);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
		}
	}

	return retval;
}


// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)& lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}



void OnPacketProcess(LPPACKETHEADER pHeader)
{
	switch (pHeader->id)
	{
	case PKT_MY_LOGIN:
	{
		LPMYLOGIN pMyLogin = (LPMYLOGIN)pHeader;
		if (!strcmp(g_myStrID, pMyLogin->playerid))
		{
			g_arrayUser[g_totalUser].dwUserID = pMyLogin->userID;
			strcpy(g_arrayUser[g_totalUser].strUserID, pMyLogin->playerid);
			g_totalUser++;

			g_isLogin = TRUE;
			g_myID = pMyLogin->userID;

			g_gameState = GAME_SCENE_STATE::GAME;

			pMyLogin->id = PKT_USER_LOGIN;
			OnSendPacket((char*)pMyLogin, pMyLogin->size);
		}
	}
	break;
	case PKT_USER_LOGIN:
	{
		LPMYLOGIN pUserLogin = (LPMYLOGIN)pHeader;
		//다른 유저 접속...
		if (strcmp(g_myStrID, pUserLogin->playerid))
		{
			if (isConnect(pUserLogin->playerid))
				return;

			g_arrayUser[g_totalUser].dwUserID = pUserLogin->userID;
			strcpy(g_arrayUser[g_totalUser].strUserID, pUserLogin->playerid);
			g_totalUser++;

			MYLOGIN myLogin;
			myLogin.id = PKT_LOGIN_NOTIFY;
			myLogin.size = sizeof(MYLOGIN);
			strcpy(myLogin.playerid, g_myStrID);

			OnSendPacket((char*)& myLogin, myLogin.size);
		}
	}
	break;
	case PKT_LOGIN_NOTIFY:
	{
		LPMYLOGIN pUserLogin = (LPMYLOGIN)pHeader;

		//다른 유저 접속...
		if (strcmp(g_myStrID, pUserLogin->playerid))
		{
			if (isConnect(pUserLogin->playerid))
				return;

			g_arrayUser[g_totalUser].dwUserID = pUserLogin->userID;
			strcpy(g_arrayUser[g_totalUser].strUserID, pUserLogin->playerid);
			g_totalUser++;
		}
	}
	break;
	case PKT_LOGOUT:
	{
		LPMYLOGIN pUserLogOut = (LPMYLOGIN)pHeader;
		LPUSER pUser = FindUser(pUserLogOut->playerid);
		if (pUser != NULL)
		{
			pUser->dwUserID = 0; //logout....
		}
	}
	break;
	case PKT_SNAILMOVE:
	{
		LPSNAILMOVE pUserLogOut = (LPSNAILMOVE)pHeader;
		g_myImage.xpos = pUserLogOut->xpos;
		g_myImage.ypos = pUserLogOut->ypos;
	}
	break;
	}
}


BOOL isConnect(char* strID)
{
	for (int i = 0; i < g_totalUser; i++)
	{
		if (!strcmp(strID, g_arrayUser[i].strUserID))
		{
			if (g_arrayUser[i].dwUserID == 0)
				return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

LPUSER FindUser(char* strID)
{
	for (int i = 0; i < g_totalUser; i++)
	{
		if (!strcmp(strID, g_arrayUser[i].strUserID))
		{
			if (g_arrayUser[i].dwUserID == 0)
				return NULL;

			return &g_arrayUser[i];
		}
	}

	return NULL;
}
