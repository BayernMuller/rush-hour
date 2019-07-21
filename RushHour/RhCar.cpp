#include "stdafx.h"
#include "RhCar.h"


CRhCar::CRhCar(int nSize, bool horizontal, LOCATION &FirstLoca) : 
	m_nSize(nSize), m_bHorizontal(horizontal), m_bIsMyCar(false)
{
	for (int i = 0; i < nSize; i++)
	{
		int x = FirstLoca.x;
		int y = FirstLoca.y;
		if (horizontal)		
			m_locations.push_back(LOCATION(x + i, y));		
		else		
			m_locations.push_back(LOCATION(x, y + i));		
	}	
	m_Color = RGB(rand() % 100, rand() % 255, rand() % 255);
}

CRhCar::CRhCar(LOCATION &FirstLoca) : m_nSize(2), m_bHorizontal(true), m_bIsMyCar(true)
{
	m_Color = RGB(255, 30, 30);
	for (int i = 0; i < 2; i++)
	{
		int x = FirstLoca.x;
		int y = FirstLoca.y;
		m_locations.push_back(LOCATION(x + i, y));		
	}
}

CRhCar::~CRhCar()
{
}

void CRhCar::Move(Direction dir)
{
	if (m_bHorizontal)
	{		
		if (dir == STRAIGHT)
		{	
			if (m_locations.back().x != 5)
			{
				m_locations.erase(m_locations.begin());
				int x = m_locations.back().x;
				int y = m_locations.back().y;
				m_locations.push_back(LOCATION(x + 1, y));
			}
		}
		else
		{
			if (m_locations.begin()->x != 0)
			{
				m_locations.pop_back();
				int x = m_locations.begin()->x;
				int y = m_locations.begin()->y;
				m_locations.insert(m_locations.begin(), LOCATION(x - 1, y));
			}
		}
	}
	else
	{
		if (dir == STRAIGHT)
		{
			if (m_locations.back().y != 5)
			{
				m_locations.erase(m_locations.begin());
				int x = m_locations.back().x;
				int y = m_locations.back().y;
				m_locations.push_back(LOCATION(x, y + 1));
			}
		}
		else
		{
			if (m_locations.begin()->y != 0)
			{
				m_locations.pop_back();
				int x = m_locations.begin()->x;
				int y = m_locations.begin()->y;
				m_locations.insert(m_locations.begin(), LOCATION(x, y - 1));
			}
		}
	}
}

CRhCar & CRhCar::operator=(const CRhCar & C)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	m_bHorizontal = C.m_bHorizontal;
	m_bIsMyCar = C.m_bIsMyCar;
	m_Color = C.m_Color;
	m_locations = C.m_locations;
	m_nSize = C.m_nSize;
	return *this;
}
