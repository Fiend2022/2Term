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
void static Move(Tank* Player, Texture2D* Dir, Bullet* Bullet,int PlayerSpeedX, int PlayerSpeedY, int BullSpeedX, int BullSpeedY, int IndexDirPlayer)
{
	Player->Speed.x = PlayerSpeedX;
	Player->Speed.y = PlayerSpeedY;
	Bullet[Player->IndexOfBull].Direction.y = BullSpeedY;
	Player->Image = Dir[IndexDirPlayer];
	Bullet[Player->IndexOfBull].Direction.x = BullSpeedX;
	Player->Size.x = Dir[IndexDirPlayer].width;
	Player->Size.y = Dir[IndexDirPlayer].height;
}
void Magazine(Tank* Player, Bullet* Bull)
{
	Player->Clip--;
	Bull[Player->IndexOfBull].IsExist = true;
	Bull[Player->IndexOfBull].Position.x = Player->Position.x;
	Bull[Player->IndexOfBull].Position.y = Player->Position.y;
	Player->IndexOfBull++;
	if (Player->IndexOfBull < NUMOFFULLBULLET)
	{
		Bull[Player->IndexOfBull].Direction.x = Bull[Player->IndexOfBull - 1].Direction.x;
		Bull[Player->IndexOfBull].Direction.y = Bull[Player->IndexOfBull - 1].Direction.y;
		Bull[Player->IndexOfBull].IsExist = false;
	}
}
void PlayersAction(Tank* Player, Texture2D* Direction, Bullet* Bullet)
{
	int PlayerSpeed = TANKSPEED;
	int BulletSpeed = FIRESPEED;
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
		Move(Player, Direction, Bullet, 0, -PlayerSpeed, 0, -BulletSpeed, 0);
	else if (IsKeyDown(Player->Move.Down) && Player->Position.y < FIELDHEIGHT)
		Move(Player, Direction, Bullet, 0, PlayerSpeed, 0, BulletSpeed, 1);
	else if (IsKeyDown(Player->Move.Left) && Player->Position.x > 0)
		Move(Player, Direction, Bullet, -PlayerSpeed, 0, -BulletSpeed, 0, 2);
	else if (IsKeyDown(Player->Move.Right) && Player->Position.x < FIELDWIDHT)
		Move(Player, Direction, Bullet, PlayerSpeed, 0, BulletSpeed, 0, 3);
	if (!Bullet[Player->IndexOfBull].Direction.x && !Bullet[Player->IndexOfBull].Direction.y)
		Bullet[Player->IndexOfBull].Direction.y = -BulletSpeed;
	if (IsKeyPressed(Player->Move.Fire) && Player->CanShoot)
		Magazine(Player, Bullet);
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
	Player.HP = HEALTH;
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
		Bullet[i].Size.x = 20;
		Bullet[i].Size.y = 20;
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
void PlayersLife(Tank* Player, Bullet* Bullet, struct Bullet* EnemyClip,Texture2D* Dir, Ammunition* Box, Sound Boom)
{
	if (Player->CanMove)
	{
		PlayersAction(Player, Dir, Bullet);
		TankMove(Player);
		DrawTextureV(Player->Image, Player->Position, WHITE);
		Killing(Player, EnemyClip,Boom);
		ContactWithAmmunation(Player, Box);
	}
}
void Killing(Tank* Player, Bullet* EnemysClip, Sound Boom)
{
	for(int i = 0; i<NUMOFFULLBULLET;i++)
		if ((EnemysClip[i].Position.x < (Player->Position.x + Player->Size.x/2) && (EnemysClip[i].Position.x + EnemysClip[i].Size.x) > Player->Position.x) &&
			(EnemysClip[i].Position.y < (Player->Position.y + Player->Size.y/2) && (EnemysClip[i].Position.y + EnemysClip[i].Size.y) > Player->Position.y) &&
			(EnemysClip[i].IsExist))
		{
			Player->HP--;
			EnemysClip[i].IsExist = false;
			if (Player->HP == 0)
			{
				Player->CanMove = false;
				PlaySound(Boom);
				Player->Clip = 0;
				break;
			}
		}
}
