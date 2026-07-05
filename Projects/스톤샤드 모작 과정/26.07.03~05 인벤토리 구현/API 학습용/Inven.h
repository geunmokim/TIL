#pragma once
#include "Item.h"
class CInven
{
private:
	CInven();
	~CInven();

public:
	void Initialize();    //인벤 창크기, 격자정보 등등
	void Update();        //아이템 옮기면 반영하도록
	void Render(HDC _DC);
	void Release();


	
public:
	static CInven*Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CInven;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	void On_MouseDown();
	void On_MouseDrag();
	void On_MouseUp();



private:
	void Key_Check();
	void Place_Item(CItem* _pItem, int _iGridX, int _iGridY);
	void Remove_Item(CItem* _pDragItem); //격자에서 아이템 제거
	bool Can_Place(CItem* _pItem, int _iGridX, int _iGridY);
	void Equip_Item(CItem* _pItem, ITEM_TYPE _eType);
	
private:
	//창 상태
	bool m_bOpen;

	bool Is_In_Rect(const RECT& _tRect, const POINT& _pt)
	{
		return _pt.x >= _tRect.left && _pt.x <= _tRect.right
			&& _pt.y >= _tRect.top && _pt.y <= _tRect.bottom;

	}

	CItem* m_Grid[INVEN_ROWS][INVEN_COLS];
	//헤더에서 선언과 동시에 초기화 하고 싶으면 CItem* m_Grid[INVEN_ROWS][INVEN_COLS]{};

	//격자 정보
	int m_iCellSize;
	int m_iGridCols;
	int m_iGridRows;
	int m_iGridStartX;
	int m_iGridStartY;


	

	int m_iDragOffsetX; //클릭한 위치와 아이템 좌상단의 차이 
	int m_iDragOffsetY; 

	int m_iOriginGridX; //드래그 시작 전 원래 위치(놓기 실패 시 복귀용)
	int m_iOriginGridY;


	RECT m_tInvenRect;  //인벤토리 창,크기 위치

	//장비 슬롯
	RECT m_tWeaponSlot;
	RECT m_tArmorSlot;
	RECT m_tHatSlot;

	ITEM_INFO m_tInfo;
	//마우스의 위치 가져오기
	POINT m_tMousePos;

	CItem* m_pDragItem; //드래그 중인 아이템
	CItem* m_pWeaponItem;   // 장착된 무기
	CItem* m_pArmorItem;    // 장착된 갑옷
	CItem* m_pHatItem;      // 장착된 모자

	static CInven* m_pInstance;

};

