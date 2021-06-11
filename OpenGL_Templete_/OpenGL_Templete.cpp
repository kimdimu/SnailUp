// OpenGL_Templete.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
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

// ���� �Լ� ���� ��� �� ����
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
BigStone		s_bigstone; // ū��1
BigStone		s_bigstone2; // ū��2
SmallStone_img      s_smallstone; // ������1
SmallStone_img      s_smallstone2; // ������2
SmallStone_img      s_snailloseImg; // �� �̹���
SmallStone_img      s_snailwinImg; // �� �̹���
SmallStone_img      s_titleImg; // Ÿ��Ʋ �̹���
SmallStone_img      s_snailBwinImg; // b�� �̹���
SmallStone_img      s_snailYwinImg; // y�� �̹���
SmallStone_img      s_snailRwinImg; // r�� �̹���
SnailAI      s_snailBai; // 1p blue
SnailAI      s_snailYai; // 2p yellow
SnailAI      s_snailRai; // 3p red
Endline      s_endline; // ����� �̹���

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
	// ���� �ʱ�ȭ
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
		// ���� ����
		WSACleanup();
		return 0;
	}

	RECT WindowRect;                            // �簢���� �»�ܰ� / ���ϴ� ���� ������
	WindowRect.left = (long)0;                    // ���� ���� 0���� ����
	WindowRect.right = (long)WINDOW_SIZE_WIDTH;               // ������ ���� ��û�� �ʺ�� ����
	WindowRect.top = (long)0;                     // ���� ���� 0���� ����
	WindowRect.bottom = (long)WINDOW_SIZE_HEIGHT;             // �Ʒ��� ���� ��û�� ���̷� ����

	DWORD        dwExStyle;                        // ������ Ȯ�� ��Ÿ��
	DWORD        dwStyle;                          // ������ ��Ÿ��

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // ������ Ȯ�� ��Ÿ��
	dwStyle = WS_OVERLAPPEDWINDOW;                    // �������� ��Ÿ��

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);        // ���� ��û�� ũ��� ������ ũ�⸦ �����Ѵ�

	HWND hwnd = CreateWindowEx(dwExStyle,                // �� ������� Ȯ�彺Ÿ��
		"OpenGL_Templete",							    // Ŭ���� �̸�
		"������ �����ϱ�",									   // ������ Ÿ��Ʋ
		WS_CLIPSIBLINGS |							    // �ʼ� ������ ��Ÿ��
		WS_CLIPCHILDREN |						        // �ʼ� ������ ��Ÿ��
		dwStyle,								    // ���õ� ������ ��Ÿ��
		0, 0,									       // â ��ġ
		WindowRect.right - WindowRect.left,			  // ������ â �ʺ� �����
		WindowRect.bottom - WindowRect.top,			   // ������ â ���̸� �����
		NULL,												     // �θ� ������ ����
		NULL,									     // �޴� ����
		hInstance,										// �ν��Ͻ�
		NULL);										 // WM_CREATE�� �ƹ��͵� �������� ����


	if (hwnd == NULL)
	{
		// ���� ����
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
		//������ �޼����� ������� �޼����� ó���Ѵ�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //�޼����� ���� ��� ���� ������ �����Ѵ�.
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
	// ���� ����
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


// ���� �Լ� ���� ��� �� ����
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
		//�ٸ� ���� ����...
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

		//�ٸ� ���� ����...
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
