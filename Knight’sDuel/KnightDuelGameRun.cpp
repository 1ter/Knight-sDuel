#include "KnightDuelManager.h" 
#include <Windows.h>
#include <ctime>

int main()
{
	Sleep(1000);
	srand(time(0));

	KnightDuelManager Game;
	Game.RunGame();

	return 0;
}


