#pragma once
#include <Windows.h>
#include "MyObject.h"

class CMapSubObject
{
protected:
	RECT	m_rcImg;
	Vector2 m_Position;
	float		 m_speedScroll;
	float      m_xScrollOffset;
	float      m_yScrollOffset;

	int         m_layer;

public:
	CMapSubObject();
	~CMapSubObject();

	void SetLayer(int layer) { m_layer = layer; }
	int GetLayer() { return m_layer; }

	void SetPosition(float x, float y);
	void SetObectRect(int left, int top, int right, int bottom);
	void SetScrollSpeed(float speed) { m_speedScroll = speed; }
	void SetScrollOffset(float x, float y) { m_xScrollOffset = x; m_yScrollOffset = y; }
	float OnUpdate(dsTexture * pTexture, DWORD tick);
	void OnUpdate(dsTexture * pTexture, DWORD tick, float offset);
	void Draw(dsTexture * pTexture);
	void Draw(dsTexture * pTexture, float xDst, float yDst, float xSrc, float ySrc, float width, float height);
};

