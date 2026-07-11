#include "framework.h"
#include "Bandit_Cleaver.h"
#include "BmpMgr.h"

CBandit_Cleaver::CBandit_Cleaver()
{

}
CBandit_Cleaver::~CBandit_Cleaver()
{

}

void CBandit_Cleaver::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(_T("../Image/Monster/Melee/s_bandit_goon_cleaver01_0.bmp"), _T("Bandit_Cleaver"));

	m_tInfo.iCX = TILECX;
	m_tInfo.iCY = TILECY;

	m_iSrcCX = 28;  //BMP의 크기
	m_iSrcCY = 34;  //BMP의 크기

	Set_GridPos(8, 8); // 시작 칸 위치 //일단 임의로 잡아둔 값
	m_iHp = 45;
	m_iAtk = 10;
	m_iExp = 100;

	m_pImageKey = _T("Bandit_Cleaver");
	m_eRenderID = RENDERID::OBJECT;
	m_bDead = false;
}
