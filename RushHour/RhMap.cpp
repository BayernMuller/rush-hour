#include "stdafx.h"
#include "RhMap.h"


CRhMap::CRhMap(CWnd* pWnd) : m_pParent(pWnd), m_pSelectedCar(nullptr)
{
	srand((int)time(NULL));
	InitMap();
}

CRhMap::~CRhMap()
{

}

void CRhMap::DrawCar(CDC * pDC)
{
	for (int i = 0; i < (int)m_Car.size(); i++)
	{
		int x = m_Car[i].GetLocation()->at(0).x;
		int y = m_Car[i].GetLocation()->at(0).y;
		int x_ = (m_Car[i].GetLocation()->end() - 1)->x;
		int y_ = (m_Car[i].GetLocation()->end() - 1)->y;	
		CBrush brush, *pOldbrush;
		brush.CreateSolidBrush(m_Car[i].GetColor());
		pOldbrush = pDC->SelectObject(&brush);
		CRect rect(m_Room[x][y].TopLeft(), m_Room[x_][y_].BottomRight());
		pDC->Rectangle(&rect);
		pDC->SelectObject(pOldbrush);
		brush.DeleteObject();
	}
}

void CRhMap::DrawMap(CDC * pDC)
{	
	CRect rect;
	m_pParent->GetClientRect(&rect);
	CBrush brush, *pOldbrush;
	brush.CreateSolidBrush(RGB(200,200,200));	
	pOldbrush = pDC->SelectObject(&brush);
	rect.SetRect(rect.left - 1, rect.top - 1, rect.right + 1, rect.bottom + 1);
	pDC->Rectangle(&rect);
	pDC->SelectStockObject(NULL_PEN);	
	pDC->SelectObject(pOldbrush);
	brush.DeleteObject();
	pDC->Rectangle(&m_Rect);	
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			brush.CreateSolidBrush(RGB(240, 240, 240));
			pOldbrush = pDC->SelectObject(&brush);
			pDC->Rectangle(&m_Room[i][j]);
			pDC->SelectObject(pOldbrush);
			brush.DeleteObject();
		}
	}	
	pDC->SelectObject(pOldbrush);
	brush.DeleteObject();

	CRect Exit(m_Room[5][2].right, m_Room[5][2].top, rect.right + 10, m_Room[5][2].bottom);
	pDC->Rectangle(&Exit);
	pDC->SetTextColor(RGB(150, 150, 150));
	pDC->DrawText(_T("EXIT"), &Exit, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CRhMap::DrawSelected(CDC * pDC)
{
	if (m_pSelectedCar == nullptr)
		return;
	std::vector<LOCATION>* pVec = m_pSelectedCar->GetLocation();
	int size = pVec->size();
	int x1 = pVec->at(0).x;
	int y1 = pVec->at(0).y;
	int x2 = pVec->at(size - 1).x;
	int y2 = pVec->at(size - 1).y;

	CPen pen, *poldpen;
	pen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	poldpen = pDC->SelectObject(&pen);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(CRect(m_Room[x1][y1].TopLeft(), m_Room[x2][y2].BottomRight()));
	pDC->SelectObject(poldpen);
	pen.DeleteObject();
	pDC->SelectStockObject(WHITE_BRUSH);
}

void CRhMap::OnClick(CPoint & pt)
{
	if (m_Rect.PtInRect(pt) == FALSE && m_pSelectedCar != nullptr)
	{
		m_pSelectedCar = nullptr;
		return;
	}
	bool IsEnd = false;
	for (int i = 0; i < (int)m_Car.size(); i++)
	{		
		for (int j = 0; j < (int)m_Car[i].GetLocation()->size(); j++)
		{
			int x = m_Car[i].GetLocation()->at(j).x;		
			int y = m_Car[i].GetLocation()->at(j).y;
			int x_ = (m_Car[i].GetLocation()->end() - 1)->x;
			int y_ = (m_Car[i].GetLocation()->end() - 1)->y;
			if (CRect(m_Room[x][y].TopLeft(), m_Room[x_][y_].BottomRight()).PtInRect(pt))
			{
				m_pSelectedCar = &m_Car[i];
				IsEnd = true;
				break;
			}	
		}
		if (IsEnd)
			break;
	}
	if (!IsEnd)
		m_pSelectedCar = nullptr;
}

void CRhMap::OnMove(const int ID)
{
	if (m_pSelectedCar == nullptr)
		return; 

	std::vector<LOCATION>* pVec = m_pSelectedCar->GetLocation();
	switch (ID)
	{
	case UM_UP:	
		if (!IsThereOwner(LOCATION(pVec->begin()->x, pVec->begin()->y - 1)))
			m_pSelectedCar->Move(BACK);
		break;
	case UM_LEFT:
		if (!IsThereOwner(LOCATION(pVec->begin()->x - 1, pVec->begin()->y)))
			m_pSelectedCar->Move(BACK);
		break;
	case UM_RIGHT:
		if (CheckEnd())
			return;
		if (!IsThereOwner(LOCATION(pVec->back().x + 1, pVec->back().y)))		
			m_pSelectedCar->Move(STRAIGHT);		
		break;
	case UM_DOWN:
		if (!IsThereOwner(LOCATION(pVec->back().x, pVec->back().y + 1)))
			m_pSelectedCar->Move(STRAIGHT);
		break;
	default:
		return;
	}
}

void CRhMap::CheckDirection(CPoint & ClickPt, CPoint & point)
{
	if (m_pSelectedCar->GetIsHorizontal())
	{
		if (ClickPt.x - GetWidth() > point.x)
		{
			OnMove(UM_LEFT);
			ClickPt = point;
		}
		else if (ClickPt.x + GetWidth() < point.x)
		{
			OnMove(UM_RIGHT);
			ClickPt = point;
		}
	}
	else
	{
		if (ClickPt.y - GetWidth() > point.y)
		{
			OnMove(UM_UP);
			ClickPt = point;
		}
		else if (ClickPt.y + GetWidth() < point.y)
		{
			OnMove(UM_DOWN);
			ClickPt = point;
		}
	}
}

void CRhMap::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
	{// 저장
		ar << m_Car.size();
		for (int i = 0; i < (int)m_Car.size(); i++)
		{
			ar << m_Car[i].GetIsMyCar();
			ar << m_Car[i].GetIsHorizontal();
			ar << m_Car[i].GetSize();
			ar << m_Car[i].GetLocation()->begin()->x;
			ar << m_Car[i].GetLocation()->begin()->y;
		}
	}
	else
	{// 불러오기
		m_Car.clear();
		int size;
		ar >> size;
		for (int i = 0; i < size; i++)
		{
			bool isMyCar, horizon;
			int carsize, x, y;			
			ar >> isMyCar >> horizon >> carsize >> x >> y;					
			if (isMyCar)
				m_Car.push_back(CRhCar(LOCATION(x, y)));
			else			
				m_Car.push_back(CRhCar(carsize, horizon, LOCATION(x, y)));
		}
		m_MapSave.clear();
		m_MapSave = m_Car;
	}
}

void CRhMap::OnClear()
{
	m_Car.clear();
	m_Car = m_MapSave;
}

bool CRhMap::CheckEnd()
{
	if (m_pSelectedCar->GetIsMyCar() && m_pSelectedCar->GetLocation()->back().x == 5)
	{
		AfxMessageBox(_T("성공!"));
		m_pSelectedCar = nullptr;
		return true;
	}
	return false;
}

bool CRhMap::IsThereOwner(LOCATION loc)
{
	for (int i = 0; i < (int)m_Car.size(); i++)
	{
		for (int j = 0; j < (int)m_Car[i].GetLocation()->size(); j++)
		{
			if (m_Car[i].GetLocation()->at(j) == loc)
			{
				return true;
			}
		}
	}
	return false;
}

void CRhMap::InitMap()
{
	m_pParent->GetClientRect(&m_Rect);
	int halfW = m_Rect.Width() / 2;
	m_Rect.SetRect(0, m_Rect.CenterPoint().y - halfW, m_Rect.CenterPoint().x + halfW, m_Rect.CenterPoint().y + halfW);
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
	m_pSelectedCar = nullptr;
}

