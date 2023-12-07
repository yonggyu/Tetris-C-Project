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
#define BY 1
#define BW 10
#define BH 20

//���� ���
void normalGame();
void ItemGame();

void selectMenu();
void howToPlay();
void DrawScreen();
BOOL ProcessKey();
BOOL ItemProcessKey(int* itemList);
void activateItem(int item);
void PrintBrick(BOOL Show);
int GetAround(int x, int y, int b, int r);
BOOL MoveDown();
void TestFull();

void itemListPush(int data);
int itemListPop();
int itemListSize();
void DrawItemList();

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
int nx, ny;
int brick, rot;
int BlockDelay = 1000;

//���� ƽ
void GameDelay();
int IsTimerOn = 0;

//itemListStack
int itemList[2];
int Top = 0;

int main()
{
	printf("\n\n\n\n");
	printf("           #####   #####   #####   ####    #####    ### \n");
	printf("             #     #         #     #   #     #     #    \n");
	printf("             #     ###       #     ####      #     #### \n");
	printf("             #     #         #     ##        #         #\n");
	printf("             #     #####     #     # ##    #####   #### \n");

	selectMenu();

}

void selectMenu()
{
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
	printf("���� ���");
	gotoxy(24, 17);
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
			if (y < 17)
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
	case 4:
	{
		howToPlay();
		break;
	}
	case 5:
	{
		exit(0);
	}
	}
}

void howToPlay()
{
	clrscr();
	putsxy(50, 5, "���� ���");

	int ch = getch();
	if (ch == ' ' || ch == '\r')
	{
		clrscr();
		selectMenu();
	}
}

void normalGame()
{
	int nFrame, nStay;
	int x, y;

	showcursor(FALSE);
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
	for (; 1;) {
		brick = random(sizeof(Shape) / sizeof(Shape[0]));
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
	clrscr();
	putsxy(30, 11, "G A M E  O V E R");
	showcursor(TRUE);
	selectMenu();
}

void GameDelay(int* time, int limitTime)
{
	*time += 100;

	if (*time > limitTime)
	{
		IsTimerOn = 0;
		*time = 0;
		BlockDelay = 1000;
		return;
	}
}

void ItemGame()
{
	int nFrame, nStay;
	int x, y;

	showcursor(FALSE);
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
	for (; 1;) {
		brick = random(sizeof(Shape) / sizeof(Shape[0]));
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
				// ��ź ������
				break;
			}
			case 3:
			{
				itemListPush(3);
				//���� ���� ���� ������ ������ �ٲ��ִ� ������
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
			delay(BlockDelay / 20);
			if (IsTimerOn)
			{
				GameDelay(&time, 5000);
			}
		}
	}
	clrscr();
	putsxy(30, 11, "G A M E  O V E R");
	showcursor(TRUE);
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
			putsxy(50, line, "��ź ������");
			break;

		case 3:
			putsxy(50, line, "������ �� ������");
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
				exit(0);
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

			case ESC:
				exit(0);
			}
		}
	}
	return FALSE;
}

void activateItem(int item)
{
	switch (1)
	{
		// �ð� ���� ������
	case 0:
		//3�� pause
		putsxy(50, 18, "�ð� ���� �������� ���Ǿ����ϴ�.            ");
		delay(3000);
		DrawItemList();
		break;

		//�ӵ� ���� ������
	case 1:
		putsxy(50, 18, "�ӵ� ���� �������� ���Ǿ����ϴ�.            ");
		IsTimerOn = 1;
		BlockDelay = 2000;
		DrawItemList();
		break;

		//��ź ������
	case 2:
		putsxy(50, 18, "��ź �������� ���Ǿ����ϴ�.                 ");
		DrawItemList();
		break;

		//������ �� ������
	case 3:
		putsxy(50, 18, "������ �� �������� ���Ǿ����ϴ�.          ");
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

int GetAround(int x, int y, int b, int r)
{
	int k = EMPTY;

	// ������ ������ Ÿ�� ��� �� ���� ū ���� ã�´�.
	for (int i = 0; i < 4; i++) {
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
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

	// �������� ������ ���� ����
	for (int y = 1; y < BH + 1; y++) {
		BOOL bFull = TRUE;
		for (int x = 1; x < BW + 1; x++) {
			if (board[x][y] == EMPTY) {
				bFull = FALSE;
				break;
			}
		}
		// ������ ���� á���� �� ���� �����Ѵ�.
		if (bFull) {
			for (int ty = y; ty > 1; ty--) {
				for (int x = 1; x < BW + 1; x++) {
					board[x][ty] = board[x][ty - 1];
				}
			}
			DrawScreen();
			delay(200);
		}
	}
}