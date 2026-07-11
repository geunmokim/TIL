#include "framework.h"
#include "AncientTroll.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "Sight.h"
#include "SoundMgr.h"
#include "ObjMgr.h"
#include "Font.h"
#include "NewPlayer.h"
#include "Effect.h"

CAncientTroll::CAncientTroll()
	:m_bIsAttacking(false), m_iAttackSrcCX(0), m_iAttackSrcCY(0)
	, m_bIsDying(false), m_iDeadSrcCX(0), m_iDeadSrcCY(0)
{
	ZeroMemory(&m_tAttackFrame, sizeof(m_tAttackFrame));
	ZeroMemory(&m_tDeadFrame, sizeof(m_tDeadFrame));
}
CAncientTroll::~CAncientTroll()
{

}

void CAncientTroll::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Monster/Melee/ancienttroll_sheet.bmp"), _T("AncientTroll"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Monster/Melee/ancienttroll_clawswipe_sheet.bmp"), _T("AncientTroll_Attack"));
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Monster/Melee/ancienttroll_dead_sheet.bmp"), _T("AncientTroll_Dead"));
	
	
	m_tInfo.iCX = TILECX;
	m_tInfo.iCY = TILECY;

	m_iSrcCX = 101;  //스프라이트 시트 1프레임당 크기
	m_iSrcCY = 105;  //스프라이트 시트 1프레임당 크기

	//idle
	m_tFrame.iStartX = 0;
	m_tFrame.iEndX = 20;
	m_tFrame.iStateY = 0;
	m_tFrame.dwDelayTime = 120;
	m_tFrame.dwTime = GetTickCount();

	//attack
	m_iAttackSrcCX = 118;
	m_iAttackSrcCY = 108;
	m_tAttackFrame.iStartX = 0;
	m_tAttackFrame.iEndX = 15;
	m_tAttackFrame.iStateY = 0;
	m_tAttackFrame.dwDelayTime = 60;   //idle보다 빠르게 -> 스와이프가 재빠르게 보이도록
	m_tAttackFrame.dwTime = GetTickCount();
	m_bIsAttacking = false;

	//dead
	m_iDeadSrcCX = 129;
	m_iDeadSrcCY = 125;
	m_tDeadFrame.iStartX = 0;
	m_tDeadFrame.iEndX = 18;
	m_tDeadFrame.iStateY = 0;
	m_tDeadFrame.dwDelayTime = 100;
	m_tDeadFrame.dwTime = GetTickCount();
	m_bIsDying = false;

	Set_GridPos(30, 40); // 하단-우측 보스룸, 시작 칸 위치 //일단 임의로 잡아둔 값
	m_iHp = 250;
	m_iAtk = 30;
	m_iExp = 500;

	m_pImageKey = _T("AncientTroll");
	m_eRenderID = RENDERID::OBJECT;
	m_bDead = false;
}

int CAncientTroll::Update()
{
	//사망 애니메이션이 모두 재생되고 나면 m_bDead = true로 사망 처리
	if (m_bIsDying)
	{
		Update_Rect();

		if (m_tDeadFrame.dwTime + m_tDeadFrame.dwDelayTime < GetTickCount())
		{
			++m_tDeadFrame.iStartX;
			m_tDeadFrame.dwTime = GetTickCount();

			if (m_tDeadFrame.iStartX >= m_tDeadFrame.iEndX)
			{
				m_bDead = true;
				return OBJ_DEAD;
			}
		}
		return OBJ_NOEVENT;
	}
	if (m_bDead)
		return OBJ_DEAD;
	Update_Rect();

	if (m_bIsAttacking)
	{
		if (m_tAttackFrame.dwTime + m_tAttackFrame.dwDelayTime < GetTickCount())
		{
			++m_tAttackFrame.iStartX;
			m_tAttackFrame.dwTime = GetTickCount();

			if (m_tAttackFrame.iStartX >= m_tAttackFrame.iEndX)
				m_bIsAttacking = false;   //마지막 프레임까지 재생 끝 -> idle로 복귀
		}
	}
	else
	{
		Update_Frame();   //CObj의 프레임 반복재생 로직 재사용 -> idle 애니메이션
	}

	return OBJ_NOEVENT;
}

void CAncientTroll::Render(HDC _DC)
{

	if (CSight::Get_Instance()->Get_Visibility(m_tGridPos.row, m_tGridPos.col) != VISIBLE)
		return;

	const TCHAR* pImageKey;
	int iCurSrcCX, iCurSrcCY, iCurFrameX;

	if (m_bIsDying)
	{
		pImageKey = _T("AncientTroll_Dead");
		iCurSrcCX = m_iDeadSrcCX;
		iCurSrcCY = m_iDeadSrcCY;
		iCurFrameX = m_tDeadFrame.iStartX;
	}
	else if (m_bIsAttacking)
	{
		pImageKey = _T("AncientTroll_Attack");
		iCurSrcCX = m_iAttackSrcCX;
		iCurSrcCY = m_iAttackSrcCY;
		iCurFrameX = m_tAttackFrame.iStartX;
	}
	else
	{
		pImageKey = _T("AncientTroll");
		iCurSrcCX = m_iSrcCX;
		iCurSrcCY = m_iSrcCY;
		iCurFrameX = m_tFrame.iStartX;
	}

	HDC hMonsterDC = CBmpMgr::Get_Instance()->Find_Image(pImageKey);

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ShakeScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ShakeScrollY();

	int iDestX = m_tRect.left + iScrollX;
	int iDestY = m_tRect.top + iScrollY;

	int iSrcX = iCurFrameX * iCurSrcCX;

	if (m_bFlipX)
	{
		HDC hTempDC = CreateCompatibleDC(_DC);
		HBITMAP hTempBmp = CreateCompatibleBitmap(_DC, iCurSrcCX, iCurSrcCY);
		HBITMAP hOldBmp = (HBITMAP)SelectObject(hTempDC, hTempBmp);

		RECT rc = { 0, 0, m_iSrcCX, m_iSrcCY };
		FillRect(hTempDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

		StretchBlt(hTempDC, m_iSrcCX - 1, 0, -m_iSrcCX, m_iSrcCY,
			hMonsterDC, iSrcX, 0, m_iSrcCX, m_iSrcCY, SRCCOPY);

		GdiTransparentBlt(_DC,
			iDestX, iDestY, m_iSrcCX, m_iSrcCY,
			hTempDC, 0, 0, m_iSrcCX, m_iSrcCY,
			RGB(255, 255, 255));

		SelectObject(hTempDC, hOldBmp);
		DeleteObject(hTempBmp);
		DeleteDC(hTempDC);
	}
	else
	{
		GdiTransparentBlt(_DC,
			iDestX, iDestY, m_iSrcCX, m_iSrcCY,
			hMonsterDC, iSrcX, 0, m_iSrcCX, m_iSrcCY,
			RGB(255, 255, 255));
	}
}

void CAncientTroll::Attack(CObj* _pTarget)
{
	
	CMonster_Melee::Attack(_pTarget);
	CScrollMgr::Get_Instance()->Start_Shake(6.f, 200);

	//공격 시 공격 애니메이션 재생 후 다시 idle로  
	m_bIsAttacking = true;
	m_tAttackFrame.iStartX = 0;
	m_tAttackFrame.dwTime = GetTickCount();

}

void CAncientTroll::Play_AttackSound()
{
	CSoundMgr::Get_Instance()->PlaySound(_T("snd_troll_boss_troll_regeneration_cast.wav"), CSoundMgr::MONSTER);
}

void CAncientTroll::Play_AlertSound()
{
	CSoundMgr::Get_Instance()->PlaySound(_T("snd_troll_boss_alert.wav"), CSoundMgr::MONSTER);

	//보스 발견 시 평상시 bgm을 끄고 보스전 bgm으로 전환
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
	CSoundMgr::Get_Instance()->PlayBGM(_T("snd_prologue_boss_battle.ogg"));
}

void CAncientTroll::Play_DeathSound()
{
	CSoundMgr::Get_Instance()->PlaySound(_T("snd_troll_boss_death.wav"), CSoundMgr::MONSTER);
}

void CAncientTroll::On_Turn()
{
	//사망 애니메이션 재생 중이면 턴 행동(추격/공격) 하지 않음
	if (m_bIsDying)
		return;

	CMonster_Melee::On_Turn();
}
void CAncientTroll::Take_Damage(int _dmg)
{
	//사망 애니메이션 재생 중이면 데미지 안받게끔
	if (m_bIsDying)
		return;
	m_iHp -= _dmg;

	CFont* pFont = new CFont(_dmg, m_tGridPos);
	pFont->Initialize();
	CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pFont);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::EFFECT, pFont);

	CEffect* pEffect = new CEffect(EFFECT_ATTACK);
	pEffect->Set_GridPos(m_tGridPos.row, m_tGridPos.col);
	pEffect->Initialize();
	CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);
	CObjMgr::Get_Instance()->Add_Object(RENDERID::EFFECT, pEffect);

	//보스가 맞을 때도 화면 흔들림(공격받을 때보다 약하게)
	CScrollMgr::Get_Instance()->Start_Shake(4.f, 150);

	if (m_iHp <= 0)
	{
		//바로 죽이지 않고 "죽어가는 중" 상태로 전환 -> 사망 애니메이션 다 재생된 뒤에 진짜로 죽음
		m_bIsDying = true;
		m_tDeadFrame.iStartX = 0;
		m_tDeadFrame.dwTime = GetTickCount();

		Play_DeathSound();

		CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
		if (pPlayer)
			((CNewPlayer*)pPlayer)->Add_Exp(Get_Exp());

		//보스전 종료(사망) 시 평상시 BGM으로 복귀
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BGM);
		CSoundMgr::Get_Instance()->PlayBGM(_T("snd_battle_theme.ogg"));
	}
}