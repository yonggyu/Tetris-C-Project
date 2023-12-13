#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define randomize() srand((unsigned)time(NULL))
#define random(n) (rand() % (n))
#define delay(n) Sleep(n)
#define clrscr() system("cls")
#define gotoxy(x,y) { COORD Cur = {x, y}; \
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);}
#define showcursor(bShow) { CONSOLE_CURSOR_INFO CurInfo = {20, bShow}; \
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo); }


enum { CTRL = 17, ALT = 18, ESC = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };
#define putsxy(x, y, s) {gotoxy(x, y);puts(s);}
#define BX 5
#define BX2 70
#define BY 1
#define BW 10
#define BH 20

//���� ���
void normalGame();
void ItemGame();
void multiPlayGame();

void selectMenu();
void DrawItemGameRules();
void DrawScreen();
int DrawGameOver();
BOOL ProcessKey();
BOOL ItemProcessKey(int* itemList);
void activateItem(int item);
void PrintBrick(BOOL Show);
void DrawNext(int nextBrick);
int GetAround(int x, int y, int b, int r);
BOOL MoveDown();
void TestFull();

void itemListPush(int data);
int itemListPop();
int itemListSize();
void itemListSwap();
void DrawItemList();

//Player2 �Լ�
BOOL MoveDown2();
BOOL ProcessKey2();
void PrintBrick2(BOOL Show);
void DrawScreen2();
void TestFull2();
int GetAround2(int x, int y, int b, int r);

struct Point {
	int x, y;
};
struct Point Shape[][4][4] = {
	{ { 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 },{ 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 } },
	{ { 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 } },
	{ { 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 },{ 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 } },
	{ { 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 },{ 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 } },
	{ { 0,0,-1,0,1,0,-1,-1 },{ 0,0,0,-1,0,1,-1,1 },{ 0,0,-1,0,1,0,1,1 },{ 0,0,0,-1,0,1,1,-1 } },
	{ { 0,0,1,0,-1,0,1,-1 },{ 0,0,0,1,0,-1,-1,-1 },{ 0,0,1,0,-1,0,-1,1 },{ 0,0,0,-1,0,1,1,1 } },
	{ { 0,0,-1,0,1,0,0,1 },{ 0,0,0,-1,0,1,1,0 },{ 0,0,-1,0,1,0,0,-1 },{ 0,0,-1,0,0,-1,0,1 } },
};

enum { EMPTY, BRICK, WALL };
char arTile[3][4] = { ". ","��","��" };
int board[BW + 2][BH + 2];
int board2[BW + 2][BH + 2];
int nx, ny, nx2, ny2;
int brick, rot, brick2, rot2;

//���� ƽ
void GameDelay();
int IsTimerOn = 0;

//itemListStack
int itemList[2];
int Top = 0;

int isLineItemEnable = 0;
int isBlockChangeItemEnable = 0;
int isItemUsed = 0;

int main()
{

	selectMenu();

}

void selectMenu()
{
	showcursor(FALSE);

	putsxy(11, 5, "#####   #####   #####   ####    #####    ### ");
	putsxy(11, 6, "  #     #         #     #   #     #     #    ");
	putsxy(11, 7, "  #     ###       #     #   #     #      ### ");
	putsxy(11, 8, "  #     #         #     ##        #         #");
	putsxy(11, 9, "  #     #####     #     # ##    #####   #### ")

		Top = 0;

	gotoxy(24 - 2, 12);
	printf("> �⺻ ���");
	gotoxy(24, 13);
	printf("������ ���");
	gotoxy(24, 14);
	printf("���� ���");
	gotoxy(24, 15);
	printf("2�� ��Ƽ�÷��� ���");
	gotoxy(24, 16);
	printf("���� ���� (ESC)");

	int x = 24;
	int y = 12;
	int menuNum;
	int b = 1;

	while (b)
	{
		int n = getch();

		switch (n)
		{
		case UP: {
			if (y > 12)
			{
				gotoxy(x - 2, y);
				printf(" ");
				gotoxy(x - 2, --y);
				printf(">");
			}
			break;
		}

		case DOWN: {
			if (y < 16)
			{
				gotoxy(x - 2, y);
				printf(" ");
				gotoxy(x - 2, ++y);
				printf(">");
			}
			break;
		}

		case '\r': {
			menuNum = y - 12;
			b = 0;
			break;
		}


		}
	}

	switch (menuNum)
	{
	case 0:
	{
		normalGame();
		break;
	}

	case 1:
	{
		ItemGame();
		break;
	}
	case 3:
	{
		multiPlayGame();
		break;
	}
	case 4:
	{
		exit(0);
	}
	}
}

void DrawItemGameRules()
{
	clrscr();
	putsxy(50, 5, "������ ��� ���");

	int ch = getch();
	if (ch == ' ' || ch == '\r')
	{
		return;
	}
}

void normalGame()
{
	int nFrame, nStay;
	int x, y;

	randomize();
	clrscr();

	// �����ڸ��� ��, �������� �� �������� �ʱ�ȭ�Ѵ�.
	for (x = 0; x < BW + 2; x++) {
		for (y = 0; y < BH + 2; y++) {
			if (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) {
				board[x][y] = WALL;
			}
			else {
				board[x][y] = EMPTY;
			}
		}
	}
	DrawScreen();

	putsxy(50, 3, "Normal Mode");
	putsxy(50, 5, "How To Play?")
		putsxy(50, 6, "�¿�:�̵�, ��:ȸ��, �Ʒ�:����");
	putsxy(50, 7, "����:���� ����");
	nFrame = 20;

	// ��ü ���� ����
	int nextBrick = random(sizeof(Shape) / sizeof(Shape[0]));
	while (1) {
		brick = nextBrick;
		nextBrick = random(sizeof(Shape) / sizeof(Shape[0]));
		DrawNext(nextBrick);
		nx = BW / 2;
		ny = 3;
		rot = 0;
		PrintBrick(TRUE);

		if (GetAround(nx, ny, brick, rot) != EMPTY) break;

		// ���� �ϳ��� �ٴڿ� ���� �������� ����
		nStay = nFrame;
		for (; 2;) {
			if (--nStay == 0) {
				nStay = nFrame;
				if (MoveDown()) break;
			}
			if (ProcessKey()) break;
			delay(1000 / 20);
		}

	}
	DrawGameOver();
	selectMenu();
}

void GameDelay(int* time, int limitTime)
{
	*time += 100;
	delay(15);

	if (*time > limitTime)
	{
		IsTimerOn = 0;
		*time = 0;
		return;
	}
}

void ItemGame()
{
	int nFrame, nStay;
	int x, y;

	DrawItemGameRules();

	randomize();
	clrscr();

	// �����ڸ��� ��, �������� �� �������� �ʱ�ȭ�Ѵ�.
	for (x = 0; x < BW + 2; x++) {
		for (y = 0; y < BH + 2; y++) {
			if (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) {
				board[x][y] = WALL;
			}
			else {
				board[x][y] = EMPTY;
			}
		}
	}

	DrawScreen();

	putsxy(50, 3, "Item Mode");
	putsxy(50, 5, "How To Play?")
		putsxy(50, 6, "�¿�:�̵�, ��:ȸ��, �Ʒ�:����");
	putsxy(50, 7, "����:���� ����");
	putsxy(50, 8, "������ ���:Z, ������ ���� ����:X");
	putsxy(50, 12, "������ ���");
	nFrame = 20;

	// ��ü ���� ����
	int nextBrick = random(sizeof(Shape) / sizeof(Shape[0]));
	while (1) {
		brick = nextBrick;
		/* if (isBlockChangeItemEnable != 1)
		{
			brick = nextBrick;
		}
		else
		{
			brick = Shape[1];
			isBlockChangeItemEnable = 0;
		} */
		nextBrick = random(sizeof(Shape) / sizeof(Shape[0]));
		DrawNext(nextBrick);
		nx = BW / 2;
		ny = 3;
		rot = 0;
		PrintBrick(TRUE);

		// 5% Ȯ���� ������ ���� (��, �������� �ִ� ������ 2��.)
		//���� �̿��Ͽ� ���� itemList
		if (random(5) == 0 && itemListSize() < 2)
		{
			switch (random(4))
			{
			case 0:
			{
				itemListPush(0);
				//�ð� ���� ������
				break;
			}
			case 1:
			{
				itemListPush(1);
				// �ӵ� ���� ������
				break;
			}
			case 2:
			{
				itemListPush(2);
				// �� ��ȯ ������
				break;
			}
			case 3:
			{
				itemListPush(3);
				//���� Ŭ���� ������
				break;
			}
			}
			DrawItemList();
		}

		if (GetAround(nx, ny, brick, rot) != EMPTY) break;

		// ���� �ϳ��� �ٴڿ� ���� �������� ����
		int time = 0;
		nStay = nFrame;
		for (; 2;) {
			if (--nStay == 0) {
				nStay = nFrame;
				if (MoveDown()) break;
			}
			if (ItemProcessKey(itemList)) break;
			delay(1000 / 20);
			if (IsTimerOn)
			{
				GameDelay(&time, 5000);
			}
		}

		// ���� �ٴڿ� ����� �� ������ ��� ���� Ȯ��
		if (isItemUsed == 1) {
			isLineItemEnable = 1;
			TestFull();
			isItemUsed = 0; // ������ ��� �� ��� ���� �ʱ�ȭ
		}
	}
	DrawGameOver();
	selectMenu();
}

void itemListPush(int data)
{
	if (Top >= 2) {
		return;
	}

	itemList[Top] = data;
	Top++;
}

int itemListPop()
{
	if (Top == 0) {
		return;
	}

	int temp = itemList[0];

	for (int i = 0; i < Top - 1; i++)
	{
		itemList[i] = itemList[i + 1];
	}

	Top--;

	return temp;
}


int itemListSize()
{
	return Top;
}

void itemListSwap()
{
	if (Top < 2)
	{
		return;
	}
	int temp = itemList[0];
	itemList[0] = itemList[Top - 1];
	itemList[Top - 1] = temp;
}

void DrawItemList()
{
	int i = 0;
	int line = 13;
	putsxy(50, line, "                                     ");
	putsxy(50, line + 1, "                                      ");
	// ������ �ؽ�Ʈ �ʱ�ȭ �� �����
	for (int i = 0; i < itemListSize(); i++) {
		if (itemListSize() == 0) {
			break;
		}
		switch (itemList[i])
		{
		case 0:
			putsxy(50, line, "�ð� ���� ������");
			break;

		case 1:
			putsxy(50, line, "�ӵ� ���� ������");
			break;

		case 2:
			putsxy(50, line, "�� ��ȯ ������");
			break;

		case 3:
			putsxy(50, line, "���� Ŭ���� ������");
			break;
		}
		line++;  // ���� �ٷ� �̵�
	}
}

void DrawScreen()
{
	for (int x = 0; x < BW + 2; x++) {
		for (int y = 0; y < BH + 2; y++) {
			putsxy(BX + x * 2, BY + y, arTile[board[x][y]]);
		}
	}

}

void DrawScreen2()
{
	for (int x = 0; x < BW + 2; x++) {
		for (int y = 1; y < BH + 2; y++) {
			putsxy(BX2 + x * 2, BY + y, arTile[board2[x][y]]);
		}
	}
}

void DrawNext(int nextBrick)
{
	int x, y, i;
	putsxy(34, 1, "< NEXT >");
	for (x = 34; x <= 42; x += 2) {
		for (y = 3; y <= 6; y++) {
			putsxy(x, y, arTile[EMPTY]);
		}
	}
	for (i = 0; i < 4; i++) {
		putsxy(38 + (Shape[nextBrick][0][i].x) * 2, 5 + Shape[nextBrick][0][i].y, arTile[BRICK]);
	}
}

int DrawGameOver()
{
	clrscr();
	putsxy(30, 11, "G A M E  O V E R");
	putsxy(30, 13, "�ƹ� Ű�� ������ �޴� ȭ������ ���ư��ϴ�.")
		int ch = getch();
	if (ch == ' ' || ch == '\r')
	{
		clrscr();
		return;
	}
	return;
}

void multiPlayGame()
{
	int nFrame, nStay;
	int x, y;

	randomize();
	clrscr();

	// �����ڸ��� ��, �������� �� �������� �ʱ�ȭ�Ѵ�.
	for (x = 0; x < BW + 2; x++) {
		for (y = 0; y < BH + 2; y++) {
			if (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) {
				board[x][y] = WALL;
				board2[x][y] = WALL;
			}
			else {
				board[x][y] = EMPTY;
				board2[x][y] = EMPTY;
			}
		}
	}
	DrawScreen();
	DrawScreen2();

	putsxy(50, 3, "Normal Mode");
	putsxy(50, 5, "How To Play?")
		putsxy(50, 6, "�¿�:�̵�, ��:ȸ��, �Ʒ�:����");
	putsxy(50, 7, "����:���� ����");
	nFrame = 20;

	// ��ü ���� ����
	int nextBrick1 = random(sizeof(Shape) / sizeof(Shape[0]));
	int nextBrick2 = random(sizeof(Shape) / sizeof(Shape[0]));
	while (1) {
		brick = nextBrick1;
		brick2 = nextBrick2;
		nextBrick1 = random(sizeof(Shape) / sizeof(Shape[0]));
		nextBrick2 = random(sizeof(Shape) / sizeof(Shape[0]));
		//DrawNext ����
		nx = BW / 2;
		ny = 3;
		rot = 0;
		PrintBrick(TRUE);
		nx2 = BW / 2;
		ny2 = 3;
		rot2 = 0;
		PrintBrick2(TRUE);

		if (GetAround(nx, ny, brick, rot) != EMPTY || GetAround2(nx2, ny2, brick2, rot2) != EMPTY) break;

		// ���� �ϳ��� �ٴڿ� ���� �������� ����
		nStay = nFrame;
		for (; 2;) {
			if (--nStay == 0) {
				nStay = nFrame;
				if (MoveDown()) break;
				if (MoveDown2()) break;
			}
			//if (ProcessKey()) break;
			if (ProcessKey2()) break;
			delay(1000 / 20);
		}

	}
	DrawGameOver();
	selectMenu();
}


BOOL ProcessKey()
{
	if (kbhit()) {
		int ch = getch();
		if (ch == 0xE0 || ch == 0) {
			ch = getch();
			switch (ch) {
			case LEFT:
				if (GetAround(nx - 1, ny, brick, rot) == EMPTY) {
					PrintBrick(FALSE);
					nx--;
					PrintBrick(TRUE);
				}
				break;
			case RIGHT:
				if (GetAround(nx + 1, ny, brick, rot) == EMPTY) {
					PrintBrick(FALSE);
					nx++;
					PrintBrick(TRUE);
				}
				break;
			case UP:
				if (GetAround(nx, ny, brick, (rot + 1) % 4) == EMPTY) {
					PrintBrick(FALSE);
					rot = (rot + 1) % 4;
					PrintBrick(TRUE);
				}
				break;
			case DOWN:
				if (MoveDown()) {
					return TRUE;
				}
				break;
			}
		}
		else {
			switch (ch) {
			case ' ':
				while (MoveDown() == FALSE) { ; }
				return TRUE;
			case ESC:
				clrscr();
				selectMenu();
				break;
			}
		}
	}
	return FALSE;
}

//������ ����� ��� �� �Լ��� �Է��� ����.
BOOL ItemProcessKey(int* itemList)
{
	if (kbhit()) {
		int ch = getch();
		if (ch == 0xE0 || ch == 0) {
			ch = getch();
			switch (ch) {
			case LEFT:
				if (GetAround(nx - 1, ny, brick, rot) == EMPTY) {
					PrintBrick(FALSE);
					nx--;
					PrintBrick(TRUE);
				}
				break;
			case RIGHT:
				if (GetAround(nx + 1, ny, brick, rot) == EMPTY) {
					PrintBrick(FALSE);
					nx++;
					PrintBrick(TRUE);
				}
				break;
			case UP:
				if (GetAround(nx, ny, brick, (rot + 1) % 4) == EMPTY) {
					PrintBrick(FALSE);
					rot = (rot + 1) % 4;
					PrintBrick(TRUE);
				}
				break;
			case DOWN:
				if (MoveDown()) {
					return TRUE;
				}
				break;
			}
		}
		else {
			switch (ch) {
			case ' ':
				while (MoveDown() == FALSE) { ; }
				return TRUE;

			case 'z':
			case 'Z':
				if (itemListSize() != 0) {
					activateItem(itemListPop());
				}
				break;

			case 'x':
			case 'X':
				itemListSwap();
				DrawItemList();
				break;

			case ESC:
				clrscr();
				selectMenu();
				break;
			}
		}
	}
	return FALSE;
}

BOOL MoveDown2()
{
	// �ٴڿ� ������� ����á���� �����ϰ� TRUE�� �����Ѵ�.
	if (GetAround2(nx2, ny2 + 1, brick2, rot2) != EMPTY) {
		TestFull2();
		return TRUE;
	}
	// ���� ���߿� �� ������ ��ĭ �Ʒ��� ������.
	PrintBrick2(FALSE);
	ny2++;
	PrintBrick2(TRUE);
	return FALSE;
}

BOOL ProcessKey2()
{
	if (kbhit()) {
		int ch = getch();
		if (ch == 0xE0 || ch == 0) {
			ch = getch();
			switch (ch) {
			case LEFT:
				if (GetAround2(nx2 - 1, ny2, brick2, rot2) == EMPTY) {
					PrintBrick2(FALSE);
					nx2--;
					PrintBrick2(TRUE);
				}
				break;
			case RIGHT:
				if (GetAround2(nx2 + 1, ny2, brick2, rot2) == EMPTY) {
					PrintBrick2(FALSE);
					nx2++;
					PrintBrick2(TRUE);
				}
				break;
			case UP:
				if (GetAround2(nx2, ny2, brick2, (rot2 + 1) % 4) == EMPTY) {
					PrintBrick2(FALSE);
					rot2 = (rot2 + 1) % 4;
					PrintBrick2(TRUE);
				}
				break;
			case DOWN:
				if (MoveDown2()) {
					return TRUE;
				}
				break;
			}
		}
		else {
			switch (ch) {
			case 'C':
			case 'c':
				while (MoveDown() == FALSE) { ; }
				return TRUE;
			case 'A':
			case 'a':
				if (GetAround(nx - 1, ny, brick, rot) == EMPTY) {
					PrintBrick(FALSE);
					nx--;
					PrintBrick(TRUE);
				}
				break;
			case 'D':
			case 'd':
				if (GetAround(nx + 1, ny, brick, rot) == EMPTY) {
					PrintBrick(FALSE);
					nx++;
					PrintBrick(TRUE);
				}
				break;
			case 'W':
			case 'w':
				if (GetAround(nx, ny, brick, (rot + 1) % 4) == EMPTY) {
					PrintBrick(FALSE);
					rot = (rot + 1) % 4;
					PrintBrick(TRUE);
				}
				break;
			case 'S':
			case 's':
				if (MoveDown()) {
					return TRUE;
				}
				break;
			case 'l':
			case 'L':
				while (MoveDown2() == FALSE) { ; }
				return TRUE;
			case ESC:
				clrscr();
				selectMenu();
				break;
			}
		}
	}
	return FALSE;
}


void activateItem(int item)
{
	switch (1)
	{
	case 0:
		//3�� pause
		putsxy(50, 18, "�ð� ���� �������� ���Ǿ����ϴ�.            ");
		delay(3000);
		DrawItemList();
		break;

	case 1:
		putsxy(50, 18, "�ӵ� ���� �������� ���Ǿ����ϴ�.            ");
		IsTimerOn = 1;
		DrawItemList();
		break;

	case 2:
		putsxy(50, 18, "�� ��ȯ �������� ���Ǿ����ϴ�.             ");
		isBlockChangeItemEnable = 1;
		DrawItemList();
		break;

	case 3:
		putsxy(50, 18, "���� Ŭ���� �������� ���Ǿ����ϴ�.          ");
		isItemUsed = 1;
		DrawItemList();
		break;
	}
}

void PrintBrick(BOOL Show)
{
	for (int i = 0; i < 4; i++) {
		gotoxy(BX + (Shape[brick][rot][i].x + nx) * 2, BY + Shape[brick][rot][i].y + ny);
		puts(arTile[Show ? BRICK : EMPTY]);
	}
}

void PrintBrick2(BOOL Show)
{
	for (int i = 0; i < 4; i++) {
		gotoxy(BX2 + (Shape[brick2][rot2][i].x + nx2) * 2, BY + Shape[brick2][rot2][i].y + ny2);
		puts(arTile[Show ? BRICK : EMPTY]);
	}
}

int GetAround(int x, int y, int b, int r)
{
	int k = EMPTY;

	// ������ ������ Ÿ�� ��� �� ���� ū ���� ã�´�.
	for (int i = 0; i < 4; i++) {
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	}
	return k;
}

int GetAround2(int x, int y, int b, int r)
{
	int k = EMPTY;

	// ������ ������ Ÿ�� ��� �� ���� ū ���� ã�´�.
	for (int i = 0; i < 4; i++) {
		k = max(k, board2[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	}
	return k;
}

BOOL MoveDown()
{
	// �ٴڿ� ������� ����á���� �����ϰ� TRUE�� �����Ѵ�.
	if (GetAround(nx, ny + 1, brick, rot) != EMPTY) {
		TestFull();
		return TRUE;
	}
	// ���� ���߿� �� ������ ��ĭ �Ʒ��� ������.
	PrintBrick(FALSE);
	ny++;
	PrintBrick(TRUE);
	return FALSE;
}

void TestFull()
{
	// �ٴڿ� �������� ���� ���
	for (int i = 0; i < 4; i++) {
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = BRICK;
	}

	// isLineItemEnable�� Ȱ��ȭ�Ǿ��� �� ���� �Ʒ��� �ִ� ���� ����
	if (isLineItemEnable != 0) {
		for (int x = 1; x < BW + 1; x++) {
			board[x][BH] = EMPTY;
		}
		for (int ty = BH; ty > 1; ty--) {
			for (int x = 1; x < BW + 1; x++) {
				board[x][ty] = board[x][ty - 1];
			}
		}
		for (int x = 1; x < BW + 1; x++) {
			board[x][1] = EMPTY;  // ���� ������ ���� ����־�� ��
		}
		// ������ ��� �� isLineItemEnable�� �ٽ� 0���� ����
		isLineItemEnable = 0;
		DrawScreen();
		delay(200);
	}

	// �������� ������ ���� ����
	for (int y = 1; y < BH + 1; y++) {
		BOOL bFull = TRUE;
		for (int x = 1; x < BW + 1; x++) {
			if (board[x][y] == EMPTY) {
				bFull = FALSE;
				break;
			}
		}
		// �� ���� ���� á���� �� ���� �����Ѵ�.
		if (bFull) {
			for (int ty = y; ty > 1; ty--) {
				for (int x = 1; x < BW + 1; x++) {
					board[x][ty] = board[x][ty - 1];
				}
			}
			for (int x = 1; x < BW + 1; x++) {
				board[x][1] = EMPTY;  // ���� ������ ���� ����־�� ��
			}
			DrawScreen();
			delay(200);
		}
	}
}

void TestFull2()
{
	// �ٴڿ� �������� ���� ���
	for (int i = 0; i < 4; i++) {
		board2[nx2 + Shape[brick2][rot2][i].x][ny + Shape[brick2][rot2][i].y] = BRICK;
	}

	// isLineItemEnable�� Ȱ��ȭ�Ǿ��� �� ���� �Ʒ��� �ִ� ���� ����
	if (isLineItemEnable != 0) {
		for (int x = 1; x < BW + 1; x++) {
			board2[x][BH] = EMPTY;
		}
		for (int ty = BH; ty > 1; ty--) {
			for (int x = 1; x < BW + 1; x++) {
				board2[x][ty] = board2[x][ty - 1];
			}
		}
		for (int x = 1; x < BW + 1; x++) {
			board2[x][1] = EMPTY;  // ���� ������ ���� ����־�� ��
		}
		// ������ ��� �� isLineItemEnable�� �ٽ� 0���� ����
		isLineItemEnable = 0;
		DrawScreen2();
		delay(200);
	}

	// �������� ������ ���� ����
	for (int y = 1; y < BH + 1; y++) {
		BOOL bFull = TRUE;
		for (int x = 1; x < BW + 1; x++) {
			if (board2[x][y] == EMPTY) {
				bFull = FALSE;
				break;
			}
		}
		// �� ���� ���� á���� �� ���� �����Ѵ�.
		if (bFull) {
			for (int ty = y; ty > 1; ty--) {
				for (int x = 1; x < BW + 1; x++) {
					board2[x][ty] = board2[x][ty - 1];
				}
			}
			for (int x = 1; x < BW + 1; x++) {
				board2[x][1] = EMPTY;  // ���� ������ ���� ����־�� ��
			}
			DrawScreen2();
			delay(200);
		}
	}
}