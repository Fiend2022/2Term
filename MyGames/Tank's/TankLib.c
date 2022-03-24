#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TankLib.h"
int NumOfBoxInField = 0;


void ObjectMove(Bullet* Obj)
{
	Obj->Position.x += Obj->Direction.x;
	Obj->Position.y += Obj->Direction.y;

}
void TankMove(Tank* Obj)
{
	Obj->Position.x += Obj->Speed.x;
	Obj->Position.y += Obj->Speed.y;
}

void ObjectInit(Object* Obj, float SpeedX, float SpeedY, float PosX, float PosY, char Type)
{
	Obj->Speed.x = SpeedX;
	Obj->Speed.y = SpeedY;
	Obj->Position.x = SpeedX;
	Obj->Position.y = SpeedY;
	Obj->Type = Type;
}

void PlayersAction(Tank* Player, Texture2D* Direction, Bullet* Bullet)
{
	Player->CanShoot = true;
	static int PlayerSpeed = TANKSPEED;
	static int BulletSpeedX = FIRESPEED;
	static int BulletSpeedY = FIRESPEED;
	Player->Speed.x = 0;
	Player->Speed.y = 0;
	if (Player->Type == '1') 
	{
		if (Player->Clip == 0)
		{
			IndexOfBullFirstPlayer = 0;
			Player->CanShoot = false;
		}
		else
			Player->CanShoot = true;
		if (IsKeyDown(KEY_UP) && Player->Position.y > 0)
		{
			Player->Speed.y = -PlayerSpeed;
			Bullet[IndexOfBullFirstPlayer].Direction.y = -BulletSpeedY;
			Player->Image = Direction[0];
			Bullet[IndexOfBullFirstPlayer].Direction.x = 0;
		}
		else if (IsKeyDown(KEY_DOWN) && Player->Position.y < 600)
		{
			Player->Speed.y = PlayerSpeed;
			Bullet[IndexOfBullFirstPlayer].Direction.y = BulletSpeedY;
			Bullet[IndexOfBullFirstPlayer].Direction.x = 0;
			Player->Image = Direction[1];
		}
		else if (IsKeyDown(KEY_LEFT) && Player->Position.x > 0)
		{
			Player->Speed.x = -PlayerSpeed;
			Bullet[IndexOfBullFirstPlayer].Direction.x = -BulletSpeedX;
			Bullet[IndexOfBullFirstPlayer].Direction.y = 0;
			Player->Image = Direction[2];
		}
		else if (IsKeyDown(KEY_RIGHT) && Player->Position.x < 850)
		{
			Player->Speed.x = PlayerSpeed;
			Bullet[IndexOfBullFirstPlayer].Direction.x = BulletSpeedX;
			Player->Image = Direction[3];
			Bullet[IndexOfBullFirstPlayer].Direction.y = 0;
		}
		if (!Bullet[IndexOfBullFirstPlayer].Direction.x && !Bullet[IndexOfBullFirstPlayer].Direction.y)
			Bullet[IndexOfBullFirstPlayer].Direction.y = -BulletSpeedY;
		if (IsKeyPressed(KEY_SPACE) && Player->CanShoot)
		{
			Player->Clip--;
			Bullet[IndexOfBullFirstPlayer].IsExist = true;
			Bullet[IndexOfBullFirstPlayer].Position.x = Player->Position.x;
			Bullet[IndexOfBullFirstPlayer].Position.y = Player->Position.y;
			IndexOfBullFirstPlayer++;
			if (IndexOfBullFirstPlayer < NUMOFFULLBULLET)
			{
				Bullet[IndexOfBullFirstPlayer].Direction.x = Bullet[IndexOfBullFirstPlayer - 1].Direction.x;
				Bullet[IndexOfBullFirstPlayer].Direction.y = Bullet[IndexOfBullFirstPlayer - 1].Direction.y;
				Bullet[IndexOfBullFirstPlayer].IsExist = false;
			}
		}
	}
	else if (Player->Type == '2')
	{
		if (Player->Clip == 0)
		{
			IndexOfBullFirstPlayer = 0;
			Player->CanShoot = false;
		}
		else
			Player->CanShoot = true;
		if (IsKeyDown(KEY_W) && Player->Position.y > 0)
		{
			Player->Speed.y = -PlayerSpeed;
			Bullet[IndexOfBullSecondPlayer].Direction.y = -BulletSpeedY;
			Player->Image = Direction[0];
			Bullet[IndexOfBullSecondPlayer].Direction.x = 0;
		}
		else if (IsKeyDown(KEY_S) && Player->Position.y < 800)
		{
			Player->Speed.y = PlayerSpeed;
			Bullet[IndexOfBullSecondPlayer].Direction.y = BulletSpeedY;
			Bullet[IndexOfBullSecondPlayer].Direction.x = 0;
			Player->Image = Direction[1];
		}
		else if (IsKeyDown(KEY_A) && Player->Position.x > 0)
		{
			Player->Speed.x = -PlayerSpeed;
			Bullet[IndexOfBullSecondPlayer].Direction.x = -BulletSpeedX;
			Bullet[IndexOfBullSecondPlayer].Direction.y = 0;
			Player->Image = Direction[2];
		}
		else if (IsKeyDown(KEY_D) && Player->Position.x < 850)
		{
			Player->Speed.x = PlayerSpeed;
			Bullet[IndexOfBullSecondPlayer].Direction.x = BulletSpeedX;
			Player->Image = Direction[3];
			Bullet[IndexOfBullSecondPlayer].Direction.y = 0;
		}
		if (!Bullet[IndexOfBullSecondPlayer].Direction.x && !Bullet[IndexOfBullSecondPlayer].Direction.y)
			Bullet[IndexOfBullSecondPlayer].Direction.y = -BulletSpeedY;
		if (IsKeyPressed(KEY_ENTER) && Player->CanShoot)
		{
			Player->Clip--;
			Bullet[IndexOfBullSecondPlayer].IsExist = true;
			Bullet[IndexOfBullSecondPlayer].Position.x = Player->Position.x;
			Bullet[IndexOfBullSecondPlayer].Position.y = Player->Position.y;
			IndexOfBullSecondPlayer++;
			if (IndexOfBullSecondPlayer < NUMOFFULLBULLET)
			{
				Bullet[IndexOfBullSecondPlayer].Direction.x = Bullet[IndexOfBullSecondPlayer - 1].Direction.x;
				Bullet[IndexOfBullSecondPlayer].Direction.y = Bullet[IndexOfBullSecondPlayer - 1].Direction.y;
				Bullet[IndexOfBullSecondPlayer].IsExist = false;
			}
		}
	}
	BulletMove(Bullet, *Player, NUMOFFULLBULLET);
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
void BulletMove(Bullet* Bullet, Tank Player, int Num)
{
	for (int i = 0; i < Num;i++)
	{
		if (Bullet[i].IsExist)
		{
			ObjectMove(&Bullet[i]);
			DrawTexture(Bullet[i].Image, Bullet[i].Position.x + (Player.Size.x), Bullet[i].Position.y + (Player.Size.y), WHITE);
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
Tank PlayerInit(int X, int Y, char Type, char* File)
{
	Tank Player = { 0 };
	Player.Position = (Vector2){ (float)X, (float)Y };
	Player.Size.x = 58;
	Player.Size.y = 56;
	Player.Speed.x = TANKSPEED;
	Player.Speed.y = TANKSPEED;
	Player.Type = Type;
	Player.CanShoot = true;
	Player.CanMove = true;
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
			if (Player->Type == '1')
				IndexOfBullFirstPlayer = 0;
			else if (Player->Type == '2')
				IndexOfBullSecondPlayer = 0;
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