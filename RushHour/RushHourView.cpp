
// RushHourView.cpp : CRushHourView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "RushHour.h"
#endif

#include "RushHourDoc.h"
#include "RushHourView.h"
#include "RhMap.h"
#include "UserMap.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRushHourView

IMPLEMENT_DYNCREATE(CRushHourView, CView)

BEGIN_MESSAGE_MAP(CRushHourView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_32773, &CRushHourView::OnCreateMode)
	ON_COMMAND(ID_32774, &CRushHourView::OnTestMap)
	ON_COMMAND(ID_32775, &CRushHourView::OnSaveMap)
	ON_COMMAND(ID_32776, &CRushHourView::OnOpenMap)
	ON_COMMAND(ID_32777, &CRushHourView::OnClear)
END_MESSAGE_MAP()

// CRushHourView 생성/소멸

CRushHourView::CRushHourView() : m_pMap(nullptr), m_nSelectedBlock(NULL_BLOCK), m_MovingPt(-500,-500)
{
	// TODO: 여기에 생성 코드를 추가합니다.
}



CRushHourView::~CRushHourView()
{
	delete m_pMap;
}

BOOL CRushHourView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	cs.style &= ~WS_BORDER;
	return CView::PreCreateWindow(cs);
}

// CRushHourView 그리기


void CRushHourView::OnDraw(CDC* pDC)
{
	CRushHourDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.	
	CDC MemDC;
	CRect rect;
	GetClientRect(&rect);
	MemDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap *pOldBitmap = (CBitmap*)MemDC.SelectObject(&bitmap);
	if (m_pMap != nullptr)
	{
		m_pMap->DrawMap(&MemDC);
		m_pMap->DrawCar(&MemDC);
		m_pMap->DrawSelected(&MemDC);
		CUserMap *pUM = dynamic_cast<CUserMap*>(m_pMap);
		if (pUM != nullptr)
		{
			pUM->DrawBlock(&MemDC);
			if (m_nSelectedBlock != NULL_BLOCK)
			{
				pUM->DrawDragingBlock(&MemDC, m_MovingPt, m_nSelectedBlock);
			}
		}
	}
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();	
}


// CRushHourView 인쇄

BOOL CRushHourView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CRushHourView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CRushHourView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CRushHourView 진단

#ifdef _DEBUG
void CRushHourView::AssertValid() const
{
	CView::AssertValid();
}

void CRushHourView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRushHourDoc* CRushHourView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRushHourDoc)));
	return (CRushHourDoc*)m_pDocument;
}
#endif //_DEBUG




// CRushHourView 메시지 처리기


void CRushHourView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	m_pMap = new CRhMap(this);
}


void CRushHourView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CUserMap *pUM = dynamic_cast<CUserMap*>(m_pMap);
	if (pUM != nullptr)
	{
		m_nSelectedBlock = pUM->SelectBlock(point);
	}
	if (m_pMap != nullptr)
	{
		m_pMap->OnClick(point);
	}	
	m_pt = point;
	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}

void CRushHourView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CUserMap *pUM = dynamic_cast<CUserMap*>(m_pMap);
	if (pUM != nullptr && m_nSelectedBlock != NULL_BLOCK && nFlags & VK_LBUTTON)
	{
		m_MovingPt = point;
		Invalidate(FALSE);
		return;
	}
	if (nFlags & VK_LBUTTON && m_pMap->GetSelectedCar() != nullptr)
	{
		m_pMap->CheckDirection(m_pt, point);
		Invalidate(FALSE);
	}
	CView::OnMouseMove(nFlags, point);
}


BOOL CRushHourView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}

void CRushHourView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CUserMap *pUM = dynamic_cast<CUserMap*>(m_pMap);
	if (m_nSelectedBlock != NULL_BLOCK && pUM != nullptr)
	{		
		pUM->OnPutBlock(point, m_nSelectedBlock);
		m_nSelectedBlock = NULL_BLOCK;
		m_MovingPt = CPoint(-1000, -1000);
		Invalidate(FALSE);
	}	
	CView::OnLButtonUp(nFlags, point);
}


void CRushHourView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nChar == VK_DELETE)
	{
		CUserMap *pUM = dynamic_cast<CUserMap*>(m_pMap);
		if (pUM)
			pUM->RemoveBlock();
		Invalidate(FALSE);
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRushHourView::OnCreateMode()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CUserMap *pUM = dynamic_cast<CUserMap*>(m_pMap);
	if (pUM)
	{
		delete m_pMap;
		m_pMap = new CRhMap(this);
		return;
	}
	delete m_pMap;
	m_pMap = new CUserMap(this);
	Invalidate(FALSE);
}

void CRushHourView::OnTestMap()
{
	CUserMap *pUM = dynamic_cast<CUserMap*>(m_pMap);
	if (pUM)
	{
		pUM->SwitchTestMode();
		Invalidate(FALSE);
	}
}

void CRushHourView::OnSaveMap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CUserMap *pUM = dynamic_cast<CUserMap*>(m_pMap);
	if (pUM)
	{
		if (pUM->CanSave())
		{
			CString str(_T("Rush Hour Map (*.map)|*.map|모든 파일 (*.*)|*.*|"));
			CFileDialog SaveDlg(FALSE, _T("map"), _T("*.map"), OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, str, NULL);
			if (SaveDlg.DoModal() == IDOK)
			{
				CString strPathName = SaveDlg.GetPathName();
				CFile file;
				file.Open(strPathName, CFile::modeCreate | CFile::modeWrite);
				CArchive ar(&file, CArchive::store);
				m_pMap->Serialize(ar);
				ar.Close();
				file.Close();
				Invalidate(FALSE);
			}
		}		
		else
		{
			AfxMessageBox(_T("맵 테스트를 완료하십시오."));
		}
	}
}

void CRushHourView::OnOpenMap()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	
	CString str(_T("Rush Hour Map (*.map)|*.map|모든 파일 (*.*)|*.*|"));
	CFileDialog OpenDlg(TRUE, _T("map"), _T("*.map"), OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, str, NULL);
	if (OpenDlg.DoModal() == IDOK)
	{
		CString strPathName = OpenDlg.GetPathName();
		CFile file;
		file.Open(strPathName, CFile::modeRead);
		CArchive ar(&file, CArchive::load);		
		m_pMap->Serialize(ar);
		ar.Close();
		file.Close();
		Invalidate(FALSE);
	}
}

void CRushHourView::OnClear()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CUserMap *pUM = dynamic_cast<CUserMap*>(m_pMap);
	if (pUM)
	{
		AfxMessageBox(_T("창작모드에서 사용불가"));
		return;
	}
	m_pMap->OnClear();
	Invalidate(FALSE);
}
