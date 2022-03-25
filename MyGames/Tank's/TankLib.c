#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TankLib.h"
int NumOfBoxInField = 0;


void BulletMove(Bullet* Obj)
{
	Obj->Position.x += Obj->Direction.x;
	Obj->Position.y += Obj->Direction.y;

}
void TankMove(Tank* Obj)
{
	Obj->Position.x += Obj->Speed.x;
	Obj->Position.y += Obj->Speed.y;
}


void PlayersAction(Tank* Player, Texture2D* Direction, Bullet* Bullet)
{
	Player->CanShoot = true;
	static int PlayerSpeed = TANKSPEED;
	static int BulletSpeedX = FIRESPEED;
	static int BulletSpeedY = FIRESPEED;
	Player->Speed.x = 0;
	Player->Speed.y = 0;
	if (Player->Clip == 0)
	{
		Player->IndexOfBull = 0;
		Player->CanShoot = false;
	}
	else
		Player->CanShoot = true;
	if (IsKeyDown(Player->Move.Up) && Player->Position.y > 0)
	{
		Player->Speed.y = -PlayerSpeed;
		Bullet[Player->IndexOfBull].Direction.y = -BulletSpeedY;
		Player->Image = Direction[0];
		Bullet[Player->IndexOfBull].Direction.x = 0;
	}
	else if (IsKeyDown(Player->Move.Down) && Player->Position.y < FIELDHEIGHT)
	{
		Player->Speed.y = PlayerSpeed;
		Bullet[Player->IndexOfBull].Direction.y = BulletSpeedY;
		Bullet[Player->IndexOfBull].Direction.x = 0;
		Player->Image = Direction[1];
	}
	else if (IsKeyDown(Player->Move.Left) && Player->Position.x > 0)
	{
		Player->Speed.x = -PlayerSpeed;
		Bullet[Player->IndexOfBull].Direction.x = -BulletSpeedX;
		Bullet[Player->IndexOfBull].Direction.y = 0;
		Player->Image = Direction[2];
		

	}
	else if (IsKeyDown(Player->Move.Right) && Player->Position.x < FIELDWIDHT)
	{
		Player->Speed.x = PlayerSpeed;
		Bullet[Player->IndexOfBull].Direction.x = BulletSpeedX;
		Player->Image = Direction[3];
		Bullet[Player->IndexOfBull].Direction.y = 0;

	}
	if (!Bullet[Player->IndexOfBull].Direction.x && !Bullet[Player->IndexOfBull].Direction.y)
		Bullet[Player->IndexOfBull].Direction.y = -BulletSpeedY;
	if (IsKeyPressed(Player->Move.Fire) && Player->CanShoot)
	{
		Player->Clip--;
		Bullet[Player->IndexOfBull].IsExist = true;
		Bullet[Player->IndexOfBull].Position.x = Player->Position.x;
		Bullet[Player->IndexOfBull].Position.y = Player->Position.y;
		Player->IndexOfBull++;
		if (Player->IndexOfBull < NUMOFFULLBULLET)
		{
			Bullet[Player->IndexOfBull].Direction.x = Bullet[Player->IndexOfBull - 1].Direction.x;
			Bullet[Player->IndexOfBull].Direction.y = Bullet[Player->IndexOfBull - 1].Direction.y;
			Bullet[Player->IndexOfBull].IsExist = false;
		}
	}

	BulletFlight(Bullet, *Player, NUMOFFULLBULLET);
}
void Recharge(Tank* Player, Ammunition* Box)
{
	if (Player->Clip <= CRITICALFEWBULLETS && NumOfBoxInField < NUMOFAMMUNITION)
	{
		for (int i = 0; i < NUMOFAMMUNITION;i++)
			if (!Box[i].IsExist)
			{
				InventoryUpdate(&Box[i], true);
				NumOfBoxInField++;
			}
	}
	for (int i = 0; i < NUMOFAMMUNITION;i++)
	{
		if (Box[i].IsExist)
			DrawTextureV(Box[i].Image, Box[i].Position, WHITE);
	}
}
void BulletFlight(Bullet* Bullet, Tank Player, int Num)
{
	for (int i = 0; i < Num;i++)
	{
		if (Bullet[i].IsExist)
		{
			BulletMove(&Bullet[i]);
			DrawTexture(Bullet[i].Image, Bullet[i].Position.x + (Player.Size.x)/2, Bullet[i].Position.y + (Player.Size.y)/2, WHITE);
		}
		if (Bullet[i].Position.x > SCREENWIDHT || Bullet[i].Position.y > SCREENHEIGHT || Bullet[i].Position.x < 0 || Bullet[i].Position.y < 0)
		{
			Bullet[i].IsExist = false;
			Bullet[i].Position.x = 0;
			Bullet[i].Position.y = 0;
			Bullet[i].Direction.y = 0;
			Bullet[i].Direction.x = 0;
		}
	}
}
Tank PlayerInit(int X, int Y, char Type, char* File, float SizeX, float SizeY, int Up, int Down, int Left, int Right, int Fire)
{
	Tank Player = { 0 };
	Player.Position = (Vector2){ (float)X, (float)Y };
	Player.Size.x = SizeX;
	Player.Size.y = SizeY;
	Player.Speed.x = TANKSPEED;
	Player.Speed.y = TANKSPEED;
	Player.Type = Type;
	Player.CanShoot = true;
	Player.CanMove = true;
	Player.Move.Up = Up;
	Player.Move.Down = Down;
	Player.Move.Left = Left;
	Player.Move.Right = Right;
	Player.Move.Fire = Fire;
	Player.Clip = NUMOFFULLBULLET;
	Player.Image = LoadTexture(File);
	Player.HP = 5;
	return Player;
}
Ammunition* FullAmmunitionInit()
{
	Ammunition* Box = (Ammunition*)malloc(sizeof(Ammunition) * NUMOFAMMUNITION);
	for (int i = 0; i < NUMOFAMMUNITION; i++)
	{
		InventoryUpdate(&Box[i], false);
	}
	return Box;
}
void InventoryUpdate(Ammunition* Box, bool IsExist)
{
	Box->Image = LoadTexture("textures\\Ammunition.png");
	Box->Position.x = rand() % 835 + 10;
	Box->Position.y = rand() % 585 + 10;
	Box->IsExist = IsExist;
	Box->Size.x = 66;
	Box->Size.y = 37;
}
Bullet* BulletInit()
{
	Bullet* Bullet = (struct Bullet*)calloc(NUMOFFULLBULLET, sizeof(struct Bullet));
	for (int i = 0; i < NUMOFFULLBULLET; i++)
	{
		Bullet[i].Image = LoadTexture("textures\\Fire.png");
		Bullet[i].IsExist = false;
		Bullet[i].Direction.x = 0;
		Bullet[i].Direction.y = 0;
	}
	return Bullet;
}
void ContactWithAmmunation(Tank* Player, Ammunition* Box)
{
	for (int i = 0; i < NUMOFAMMUNITION;i++)
	{
		if ((Player->Position.x < (Box[i].Position.x + Box[i].Size.x) && (Player->Position.x + Player->Size.x) > Box[i].Position.x) &&
			(Player->Position.y < (Box[i].Position.y + Box[i].Size.y) && (Player->Position.y + Player->Size.y) > Box[i].Position.y) &&
			(Box[i].IsExist) && (Player->Clip < 20))
		{
			Player->Clip = 20;
			Box[i].IsExist = false;
			Player->IndexOfBull = 0;
			NumOfBoxInField--;
		}
	}
}
void PlayersLife(Tank* Player, Bullet* Bullet, Texture2D* Dir, Ammunition* Box)
{
	if (Player->CanMove)
	{
		PlayersAction(Player, Dir, Bullet);
		TankMove(Player);
		DrawTextureV(Player->Image, Player->Position, WHITE);
	}
	ContactWithAmmunation(Player, Box);
}