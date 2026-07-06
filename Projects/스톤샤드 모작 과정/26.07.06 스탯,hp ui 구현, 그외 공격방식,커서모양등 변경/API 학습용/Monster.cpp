#include "framework.h"
#include "Monster.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "SoundMgr.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
CMonster::CMonster()
{
}

CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize()
{
	m_tInfo.iCX = 300;
	m_tInfo.iCY = 300;
	m_eRenderID = RENDERID::OBJECT;


	//m_pTarget = CObjMgr::Get_Instance()->Get_Player(); 

}

int CMonster::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		m_fAngle += 5.f;
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		m_fAngle -= 5.f;
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
		CSoundMgr::Get_Instance()->PlaySound(_T("tdryes00.wav"), CSoundMgr::MONSTER);

		
	}

	Update_Rect();
	return OBJ_NOEVENT;
}

void CMonster::Late_Update()
{
	//몬스터의 이미지를 회전시키는게 아님
	//몬스터는 그냥 사각형 모양이고 그위에 bmp를 덧씌운것
	//그래서 몬스터를 회전시키는건 이미지의 회전이 아닌 사각형을 회전시키고 그위에 이미지를 실시간으로 덧씌우는것

	float fCenterX = (float)(m_tInfo.iCX >> 1);
	float fCenterY = (float)(m_tInfo.iCY >> 1);
	float fDis = sqrtf(fCenterX * fCenterX + fCenterY * fCenterY);

	m_tPos[0].x = (LONG)(fCenterX + cosf((135.f + m_fAngle) * PI / 180.f) * fDis);
	m_tPos[0].y = (LONG)(fCenterY - sinf((135.f + m_fAngle) * PI / 180.f) * fDis);

	m_tPos[1].x = (LONG)(fCenterX + cosf((45.f + m_fAngle) * PI / 180.f) * fDis);
	m_tPos[1].y = (LONG)(fCenterY - sinf((45.f + m_fAngle) * PI / 180.f) * fDis);

	m_tPos[2].x = (LONG)(fCenterX + cosf((225.f + m_fAngle) * PI / 180.f) * fDis);
	m_tPos[2].y = (LONG)(fCenterY - sinf((225.f + m_fAngle) * PI / 180.f) * fDis);

	//각도가 45,135(90+45),225(180 +45)인 이유는 PlgBlt때문 
	// //plgBit는 사각형 비트맵을 그릴 때 좌상단, 우상단, 좌하단 세 꼭짓점의 좌표를 받아서, 그 세 점이 만드는 평행사변형 안에 비트맵을 끼워 넣어 그림
	//회전 없는 직사각형이라면, 중심에서 각 꼭짓점까지의 방향은 정확히 대각선 방향이므로 45,135,225가 된것
}

void CMonster::Render(HDC _DC)
{
	//Rectangle(_DC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);


	HDC		hMonsterDC = CBmpMgr::Get_Instance()->Find_Image(_T("Monster"));
	HDC		hPlgDC = CBmpMgr::Get_Instance()->Find_Image(_T("Plg"));              //Plg는 monster의 배경 색과 같아야한다
	HDC		hResetDC = CBmpMgr::Get_Instance()->Find_Image(_T("Reset"));          //Reset도 마찬가지로 monster의 배경 색과 같아야한다
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	//hPlgDC와 hResetDC는 역할이 다른 도화지 
	PlgBlt(hPlgDC                                            //hPlgDC를 "회전된 몬스터"로 덮어씀  //hPlgDC는 이번 프레임 한 번 쓰고 버리는 임시 작업대
		, m_tPos
		, hMonsterDC
		, 0, 0
		, 300, 300
		, NULL, NULL, NULL);
	 
	GdiTransparentBlt(_DC                                    //hPlgDC를 화면에 합성 (읽기만 함)
		, m_tRect.left + iScrollX, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hPlgDC
		, 0, 0
		, 300, 300
		, RGB(255, 255, 255));

	BitBlt(hPlgDC, 0, 0, 300, 300, hResetDC, 0, 0, SRCCOPY);  //hPlgDC를 다시 흰색으로 덮어씀  //Reset은 hPlgDC를 다시 흰색으로 되돌리기 위한 원본 자료


	/*
	BOOL PlgBlt(         //회전된 이미지나 비스듬한 사각형 모양으로 그리고 싶을 때 쓰는 함수
    HDC      hdcDest,    // 그릴 대상 DC (보통 화면 백버퍼)
    const POINT* lpPoint, // 목적지의 세 꼭짓점 좌표 배열 (정확히 3개)
    HDC      hdcSrc,     // 원본 비트맵이 선택되어 있는 DC
    int      xSrc,       // 원본에서 가져올 영역의 시작 x
    int      ySrc,       // 원본에서 가져올 영역의 시작 y
    int      nWidth,     // 원본 영역의 너비
    int      nHeight,    // 원본 영역의 높이
    HBITMAP  hbmMask,    // 마스크 비트맵 (없으면 NULL)
    int      xMask,      // 마스크의 x 오프셋
    int      yMask       // 마스크의 y 오프셋
);



    BOOL GdiTransparentBlt(                          //특정 색상을 지정해서 그 색상만 투명하게 처리
	HDC  hdcDest,        // 그려질 대상 DC
	int  xoriginDest,     // 대상 사각형 좌상단 x
	int  yoriginDest,     // 대상 사각형 좌상단 y
	int  wDest,           // 대상 사각형 너비
	int  hDest,           // 대상 사각형 높이
	HDC  hdcSrc,          // 원본(소스) DC
	int  xoriginSrc,      // 원본 사각형 좌상단 x
	int  yoriginSrc,      // 원본 사각형 좌상단 y
	int  wSrc,            // 원본 사각형 너비
	int  hSrc,            // 원본 사각형 높이
	UINT crTransparent    // 투명 처리할 색상 (RGB 값)
);
	
	
	*/
}

void CMonster::Release()
{

}