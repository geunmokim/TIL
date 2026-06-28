#pragma once
class CLine
{
public:
	CLine();
	CLine(LINEPOS& _tLeftPos, LINEPOS& _tRightPos);
	~CLine();

public:
	void Render(HDC _DC);
public:
	const LINEINFO& Get_Info() const { return m_tInfo; }

private:
	LINEINFO m_tInfo;
};

