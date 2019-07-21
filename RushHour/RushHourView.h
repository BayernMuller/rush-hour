
// RushHourView.h : CRushHourView 클래스의 인터페이스
//

#pragma once
class CRhMap;



class CRushHourView : public CView
{
protected: // serialization에서만 만들어집니다.
	CRushHourView();
	DECLARE_DYNCREATE(CRushHourView)

// 특성입니다.
public:
	CRushHourDoc* GetDocument() const;
// 작업입니다.
private:
	CRhMap *m_pMap;
	CPoint m_pt;
	CPoint m_MovingPt;
	BLOCK m_nSelectedBlock;
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CRushHourView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPlayMap();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);	
	afx_msg void OnCreateMode();
	afx_msg void OnTestMap();
	afx_msg void OnSaveMap();
	afx_msg void OnOpenMap();
	afx_msg void OnClear();
};

#ifndef _DEBUG  // RushHourView.cpp의 디버그 버전
inline CRushHourDoc* CRushHourView::GetDocument() const
   { return reinterpret_cast<CRushHourDoc*>(m_pDocument); }
#endif

