#pragma once

class CItem 
{
public:
	CItem();
	CItem(const ITEM_INFO& _tInfo);
    ~CItem();

public:
	void Initialize();
	void Render(HDC _DC,int _iPixelX, int _iPixelY);
	void Release();

public:
	ITEM_TYPE Get_Type() { return m_eType; }
	int Get_SrcWidth() { return m_iSrcWidth; }
	int Get_SrcHeight() { return m_iSrcHeight; }
	int Get_Hp() { return m_iHp; }
	int Get_Def() { return m_iDef; }
	int Get_Atk() { return m_iAtk; }

	//슬롯에서 아이템 원본 크기를 사용하기 위해 //이걸 안쓰고 인벤에서의 아이템의 크기를 사용하면 무기슬롯에서 슬롯 위쪽으로 장착됨
	void Render_Original(HDC _DC, int _iPixelX, int _iPixelY);


public:
	int Get_GridX() { return m_iGridX; }
	int Get_GridY() { return m_iGridY; }
	int Get_Width() { return m_iWidth; }
	int Get_Height() { return m_iHeight; }
	void Set_GridPos(int _x, int _y) { m_iGridX = _x; m_iGridY = _y; }

private:
	int m_iWidth; //가로 몇 칸
	int m_iHeight; //세로 몇 칸

	//현재 인벤토리 격자 내 위치
	int m_iGridX; //몇 열
	int m_iGridY; //몇 행

	//아이템 스탯
	int m_iHp;
	int m_iDef;
	int m_iAtk;

	//헤더에 원본 스프라이트 크기 추가
	int m_iSrcWidth;  //원본 픽셀 가로
	int m_iSrcHeight; //원본 픽셀 세로 


	//아이템 종류
	ITEM_TYPE m_eType;
	const TCHAR* m_pImageKey;
	const TCHAR* m_pPath;


};

