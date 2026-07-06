#include "framework.h"
#include "Bitmap.h"

CMyBitmap::CMyBitmap()
{

}

CMyBitmap::~CMyBitmap()
{
	Release();
}

void CMyBitmap::Release()
{
	//SelectObject와 DeleteObject는 세트로 사용된다
	SelectObject(m_hMemDC, m_hOldBmp);                   // SelectObject  → DC에서 비트맵을 떼어내고 원래 것으로 복구  //DC에 객체를 끼우는 함수
	DeleteObject(m_hBitmap);                             // 떼어낸 비트맵 핸들 삭제  //GDI 객체 삭제


	DeleteDC(m_hMemDC);  //DC삭제

}

bool CMyBitmap::Load_Bmp(const TCHAR* _pFilePath)
{
	m_hBitmap = (HBITMAP)LoadImage(NULL, _pFilePath
		, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	if (NULL == m_hBitmap)
		return false;

	HDC hDC = GetDC(g_hWnd);
	m_hMemDC = CreateCompatibleDC(hDC); //기존 디시와 같은 똑같은 디시를 만들자
	ReleaseDC(g_hWnd, hDC); //get으로 빌려왔으면 release create로 //만들었으면 delete

	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	//기존 사용하던 gdi오브젝트를 다시 반환 받음

	//메세지 박스를 띄워서 실수를 막자

	return true;
}