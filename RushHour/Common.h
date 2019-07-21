#pragma once

struct LOCATION
{
	int x, y;
	inline LOCATION(int ix, int iy) { x = ix; y = iy; }
	inline LOCATION() {}
	bool operator==(LOCATION loc) { return x == loc.x && y == loc.y; }
};

struct CARINFO
{
	const int size;
	const bool horizontal;
	const LOCATION location;
	CARINFO(int nsize, bool bhorizontal, LOCATION loc) 
		: location(loc), size(nsize), horizontal(bhorizontal) {}	
};

enum BLOCK { NULL_BLOCK = -1, V_THREE, V_TWO, H_THREE, H_TWO, MYCAR };
enum Direction { BACK, STRAIGHT };
const int UM_UP    = WM_USER + 1;
const int UM_LEFT  = WM_USER + 2;
const int UM_RIGHT = WM_USER + 3;
const int UM_DOWN  = WM_USER + 4;
const int UM_CLEAR = WM_USER + 5;