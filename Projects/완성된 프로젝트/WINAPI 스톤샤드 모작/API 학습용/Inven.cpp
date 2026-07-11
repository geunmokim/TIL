#include "framework.h"
#include "Inven.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "Item.h"
#include "ObjMgr.h"
#include "NewPlayer.h"
#include "SoundMgr.h"


CInven* CInven::m_pInstance = nullptr;
CInven::CInven()
	:m_iCellSize(0), m_iGridCols(0), m_iGridRows(0), m_bOpen(false), m_iGridStartX(0), m_iGridStartY(0), m_iDragOffsetX(0)
	, m_iDragOffsetY(0), m_iOriginGridX(0), m_iOriginGridY(0), m_pDragItem(nullptr), m_pWeaponItem(nullptr), m_pArmorItem(nullptr),m_pHatItem(nullptr)
{
	memset(m_Grid, 0, sizeof(m_Grid));
	ZeroMemory(&m_tInvenRect, sizeof(m_tInvenRect));
	ZeroMemory(&m_tArmorSlot, sizeof(m_tArmorSlot));
	ZeroMemory(&m_tWeaponSlot, sizeof(m_tWeaponSlot));
	ZeroMemory(&m_tHatSlot, sizeof(m_tHatSlot));
	ZeroMemory(&m_tMousePos, sizeof(m_tMousePos));
	
	

}
CInven::~CInven()
{
	
}

void CInven::Initialize()
{
	//장비 장착하고 사망시 재시작했을때 장비가 장착슬롯에 장착되어있는 현상 해결용
	for (int r = 0; r < INVEN_ROWS; ++r)
	{
		for (int c = 0; c < INVEN_COLS; ++c)
		{
			if (m_Grid[r][c])
			{
				CItem* pItem = m_Grid[r][c];
				//같은 아이템이 여러 칸을 차지할 수 있으므로, 그 아이템이 있는 칸을 전부 nullptr로 비운 뒤 한 번만 delete
				for (int r2 = 0; r2 < INVEN_ROWS; ++r2)
					for (int c2 = 0; c2 < INVEN_COLS; ++c2)
						if (m_Grid[r2][c2] == pItem)
							m_Grid[r2][c2] = nullptr;
				SAFE_DELETE(pItem);
			}
		}
	}
	SAFE_DELETE(m_pWeaponItem);
	SAFE_DELETE(m_pArmorItem);
	SAFE_DELETE(m_pHatItem);
	m_pDragItem = nullptr;
	m_bOpen = false;


	m_iCellSize = 27;   // 셀 크기
	m_iGridCols = 10;    // 가로 9칸
	m_iGridRows = 5;    // 세로 7칸
	m_iGridStartX = 24;  // 격자 시작 X 오프셋
	m_iGridStartY = 169; // 격자 시작 Y 오프셋
	
	//Image 경로 출력
	m_tInvenRect = { 480,50,800,410 }; //인벤 창 위치 //나중에 조정할것
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Inventory.bmp"), _T("Inventory"));

	ITEM_INFO tWeaponInfo = { 1, 3, 15, 69, ITEM_WEAPON,0,0, 10, _T("../Image/Item/weapon.bmp"), _T("Weapon") };
	CItem* Cleaver = new CItem(tWeaponInfo);
	Cleaver->Initialize();
	Place_Item(Cleaver, 0, 0); //0열 0행에 자동으로 1x3크기로 채움

	ITEM_INFO tArmorInfo = { 2, 3, 50, 75, ITEM_ARMOR,10,10,0,_T("../Image/Item/gambeson.bmp"), _T("Gambeson") };
	CItem* Gambeson = new CItem(tArmorInfo);
	Gambeson->Initialize();
	Place_Item(Gambeson, 2, 0);

	ITEM_INFO tHatInfo = { 2, 2, 49, 41, ITEM_HAT, 5, 5, 0, _T("../Image/Item/hat.bmp"), _T("Hat") };
	CItem* Hat = new CItem(tHatInfo);
	Hat->Initialize();
	Place_Item(Hat, 5, 0);


	// 장비 슬롯 위치 (인벤토리 창 기준 오프셋)
	 // 무기 슬롯: 1x3칸 = 27x81px
	m_tWeaponSlot = {
		m_tInvenRect.left + 24,
		m_tInvenRect.top + 14,
		m_tInvenRect.left + 24 + 54,
		m_tInvenRect.top + 14 + 135
	};

	// 갑옷 슬롯: 2x3칸 = 54x81px
	m_tArmorSlot = {
		m_tInvenRect.left + 78,
		m_tInvenRect.top + 14,
		m_tInvenRect.left + 78 + 54,
		m_tInvenRect.top + 14 + 81
	};

	// 모자 슬롯: 2x2칸 = 54x54px
	m_tHatSlot = {
		m_tInvenRect.left + 187,
		m_tInvenRect.top + 14,
		m_tInvenRect.left + 187 + 54,
		m_tInvenRect.top + 14 + 58
	};
	
}


void CInven::Update()
{
	Key_Check();
	
}

void CInven::Render(HDC _DC)
{
	if (!m_bOpen)
		return;

	
	HDC hInvenDC = CBmpMgr::Get_Instance()->Find_Image(_T("Inventory"));
	BitBlt(_DC, m_tInvenRect.left
		, m_tInvenRect.top
		, 320, 360, hInvenDC
		, 0, 0, SRCCOPY);
	
	for (int r = 0; r < INVEN_ROWS; ++r)
	{
		for (int c = 0; c < INVEN_COLS; ++c)
		{
			if (m_Grid[r][c] == nullptr)//칸이 비어있을떄
				continue;

			
			
			// 이 칸이 아이템의 좌상단인지 체크
			if (m_Grid[r][c]->Get_GridY() != r || m_Grid[r][c]->Get_GridX() != c)
				continue;

			/*
			원하는건
			cleaver 하나가 세개로 쪼개져서 상단,중단,하단의 모습을 그려주길 원하는데
			좌상단 체크 안하면 그냥 cleaver전체 x3으로 그려짐
			좌상단을 기준으로 잡은 이유는 아이템이 차지하는 영역의 시작점이 좌상단이고
			GdiTransparentBlt도 그리는 시작점을 좌상단 기준으로 잡기떄문
			*/


			// 격자 칸 번호 → 실제 픽셀 위치 계산
			int iPixelX = m_tInvenRect.left + m_iGridStartX + c * 27;
			//m_tInvenRect.left -> 인벤토리 창 자체가 화면 어디서 시작하는지
			//m_iGridStartX -> 창 안에서 격자가 시작하는 오프셋
			//c*27 ->몇번째 열인지 * 셀 크기                                   
			int iPixelY = m_tInvenRect.top + m_iGridStartY + r * 27;

			m_Grid[r][c]->Render(_DC, iPixelX, iPixelY);
		}
	}


	if (m_pDragItem != nullptr)
	{
		m_pDragItem->Render(_DC, m_tMousePos.x - m_iDragOffsetX,
			                     m_tMousePos.y - m_iDragOffsetY);
		/*
		m_tMousePos.x에서 Offset을 뺴는 이유는
		오프셋 없이 그냥 마우스 위치에 그리면 아이템 좌상단이 마우스 커서에 딱붙어서 그려진다
		클릭한 위치에 따라 아이템이 위로 튀어오르는것처럼 보일 수 있다

		
		*/
	}
	// 장비 슬롯 렌더링
	if (m_pWeaponItem != nullptr)
	{
		int iSlotW = m_tWeaponSlot.right - m_tWeaponSlot.left;
		int iSlotH = m_tWeaponSlot.bottom - m_tWeaponSlot.top;

		int iOffsetX = (iSlotW - m_pWeaponItem->Get_SrcWidth()) / 2;
		int iOffsetY = (iSlotH - m_pWeaponItem->Get_SrcHeight()) / 2;
		m_pWeaponItem->Render_Original(_DC, m_tWeaponSlot.left + iOffsetX, m_tWeaponSlot.top + iOffsetY);
	}

	if (m_pArmorItem != nullptr)
	{
		int iSlotW = m_tArmorSlot.right - m_tArmorSlot.left;
		int iSlotH = m_tArmorSlot.bottom - m_tArmorSlot.top;

		int iOffsetX = (iSlotW - m_pArmorItem->Get_SrcWidth()) / 2;
		int iOffsetY = (iSlotH - m_pArmorItem->Get_SrcHeight()) / 2;
		m_pArmorItem->Render_Original(_DC, m_tArmorSlot.left + iOffsetX, m_tArmorSlot.top + iOffsetY);
	}

	if (m_pHatItem != nullptr)
	{
		int iSlotW = m_tHatSlot.right - m_tHatSlot.left;
		int iSlotH = m_tHatSlot.bottom - m_tHatSlot.top;

		int iOffsetX = (iSlotW - m_pHatItem->Get_SrcWidth()) / 2;
		int iOffsetY = (iSlotH - m_pHatItem->Get_SrcHeight()) / 2;
		m_pHatItem->Render_Original(_DC, m_tHatSlot.left + iOffsetX, m_tHatSlot.top + iOffsetY);
	}
	
	




}
void CInven::Key_Check()
{
	if (CKeyMgr::Get_Instance()->Key_Down('I'))
		m_bOpen = !m_bOpen;  //true/false 토글

	if (!m_bOpen)
		return;


	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON) && m_pDragItem == nullptr)
		On_MouseDown(); //집기

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
		On_MouseDrag();   // 드래그 중

	if (CKeyMgr::Get_Instance()->Key_Up(VK_LBUTTON))
		On_MouseUp();     // 놓기
	
		

}


void CInven::Place_Item(CItem* _pItem, int _iGridX, int _iGridY)
{

	//아이템 크기만큼 격자 칸을 채움
	for (int r = _iGridY; r < _iGridY + _pItem->Get_Height(); ++r)   //아이템의 세로크기만큼 반복
		for (int c = _iGridX; c < _iGridX + _pItem->Get_Width(); ++c)//아이템의 가로크기 만큼 반복
			m_Grid[r][c] = _pItem;

	//아이템에 자신의 격자 위치 저장
	_pItem->Set_GridPos(_iGridX, _iGridY);
}

//해당칸을 채우는 place_item의 반대버전 
void CInven::Remove_Item(CItem* _pItem)
{
	int iGridX = _pItem->Get_GridX();
	int iGridY = _pItem->Get_GridY();

	for (int r = iGridY; r < iGridY + _pItem->Get_Height(); ++r)
		for (int c = iGridX; c < iGridX + _pItem->Get_Width(); ++c)
			m_Grid[r][c] = nullptr; //해당칸을 비운다
}


void CInven::On_MouseDown()
{
	if (m_pDragItem != nullptr)  // 이미 드래그 중이면 무시
		return;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	// 장비 슬롯 클릭 체크
	if (Is_In_Rect(m_tWeaponSlot, pt) && m_pWeaponItem != nullptr)
	{
		m_pDragItem = m_pWeaponItem;
		m_iOriginGridX = -1;  // 슬롯에서 나온 아이템임을 표시 //아이템이 격자 밖이면 (음수) 원래 아이템 위치로 돌아가는 매커니즘 이용
		m_iOriginGridY = -1;
		m_pWeaponItem = nullptr;

		// 스탯 제거
		CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
		if (pPlayer)
		{
			CNewPlayer* pNewPlayer = (CNewPlayer*)pPlayer;
			pNewPlayer->Add_Atk(-m_pDragItem->Get_Atk());
			pNewPlayer->Add_Def(-m_pDragItem->Get_Def());
		}
		m_iDragOffsetX = pt.x - m_tWeaponSlot.left;
		m_iDragOffsetY = pt.y - m_tWeaponSlot.top;
		Play_EquipSound(ITEM_WEAPON); //무기 슬롯에서 드래그 시작 시 사운드
		return;
	}
	else if (Is_In_Rect(m_tArmorSlot, pt) && m_pArmorItem != nullptr)
	{
		m_pDragItem = m_pArmorItem;
		m_iOriginGridX = -1;
		m_iOriginGridY = -1;
		m_pArmorItem = nullptr;
		CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
		if (pPlayer)
		{
			CNewPlayer* pNewPlayer = (CNewPlayer*)pPlayer;
			pNewPlayer->Add_Atk(-m_pDragItem->Get_Atk());
			pNewPlayer->Add_Def(-m_pDragItem->Get_Def());
		}
		m_iDragOffsetX = pt.x - m_tArmorSlot.left;
		m_iDragOffsetY = pt.y - m_tArmorSlot.top;
		Play_EquipSound(ITEM_ARMOR); //갑옷 슬롯에서 드래그 시작 시 사운드
		return;
	}
	else if (Is_In_Rect(m_tHatSlot, pt) && m_pHatItem != nullptr)
	{
		m_pDragItem = m_pHatItem;
		m_iOriginGridX = -1;
		m_iOriginGridY = -1;
		m_pHatItem = nullptr;
		CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
		if (pPlayer)
		{
			CNewPlayer* pNewPlayer = (CNewPlayer*)pPlayer;
			pNewPlayer->Add_Atk(-m_pDragItem->Get_Atk());
			pNewPlayer->Add_Def(-m_pDragItem->Get_Def());
		}
		m_iDragOffsetX = pt.x - m_tHatSlot.left;
		m_iDragOffsetY = pt.y - m_tHatSlot.top;
		Play_EquipSound(ITEM_HAT);
		return;
	}
	
	// 픽셀 좌표 → 격자 칸 번호
	int c = (pt.x - m_tInvenRect.left - m_iGridStartX) / 27;
	int r = (pt.y - m_tInvenRect.top - m_iGridStartY) / 27;

	// 격자 범위 밖이면 무시
	if (c < 0 || c >= INVEN_COLS || r < 0 || r >= INVEN_ROWS)
		return;

	// 해당 칸에 아이템 있으면 집기
	if (m_Grid[r][c] == nullptr)
		return;

	m_pDragItem = m_Grid[r][c];
	m_iOriginGridX = m_pDragItem->Get_GridX();
	m_iOriginGridY = m_pDragItem->Get_GridY();

	// 아이템 좌상단과 클릭 위치의 픽셀 차이 저장
	m_iDragOffsetX = pt.x - (m_tInvenRect.left + m_iGridStartX + m_iOriginGridX * 27);
	m_iDragOffsetY = pt.y - (m_tInvenRect.top + m_iGridStartY + m_iOriginGridY * 27);

	// 격자에서 아이템 제거 (드래그 중이므로)
	Remove_Item(m_pDragItem);

	//격자에서 드래그 시작시 드래그 중인 아이템 종류에 따른 사운드 재생
	Play_EquipSound(m_pDragItem->Get_Type());
}

void CInven::On_MouseDrag()
{
	if (m_pDragItem == nullptr)
		return;

	// 마우스 위치 가져오기 (렌더링에서 사용할 용도)
	GetCursorPos(&m_tMousePos);
	ScreenToClient(g_hWnd, &m_tMousePos);
}

void CInven::On_MouseUp()
{

	if (m_pDragItem == nullptr)
		return;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	// 장비 슬롯 체크
	if (Is_In_Rect(m_tWeaponSlot, pt) && m_pDragItem->Get_Type() == ITEM_WEAPON)
	{
		Equip_Item(m_pDragItem, ITEM_WEAPON);
		m_pDragItem = nullptr;
		return;
	}
	else if (Is_In_Rect(m_tArmorSlot, pt) && m_pDragItem->Get_Type() == ITEM_ARMOR)
	{
		Equip_Item(m_pDragItem, ITEM_ARMOR);
		m_pDragItem = nullptr;
		return;
	}
	else if (Is_In_Rect(m_tHatSlot, pt) && m_pDragItem->Get_Type() == ITEM_HAT)
	{
		Equip_Item(m_pDragItem, ITEM_HAT);
		m_pDragItem = nullptr;
		return;
	}

	int c = (pt.x - m_tInvenRect.left - m_iGridStartX) / 27;
	int r = (pt.y - m_tInvenRect.top - m_iGridStartY) / 27;



	

	if (Can_Place(m_pDragItem, c, r)) //인벤토리 안이면서 기존 다른 아이템과 충돌하지 않았으면
	{
		Place_Item(m_pDragItem, c, r);
	}
	else
	{
		if (m_iOriginGridX == -1)
			Equip_Item(m_pDragItem, m_pDragItem->Get_Type());
		else
			Place_Item(m_pDragItem, m_iOriginGridX, m_iOriginGridY);
	}
		

	m_pDragItem = nullptr; //위에서 어떤 경우든 아이템은 Place_Item으로 해당 좌표에 들어가게 되므로 지워준다

	
	

}

bool CInven::Can_Place(CItem* _pItem, int _iGridX, int _iGridY)
{
	if (_iGridX < 0 || _iGridY < 0)
		return false;
	//인벤토리 범위 안인지 밖인지 체크
	if (_iGridX + _pItem->Get_Width() > INVEN_COLS)
		return false;
	if (_iGridY + _pItem->Get_Height() > INVEN_ROWS)
		return false;

	// 기존 위치에 아이템이 있는지 충돌 체크
	for (int r = _iGridY; r < _iGridY + _pItem->Get_Height(); ++r)
		for (int c = _iGridX; c < _iGridX + _pItem->Get_Width(); ++c)
			if (m_Grid[r][c] != nullptr)
				return false;

	return true;
}

void CInven::Equip_Item(CItem* _pItem, ITEM_TYPE _eType)
{
	CItem** pSlot = nullptr;
	/*
	CItem* pSlot = m_pWeaponItem로 하면 포인터 값의 복사본이라서
	*pSlot = _pItem을 해도 실제 m_pWeaponItem이 안바뀐다
	*/

	switch (_eType)
	{
	case ITEM_WEAPON:
		pSlot =&m_pWeaponItem;
		break;
	case ITEM_ARMOR:
		pSlot = &m_pArmorItem;
		break;
	case ITEM_HAT:
		pSlot = &m_pHatItem;
		break;
	}

	if (pSlot == nullptr)
		return;

	//슬롯에 이미 장착된 아이템이 있다면
	if (*pSlot != nullptr)
	{
		CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
		if (pPlayer)
		{
			CNewPlayer* pNewPlayer = (CNewPlayer*)pPlayer;
			pNewPlayer->Add_Atk(-(*pSlot)->Get_Atk());
			pNewPlayer->Add_Def(-(*pSlot)->Get_Def());
		}
		//원래 있던 자리에 기존 아이템을 돌려놓는다
		Place_Item(*pSlot, m_iOriginGridX, m_iOriginGridY);
		*pSlot = nullptr;
	}
	//장착된 아이템이 없다면 
	*pSlot = _pItem; //새 아이템을 장착

	//아이템을 장착시 장착 완료 사운드
	Play_EquipSound(_eType);

	//장착한 아이템 만큼 플레이어 스텟에 +
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
	if (pPlayer)
	{
		CNewPlayer* pNewPlayer = (CNewPlayer*)pPlayer;
		pNewPlayer->Add_Atk(_pItem->Get_Atk());
		pNewPlayer->Add_Def(_pItem->Get_Def());

	}



}


void CInven::Release()
{

}

void CInven::Play_EquipSound(ITEM_TYPE _eType)
{
	if (_eType == ITEM_WEAPON)
		CSoundMgr::Get_Instance()->PlaySound(_T("snd_weapon_piercing_impact_3.wav"), CSoundMgr::UI);
	else //ITEM_ARMOR, ITEM_HAT
		CSoundMgr::Get_Instance()->PlaySound(_T("snd_out_cloth_armor_equip_footstep_6.wav"), CSoundMgr::UI);
}