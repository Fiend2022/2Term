#include "raylib.h"
#include "raymath.h"
#include <math.h>
#define WIDTH 1920
#define HEIGHT 1020
#define BARLEN 400
#define SYMORDER 17
int SymIteration = 0;
#define COMMPRESION 1.8
typedef struct Line
{
	Vector2 Begin;
	Vector2 End;
	float Length;
}line;
void DrawTree(line Branch, int SymOrder, int Dir);
void RotatedBranch(line Branch, int SymOrder, int Dir, float Offset, float x, float y);
void TreeInit(line* Tree);
void RectangularBranches(line Branch,int SymOrder, int Dir, float Offset, float x, float y);
int main()
{
	line Tree;
	InitWindow(WIDTH, HEIGHT, "Fractal's wood");
	static int FPS = 10;
	while (!WindowShouldClose())
	{
		SetTargetFPS(FPS);
		if (SymIteration > SYMORDER)
			SymIteration = 0;
		++SymIteration;
		BeginDrawing();
			if (IsKeyDown(KEY_UP) && FPS <60)
				FPS++;
			else if (IsKeyDown(KEY_DOWN)&& FPS >5)
				FPS--;
			ClearBackground(WHITE);
			TreeInit(&Tree);
			DrawTree(Tree, 1,1);
			DrawText(TextFormat("FPS: %i",FPS),10,10,25,BLACK);
			DrawText("Press Up/Down to change rendering speed", 20, 30, 25, BLACK);
		EndDrawing();
	}
}
void RotatedBranch(line Branch, int SymOrder, int Dir, float Offset, float x, float y)
{
	DrawLineEx(Branch.Begin, Branch.End, 3.0f, BLACK);
	Branch.Length /= COMMPRESION;
	Offset = Branch.Length / sqrt(2);
	x += Offset;
	y -= Offset;
	Branch.Begin.x = Branch.End.x;
	Branch.Begin.y = Branch.End.y;
	Branch.End.x = x;
	Branch.End.y = y;
	DrawTree(Branch, SymOrder + 1, 1);
	x -= 2 * Offset;
	Branch.End.x = x;
	DrawTree(Branch, SymOrder + 1, -1);
}
void TreeInit(line* Tree)
{
	Tree->Begin.x = (float)WIDTH / 2;
	Tree->Begin.y = (float)HEIGHT;
	Tree->End.y = (float)HEIGHT - BARLEN;
	Tree->End.x = (float)WIDTH / 2;
	Tree->Length = BARLEN;
}
void DrawTree(line Branch, int SymOrder, int Dir)
{
	float Offset = 0;
	float x = Branch.End.x;
	float y = Branch.End.y;
	if (SymOrder == SymIteration)
		return;
	else if (SymOrder % 2)
		RotatedBranch(Branch, SymOrder, Dir, Offset, x, y);
	else if (SymOrder % 2 == 0 && SymOrder != 0)
		RectangularBranches(Branch, SymOrder, Dir, Offset, x, y);
}
void RectangularBranches(line Branch, int SymOrder, int Dir, float Offset, float x, float y)
{
	DrawLineEx(Branch.Begin, Branch.End, 3.0f, BLACK);
	Branch.Length /= COMMPRESION;
	Branch.Begin.x = Branch.End.x;
	Branch.Begin.y = Branch.End.y;
	Branch.End.x += Dir * Branch.Length;
	DrawTree(Branch, SymOrder + 1, 1);
	Branch.End.x -= Dir * Branch.Length;
	Branch.End.y -= Branch.Length;
	DrawTree(Branch, SymOrder + 1, 1);
}