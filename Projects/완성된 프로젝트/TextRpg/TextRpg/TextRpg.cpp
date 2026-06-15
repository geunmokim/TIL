#include "stdafx.h"
#include "MainGame.h"

int main()
{
	srand(unsigned(time(NULL)));

	CMainGame mainGame;

	if (mainGame.Initialize())
		mainGame.Progress();

	return 0;
}