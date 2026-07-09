#include "framework.h"
#include "Ghoul.h"
#include "BmpMgr.h"
#include "SoundMgr.h"
CGhoul::CGhoul()
{

}
CGhoul::CGhoul(GHOUL_SIZE _eSize)
	:m_eSize(_eSize)
{

}
CGhoul::~CGhoul()
{

}

void CGhoul::Initialize()
{
	Initialize(m_eSize);
}

void CGhoul::Initialize(GHOUL_SIZE _eSize)
{
	m_eSize = _eSize;

	m_tInfo.iCX = TILECX;
	m_tInfo.iCY = TILECY;

	if (m_eSize == GHOUL_SMALL)
	{
		CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Monster/Melee/ghoul_small.bmp"), _T("Ghoul_Small"));

		m_iSrcCX = 27;  //BMP의 크기
		m_iSrcCY = 35;  //BMP의 크기

		Set_GridPos(23,4 ); // 시작 칸 위치 //일단 임의로 잡아둔 값
		m_iHp = 35;
		m_iAtk = 8;
		m_iExp = 40;

		m_pImageKey = _T("Ghoul_Small");
	}
	else //GHOUL_LARGE
	{
		CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Monster/Melee/ghoul_large.bmp"), _T("Ghoul_Large"));

		m_iSrcCX = 32;  //BMP의 크기
		m_iSrcCY = 37;  //BMP의 크기

		Set_GridPos(30, 18); // 시작 칸 위치 //일단 임의로 잡아둔 값
		m_iHp = 70;
		m_iAtk = 18;
		m_iExp = 150;

		m_pImageKey = _T("Ghoul_Large");
	}

	m_eRenderID = RENDERID::OBJECT;
	m_bDead = false;
}

void CGhoul::Play_AlertSound()
{
	CSoundMgr::Get_Instance()->PlaySound(_T("snd_ghoul_alert_3.wav"), CSoundMgr::MONSTER);
}
void CGhoul::Play_DeathSound()
{
	CSoundMgr::Get_Instance()->PlaySound(_T("snd_ghoul_death_1.wav"), CSoundMgr::MONSTER);
}