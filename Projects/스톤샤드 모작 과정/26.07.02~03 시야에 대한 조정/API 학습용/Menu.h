#pragma once
#include "Scene.h"
class CMyMenu : public CScene
{
public:
	CMyMenu();
	virtual ~CMyMenu();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Late_Update();
	virtual void Render(HDC _DC);
	virtual void Release() override;

};

