#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<time.h>

#define WIDTH 78
#define HEIGHT 23

#define FREQ_FOOD_SPAWN 3
#define FREQ_SNAKE_MOVE 3

#define KEY_RIGHT 77
#define KEY_DOWN  80
#define KEY_UP    72
#define KEY_LEFT  75

#define FOOD      99
#define WALL      '@'
#define EMPTY     0

char screen[WIDTH + 1][HEIGHT + 1];

int headX, headY, tailX, tailY, length, neckX, neckY;

char isScreenChange = 0, isGameOver = 0, isSnakeMoved = 0, isFoodSpawned = 0;

void createFood() {
    
    if (time(NULL) % FREQ_FOOD_SPAWN != 0) {
        isFoodSpawned = 0;
        return;
    }
    
    if (isFoodSpawned) return;
    
    int foodLocation = random((WIDTH - 1) * (HEIGHT - 1) - length) + 1;
    
	for (int i = 1, k = 1; i <= WIDTH; i++)
		for(int j = 1; j <= HEIGHT; j++) {
            if (screen[i][j] == 0) {
                k++;
            }
            if (foodLocation == k) {
                screen[i][j] = FOOD;
                isFoodSpawned = 1;
                isScreenChange = 1;
            }
        }
}

void initializeScreen() {
	for(int i = 0; i < WIDTH; i++) 
		for(int j = 0; j < HEIGHT; j++) screen[i][j] = EMPTY;
}

void displayScreen() {
    if (isScreenChange == 0) return;
    
	clrscr();
	for(int i = 1; i <= WIDTH; i++) 
		for(int j = 1; j <= HEIGHT; j++) {
            int element = screen[i][j];
			gotoxy(i, j);
			if (element == KEY_UP || element == KEY_DOWN || 
                element == KEY_LEFT || element == KEY_RIGHT)
				printf("X");
            if (element == FOOD) printf("O");
            if (element == WALL) printf("%c", WALL);
		}
    gotoxy(1, HEIGHT + 1);
    printf("Score: %d", length - 6);
    isScreenChange = 0;
}

void moveTail() {
    int direction = screen[tailX][tailY];
    if (direction == KEY_RIGHT) {
        int newTailX = tailX >= WIDTH ? tailX: tailX + 1;
        screen[tailX][tailY] = 0;
        tailX = newTailX;
    }
    else if (direction == KEY_DOWN) {
        int newTailY = tailY >= HEIGHT ? tailY: tailY + 1;
        screen[tailX][tailY] = 0;
        tailY = newTailY;
    }
    else if (direction == KEY_UP) {
        int newTailY = tailY <= 1 ? tailY: tailY - 1;
        screen[tailX][tailY] = 0;
        tailY = newTailY;
    }
    else if (direction == KEY_LEFT) {
        int newTailX = tailX <= 1 ? tailX: tailX - 1;
        screen[tailX][tailY] = 0;
        tailX = newTailX;
    }
    isScreenChange = 1;
}

char moveHead() {
    int direction = screen[headX][headY];

    neckX = headX;
    neckY = headY;
    
    char front = 0;
    if      (direction == KEY_RIGHT && headX < WIDTH)  headX++;
    else if (direction == KEY_DOWN  && headY < HEIGHT) headY++;
    else if (direction == KEY_UP    && headY > 1)      headY--;
    else if (direction == KEY_LEFT  && headX > 1)      headX--;
    
    front = screen[headX][headY];
    screen[headX][headY] = direction;
    
    return front;
}

void moveSnake() {
    if(clock() % FREQ_SNAKE_MOVE != 0) {
        isSnakeMoved = 0;
        return;
    }
    
    if(isSnakeMoved) return;
    
    neckX = headX;
    neckY = headY;
    
    char front = moveHead();
    
    if (front == FOOD) length++;
    else if (front == WALL) isGameOver = 1;
    else if (
            front == KEY_LEFT || front == KEY_RIGHT || 
            front == KEY_DOWN || front == KEY_UP
        ) 
        isGameOver = 1;
    else if (front == EMPTY) moveTail();
    
    isSnakeMoved = 1;
    isScreenChange = 1;
}

void buildSnake() {
	screen[1][1] = screen[2][1] = screen[3][1] = 77;
	screen[5][1] = screen[6][1] = screen[4][1] = 77;
    
    headX = 6; 
    neckX = 5;
    tailX = 1;
    
    headY = neckY = tailY = 1;
     
    length = 6;
}

void faceSnake(char moveKey) {
    if      (moveKey == KEY_UP    && headY - 1 != neckY) screen[headX][headY] = moveKey;
    else if (moveKey == KEY_DOWN  && headY + 1 != neckY) screen[headX][headY] = moveKey;
    else if (moveKey == KEY_LEFT  && headX - 1 != neckX) screen[headX][headY] = moveKey;
    else if (moveKey == KEY_RIGHT && headX + 1 != neckX) screen[headX][headY] = moveKey;
}

void main(void) {
    char basicKey = 0, extKey = 0;
    srand(time(NULL) % 256);

	initializeScreen();
    buildSnake();
    
    isScreenChange = 1;
	displayScreen();
    while (basicKey != 'a') {
        int isHit = kbhit();
        basicKey = extKey = 0;

        moveSnake();
        createFood();
        if(isGameOver) break;

        if (!isHit) {
            displayScreen();
            continue;
        }

        basicKey = getch();
        extKey = !basicKey ? getch() : 0;

        faceSnake(extKey);

		displayScreen();
    }
    clrscr();
    printf("Game Over! Your Total Score is: %d", length - 6);
    getch();
}
