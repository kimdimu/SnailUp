#pragma once
#include"MyObject.h"

#pragma pack(push, 1)

typedef struct _tgPACKETHEADER
{
	unsigned int id;
	unsigned int size;
	unsigned int userID;
} PACKETHEADER, *LPPACKETHEADER;


#define    PKT_LOGOUT       0xbbbbbbbb
#define    PKT_LOGIN_NOTIFY 0xaacc0001
#define    PKT_USER_LOGIN   0xaabb0001
#define    PKT_MY_LOGIN     0xaaaa0001
typedef struct _tgMyLogin : public PACKETHEADER
{
	char playerid[30];
}MYLOGIN, *LPMYLOGIN;

#define    PKT_CLICK		0xa0000001
typedef struct _tgClick : public PACKETHEADER
{
	int x;
	int y;
}CLICK, *LPCLICK;

#define  PKT_OBJECTMOVE     0xbb000000
typedef struct _tgOBEJCTMOVE : public _tgMyLogin
{
	Vector2 a[4];
}OBJECTMOVE, *LPOBJECTMOVE;

#define  PKT_SNAILMOVE     0xdd000000
typedef struct _tgSNAILMOVE : public _tgMyLogin
{
	int idx;
	int xpos;
	int ypos;
}SNAILMOVE, * LPSNAILMOVE;

#pragma pack(pop)