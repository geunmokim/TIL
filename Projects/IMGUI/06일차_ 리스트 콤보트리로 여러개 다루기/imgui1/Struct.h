#pragma once

struct MonsterData
{
	char Name[32] = "Ghoul";
	int Hp = 80;
	float Atk = 12.5f;
	bool bLarge = false;
	float Color[3] = { 0.6f, 0.2f, 0.2f };
};