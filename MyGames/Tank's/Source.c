#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TankLib.h"


int main()
{
	const int ScreenWidth = 1000;
	const int ScreenHeight = 750;
	InitWindow(ScreenWidth, ScreenHeight, "Tanks");
	srand(time(NULL));
	Texture2D Background = LoadTexture("textures\\Field.png");
	Vector2 FieldPosition = { (float)((ScreenWidth / 2) - (Background.width / 2)),(float)((ScreenHeight / 2) - (Background.height) / 2) };
	static int CountOfAmmunition = 0;
	Tank PlayerOne = PlayerInit(900,10,'1', "textures\\TankUp.png", 125, 120);
	Tank PlayerTwo = PlayerInit(20,10,'2', "enemy\\TankUp.png", 58,56);
	Ammunition* Box = FullAmmunitionInit();
	Texture2D DirOfFirst[4];
	Texture2D DirOfSecond[4];
	Bullet* BulletOfPlayerOne = BulletInit();
	Bullet* BulletOfPlayerTwo = BulletInit();
	DirOfFirst[0] = LoadTexture("textures\\TankUp.png");
	DirOfFirst[1] = LoadTexture("textures\\TankDown.png");
	DirOfFirst[2] = LoadTexture("textures\\TankLeft.png");
	DirOfFirst[3] = LoadTexture("textures\\TankRight.png");
	DirOfSecond[0] = LoadTexture("enemy\\TankUp.png");
	DirOfSecond[1] = LoadTexture("enemy\\TankDown.png");
	DirOfSecond[2] = LoadTexture("enemy\\TankLeft.png");
	DirOfSecond[3] = LoadTexture("enemy\\TankRight.png");
	SetTargetFPS(60);
	while (WindowShouldClose() == 0)
	{
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawTextureV(Background, FieldPosition, WHITE);
			Recharge(&PlayerOne, Box);
			Recharge(&PlayerTwo, Box);
			DrawText(TextFormat("%i/%i\nHP:%i", PlayerTwo.Clip,  NUMOFFULLBULLET, PlayerTwo.HP), 10, 10, 25, WHITE);
			DrawText(TextFormat("%i/%i\nHP:%i", PlayerOne.Clip, NUMOFFULLBULLET, PlayerOne.HP), 900, 10, 25, WHITE);
			PlayersLife(&PlayerOne, BulletOfPlayerOne, DirOfFirst, Box);
			PlayersLife(&PlayerTwo, BulletOfPlayerTwo, DirOfSecond, Box);
 		EndDrawing();
	}
	CloseWindow();
}
