#pragma once
class CMainEditor
{
public:
	CMainEditor();
	~CMainEditor();

public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render();
	void Release();

private:
	HDC m_hDC;

};

