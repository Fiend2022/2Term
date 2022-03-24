#pragma once

#include "raylib.h"
#include "raymath.h"

#ifndef TANKLIB
#define TANKLIB

#define TANKSPEED 4
#define FIRESPEED 10
#define NUMOFFULLBULLET 20
#define NUMOFAMMUNITION 3
#define CRITICALFEWBULLETS 5
#define SCREENWIDHT 1000
#define SCREENHEIGHT 750
#define HEALTH 5
static int IndexOfBullFirstPlayer = 0;
static int IndexOfBullSecondPlayer = 0;

typedef struct Object
{
	Vector2 Position;
	Vector2 Speed;
	Vector2 Size;
	Texture2D Image;
	char Type;
	bool CanShoot;
	bool CanMove;
	int Clip;
	int HP;
} Tank,  Object;

typedef struct Bullet
{
	Vector2 Position;
	Vector2 Direction;
	Texture2D Image;
	bool IsExist;
} Bullet;

typedef struct Ammunition
{
	Vector2 Position;
	Vector2 Size;
	bool IsExist;
	Texture2D Image;
} Ammunition;

void ObjectMove(Bullet* Obj);
void PlayersAction(Tank* Player, Texture2D* Direction, Bullet* Bullet);
void ObjectInit(Object* Obj, float SpeedX, float SpeedY, float PosX, float PosY, char Type);
void TankMove(Tank* Obj);
void Recharge(Tank* Player, Ammunition* Box);
void BulletMove(Bullet* Bullet, Tank Player, int Num);
Tank PlayerInit(int X, int Y, char Type, char* File);
void InventoryUpdate(Ammunition* Box, bool);
Ammunition* FullAmmunitionInit();
Bullet* BulletInit();
void PlayersLife(Tank* Player, Bullet* Bullet, Texture2D* Dir, Ammunition* Box);

#endif // !1
