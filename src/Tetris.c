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

//게임 모드
void normalGame();
void ItemGame();

void selectMenu();
void howToPlay();
void DrawScreen();
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
char arTile[3][4] = { ". ","■","□" };
int board[BW + 2][BH + 2];
int nx, ny;
int brick, rot;
int BlockDelay = 1000;

//게임 틱
void GameDelay();
int IsTimerOn = 0;

//itemListStack
int itemList[2];
int Top = 0;

int isLineItemEnable = 0;
int isItemUsed = 0;

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
	printf("> 기본 모드");
	gotoxy(24, 13);
	printf("아이템 모드");
	gotoxy(24, 14);
	printf("생존 모드");
	gotoxy(24, 15);
	printf("2인 멀티플레이 모드");
	gotoxy(24, 16);
	printf("게임 방법");
	gotoxy(24, 17);
	printf("게임 종료 (ESC)");

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
	putsxy(50, 5, "게임 방법");

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

	// 가장자리는 벽, 나머지는 빈 공간으로 초기화한다.
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
	putsxy(50, 6, "좌우:이동, 위:회전, 아래:내림");
	putsxy(50, 7, "공백:전부 내림");
	nFrame = 20;

	// 전체 게임 루프
	int nextBrick = random(sizeof(Shape) / sizeof(Shape[0]));
	while(1) {
		brick = nextBrick;
		nextBrick = random(sizeof(Shape) / sizeof(Shape[0]));
		DrawNext(nextBrick);
		nx = BW / 2;
		ny = 3;
		rot = 0;
		PrintBrick(TRUE);

		if (GetAround(nx, ny, brick, rot) != EMPTY) break;

		// 벽돌 하나가 바닥에 닿을 때까지의 루프
		nStay = nFrame;
		for(; 2;) {
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

	// 가장자리는 벽, 나머지는 빈 공간으로 초기화한다.
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
		putsxy(50, 6, "좌우:이동, 위:회전, 아래:내림");
	putsxy(50, 7, "공백:전부 내림");
	putsxy(50, 8, "아이템 사용:Z, 아이템 순서 변경:X");
	putsxy(50, 12, "아이템 목록");
	nFrame = 20;

	// 전체 게임 루프
	for (; 1;) {
		brick = random(sizeof(Shape) / sizeof(Shape[0]));
		nx = BW / 2;
		ny = 3;
		rot = 0;
		PrintBrick(TRUE);

		// 5% 확률로 아이템 생성 (단, 아이템의 최대 개수는 2개.)
		//스택 이용하여 수정 itemList
		if (random(5) == 0 && itemListSize() < 2)
		{
			switch (random(4))
			{
			case 0:
			{
				itemListPush(0);
				//시간 정지 아이템
				break;
			}
			case 1:
			{
				itemListPush(1);
				// 속도 지연 아이템
				break;
			}
			case 2:
			{
				itemListPush(2);
				// 폭탄 아이템
				break;
			}
			case 3:
			{
				itemListPush(3);
				//다음 나올 블럭을 일자형 블럭으로 바꿔주는 아이템
				break;
			}
			}
			DrawItemList();
		}

		if (GetAround(nx, ny, brick, rot) != EMPTY) break;

		// 벽돌 하나가 바닥에 닿을 때까지의 루프
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

		// 블럭이 바닥에 닿았을 때 아이템 사용 여부 확인
		if (isItemUsed == 1) {
			isLineItemEnable = 1;
			TestFull();
			isItemUsed = 0; // 아이템 사용 후 사용 여부 초기화
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
	// 아이템 텍스트 초기화 후 덮어쓰기
	for (int i = 0; i < itemListSize(); i++) {
		if (itemListSize() == 0) {
			break;
		}
		switch (itemList[i])
		{
		case 0:
			putsxy(50, line, "시간 정지 아이템");
			break;

		case 1:
			putsxy(50, line, "속도 지연 아이템");
			break;

		case 2:
			putsxy(50, line, "폭탄 아이템");
			break;

		case 3:
			putsxy(50, line, "라인 클리어 아이템");
			break;
		}
		line++;  // 다음 줄로 이동
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

//아이템 모드의 경우 이 함수로 입력을 받음.
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
	switch (item)
	{
		// 시간 정지 아이템
	case 0:
		//3초 pause
		putsxy(50, 18, "시간 정지 아이템이 사용되었습니다.            ");
		delay(3000);
		DrawItemList();
		break;

		//속도 지연 아이템
	case 1:
		putsxy(50, 18, "속도 지연 아이템이 사용되었습니다.            ");
		IsTimerOn = 1;
		BlockDelay = 2000;
		DrawItemList();
		break;

		//폭탄 아이템
	case 2:
		putsxy(50, 18, "폭탄 아이템이 사용되었습니다.                 ");
		DrawItemList();
		break;

		//라인 클리어 아이템
	case 3:
		putsxy(50, 18, "라인 클리어 아이템이 사용되었습니다.          ");
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

int GetAround(int x, int y, int b, int r)
{
	int k = EMPTY;

	// 벽돌이 차지한 타일 모양 중 가장 큰 값을 찾는다.
	for (int i = 0; i < 4; i++) {
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	}
	return k;
}

BOOL MoveDown()
{
	// 바닥에 닿았으면 가득찼는지 점검하고 TRUE를 리턴한다.
	if (GetAround(nx, ny + 1, brick, rot) != EMPTY) {
		TestFull();
		return TRUE;
	}
	// 아직 공중에 떠 있으면 한칸 아래로 내린다.
	PrintBrick(FALSE);
	ny++;
	PrintBrick(TRUE);
	return FALSE;
}

void TestFull()
{
	// 바닥에 내려앉은 벽돌 기록
	for (int i = 0; i < 4; i++) {
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = BRICK;
	}

	// isLineItemEnable이 활성화되었을 때 가장 아래에 있는 행을 삭제
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
			board[x][1] = EMPTY;  // 새로 내려온 행은 비어있어야 함
		}
		// 아이템 사용 후 isLineItemEnable을 다시 0으로 설정
		isLineItemEnable = 0;
		DrawScreen();
		delay(200);
	}

	// 수평으로 가득찬 벽돌 제거
	for (int y = 1; y < BH + 1; y++) {
		BOOL bFull = TRUE;
		for (int x = 1; x < BW + 1; x++) {
			if (board[x][y] == EMPTY) {
				bFull = FALSE;
				break;
			}
		}
		// 한 줄이 가득 찼으면 이 줄을 제거한다.
		if (bFull) {
			for (int ty = y; ty > 1; ty--) {
				for (int x = 1; x < BW + 1; x++) {
					board[x][ty] = board[x][ty - 1];
				}
			}
			for (int x = 1; x < BW + 1; x++) {
				board[x][1] = EMPTY;  // 새로 내려온 행은 비어있어야 함
			}
			DrawScreen();
			delay(200);
		}
	}
}
