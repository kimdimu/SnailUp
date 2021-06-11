#pragma once
#include "MyObject.h"
#include <map>

using namespace std;



class CMapSubObject;
class CMapObject : public CMyObject
{
protected:
	typedef map<int, CMapSubObject*>  MAP_SUBOBJ;
	MAP_SUBOBJ	m_mapSubObject;
	int scean; //�� ��ȯ
	int startx; // ai���  ������ ���� ��ġ
	bool blueout; // �Ķ� ai ��� 
	bool yellowout; // ��� ai ���
	bool redout; // ���� ai ���

	CMapSubObject* m_pBackground;


public:
	CMapObject();
	virtual ~CMapObject();

	bool Create(char* filename);
	void OnDrawBackground(int x, int y);
	void OnDraw(int x, int y);
	void OnUpdate(DWORD tick);
};

