#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
using namespace std;

#include <stdio.h>
#include <Windows.h>
#include "SpatialAnalysis.h"


void PlayerMove(player* Player, wstring Map, float ElapsedTime)
{
	if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		Player->Dir -= (Player->Speed * 0.75f) * ElapsedTime;
	else if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		Player->Dir += (Player->Speed * 0.75f) * ElapsedTime;
	else if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
	{
		Player->Coordinates.X += sinf(Player->Dir) * Player->Speed * ElapsedTime;;
		Player->Coordinates.Y += cosf(Player->Dir) * Player->Speed * ElapsedTime;;
		if (Map.c_str()[(int)Player->Coordinates.X * MAPWIDTH + (int)Player->Coordinates.Y] == '#')
		{
			Player->Coordinates.X -= sinf(Player->Dir) * Player->Speed * ElapsedTime;;
			Player->Coordinates.Y -= cosf(Player->Dir) * Player->Speed * ElapsedTime;;
		}
	}
	else if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
	{
		Player->Coordinates.X -= sinf(Player->Dir) * Player->Speed * ElapsedTime;;
		Player->Coordinates.Y -= cosf(Player->Dir) * Player->Speed * ElapsedTime;;
		if (Map.c_str()[(int)Player->Coordinates.X * MAPWIDTH + (int)Player->Coordinates.Y] == '#')
		{
			Player->Coordinates.X += sinf(Player->Dir) * Player->Speed * ElapsedTime;;
			Player->Coordinates.Y += cosf(Player->Dir) * Player->Speed * ElapsedTime;;
		}
	}
}

wstring MapInit()
{
	wstring Map;
	Map += L"#########.......";
	Map += L"#...............";
	Map += L"#.......########";
	Map += L"#..............#";
	Map += L"#......##......#";
	Map += L"#......##......#";
	Map += L"#..............#";
	Map += L"###............#";
	Map += L"##.............#";
	Map += L"#......####..###";
	Map += L"#......#.......#";
	Map += L"#......#.......#";
	Map += L"#..............#";
	Map += L"#......#########";
	Map += L"#..............#";
	Map += L"################";
	return Map;
}
void WorkWithRayAngle(player* Player, int x)
{
	float RayAngle = (Player->Dir - Player->AngleOfView / 2.0f) + ((float)x / (float)ScreenWidth) * Player->AngleOfView;
	Player->RayDistanceToWall = 0.0f;
	Player->RayTouchWall = false;
	Player->Eye.X = sinf(RayAngle);
	Player->Eye.Y = cosf(RayAngle);
}
void RayBehaivorAnalysis(player* Player, Coordinates* PointRayEnd, wstring Map)
{
	Player->RayDistanceToWall += 0.1f;
	PointRayEnd->X = Player->Coordinates.X + Player->Eye.X * Player->RayDistanceToWall;
	PointRayEnd->Y = Player->Coordinates.Y + Player->Eye.Y * Player->RayDistanceToWall;
	if (PointRayEnd->X < 0 || PointRayEnd->X >= MAPWIDTH || PointRayEnd->Y < 0 || PointRayEnd->Y >= MAPHEIGHT)
	{
		Player->RayTouchWall = true;
		Player->RayDistanceToWall = VIEWDIST;
	}
	else if (Map.c_str()[(int)PointRayEnd->X * MAPWIDTH + (int)PointRayEnd->Y] == '#')
		Player->RayTouchWall = true;
}
void AnalysisDistance(short* Texture, player Player)
{
	if (Player.RayDistanceToWall <= VIEWDIST / 4.0f)
		*Texture = 0x2588;
	else if (Player.RayDistanceToWall < VIEWDIST / 3.0f)
		*Texture = 0x2593;
	else if (Player.RayDistanceToWall < VIEWDIST / 2.0f)
		*Texture = 0x2592;
	else if (Player.RayDistanceToWall < VIEWDIST)
		*Texture = 0x2591;
	else
		*Texture = ' ';
}
void RenderingByColumns(wchar_t* Screen, int x, short* Texture, float Ceiling, float Floor)
{
	for (int y = 0;y < ScreenHeight;y++)
	{
		if (y <= Ceiling)
			Screen[x + y * ScreenWidth] = ' ';
		else if (y <= Floor && y > Ceiling)
			Screen[x + y * ScreenWidth] = *Texture;
		else
		{
			float FloorPosition = 1.0f - ((float)y - ScreenHeight / 2.0) / ((float)ScreenHeight / 2.0);
			if (FloorPosition < 0.25)
				*Texture = '#';
			else if (FloorPosition < 0.5)
				*Texture = 'x';
			else if (FloorPosition < 0.75)
				*Texture = '~';
			else if (FloorPosition < 0.9)
				*Texture = '-';
			else
				*Texture = ' ';
			Screen[y * ScreenWidth + x] = *Texture;

		}
	}
}
void MainLoop(player* Player, wchar_t* Screen, wstring Map, Coordinates* PointRayEnd, float ElapsedTime)
{
	for (int x = 0; x < ScreenWidth;x++)
	{
		WorkWithRayAngle(Player, x);
		while (!Player->RayTouchWall && Player->RayDistanceToWall < VIEWDIST)
		{
			RayBehaivorAnalysis(Player, PointRayEnd, Map);
			short Texture = ' ';
			float Ceiling = (float)(ScreenHeight / 2.0) - ScreenHeight / ((float)Player->RayDistanceToWall);
			float Floor = ScreenHeight - Ceiling;
			AnalysisDistance(&Texture, *Player);
			RenderingByColumns(Screen, x, &Texture, Ceiling, Floor);
		}
		swprintf_s(Screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f", Player->Coordinates.X, Player->Coordinates.Y, Player->Dir);
		for (int nx = 0; nx < MAPWIDTH; nx++)
			for (int ny = 0; ny < MAPWIDTH; ny++)
			{
				Screen[(ny + 1) * ScreenWidth + nx] = Map[ny * MAPWIDTH + nx];
			}
		Screen[((int)Player->Coordinates.X + 1) * ScreenWidth + (int)Player->Coordinates.Y] = 'P';
	}
}