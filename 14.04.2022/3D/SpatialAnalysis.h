#pragma once


#define VIEWDIST 16.0f
#define MAPHEIGHT 16
#define MAPWIDTH 16
const int ScreenWidth = 120;
const int ScreenHeight = 40;
#define PLAYERSPEED 2.5f

typedef struct Coordinates
{
	float X;
	float Y;
}coord;
struct player
{
	coord Coordinates;
	float Dir;
	float AngleOfView;
	float Speed;
	bool RayTouchWall;
	coord Eye;
	float RayDistanceToWall;
	void PlayerInit()
	{
		Coordinates.X = 5.0f;
		Coordinates.Y = 5.0f;
		Dir = 0.0f;
		AngleOfView = 3.14159 / 4.0;
		Speed = PLAYERSPEED;
		RayTouchWall = false;
		Eye.X = 0;
		Eye.Y = 0;
		RayDistanceToWall = 0.0f;
	}
};

void WorkWithRayAngle(player* Player, int x);
void RayBehaivorAnalysis(player* Player, Coordinates* PointRayEnd, wstring Map);
void AnalysisDistance(short* Texture, player Player);
void RenderingByColumns(wchar_t* Screen, int x, short* Texture, float Ceiling, float Floor);
void MainLoop(player* Player, wchar_t* Screen, wstring Map, Coordinates* PointRayEnd, float ElapsedTime);
void PlayerMove(player* Player, wstring Map, float ElapsedTime);
wstring MapInit();