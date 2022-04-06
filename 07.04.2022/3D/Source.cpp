#include <iostream>
#include <chrono>
#include <Windows.h>
#include <stdio.h>

using namespace std;
#define VIEWDIST 30.0f
#define MAPHEIGHT 16
#define MAPWIDHT 16
const int ScreenWidht = 120;
const int ScreenHeight = 40;
#define PLAYERSPEED 5.0f


typedef struct Coordinates
{
	float X;
	float Y;
}coord;
typedef struct Player
{
	coord Coordinates;
	float Dir;
	float AngleOfView;
	float Speed;
	bool TouchWall;
	coord Eye;
	float DistanceToWall;
	void PlayerInit()
	{
		Coordinates.X = 1.0f;
		Coordinates.Y = 1.0f;
		Dir = 0.0f;
		AngleOfView = 3.14159 / 3;
		Speed = PLAYERSPEED;
		TouchWall = false;
		Eye.X = 0;
		Eye.Y = 0;
		DistanceToWall = 0.0f;
	}
}player;

void DrawMap(wstring* Map)
{
	*Map += L"################";
	*Map += L"#..............#";
	*Map += L"#..............#";
	*Map += L"#..............#";
	*Map += L"#..............#";
	*Map += L"#..............#";
	*Map += L"#..............#";
	*Map += L"#..............#";
	*Map += L"#..............#";
	*Map += L"#..............#";
	*Map += L"#..............#";
	*Map += L"#..............#";
	*Map += L"#..............#";
	*Map += L"#..............#";
	*Map += L"#..............#";
	*Map += L"################";
}

int main()
{
	player Player;
	Player.PlayerInit();
	wchar_t* Screen = new wchar_t[ScreenHeight * ScreenWidht + 1];
	HANDLE Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(Console);
	wstring Map;
	DrawMap(&Map);
	auto Start = chrono::system_clock::now(); 
	auto End= chrono::system_clock::now(); 
	while (1)
	{
		End = chrono::system_clock::now();
		chrono::duration<float> Time = End - Start;
		Start = End;
		float ElapsedTime = Time.count();
		if (GetAsyncKeyState((int)'A'))
			Player.Dir -= (1.5) * ElapsedTime;
		else if(GetAsyncKeyState((int)'D'))
			Player.Dir += (1.5) * ElapsedTime;
		else if (GetAsyncKeyState((int)'W'))
		{
			Player.Coordinates.X += sinf(Player.Dir) * Player.Speed * ElapsedTime;
			Player.Coordinates.Y += cosf(Player.Dir) * Player.Speed * ElapsedTime;
			if (Map[(int)Player.Coordinates.Y * MAPWIDHT + Player.Coordinates.Y] == '#')
			{
				Player.Coordinates.X = sinf(Player.Dir) * Player.Speed * ElapsedTime;
				Player.Coordinates.Y = cosf(Player.Dir) * Player.Speed * ElapsedTime;
			}
		}
		else if (GetAsyncKeyState((int)'S'))
		{
			Player.Coordinates.X = sinf(Player.Dir) * Player.Speed * ElapsedTime;
			Player.Coordinates.Y = cosf(Player.Dir) * Player.Speed * ElapsedTime;
			if (Map[(int)Player.Coordinates.Y * MAPWIDHT + Player.Coordinates.Y] == '#')
			{
				Player.Coordinates.X += sinf(Player.Dir) * Player.Speed * ElapsedTime;
				Player.Coordinates.Y += cosf(Player.Dir) * Player.Speed * ElapsedTime;
			}
		}
		for (int x = 0; x < ScreenWidht;x++)
		{
			float RayAngle = (Player.Dir - Player.AngleOfView / 2.0f) + ((float)x / (float)ScreenWidht) * Player.AngleOfView;
			Player.DistanceToWall = 0.0f;
			Player.TouchWall = false;
			Player.Eye.X = sinf(RayAngle);
			Player.Eye.Y = cosf(RayAngle);
			while (!Player.TouchWall && Player.DistanceToWall < VIEWDIST)
			{
				Player.DistanceToWall += 0.1f;

			}

		}
	}
	return 0;
}