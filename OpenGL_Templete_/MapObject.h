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
	int scean; //¾À º¯È¯
	int startx; // ai¸ðµå  ´ÞÆØÀÌ ½ÃÀÛ À§Ä¡
	bool blueout; // ÆÄ¶õ ai »ç¸Á 
	bool yellowout; // ³ë¶õ ai »ç¸Á
	bool redout; // »¡°£ ai »ç¸Á

	CMapSubObject* m_pBackground;


public:
	CMapObject();
	virtual ~CMapObject();

	bool Create(char* filename);
	void OnDrawBackground(int x, int y);
	void OnDraw(int x, int y);
	void OnUpdate(DWORD tick);
};

