#pragma once
#include <vector>
#include "RhCar.h"

typedef std::vector<CRhCar> CCarVec;

class CRhMap
{
public:
	CRhMap(CWnd* pWnd);
	virtual ~CRhMap();
	
	void DrawCar(CDC* pDC);
	void DrawMap(CDC* pDC);	
	void DrawSelected(CDC* pDC);
	void OnClick(CPoint& pt);	
	void CheckDirection(CPoint &ClickPt, CPoint &point);
	void Serialize(CArchive &ar);
	void OnClear();

	bool IsThereOwner(LOCATION loc);
	int GetWidth() { return m_Room[0][0].Width(); }

	virtual void InitMap();
	virtual void OnMove(const int ID);	
	virtual bool CheckEnd();
	CRhCar* GetSelectedCar() { return m_pSelectedCar; }

protected:
	CWnd* m_pParent;
	CRect m_Rect;
	CRect m_Room[6][6];
	CCarVec m_Car;
	CCarVec m_MapSave;
	CRhCar* m_pSelectedCar;
	int m_Width;

private:
};

