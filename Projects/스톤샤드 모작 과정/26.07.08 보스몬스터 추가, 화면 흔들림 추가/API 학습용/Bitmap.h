#pragma once
class CMyBitmap
{
public:
	CMyBitmap();
	~CMyBitmap();

public:
	void Release();

public:
	bool Load_Bmp(const TCHAR* _pFilePath);
	HDC Get_DC() { return m_hMemDC; }

private:
	HDC m_hMemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBmp;
};

