#pragma once
#include "Scene.h"

class CMyEdit : public CScene
{
public:
	CMyEdit();
	virtual ~CMyEdit();
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

