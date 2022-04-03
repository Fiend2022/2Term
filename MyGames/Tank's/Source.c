#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TankLib.h"

Texture2D DirOfFirst[4];
Texture2D DirOfSecond[4];

void InitBeginGame(Tank* PlayerOne, Tank* PlayerTwo)
{
	*PlayerOne = PlayerInit(900, 10, '1', "textures\\TankUp.png", DirOfFirst[0].width, DirOfSecond[0].height, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER);
	*PlayerTwo = PlayerInit(20, 10, '2', "enemy\\TankUp.png", DirOfSecond[0].width, DirOfSecond[0].height, KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACE);
}

int main()
{	const int ScreenWidth = 1000;
	const int ScreenHeight = 750;
	InitWindow(ScreenWidth, ScreenHeight, "Tanks");
	InitAudioDevice();
	srand(time(NULL));
	Texture2D Background = LoadTexture("textures\\Field.png");
	Sound Boom = LoadSound("boom.wav");
	Vector2 FieldPosition = { (float)((ScreenWidth / 2) - (Background.width / 2)),(float)((ScreenHeight / 2) - (Background.height) / 2) };
	Ammunition* Box = FullAmmunitionInit();
	DirOfFirst[0] = LoadTexture("textures\\TankUp.png");
	DirOfFirst[1] = LoadTexture("textures\\TankDown.png");
	DirOfFirst[2] = LoadTexture("textures\\TankLeft.png");
	DirOfFirst[3] = LoadTexture("textures\\TankRight.png");
	DirOfSecond[0] = LoadTexture("enemy\\TankUp.png");
	DirOfSecond[1] = LoadTexture("enemy\\TankDown.png");
	DirOfSecond[2] = LoadTexture("enemy\\TankLeft.png");
	DirOfSecond[3] = LoadTexture("enemy\\TankRight.png");
	Bullet* BulletOfPlayerOne, * BulletOfPlayerTwo;
	BulletOfPlayerOne = (Bullet*)malloc(sizeof(Bullet)*NUMOFFULLBULLET);
	BulletOfPlayerTwo = (Bullet*)malloc(sizeof(Bullet) * NUMOFFULLBULLET);
	BulletOfPlayerOne = BulletInit();
	BulletOfPlayerTwo = BulletInit();
	Tank PlayerOne, PlayerTwo;
	InitBeginGame(&PlayerOne, &PlayerTwo);
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
			PlayersLife(&PlayerOne, BulletOfPlayerOne, BulletOfPlayerTwo, DirOfFirst, Box, Boom);
			PlayersLife(&PlayerTwo, BulletOfPlayerTwo, BulletOfPlayerOne, DirOfSecond, Box, Boom);
			if (!PlayerOne.CanMove || !PlayerTwo.CanMove)
			{
				free(BulletOfPlayerOne);
				free(BulletOfPlayerTwo);
				InitBeginGame(&PlayerOne, &PlayerTwo);
				BulletOfPlayerOne = BulletInit();
				BulletOfPlayerTwo = BulletInit();
			}
 		EndDrawing();
	}
	free(BulletOfPlayerOne);
	free(BulletOfPlayerTwo);
	CloseWindow();
}
