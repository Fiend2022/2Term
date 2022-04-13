#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
using namespace std;

#include <stdio.h>
#include <Windows.h>



#define VIEWDIST 16.0f
#define MAPHEIGHT 16
#define MAPWIDTH 16
const int ScreenWidth = 120;
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
		Coordinates.X = 5.0f;
		Coordinates.Y = 5.0f;
		Dir = 1.0f;
		AngleOfView = 3.14159 / 4.0;
		Speed = PLAYERSPEED;
		TouchWall = false;
		Eye.X = 0;
		Eye.Y = 0;
		DistanceToWall = 0.0f;
	}
}player;


int main()
{
	player Player;
	Coordinates PointRayEnd;
	Player.PlayerInit();
	wchar_t* Screen = new wchar_t[ScreenHeight * ScreenWidth + 1];
	HANDLE Console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(Console);
	wstring Map;
	Map += L"################";
	Map += L"#...............";
	Map += L"#.......########";
	Map += L"#..............#";
	Map += L"#..............#";
	Map += L"#..............#";
	Map += L"#..............#";
	Map += L"#..............#";
	Map += L"#..............#";
	Map += L"#..............#";
	Map += L"#..............#";
	Map += L"#..............#";
	Map += L"#..............#";
	Map += L"#......#########";
	Map += L"#..............#";
	Map += L"################";
	auto Start = chrono::system_clock::now(); 
	auto End= chrono::system_clock::now(); 
	DWORD BytesWritten;
	while (1)
	{
		End = chrono::system_clock::now();
		chrono::duration<float> Time = End - Start;
		Start = End;
		float ElapsedTime = Time.count();
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			Player.Dir -= (Player.Speed * 0.75f) * ElapsedTime;
		else if(GetAsyncKeyState((unsigned short)'D') & 0x8000)
			Player.Dir += (Player.Speed * 0.75f) * ElapsedTime;
		else if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			Player.Coordinates.X += sinf(Player.Dir) * Player.Speed * ElapsedTime;;
			Player.Coordinates.Y += cosf(Player.Dir) * Player.Speed * ElapsedTime;;
			if (Map.c_str()[(int)Player.Coordinates.X * MAPWIDTH + (int)Player.Coordinates.Y] == '#')
			{
				Player.Coordinates.X -= sinf(Player.Dir) * Player.Speed * ElapsedTime;;
				Player.Coordinates.Y -= cosf(Player.Dir) * Player.Speed * ElapsedTime;;
			}
		}
		else if (GetAsyncKeyState((unsigned short)'S')&0x8000)
		{
			Player.Coordinates.X -= sinf(Player.Dir) * Player.Speed * ElapsedTime;;
			Player.Coordinates.Y -= cosf(Player.Dir) * Player.Speed * ElapsedTime;;
			if (Map.c_str()[(int)Player.Coordinates.X * MAPWIDTH + (int)Player.Coordinates.Y] == '#')
			{
				Player.Coordinates.X += sinf(Player.Dir) * Player.Speed * ElapsedTime;;
				Player.Coordinates.Y += cosf(Player.Dir) * Player.Speed * ElapsedTime;;
			}
		}
		for (int x = 0; x < ScreenWidth;x++)
		{
			float RayAngle = (Player.Dir - Player.AngleOfView / 2.0f) + ((float)x / (float)ScreenWidth) * Player.AngleOfView;
			Player.DistanceToWall = 0.0f;
			Player.TouchWall = false;
			bool Boundary = false;
			Player.Eye.X = sinf(RayAngle);
			Player.Eye.Y = cosf(RayAngle);
			while (!Player.TouchWall && Player.DistanceToWall < VIEWDIST)
			{
				Player.DistanceToWall += 0.1f;
				PointRayEnd.X = Player.Coordinates.X + Player.Eye.X * Player.DistanceToWall;
				PointRayEnd.Y = Player.Coordinates.Y + Player.Eye.Y * Player.DistanceToWall;
				if (PointRayEnd.X < 0 || PointRayEnd.X >= MAPWIDTH || PointRayEnd.Y < 0 || PointRayEnd.Y >= MAPHEIGHT)
				{
					Player.TouchWall = true;
					Player.DistanceToWall = VIEWDIST;
				}
				else if (Map.c_str()[ (int)PointRayEnd.Y * MAPWIDTH + (int)PointRayEnd.X ] == '#')
					Player.TouchWall = true;
				short Texture = ' ';
				float Ceiling = (float)(ScreenHeight / 2.0) - ScreenHeight / ((float)Player.DistanceToWall);
				float Floor = ScreenHeight - Ceiling;
				if (Player.DistanceToWall <= VIEWDIST / 4.0f)
					Texture = 0x2588;
				else if (Player.DistanceToWall < VIEWDIST / 3.0f)
					Texture = 0x2593;
				else if (Player.DistanceToWall < VIEWDIST / 2.0f)
					Texture = 0x2592;
				else if (Player.DistanceToWall < VIEWDIST)
					Texture = 0x2591;
				else
					Texture = ' ';
				for (int y = 0;y < ScreenHeight;y++)
				{
					if (y <= Ceiling)
						Screen[x + y * ScreenWidth] = ' ';
					else if (y <= Floor && y > Ceiling)
						Screen[x + y * ScreenWidth] = Texture;
					else
					{
						float FloorPosition = 1.0f - ((float)y - ScreenHeight / 2.0) / ((float)ScreenHeight / 2.0);
						if (FloorPosition < 0.25)
							Texture = '#';
						else if (FloorPosition < 0.5)
							Texture = 'x';
						else if (FloorPosition < 0.75)
							Texture = '~';
						else if (FloorPosition < 0.9)
							Texture = '-';
						else
							Texture = ' ';
						Screen[x+y*ScreenWidth] = Texture;
						
					}
				}
			}
			swprintf_s(Screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", Player.Coordinates.X, Player.Coordinates.Y, Player.Dir, 1.0f / ElapsedTime);
			for (int nx = 0; nx < MAPWIDTH; nx++)
				for (int ny = 0; ny < MAPWIDTH; ny++)
				{
					Screen[(ny + 1) * ScreenWidth + nx] = Map[ny * MAPWIDTH + nx];
				}
			Screen[((int)Player.Coordinates.X + 1) * ScreenWidth + (int)Player.Coordinates.Y] = 'P';
		}
		Screen[ScreenWidth * ScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(Console, Screen, ScreenWidth * ScreenHeight, { 0,0 }, &BytesWritten);
	}
	return 0;
}
