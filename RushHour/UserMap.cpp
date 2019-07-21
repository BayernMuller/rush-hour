#include "stdafx.h"
#include "UserMap.h"


CUserMap::CUserMap(CWnd* pWnd) : CRhMap(pWnd), m_bTestMode(false), m_bCanSave(false)
{	
	InitMap();
}

CUserMap::~CUserMap()
{
}

void CUserMap::DrawBlock(CDC * pDC)
{
	if (m_bTestMode)
	{
		pDC->DrawText(_T(" < Map Test > "), CRect(m_Rect.left, m_Rect.top - 15, m_Rect.right, m_Rect.top + 10), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		return;
	}
	const int w = 40;
	const int hw = w / 2;
	for (int i = 0; i < 5; i++)
	{
		CRect Block;
		pDC->Rectangle(&m_Block[i]);
		const int cx = m_Block[i].CenterPoint().x;
		const int cy = m_Block[i].CenterPoint().y;
		switch (i)
		{
		case 0:
			Block.SetRect(cx - hw, cy - hw - w, cx + hw, cy + hw + w);
			break;
		case 1:
			Block.SetRect(cx - hw, cy - w, cx + hw, cy + w);
			break;
		case 2:
			Block.SetRect(cx - w - hw , cy - hw, cx + hw + w, cy + hw);
			break;
		case 3:
			Block.SetRect(cx - w, cy - hw, cx +  w, cy + hw);
			break;
		case 4:
			CBrush brush, *pOldbrush;
			brush.CreateSolidBrush(RGB(255,30,30));
			pOldbrush = pDC->SelectObject(&brush);
			Block.SetRect(cx - w, cy - hw, cx + w, cy + hw);
			pDC->Rectangle(&Block);
			pDC->SelectObject(pOldbrush);
			brush.DeleteObject();
			return;
		}
		pDC->SelectStockObject(GRAY_BRUSH);
		pDC->Rectangle(&Block);
		pDC->SelectStockObject(WHITE_BRUSH);
	}
}

void CUserMap::DrawDragingBlock(CDC * pDC, CPoint & pt, BLOCK block)
{
	if (m_bTestMode)
		return;

	const int two = m_Room[0][1].bottom - m_Room[0][0].top;
	const int three = m_Room[0][2].bottom - m_Room[0][0].top;
	int width;
	int height;
	switch (block)
	{
	case V_THREE:
		width = GetWidth();
		height = three;
		break;
	case V_TWO:
		width = GetWidth();
		height = two;
		break;
	case H_THREE:
		width = three;
		height = GetWidth();
		break;
	case H_TWO: case MYCAR:
		width = two;
		height = GetWidth();
		break;
	default:
		break;
	}
	width /= 2;
	height /= 2;
	CRect rect(pt.x - width, pt.y - height, pt.x + width, pt.y + height);
	m_SelCarRect = rect;
	if (block == MYCAR)
	{
		CBrush brush, *pOldbrush;
		brush.CreateSolidBrush(RGB(255, 30, 30));
		pOldbrush = pDC->SelectObject(&brush);
		pDC->Rectangle(&rect);
		pDC->SelectObject(pOldbrush);
		brush.DeleteObject();
	}
	else
	{
		pDC->SelectStockObject(GRAY_BRUSH);
		pDC->Rectangle(&rect); 		
		pDC->SelectStockObject(WHITE_BRUSH);
	}
}

void CUserMap::OnPutBlock(CPoint & pt, BLOCK selblock)
{
	if (!m_Rect.PtInRect(pt))
		return;

	if (m_bTestMode)
		return;
	int size = 2;
	bool bHorizon = true;
	std::vector<CPoint> vec;
	int width = m_Width;
	switch (selblock)
	{	
	case V_THREE:
		bHorizon = false;
		size = 3;
		for (int i = -1; i < size - 1; i++)		
			vec.push_back(CPoint(pt.x, pt.y + (i * width)));	
		break;
	case V_TWO:
		bHorizon = false;
		for (int i = -1; i < size; i++)		
			if (i)			
				vec.push_back(CPoint(pt.x, pt.y + (i * (width / 2))));		
		break;
	case H_THREE:
		size = 3;
		for (int i = -1; i < size - 1; i++)		
			vec.push_back(CPoint(pt.x + (i * width), pt.y));		
		break;
	case H_TWO: case MYCAR:
		for (int i = -1; i < size; i++)		
			if (i)			
				vec.push_back(CPoint(pt.x + (i * (width / 2)), pt.y));			
			break;
	}
	
	LOCATION* pLoc;
	for (int i = 0; i < (int)vec.size(); i++)
	{		
		pLoc = GetLocation(vec[i]);
		if (pLoc == nullptr || IsThereOwner(*pLoc))
		{
			if (pLoc)
				delete pLoc;
			return;
		}
		delete pLoc;
	}	
	pLoc = GetLocation(vec[0]);
	if (selblock != MYCAR)
		m_Car.push_back(CRhCar(size, bHorizon, *pLoc));
	else
	{
		if (pLoc->y != 2)
			return;
		m_Car.push_back(CRhCar(*pLoc));
	}
	delete pLoc;
	m_bCanSave = false;
}

void CUserMap::RemoveBlock()
{
	m_bCanSave = false;
	if (m_bTestMode)
		return;
	auto Itr = m_Car.begin();
	for (; Itr != m_Car.end();)
	{
		if (m_pSelectedCar == &(*Itr))
		{
			Itr = m_Car.erase(Itr);
			m_pSelectedCar = nullptr;
			return;
		}
		else
		{
			++Itr;
		}
	}
}

LOCATION* CUserMap::GetLocation(CPoint & pt)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (m_Room[i][j].PtInRect(pt))
			{
				return new LOCATION(i, j);
			}
		}
	}
	return nullptr;
}

BLOCK CUserMap::SelectBlock(const CPoint & pt)
{
	bool bMyCar = false;
	for (int i = 0; i < (int)m_Car.size(); i++)
	{
		if (bMyCar = m_Car[i].GetIsMyCar())
			break;
	}

	for (int i = 0; i < 5; i++)
	{
		if (m_Block[i].PtInRect(pt))
		{
			if (!(bMyCar && i == MYCAR))
				return (BLOCK)i;
		}
	}
	return NULL_BLOCK;
}

void CUserMap::InitMap()
{
	m_pParent->GetClientRect(&m_Rect);
	m_Rect.SetRect(0, 0, m_Rect.Width(), m_Rect.Width());
	m_Rect.DeflateRect(m_Rect.Width() / 15, m_Rect.Width() / 15);
	int nW = m_Rect.Width() / 6;
	int top = m_Rect.top;
	int left = m_Rect.left;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_Room[i][j].SetRect(
				left + (i * nW),
				top + (j * nW),
				left + nW + (i * nW),
				top + nW + (j * nW));
		}
	}
	int w = m_Room[0][0].Width() / 15;
	m_Rect.SetRect(m_Room[0][0].left - w, m_Room[0][0].top - w, m_Room[5][5].right + w, m_Room[5][5].bottom + w);
	m_Width = m_Room[0][0].Width();
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_Room[i][j].DeflateRect(nW / 15, nW / 15);
		}
	}
	CRect rect;
	m_pParent->GetClientRect(&rect);
	CRect box(m_Rect.left, m_Rect.bottom + 20, m_Rect.right, rect.bottom - 20);
	w = box.Width() / 5;
	for (int i = 0; i < 5; i++)
	{
		m_Block[i].SetRect(box.left + (i * w), box.top, box.left + ((i + 1) * w), box.bottom);
	}
}

bool CUserMap::CheckEnd()
{
	if (m_bTestMode)
	{
		if (CRhMap::CheckEnd())
		{
			AfxMessageBox(_T("맵 통과. 이제 저장이 가능합니다."));
			m_bCanSave = true;
			SwitchTestMode();
			return true;
		}
	}
	return false;
}

void CUserMap::SwitchTestMode()
{
	m_bTestMode = !m_bTestMode;
	if (m_bTestMode)
	{
		m_MapSave.clear();
		m_MapSave = m_Car;
		CRhMap::InitMap();
	}
	else
	{
		InitMap();
		m_Car = m_MapSave;
	}
}

void CUserMap::OnMove(const int ID)
{
	m_bCanSave = false;
	CRhMap::OnMove(ID);	
}
