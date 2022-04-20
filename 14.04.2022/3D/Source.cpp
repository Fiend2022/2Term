#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
using namespace std;

#include <stdio.h>
#include <Windows.h>
#include "SpatialAnalysis.h"


int main()
{
	player Player;
	Coordinates PointRayEnd;
	Player.PlayerInit();
	wchar_t* Screen = new wchar_t[ScreenHeight * ScreenWidth + 1];
	HANDLE Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(Console);
	wstring Map = MapInit();
	auto Start = chrono::system_clock::now(); 
	auto End= chrono::system_clock::now(); 
	DWORD BytesWritten;
	while (1)
	{
		End = chrono::system_clock::now();
		chrono::duration<float> Time = End - Start;
		Start = End;
		float ElapsedTime = Time.count();
		PlayerMove(&Player, Map, ElapsedTime);
		MainLoop(&Player, Screen, Map, &PointRayEnd, ElapsedTime);
		Screen[ScreenWidth * ScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(Console, Screen, ScreenWidth * ScreenHeight, { 0,0 }, &BytesWritten);
	}
	return 0;
}
