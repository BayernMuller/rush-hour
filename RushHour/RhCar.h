#pragma once
#include <vector>

class CRhCar
{
public:
	// 老馆 积己磊
	CRhCar(int nSize, bool horizontal, LOCATION &FirstLoca);

	// 郴瞒 积己磊
	CRhCar(LOCATION &FirstLoca);
	~CRhCar();

	// Getter
	std::vector<LOCATION>* GetLocation() { return &m_locations; }
	COLORREF& GetColor() { return m_Color; }
	bool GetIsHorizontal() { return m_bHorizontal; }
	bool GetIsMyCar() { return m_bIsMyCar; }
	int GetSize() { return m_nSize; }
	// Func
	void Move(Direction dir);
	CRhCar& operator=(const CRhCar& C);
	
private:
	bool m_bIsMyCar;
	int m_nSize;
	bool m_bHorizontal;
	std::vector<LOCATION> m_locations;
	COLORREF m_Color;
};

