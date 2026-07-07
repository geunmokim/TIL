#pragma once
typedef struct tagInfo
{
	float	fX;
	float	fY;
	int		iCX;
	int		iCY;
}INFO;

typedef struct tagLinePos
{
	tagLinePos() { ZeroMemory(this, sizeof(tagLinePos)); }
	tagLinePos(float _x, float _y) : fX(_x), fY(_y) {}

	float	fX;
	float	fY;
}LINEPOS;

typedef struct tagLineInfo
{
	tagLineInfo() { ZeroMemory(this, sizeof(tagLineInfo)); } //tag가 앞에 붙는 이유는 typedef 떄문
	tagLineInfo(LINEPOS& _tLeftPos, LINEPOS& _tRightPos)
		: tLeftPos(_tLeftPos), tRightPos(_tRightPos) {
	}

	LINEPOS		tLeftPos;
	LINEPOS		tRightPos;
}LINEINFO;