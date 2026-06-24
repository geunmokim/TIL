#pragma once
typedef struct tagInfo
{
	float	fX;
	float	fY;
	int		iCX;
	int		iCY;
}INFO;

//c++에서 구조체는 class의 기능과 100% 동일한 기능을 가진다
//차이는 class에서는 public:을 선언 안해주면 기본이 private지만 구조체에서는 public:이 기본으로 되어있다
//그럼 class와 struct를 나눠서 사용하는건 어떤 기준?
//사용자의 암묵적 기준으로 구조체에 생성자 소멸자까지는 써도 자연스럽지만
//다형성으로 상속을 갖기 시작하면 클래스를 사용한다고한다 물론 구조체에 해도되지만 암묵적으로 다형성부터는 클래스를 사용한다고함
typedef struct tagLinePos
{
	tagLinePos() {};//{}를 안붙이면 링커에러 생김
	tagLinePos(float _x, float _y) : fX(_x), fY(_y) {}

	float	fX;
	float	fY;

}LINEPOS;

typedef struct tagLineInfo
{
	tagLineInfo() { ZeroMemory(this, sizeof(tagLineInfo)); }
	tagLineInfo(LINEPOS& _tLeftPos, LINEPOS& _tRightPos)
		: tLeftPos(_tLeftPos), tRightPos(_tRightPos) {
	}

	LINEPOS		tLeftPos;
	LINEPOS		tRightPos;
}LINEINFO;


typedef struct tagFrame
{
	int iStartX;
	int iEndX;
	int iStateY;
	DWORD dwDelayTime;
	DWORD dwTime;

}FRAME;