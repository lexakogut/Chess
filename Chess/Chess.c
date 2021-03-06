#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <time.h>
#include "Game.h"
#include <Windows.h>

Game *game;

int main()
{
	HANDLE cHandle = GetStdHandle(0);
	SetConsoleMode(cHandle, ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	game = StartGameFromPosition(START_POSITION);
	PlayGame(game);
	return 0;
}