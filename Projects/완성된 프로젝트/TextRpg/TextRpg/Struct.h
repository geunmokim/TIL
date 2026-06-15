#pragma once


#ifndef __STRUCT_H__  //헤더파일 중복 방지용으로 사용하나 요새 c++에서는 #pragma once만 써도 된다고함
#define __STRUCT_H__

typedef struct tagInfo
{
	tagInfo() {}
	tagInfo(const char* _pName, int _iAtt, int _iMaxHp)
		:iAtt(_iAtt), iMaxHp(_iMaxHp), iHp(_iMaxHp), iLevel(1)
		, iMaxExp(100), iExp(0), iGold(0)
	{
		strcpy_s(szName, sizeof(szName), _pName);
	}
	tagInfo(const char* _pName, int _iAtt, int _iMaxHp, int _iLevel)
		:iAtt(_iAtt), iMaxHp(_iMaxHp), iHp(_iMaxHp), iLevel(_iLevel)
		, iMaxExp(0), iExp(0), iGold(0)
	{
		strcpy_s(szName, sizeof(szName), _pName);

	}

	tagInfo(const char* _pName, int _iAtt, int _iMaxHp, int _iLevel, int _iGold)   //객체를 생성하는 경우가 여러가지라서 오버로딩함
		: iAtt(_iAtt), iMaxHp(_iMaxHp), iHp(_iMaxHp), iLevel(_iLevel)
		, iMaxExp(0), iExp(0), iGold(_iGold)
	{
		strcpy_s(szName, sizeof(szName), _pName);
	}

	char szName[16];
	int  iAtt;
	int iMaxHp;
	int iHp;
	int iLevel;
	int iMaxExp;
	int iExp;
	int iGold;

}INFO;





//#define SAFE_DELETE(p) if(p) {delete p; p = nullptr;}        //메모리를 생성하고 해제하는것이기 떄문에 구조체를 만드는 struct에서는 필요가 없음
//#define SAFE_DELETE_ARR(p) if(p) {delete[] p; p = nullptr;}

#endif