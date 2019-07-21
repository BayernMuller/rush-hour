#pragma once
#include "RhMap.h"

class CUserMap : public CRhMap
{
public:
	CUserMap(CWnd* pWnd);
	~CUserMap();

	void DrawBlock(CDC* pDC);
	void DrawDragingBlock(CDC*pDC, CPoint &pt, BLOCK block);	
	void OnPutBlock(CPoint &pt, BLOCK selblock);
	void RemoveBlock();
	void SwitchTestMode();
	bool CanSave() { return m_bCanSave; }

	LOCATION* GetLocation(CPoint &pt);
	BLOCK SelectBlock(const CPoint &pt);
	
	// virtual func
	void InitMap();
	bool CheckEnd();
	void OnMove(const int ID);

private:
	CRect m_Block[5];
	CRect m_SelCarRect;
	bool m_bTestMode;
	bool m_bCanSave;
};

