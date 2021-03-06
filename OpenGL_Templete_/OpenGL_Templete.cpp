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
int		idx;

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
NewSnail* snailarr[3];
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
//NewSnail		s_snailblue; //ai blue
//NewSnail		s_snailyellow; // ai yellow
//NewSnail	s_snailred; // ai red
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
bool blueout = true;
bool yellowout = true;
bool redout = true;

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
#pragma region t

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
#pragma endregion

#pragma region Create
	s_snail.Create("Character");
	s_snail.SetLayer(10);
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
			//OnIdle();
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
				tick = curTick;
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
	case WM_KEYDOWN:
	{
		if (wParam == VK_RETURN)
		{
			HWND hEdit = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 100, 200, 25, hWnd, NULL, g_hInst, NULL);
			g_OldEditFunc = SetWindowLong(hEdit, GWL_WNDPROC, (LONG)EditFunction);
			SetFocus(hEdit);

			break;
		}

		int xOffset = 0;
		int yOffset = 0;

		xOffset = 0; yOffset = 0;
		switch (wParam)
		{
		case VK_LEFT: xOffset = -10;  break;
		case VK_RIGHT: xOffset = 10;  break;
		case VK_UP: yOffset = -10;  break;
		case VK_DOWN: yOffset = 10; break;
		}

		SNAILMOVE sm;
		sm.id = PKT_SNAILMOVE;
		sm.size = sizeof(SNAILMOVE);
		sm.idx = idx;
		sm.xpos = snailarr[idx]->ReturnX();
		sm.ypos = snailarr[idx]->ReturnY();
		strcpy(sm.playerid, g_myStrID);
		OnSendPacket((char*)& sm, sm.size);
	}
	break;
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
		// socket()
		g_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (g_sock == INVALID_SOCKET) err_quit("socket()");


		// 서버 소켓 주소 구조체 초기화
		SOCKADDR_IN serverAddr;
		ZeroMemory(&serverAddr, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(30000);
		serverAddr.sin_addr.s_addr = inet_addr("192.168.219.105");
		if (connect(g_sock, (struct sockaddr*) & serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			closesocket(g_sock);
			err_quit("connect Error.......");
		}

		if (WSAAsyncSelect(g_sock, hWnd, WM_SOCKET, FD_WRITE | FD_READ | FD_CLOSE) != 0)
		{
			closesocket(g_sock);
			err_quit("WSAAsyncSelect Error.......");
		}
		g_hDC = GetDC(hWnd);

		g_OpenGL.Create(g_hDC);
	}
	break;
	case WM_SOCKET:
	{
		// 오류 발생 여부 확인
		if (WSAGETSELECTERROR(lParam))
		{
			err_quit("WSAGETSELECTERROR(lParam)...");
			break;
		}

		// 메시지 처리
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_WRITE:
		{
		}
		break;
		case FD_READ:
		{
			// 데이터 통신에 사용할 변수
			char buf[BUFSIZE];

			int ret = recv(wParam, (char*)buf, BUFSIZE, 0);
			if (ret == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
				}
				break;
			}

			int val = g_cirQueue.OnPutData(buf, ret);
			if (val != 1)
			{
				//error
				break;
			}

			PACKETHEADER* pHeader = g_cirQueue.GetPacket();
			while (pHeader != NULL)
			{
				OnPacketProcess(pHeader);

				g_cirQueue.OnPopData(pHeader->size);
				pHeader = g_cirQueue.GetPacket();
			}
		}
		break;
		case FD_CLOSE:
		{
			closesocket(wParam);
		}
		break;
		}
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
	{
		MYLOGIN logOut;
		logOut.id = PKT_LOGOUT;
		logOut.size = sizeof(MYLOGIN);
		strcpy(logOut.playerid, g_myStrID);

		OnSendPacket((char*)& logOut, logOut.size);

		PostQuitMessage(0);
	}
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
		//나 로그인 했어요~
		LPMYLOGIN pMyLogin = (LPMYLOGIN)pHeader;
		if (!strcmp(g_myStrID, pMyLogin->playerid))
		{
			snailarr[g_totalUser] = new NewSnail();
			snailarr[g_totalUser]->SetPlayer(g_totalUser);
			snailarr[g_totalUser]->Create("Character");
			snailarr[g_totalUser]->SetLayer(10);
			snailarr[g_totalUser]->SetPosition(128 * (g_totalUser + 1), 450);
			idx = g_totalUser;

			g_arrayUser[g_totalUser].dwUserID = pMyLogin->userID;
			strcpy(g_arrayUser[g_totalUser].strUserID, pMyLogin->playerid);
			g_totalUser++;

			g_isLogin = TRUE;
			g_myID = pMyLogin->userID;

			if (g_totalUser == 1)
			{
				blueout = false;
				yellowout = true;
				redout = true;
			}
			else if (g_totalUser == 2)
			{
				blueout = false;
				yellowout = false;
				redout = true;
			}
			else if(g_totalUser==3)
			{
				blueout = false;
				yellowout = false;
				redout = false;
			}

			g_gameState = GAME_SCENE_STATE::GAME;

			//다른 사람에게 내가 로그인 했다고 알려줘야지
			pMyLogin->id = PKT_USER_LOGIN;
			OnSendPacket((char*)pMyLogin, pMyLogin->size);
		}
	}
	break;
	case PKT_USER_LOGIN:
	{
		LPMYLOGIN pUserLogin = (LPMYLOGIN)pHeader;
		//어라 다른놈이 로그인 했군
		if (strcmp(g_myStrID, pUserLogin->playerid) && g_isLogin)
		{
			if (isConnect(pUserLogin->playerid))
				return;

			snailarr[g_totalUser] = new NewSnail();
			snailarr[g_totalUser]->SetPlayer(g_totalUser);
			snailarr[g_totalUser]->Create("Character");
			snailarr[g_totalUser]->SetLayer(10);
			snailarr[g_totalUser]->SetPosition(128 * (g_totalUser + 1), 450);

			g_arrayUser[g_totalUser].dwUserID = pUserLogin->userID;
			strcpy(g_arrayUser[g_totalUser].strUserID, pUserLogin->playerid);
			g_totalUser++;

			if (g_totalUser == 1)
			{
				blueout = false;
				yellowout = true;
				redout = true;
			}
			else if (g_totalUser == 2)
			{
				blueout = false;
				yellowout = false;
				redout = true;
			}
			else if(g_totalUser == 3)
			{
				blueout = false;
				yellowout = false;
				redout = false;
			}

			//내가 로그인 하고 있었으니 내 정보 보내줄게.
			//추가로 지금 내 세계의 배경 오브젝트 위치를 보내주지
			OBJECTMOVE om;
			om.id = PKT_LOGIN_NOTIFY;
			om.size = sizeof(OBJECTMOVE);
			om.a[0].x = s_bigstone.ReturnX();
			om.a[0].y = s_bigstone.ReturnY();
			om.a[1].x = s_bigstone2.ReturnX();
			om.a[1].y = s_bigstone2.ReturnY();
			om.a[2].x = s_smallstone.ReturnX();
			om.a[2].y = s_smallstone.ReturnY();
			om.a[3].x = s_smallstone2.ReturnX();
			om.a[3].y = s_smallstone2.ReturnY();
			om.a[4].x = s_endline.ReturnX();
			om.a[4].y = s_endline.ReturnY();
			strcpy(om.playerid, g_myStrID);
			OnSendPacket((char*)& om, om.size);
		}
	}
	break;
	case PKT_LOGIN_NOTIFY:
	{
		LPOBJECTMOVE pUserLogin = (LPOBJECTMOVE)pHeader;

		//어 이미 접속해있는 놈으로부터 메세지가 왔군,, 
		if (strcmp(g_myStrID, pUserLogin->playerid))
		{
			if (isConnect(pUserLogin->playerid))
				return;

			snailarr[g_totalUser] = new NewSnail();
			snailarr[g_totalUser]->SetPlayer(g_totalUser);
			snailarr[g_totalUser]->Create("Character");
			snailarr[g_totalUser]->SetLayer(10);
			snailarr[g_totalUser]->SetPosition(128 * (g_totalUser + 1), 450);
			idx = g_totalUser; 

			g_arrayUser[g_totalUser].dwUserID = pUserLogin->userID;
			strcpy(g_arrayUser[g_totalUser].strUserID, pUserLogin->playerid);
			g_totalUser++;

			s_bigstone.SetPosition(pUserLogin->a[0].x, pUserLogin->a[0].y);
			s_bigstone2.SetPosition(pUserLogin->a[1].x, pUserLogin->a[1].y);
			s_smallstone.SetPosition(pUserLogin->a[2].x, pUserLogin->a[2].y);
			s_smallstone2.SetPosition(pUserLogin->a[3].x, pUserLogin->a[3].y);
			s_endline.SetPosition(pUserLogin->a[4].x, pUserLogin->a[4].y);

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
		LPSNAILMOVE pSnailMove = (LPSNAILMOVE)pHeader;
		if (!isConnect(pSnailMove->playerid))
			return;
		if (strcmp(g_myStrID, pSnailMove->playerid))
		{
			snailarr[pSnailMove->idx]->SetPosition(pSnailMove->xpos, pSnailMove->ypos);



		}
	}
	break;
	case PKT_ISDEAD:
	{
		LPISDEAD pDead = (LPISDEAD)pHeader;
		if (strcmp(g_myStrID, pDead->playerid))
		{
			blueout = pDead->isDead[0];
			yellowout = pDead->isDead[1];
			redout = pDead->isDead[2];
		}
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
