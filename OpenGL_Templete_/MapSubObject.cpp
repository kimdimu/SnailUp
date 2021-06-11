#include "MapSubObject.h"


CMapSubObject::CMapSubObject()
{
}


CMapSubObject::~CMapSubObject()
{
}

void CMapSubObject::SetObectRect(int left, int top,
	int right, int bottom)
{
	m_rcImg.left = left; m_rcImg.top = top;
	m_rcImg.right = right; m_rcImg.bottom = bottom;
}

void CMapSubObject::SetPosition(float x, float y)
{
	m_Position.x = x; m_Position.y = y;
}

float CMapSubObject::OnUpdate(dsTexture * pTexture, DWORD tick)
{
	int widthBackground = m_rcImg.right;
	int heightBackground = m_rcImg.bottom;

	float height = (float)heightBackground - m_yScrollOffset;

	Draw(pTexture, 0.0f, m_yScrollOffset, m_rcImg.left, m_rcImg.top, m_rcImg.right, height);	

	if (m_yScrollOffset > 0.0f)
	{
		Draw(pTexture, 0.0f, 0.0f, m_rcImg.left, m_rcImg.top + height,
			    m_rcImg.right, m_yScrollOffset);
	}
	
	float timeOffset = m_speedScroll*tick / 1000.0f;
	float offset = m_yScrollOffset + timeOffset;
	float extra = offset - heightBackground;
	if (extra >= 0.0f)
		m_yScrollOffset =  extra;
	else
		m_yScrollOffset = offset;

	return timeOffset;
}


void CMapSubObject::OnUpdate(dsTexture * pTexture, DWORD tick, float offset)
{
	m_Position.y += offset;

	if ((m_Position.y + m_rcImg.bottom) < 0)
		return;

	if (m_Position.y > WINDOW_SIZE_HEIGHT)
		return;

	Draw(pTexture, m_Position.x, m_Position.y, m_rcImg.left, m_rcImg.top, m_rcImg.right, m_rcImg.bottom);

}

void CMapSubObject::Draw(dsTexture * pTexture, float xDst, float yDst, 
	float xSrc, float ySrc, float width, float height)
{
	pTexture->Draw(xDst, yDst, xSrc, ySrc, width, height, 0);
}

void CMapSubObject::Draw(dsTexture * pTexture)
{
	pTexture->Draw(m_Position.x, m_Position.y, m_rcImg.left, m_rcImg.top,
		m_rcImg.right, m_rcImg.bottom, 0);
}

