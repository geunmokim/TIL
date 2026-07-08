#pragma once
#include "Obj.h"
enum EFFECT_TYPE { EFFECT_ATTACK, EFFECT_HIT,EFFECT_END };

class CEffect : public CObj
{
public:
	CEffect();
	CEffect(EFFECT_TYPE _eType);
	virtual ~CEffect();

public:
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update()override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	EFFECT_TYPE m_eEffectType;

	int m_iFrameW;
	int m_iFrameH;
};

