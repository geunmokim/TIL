#pragma once
#include "Obj.h"

enum PROP_TYPE
{
	PROP_BASTIONTABLE03,
	PROP_BASTIONTABLE04,
	PROP_PAWNSHOP_0,
	PROP_PAWNSHOP_1,
	PROP_PAWNSHOP_2,
	PROP_PAWNSHOP_3,
	PROP_PAWNSHOP_4,
	PROP_PAWNSHOP_5,
	PROP_TYPE_END
};

class CProp : public CObj
{
public:
	CProp();
	CProp(PROP_TYPE _eType);
	virtual ~CProp();

public:
	virtual void Initialize() override;
	void Initialize(PROP_TYPE _eType);
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	PROP_TYPE m_eType;
	int m_iSrcCX;
	int m_iSrcCY;

	int m_iDestCX;
	int m_iDestCY;
};