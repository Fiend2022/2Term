#pragma once

#include "raylib.h"
#include "raymath.h"



#define TANKSPEED 4
#define FIRESPEED 10
#define NUMOFFULLBULLET 20
#define NUMOFAMMUNITION 3
#define CRITICALFEWBULLETS 5
#define SCREENWIDHT 1000
#define SCREENHEIGHT 750
#define FIELDWIDHT 900
#define FIELDHEIGHT 600
#define HEALTH 5

typedef struct KEY_MOVE
{
	int Up;
	int Down;
	int Left;
	int Right;
	int Fire;
}KEY_MOVE;

typedef struct Tank
{
	Vector2 Position;
	Vector2 Speed;
	Vector2 Size;
	Texture2D Image;
	char Type;
	bool CanShoot;
	bool CanMove;
	int Clip;
	KEY_MOVE Move;
	unsigned short IndexOfBull;
	unsigned short int HP;
} Tank;

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

void BulletMove(Bullet* Obj);
void PlayersAction(Tank* Player, Texture2D* Direction, Bullet* Bullet);
void TankMove(Tank* Obj);
void Recharge(Tank* Player, Ammunition* Box);
void BulletFlight(Bullet* Bullet, Tank Player, int Num);
Tank PlayerInit(int X, int Y, char Type, char* File, float, float, int Up, int Down, int Left, int Right, int Fire);
void InventoryUpdate(Ammunition* Box, bool);
Ammunition* FullAmmunitionInit();
Bullet* BulletInit();
void PlayersLife(Tank* Player, Bullet* Bullet, Texture2D* Dir, Ammunition* Box);


