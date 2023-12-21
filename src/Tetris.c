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

//키보드 입력 변수 정의
enum { CTRL = 17, ALT = 18, ESC = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };
#define putsxy(x, y, s) {gotoxy(x, y);puts(s);}
#define BX 5
#define BX2 70
#define BY 1
#define BW 10
#define BH 20

//게임 모드
void normalGame(); //기본 모드
void ItemGame(); //아이템 모드
void survivalGame(); //생존 모드
void multiPlayGame(); //2인 멀티플레이 모드
void initMultiPlayGame(); //멀티플레이 기능 로드
BOOL Player1(); //플레이어1 게임진행 함수
BOOL Player2(); //플레이어2 게임진행 함수

void selectMenu(); //메뉴 선택
void DrawItemGameRules(); //아이템 모드 규칙 표시
void DrawScreen(); //게임판 표시
int DrawGameOver(int gameScore, double gameTime); //게임 오버, 점수, 진행시간 출력
int DrawSurvivalGameOver(int blockCount); //생존 게임 전용 게임오버, 생성된 블럭 출력
int DrawMultiGameOver(int player); //멀티플레이 전용 게임오버, 어느 플레이어가 이겼는지 출력
BOOL ProcessKey(); //게임 키 입력 체크
BOOL ItemProcessKey(int* itemList); //아이템 모드 전용 입력 체크
void activateItem(int item); //아이템 사용 함수
void PrintBrick(BOOL Show); //블럭 출력 함수
void DrawNext(int nextBrick); //다음에 나올 블럭 보여주는 함수
int GetAround(int x, int y, int b, int r); //블럭 충돌 여부 체크
BOOL MoveDown(); //블럭 한 칸 내리는 함수
void TestFull(); //한 줄에 블럭이 가득 찼는지 확인

void itemListPush(int data); //아이템 추가
int itemListPop(); //아이템 사용 
int itemListSize(); //아이템이 몇개인지 반환
void itemListSwap(); //아이템 순서 변경
void DrawItemList(); //아이템 목록 출력

//Player2 함수
BOOL MoveDown2();
BOOL ProcessKey2(unsigned char ch);
void PrintBrick2(BOOL Show);
void DrawScreen2();
void TestFull2();
int GetAround2(int x, int y, int b, int r);

//좌표 정의 Point 구조체
struct Point {
	int x, y;
};

//벽돌 7개 정의, 회전값 포함
struct Point Shape[][4][4] = {
	{ { 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 },{ 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 } },
	{ { 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 } },
	{ { 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 },{ 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 } },
	{ { 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 },{ 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 } },
	{ { 0,0,-1,0,1,0,-1,-1 },{ 0,0,0,-1,0,1,-1,1 },{ 0,0,-1,0,1,0,1,1 },{ 0,0,0,-1,0,1,1,-1 } },
	{ { 0,0,1,0,-1,0,1,-1 },{ 0,0,0,1,0,-1,-1,-1 },{ 0,0,1,0,-1,0,-1,1 },{ 0,0,0,-1,0,1,1,1 } },
	{ { 0,0,-1,0,1,0,0,1 },{ 0,0,0,-1,0,1,1,0 },{ 0,0,-1,0,1,0,0,-1 },{ 0,0,-1,0,0,-1,0,1 } },
};

enum { EMPTY, BRICK, WALL }; //게임판 상태
char arTile[3][4] = { ". ","■","□" }; //블럭 모양
int board[BW + 2][BH + 2]; //전체 게임판 저장 배열
int board2[BW + 2][BH + 2]; //플레이어2 게임판 저장 배열
int nx, ny, nx2, ny2; //좌표값
int brick, rot, brick2, rot2; //벽돌번호, 회전모양

//속도 지연 아이템 관련 함수, 변수
void GameDelay();
int IsTimerOn = 0;

//itemListStack
int itemList[2];
int Top = 0;

int isLineItemEnable = 0; //라인 클리어 아이템 활성화 변수
int isItemUsed = 0; //라인 클리어 아이템이 사용되었는지

int nFrame1, nStay1, nFrame2, nStay2; //게임 속도
int nextBrick1, nextBrick2; //다음에 표시될 블럭 저장
int game1, game2; //멀티플레이 게임 실행 여부
int finish1, finish2; //멀티플레이 게임이 끝났는지 확인
int flag1, flag2; //멀티플레이 게임 플래그
clock_t multiStartTime; //멀티플레이 시작 시간
double multiTime; //멀티플레이 진행 시간

int score; //게임 점수 관련

int main()
{
	selectMenu();

	return 0;
}

//게임 메뉴를 보여주는 함수
void selectMenu()
{
	showcursor(FALSE); //커서를 보여주지 않는 매크로함수
	clrscr(); //화면 클리어

	putsxy(11, 5, "#####   #####   #####   ####    #####    ### ");
	putsxy(11, 6, "  #     #         #     #   #     #     #    ");
	putsxy(11, 7, "  #     ###       #     #   #     #      ### ");
	putsxy(11, 8, "  #     #         #     ##        #         #");
	putsxy(11, 9, "  #     #####     #     # ##    #####   #### ")

	Top = 0;

	//메뉴 출력
	gotoxy(24 - 2, 12);
	printf("> 기본 모드");
	gotoxy(24, 13);
	printf("아이템 모드");
	gotoxy(24, 14);
	printf("생존 모드");
	gotoxy(24, 15);
	printf("2인 멀티플레이 모드");
	gotoxy(24, 16);
	printf("게임 종료 (ESC)");

	//화면상의 x, y 좌표를 지정
	int x = 24;
	int y = 12;
	int menuNum; //메뉴 번호를 저장하는 변수
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
		normalGame(); //기본 모드 진입
		break;
	}

	case 1:
	{
		ItemGame(); //아이템 모드 진입
		break;
	}
	case 2:
	{
		survivalGame(); //생존 모드 진입
		break;
	}
	case 3:
	{
		multiPlayGame(); //2인 멀티플레이 모드 진입
		break;
	}
	case 4:
	{
		exit(0); //게임 종료
	}
	}
}

//아이템 모드의 규칙을 표시하는 함수
void DrawItemGameRules()
{
	clrscr();
	putsxy(40, 5, "아이템의 기능을 확인하세요.");
	putsxy(30, 7, "시간 정지 아이템: 3초 동안 게임이 정지됩니다.");
	putsxy(30, 8, "속도 지연 아이템: 일정 시간 동안 게임 속도가 지연됩니다.");
	putsxy(30, 9, "점수 보너스 아이템: 현재 점수에서 10000점이 추가됩니다.");
	putsxy(30, 10, "라인 클리어 아이템: 끝 라인 1줄이 제거됩니다.");
	putsxy(30, 11, "블럭 내려가면 50점, 한줄 제거되면 500점 추가");
	putsxy(30, 13, "아무 키를 눌러 게임을 시작하세요.")

	int ch = getch();
	if (ch == ' ' || ch == '\r')
	{
		return;
	}
}

//기본 모드 함수 정의
void normalGame()
{
	int nFrame, nStay; //게임 틱
	int x, y; //좌표
	double timer; //시간 저장

	randomize(); //난수 초기화
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
	DrawScreen(); //게임판을 출력하는 함수

	putsxy(50, 3, "Normal Mode");
	putsxy(50, 5, "How To Play?")
		putsxy(50, 6, "좌우:이동, 위:회전, 아래:내림");
	putsxy(50, 7, "공백:전부 내림");
	putsxy(50, 8, "블럭 내려가면 50점, 한줄 제거되면 500점 추가");
	nFrame = 20; //50ms * 20 = 1000ms(1초)
	score = 0; //점수 초기화
	clock_t startTime = clock(); //게임 시작 시간 저장

	// 전체 게임 루프
	int nextBrick = random(sizeof(Shape) / sizeof(Shape[0])); //게임시작 전 먼저 블럭을 생성
	while (1) {
		brick = nextBrick;
		nextBrick = random(sizeof(Shape) / sizeof(Shape[0])); //그 다음에 나올 블럭을 생성
		DrawNext(nextBrick); //다음에 나올 블럭 미리보기 함수
		//블럭의 좌표, 회전 여부 저장 변수
		nx = BW / 2;
		ny = 3;
		rot = 0;
		PrintBrick(TRUE); //블럭 표시 함수

		if (GetAround(nx, ny, brick, rot) != EMPTY) break; //블럭의 위치가 다른 블럭이나 게임판 밖에 있다면 동작을 취소

		// 벽돌 하나가 바닥에 닿을 때까지의 루프
		nStay = nFrame;
		for (; 2;) {
			if (--nStay == 0) { //1초 카운트
				nStay = nFrame;
				if (MoveDown()) {
					break;
				}
			}
			clock_t endTime = clock(); //현재 시간 체크
			gotoxy(50, 10);
			timer = (double)(endTime - startTime) / CLOCKS_PER_SEC; //게임 진행시간 표시
			printf("%.3lf초", timer);
			gotoxy(50, 11);
			printf("점수: %d", score); //게임 점수 표시
			if (ProcessKey()) break; //입력을 받으면 for문 탈출
			delay(1000 / 20); //50ms delay
		}

	}
	DrawGameOver(score, timer); //게임 오버 화면 출력 함수
	selectMenu(); //메뉴 선택 함수
}

//게임에 딜레이를 주는 함수 (속도 지연 아이템과 관련)
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

//아이템 모드 함수 정의
void ItemGame()
{
	int nFrame, nStay;
	int x, y;

	DrawItemGameRules();

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
	score = 0;
	clock_t startTime = clock();
	double timer;

	// 전체 게임 루프
	int nextBrick = random(sizeof(Shape) / sizeof(Shape[0]));
	while (1) {
		brick = nextBrick;
		nextBrick = random(sizeof(Shape) / sizeof(Shape[0]));
		DrawNext(nextBrick);
		nx = BW / 2;
		ny = 3;
		rot = 0;
		PrintBrick(TRUE);

		// 5% 확률로 아이템 생성 (단, 아이템의 최대 개수는 2개.)
		if (random(5) == 0 && itemListSize() < 2)
		{
			switch (random(4))
			{
			case 0:
			{
				itemListPush(0); //itemList의 마지막 인덱스에 값을 추가하는 함수
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
				// 점수 보너스 아이템
				break;
			}
			case 3:
			{
				itemListPush(3);
				//라인 클리어 아이템
				break;
			}
			}
			DrawItemList();
		}

		if (GetAround(nx, ny, brick, rot) != EMPTY) break;

		int time = 0; //GameDelay 함수에 딜레이 시간값을 넘겨주는 변수

		// 벽돌 하나가 바닥에 닿을 때까지의 루프
		nStay = nFrame;
		for (; 2;) {
			if (--nStay == 0) {
				nStay = nFrame;
				if (MoveDown()) break;
			}
			if (ItemProcessKey(itemList)) break;
			clock_t endTime = clock();
			gotoxy(50, 9);
			timer = (double)(endTime - startTime) / CLOCKS_PER_SEC;
			printf("%.3lf초", timer);
			gotoxy(50, 10);
			printf("점수: %d", score);
			delay(1000 / 20);
			if (IsTimerOn) //속도 지연 아이템을 사용하였다면?
			{
				GameDelay(&time, 5000); //약 3초 딜레이
			}
		}

		// 블럭이 바닥에 닿았을 때 아이템 사용 여부 확인
		if (isItemUsed == 1) {
			isLineItemEnable = 1;
			TestFull();
			isItemUsed = 0; // 아이템 사용 후 사용 여부 초기화
		}
	}
	DrawGameOver(score, timer);
	selectMenu();
}

//itemList 배열의 마지막 인덱스에 값을 저장하는 함수
void itemListPush(int data)
{
	if (Top >= 2) {
		return;
	}

	itemList[Top] = data;
	Top++;
}

//itemList 배열의 첫번째 인덱스의 값을 꺼내고
//뒤의 인덱스를 앞으로 한칸씩 땡기는 로직
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

//itemList 배열의 크기를 반환하는 함수
int itemListSize()
{
	return Top;
}

//아이템의 순서를 변경하는 함수
void itemListSwap()
{
	if (Top < 2)
	{
		return;
	}
	//swap 구현
	int temp = itemList[0];
	itemList[0] = itemList[Top - 1];
	itemList[Top - 1] = temp;
}

//아이템 목록을 출력하는 함수
void DrawItemList()
{
	int i = 0;
	int line = 13;
	//아이템 텍스트 초기화 후 덮어쓰기
	putsxy(50, line, "                                     ");
	putsxy(50, line + 1, "                                      ");
	for (int i = 0; i < itemListSize(); i++) {
		//아이템이 없다면 실행하지 않음
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
			putsxy(50, line, "점수 보너스 아이템");
			break;

		case 3:
			putsxy(50, line, "라인 클리어 아이템");
			break;
		}
		line++;  //다음 줄로 이동
	}
}

//생존 게임 함수 정의
void survivalGame()
{
	int nFrame, nStay;
	int x, y;
	double timer;

	score = 0;

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

	putsxy(50, 3, "Survival Mode");
	putsxy(50, 5, "How To Play?")
		putsxy(50, 6, "좌우:이동, 위:회전, 아래:내림");
	putsxy(50, 7, "공백:전부 내림");
	putsxy(50, 8, "블럭 5개 생성될 때마다 가속도 증가");
	nFrame = 20;

	// 전체 게임 루프
	int nextBrick = random(sizeof(Shape) / sizeof(Shape[0]));
	int brickCount = 0; //생성된 블럭의 개수를 카운트하는 변수
	int accumulation_brickcount = 0;
	int speedIncreaseInterval = 5; //속도 증가 간격 설정
	int initialFrameDelay = 50; //초기 nFrame = 50(ms)
	int frameDelay = initialFrameDelay; //현재 프레임 딜레이 설정

	clock_t startTime = clock();

	while (1) {
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
		for (; 2;) {
			if (--nStay == 0) {
				nStay = nFrame;
				if (MoveDown()) break;
			}
			if (ProcessKey()) break;
			clock_t endTime = clock();
			timer = (double)(endTime - startTime) / CLOCKS_PER_SEC;
			gotoxy(50, 10);
			printf("%.3lf초", timer);
			gotoxy(50, 11);
			printf("점수: %d", score);
			gotoxy(50, 12);
			printf("가속도 레벨: %d", (accumulation_brickcount / 5) + 1);
			delay(frameDelay);
		}

		brickCount++; //블럭 개수 카운트
		score += 50; //점수 증가
		accumulation_brickcount++; //누적 블럭 개수 카운트

		if (brickCount >= speedIncreaseInterval) {
			frameDelay = frameDelay * 0.8; // 프레임 딜레이 감소 (속도 증가)
			brickCount = 0; // 블럭 개수 초기화
		}
	}

	clrscr();
	DrawSurvivalGameOver(accumulation_brickcount, timer); //생존 모드 전용 게임오버 함수
	selectMenu();
}



//게임판을 그리는 함수
void DrawScreen()
{
	for (int x = 0; x < BW + 2; x++) {
		for (int y = 0; y < BH + 2; y++) {
			putsxy(BX + x * 2, BY + y, arTile[board[x][y]]);
		}
	}

}

//플레이어2의 게임판을 그리는 함수
void DrawScreen2()
{
	for (int x = 0; x < BW + 2; x++) {
		for (int y = 0; y < BH + 2; y++) {
			putsxy(BX2 + x * 2, BY + y, arTile[board2[x][y]]);
		}
	}
}

//다음에 나올 블럭을 미리 보여주는 함수
void DrawNext(int nextBrick)
{
	int x, y, i;
	putsxy(34, 2, "< NEXT >");
	//5 x 4 블럭 출력판 생성
	for (x = 34; x <= 42; x += 2) {
		for (y = 3; y <= 6; y++) {
			putsxy(x, y, arTile[EMPTY]);
		}
	}
	//블럭 그리기
	for (i = 0; i < 4; i++) {
		putsxy(38 + (Shape[nextBrick][0][i].x) * 2, 5 + Shape[nextBrick][0][i].y, arTile[BRICK]);
	}
}

//플레이어2의 다음에 나올 블럭을 미리 보여주는 함수
void DrawNext2(int nextBrick)
{
	int x, y, i;
	putsxy(100, 2, "< NEXT >");
	for (x = 100; x <= 108; x += 2) {
		for (y = 3; y <= 6; y++) {
			putsxy(x, y, arTile[EMPTY]);
		}
	}
	for (i = 0; i < 4; i++) {
		putsxy(104 + (Shape[nextBrick][0][i].x) * 2, 5 + Shape[nextBrick][0][i].y, arTile[BRICK]);
	}
}

//게임 오버 화면 출력 함수
int DrawGameOver(int gameScore, double gameTime)
{
	clrscr();
	putsxy(30, 10, "G A M E  O V E R");
	gotoxy(30, 11);
	//점수와 게임 진행 시간을 표시
	printf("점수: %d점\t시간: %.3lf초", gameScore, gameTime);
	putsxy(30, 13, "아무 키를 누르면 메뉴 화면으로 돌아갑니다.")
	int ch = getch();
	if (ch == ' ' || ch == '\r')
	{
		clrscr();
		return;
	}
	return;
}

//생존 게임 전용 게임오버 함수
int DrawSurvivalGameOver(int blockCount, double gameTime)
{
	clrscr();
	putsxy(30, 11, "G A M E  O V E R");
	gotoxy(30, 12);
	printf("시간: %.3lf초\t가속도 레벨: %d", gameTime, (blockCount / 5) + 1);
	gotoxy(30, 13);
	printf("점수: %d\t생성된 블럭 수: %d", score, blockCount);
	putsxy(30, 15, "아무 키를 누르면 메뉴 화면으로 돌아갑니다.")
		int ch = getch();
	if (ch == ' ' || ch == '\r')
	{
		clrscr();
		return;
	}
	return;
}

//2인 멀티플레이 모드 함수 정의
void multiPlayGame()
{
	unsigned char ch;
	randomize();
	clrscr();
	initMultiPlayGame(); //멀티플레이 모드의 초기 설정을 불러옵니다.

	while (1) { //멀티플레이 게임 전체 루프
		if (finish1 || finish2) { //플레이어1과 플레이어2 둘 중 게임이 진행중(1)이면 계속 진행
			if (kbhit() == 1) {
				//키가 입력되었을 때
				ch = getch();
				if (game1 == 1) { //플레이어1의 게임이 끝나지 않았을 때
					Player1();
				}
				else
				{
					DrawMultiGameOver(1);
					break;
				}
				if (game2 == 1) { //플레이어2의 게임이 끝나지 않았을 때
					Player2();
				}
				else
				{
					DrawMultiGameOver(2);
					break;
				}
				ProcessKey2(ch);
			}
			else { //키를 입력받지 않았을 때
				if (game1 == 1) {
					Player1();
				}
				else
				{
					DrawMultiGameOver(1);
					break;
				}
				if (game2 == 1) {
					Player2();
				}
				else
				{
					DrawMultiGameOver(2);
					break;
				}
			}
		}
		else {
			break; //finish1과 finish2의 값이 0이면 루프를 나온다
					//하나의 플레이어가 게임 오버된 경우
		}
		delay(1000 / 20);
	}
	clrscr();
	selectMenu();
}

//멀티플레이 모드 전용 게임오버 함수
//매개변수로 어느 플레이어가 우승하였는지 받는다
int DrawMultiGameOver(int player)
{
	clrscr();
	putsxy(30, 9, "G A M E  O V E R");
	if (player == 1)
	{
		putsxy(30, 10, "Player 1 승리")
	}
	else if (player == 2)
	{
		putsxy(30, 10, "Player 2 승리");
	}
	else
	{
		putsxy(30, 10, "무승부");
	}

	gotoxy(30, 11);
	printf("시간: %.3lf초", multiTime);
	putsxy(30, 14, "아무 키를 누르면 메뉴 화면으로 돌아갑니다.")
		int ch = getch();
	if (ch == ' ' || ch == '\r')
	{
		clrscr();
		return;
	}
	return;
}

//멀티플레이 모드 작동에 필요한 함수
void initMultiPlayGame()
{
	int x, y;

	clrscr();

	putsxy(40, 14, "MultiPlay Mode");
	putsxy(30, 15, "Player 1: 왼쪽 게임판, WASD 방향키 사용");
	putsxy(30, 16, "Player 2: 오른쪽 게임판, 화살표 방향키 사용");
	putsxy(30, 17, "블럭 내림: C(Player 1), L(Player 2)");

	// 가장자리는 벽, 나머지는 빈 공간으로 초기화한다.
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
	//플레이어1의 게임판은 왼쪽, 플레이어2의 게임판은 오른쪽에 출력한다.
	DrawScreen(); 
	DrawScreen2();

	nStay1 = nFrame1 = 20;
	nStay2 = nFrame2 = 20;
	nextBrick1 = random(sizeof(Shape) / sizeof(Shape[0])); //플레이어1의 다음 블럭 저장
	nextBrick2 = random(sizeof(Shape) / sizeof(Shape[0])); //플레이어2의 다음 블럭 저장
	game1 = game2 = 1; //1이면 게임이 시작됨
	finish1 = finish2 = 1; //1이면 게임 진행중
	flag1 = flag2 = 0; //게임 flag 0으로 초기화

	multiStartTime = clock(); //게임 시작 시간 저장
}

//플레이어1 게임 동작 함수 정의
BOOL Player1()
{
	int i;
	if (flag1 == 0) {
		brick = nextBrick1; //초기 블럭 저장
		nextBrick1 = random(sizeof(Shape) / sizeof(Shape[0])); //다음에 나올 블럭 저장
		DrawNext(nextBrick1); //플레이어1의 다음에 나올 블럭 출력

		nx = BW / 2; //플레이어1 필드 중앙에 생성
		ny = 3; //시작 좌표 3 고정
		rot = 0; //회전값은 초기값인 0
		PrintBrick(TRUE);
		flag1 = 1; //블럭이 생성되었으므로 flag값을 다시 1로 설정

	}
	
	//블럭이 떨어질 곳이 비어있거나 벽이 아니라면 플레이어1 board에 블럭 저장
	if (GetAround(BW / 2, 3, brick, rot) != EMPTY) {
		for (i = 0; i < 4; i++) {
			board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = BRICK;
		}
		game1 = 0;
		return FALSE;
	}

	//게임 틱 카운트
	if (--nStay1 == 0) {
		nStay1 = nFrame1;
		if (MoveDown()) {
			return TRUE;
		}
		clock_t endMultiTime = clock();
		multiTime = (double)(endMultiTime - multiStartTime) / CLOCKS_PER_SEC;
		gotoxy(50, 4);
		//진행 시간 초 단위로 표시
		printf("진행 시간: %d초", (int)multiTime);
	}

	return TRUE;
}

//플레이어2 게임 동작 함수 정의
BOOL Player2()
{
	int i;

	if (flag2 == 0) {
		brick2 = nextBrick2;
		nextBrick2 = random(sizeof(Shape) / sizeof(Shape[0]));
		DrawNext2(nextBrick2);

		nx2 = BW / 2;
		ny2 = 3;
		rot2 = 0;
		PrintBrick2(TRUE);
		flag2 = 1;

	}
	if (GetAround2(BW / 2, 3, brick2, rot2) != EMPTY) {
		for (i = 0; i < 4; i++) {
			board2[nx2 + Shape[brick2][rot2][i].x][ny2 + Shape[brick2][rot2][i].y] = BRICK;
		}
		game2 = 0;
		return FALSE;
	}
	if (--nStay2 == 0) {
		nStay2 = nFrame2;
		if (MoveDown2()) {
			return TRUE;
		}
	}

	return TRUE;
}

//키 입력을 받는 함수
BOOL ProcessKey()
{
	if (kbhit()) { //키보드 입력이 들어왔다면?
		int ch = getch();
		if (ch == 0xE0 || ch == 0) {
			ch = getch();
			switch (ch) {
			case LEFT:
				if (GetAround(nx - 1, ny, brick, rot) == EMPTY) { //왼쪽으로 한칸 갈 경우를 가정, 비어있는지 확인
					PrintBrick(FALSE); //표시되어 있는 블럭을 지우고
					nx--; //왼쪽으로 한 칸 이동시켜서
					PrintBrick(TRUE); //이동한 좌표에 블럭 출력
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
				if (GetAround(nx, ny, brick, (rot + 1) % 4) == EMPTY) { //회전값은 0,1,2,3 순환해야 함.
					//변경된 회전값에 블럭이 비어있다면?
					PrintBrick(FALSE); //표시되어 있는 블럭을 지우고
					rot = (rot + 1) % 4; //전역변수 rot을 회전상태를 나타내는 값으로 저장
					PrintBrick(TRUE); //변경된 회전값 출력
				}
				break;
			case DOWN:
				if (MoveDown()) { //한칸 아래로 내리는 함수 호출 후 TRUE 반환
					return TRUE;
				}
				break;
			}
		}
		else {
			switch (ch) {
			case ' ':
				while (MoveDown() == FALSE) { ; } //블럭이 바닥에 닿지 않았으면 반복문으로 계속 한칸씩 내림
				return TRUE; //바닥에 닿으면 TRUE 반환
			case ESC:
				clrscr();
				selectMenu();
				break;
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

			//Z키(대소문자 구별 X)를 입력받으면 아이템 사용
			case 'z':
			case 'Z':
				if (itemListSize() != 0) {
					activateItem(itemListPop());
				}
				break;

			//X키(대소문자 구별 X)를 입력받으면 아이템 순서 변경
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

//플레이어2 전용 키보드 입력 함수
BOOL ProcessKey2(unsigned char ch)
{

	if (ch == 0xE0 || ch == 0) {
		ch = getch();
		switch (ch) {
		case LEFT:
			if (game2 == 1 && GetAround2(nx2 - 1, ny2, brick2, rot2) == EMPTY) {
				PrintBrick2(FALSE);
				nx2--;
				PrintBrick2(TRUE);
			}
			break;
		case RIGHT:
			if (game2 == 1 && GetAround2(nx2 + 1, ny2, brick2, rot2) == EMPTY) {
				PrintBrick2(FALSE);
				nx2++;
				PrintBrick2(TRUE);
			}
			break;
		case UP:
			if (game2 == 1 && GetAround2(nx2, ny2, brick2, (rot2 + 1) % 4) == EMPTY) {
				PrintBrick2(FALSE);
				rot2 = (rot2 + 1) % 4;
				PrintBrick2(TRUE);
			}
			break;
		case DOWN:
			if (game2 == 1) {
				if (MoveDown2()) {
					return TRUE;
				}
				break;
			}

		}
	}
	else {
		switch (ch) {
		case 'C':
		case 'c':
			if (game1 == 1) {
				while (MoveDown() == FALSE) { ; }
				return TRUE;
			}
			break;
		case 'A':
		case 'a':
			if (game1 == 1 && GetAround(nx - 1, ny, brick, rot) == EMPTY) {
				PrintBrick(FALSE);
				nx--;
				PrintBrick(TRUE);
			}
			break;
		case 'D':
		case 'd':
			if (game1 == 1 && GetAround(nx + 1, ny, brick, rot) == EMPTY) {
				PrintBrick(FALSE);
				nx++;
				PrintBrick(TRUE);
			}
			break;
		case 'W':
		case 'w':
			if (game1 == 1 && GetAround(nx, ny, brick, (rot + 1) % 4) == EMPTY) {
				PrintBrick(FALSE);
				rot = (rot + 1) % 4;
				PrintBrick(TRUE);
			}
			break;
		case 'S':
		case 's':
			if (game1 == 1) {
				if (MoveDown()) {
					return TRUE;
				}
			}
			break;
		case 'l':
		case 'L':
			if (game2 == 1) {
				while (MoveDown2() == FALSE) { ; }
				return TRUE;
			}
			break;
		case ESC:
			clrscr();
			selectMenu();
			break;
		}
	}

	return FALSE;
}

//아이템 사용 함수
void activateItem(int item)
{
	switch (item)
	{
	case 0:
		//3초 pause
		putsxy(50, 18, "시간 정지 아이템이 사용되었습니다.            ");
		delay(3000); //게임 3초 정지
		DrawItemList(); //아이템 목록 갱신
		break;

	case 1:
		putsxy(50, 18, "속도 지연 아이템이 사용되었습니다.            ");
		IsTimerOn = 1; //전역변수로 속도 지연 아이템이 사용됨을 알림
		DrawItemList();
		break;

	case 2:
		putsxy(50, 18, "점수 보너스 아이템이 사용되었습니다.             ");
		score += 10000; //10000점 추가
		DrawItemList();
		break;

	case 3:
		putsxy(50, 18, "라인 클리어 아이템이 사용되었습니다.          ");
		isItemUsed = 1;//라인 클리어 아이템 사용 전역변수
		DrawItemList();
		break;
	}
}

//블럭을 표시하거나 지우는 함수
void PrintBrick(BOOL Show)
{
	for (int i = 0; i < 4; i++) {
		gotoxy(BX + (Shape[brick][rot][i].x + nx) * 2, BY + Shape[brick][rot][i].y + ny);
		//블럭 모양 좌표를 실제 좌표로 바꿈
		puts(arTile[Show ? BRICK : EMPTY]); //블럭 표시 또는 지움
	}
}

//플레이어2의 블럭을 표시하거나 지우는 함수
void PrintBrick2(BOOL Show)
{
	for (int i = 0; i < 4; i++) {
		gotoxy(BX2 + (Shape[brick2][rot2][i].x + nx2) * 2, BY + Shape[brick2][rot2][i].y + ny2);
		puts(arTile[Show ? BRICK : EMPTY]);
	}
}

//블럭 주변 정보 조회 함수
//조사할 벽돌의 좌표값과 회전값을 받음
int GetAround(int x, int y, int b, int r)
{
	int k = EMPTY;

	// 벽돌이 차지한 타일 모양 중 가장 큰 값을 찾는다.
	for (int i = 0; i < 4; i++) { //벽돌 좌표 순회
		//EMPTY<BRICK<WALL 순
		//빈칸: EMPTY, 벽돌과 충돌: BRICK, 벽과 충돌: WALL
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	}
	return k;
}

//플레이어2 전용 블럭 주변 정보 조회 함수
int GetAround2(int x, int y, int b, int r)
{
	int k = EMPTY;

	// 벽돌이 차지한 타일 모양 중 가장 큰 값을 찾는다.
	for (int i = 0; i < 4; i++) {
		k = max(k, board2[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	}
	return k;
}

//블럭을 한 칸 내리는 함수 정의
BOOL MoveDown()
{
	// 바닥에 닿았으면 가득찼는지 점검하고 TRUE를 리턴한다.
	if (GetAround(nx, ny + 1, brick, rot) != EMPTY) {
		TestFull();
		flag1 = 0; //게임 플래그 0으로 초기화
		score += 50; //블럭이 바닥에 닿으면 50점 추가
		return TRUE;
	}
	// 아직 공중에 떠 있으면 한칸 아래로 내린다.
	PrintBrick(FALSE);
	ny++;
	PrintBrick(TRUE);
	return FALSE;
}

//플레이어2 전용 블럭 한칸 내리는 함수
BOOL MoveDown2()
{
	// 바닥에 닿았으면 가득찼는지 점검하고 TRUE를 리턴한다.
	if (GetAround2(nx2, ny2 + 1, brick2, rot2) != EMPTY) {
		TestFull2();
		flag2 = 0;
		return TRUE;
	}
	// 아직 공중에 떠 있으면 한칸 아래로 내린다.
	PrintBrick2(FALSE);
	ny2++;
	PrintBrick2(TRUE);
	return FALSE;
}

//블럭을 다 채운 줄 제거하는 함수
void TestFull()
{
	// 바닥에 내려앉은 벽돌 기록
	for (int i = 0; i < 4; i++) {
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = BRICK;
	}

	//라인 클리어 아이템 관련 코드
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
	for (int y = 1; y < BH + 1; y++) { //한줄 검사
		BOOL bFull = TRUE;
		for (int x = 1; x < BW + 1; x++) {
			if (board[x][y] == EMPTY) { //블럭이 하나라도 비어있다면 for문 탈출
				bFull = FALSE;
				break;
			}
		}
		// 한 줄이 가득 찼으면 이 줄을 제거한다.
		if (bFull) {
			for (int ty = y; ty > 1; ty--) { //위의 정보까지 저장해야 기존의 더미블럭을 제거하기 위한 조건?
				for (int x = 1; x < BW + 1; x++) {
					board[x][ty] = board[x][ty - 1]; //한 줄 내림
				}
			}
			for (int x = 1; x < BW + 1; x++) {
				board[x][1] = EMPTY;  // 새로 내려온 행은 비어있어야 함
			}
			score += 500; //한줄 삭제시 500점 추가
			DrawScreen(); //게임판 갱신
			delay(200);
		}
	}
}

//플레이어2 전용 블럭을 다 채운 줄 제거하는 함수
void TestFull2()
{
	// 바닥에 내려앉은 벽돌 기록
	for (int i = 0; i < 4; i++) {
		board2[nx2 + Shape[brick2][rot2][i].x][ny2 + Shape[brick2][rot2][i].y] = BRICK;
	}

	// isLineItemEnable이 활성화되었을 때 가장 아래에 있는 행을 삭제
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
			board2[x][1] = EMPTY;  // 새로 내려온 행은 비어있어야 함
		}
		// 아이템 사용 후 isLineItemEnable을 다시 0으로 설정
		isLineItemEnable = 0;
		DrawScreen2();
		delay(200);
	}

	// 수평으로 가득찬 벽돌 제거
	for (int y = 1; y < BH + 1; y++) {
		BOOL bFull = TRUE;
		for (int x = 1; x < BW + 1; x++) {
			if (board2[x][y] == EMPTY) {
				bFull = FALSE;
				break;
			}
		}
		// 한 줄이 가득 찼으면 이 줄을 제거한다.
		if (bFull) {
			for (int ty = y; ty > 1; ty--) {
				for (int x = 1; x < BW + 1; x++) {
					board2[x][ty] = board2[x][ty - 1];
				}
			}
			for (int x = 1; x < BW + 1; x++) {
				board2[x][1] = EMPTY;  // 새로 내려온 행은 비어있어야 함
			}
			DrawScreen2();
			delay(200);
		}
	}
}
