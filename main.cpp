#include <iostream>
#include <list>
#include <thread>
using namespace std;

#include <Windows.h>

int nScreenWidth = 120;
int nScreenHeight = 30;

struct sSnakeSegment {
	int x, y;
};

const int dx[] = { 0, 1, 0, -1 };
const int dy[] = { -1, 0, 1, 0 };
int main() {
	srand(time(NULL));
	// Create Screen Buffer. OneLoneCoder.com
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	while (1) {
		//enum eDirection {NORTH = 0, EAST, SOUTH, WEST};
		list<sSnakeSegment> snake = { {60, 15}, {61, 15}, {62, 15}, {63, 15}, {64, 15}, {65, 15}, {66, 15}, {67, 15}, {68, 15}, {69, 15} };
		int foodX = 50;
		int foodY = 15;
		int score = 0;
		bool isDead = false;
		//eDirection nSnakeDirection = WEST;
		int snakeDirection = 3;
		bool keyRight = false, keyLeft = false, keyUp = false, keyRightOld = false,
			keyLeftOld = false, keyUpOld = false, keyDown = false, keyDownOld = false;
		while (!isDead) {
			// Timing & Input
			//this::sleep_for(200ms);

			// Get Keyboard Input
			// Adjust game delay based on player direction
			auto t1 = chrono::system_clock::now();
			while ((chrono::system_clock::now() - t1) < ((snakeDirection % 2) ? 120ms : 200ms)) {
				/// TO DO: Add WASD support.
				/// Make the player be able to choose between right and left and this layout.
				keyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;
				keyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;
				keyUp = (0x8000 & GetAsyncKeyState((unsigned char)('\x26'))) != 0;
				keyDown = (0x8000 & GetAsyncKeyState((unsigned char)('\x28'))) != 0;

				if (keyRight && !keyRightOld) {
					if (snakeDirection == 0 || snakeDirection == 2) {
						snakeDirection = 1;
					}
				}
				if (keyLeft && !keyLeftOld) {
					if (snakeDirection == 0 || snakeDirection == 2) {
						snakeDirection = 3;
					}
				}
				if (keyUp && !keyUpOld) {
					if (snakeDirection == 1 || snakeDirection == 3) {
						snakeDirection = 0;
					}
				}
				if (keyDown && !keyDownOld) {
					if (snakeDirection == 1 || snakeDirection == 3) {
						snakeDirection = 2;
					}
				}

				keyRightOld = keyRight;
				keyLeftOld = keyLeft;
				keyUpOld = keyUp;
				keyDownOld = keyDown;
			}

			// Game Logic
			// Update Snake Position
			snake.push_front({ snake.front().x + dx[snakeDirection], snake.front().y + dy[snakeDirection] });

			// Collision Detection
			// Snake & Food
			if (snake.front().x == foodX && snake.front().y == foodY)
			{
				score++;
				while (screen[foodY * nScreenWidth + foodX] != L' ')
				{
					foodX = rand() % nScreenWidth;
					foodY = (rand() % (nScreenHeight - 3)) + 3;
				}

				for (int i = 0; i < 5; i++)
					snake.push_back({ snake.back().x, snake.back().y });
			}
			// Snake Against Itself
			for (list<sSnakeSegment>::iterator it = snake.begin(); it != snake.end(); it++)
				if (it != snake.begin() && (*it).x == snake.front().x && (*it).y == snake.front().y)
					isDead = true;

			// Snake & World
			if (snake.front().x < 0 || snake.front().x >= nScreenWidth)
				isDead = true;
			if (snake.front().y < 3 || snake.front().y >= nScreenHeight)
				isDead = true;

			// Remove Tail
			snake.pop_back();

			// Display Frame
			for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

			for (int i = 0; i < nScreenWidth; i++) {
				screen[i] = L'=';
				screen[2 * nScreenWidth + i] = L'=';
			}

			wsprintf(&screen[nScreenWidth + 5], L"S N A K E");
			//wsprintf(&screen[nScreenWidth + 50], L"SCORE: %d", score);
			wsprintf(&screen[nScreenWidth + 50], L"keyUp: %d", keyUp);
			wsprintf(&screen[nScreenWidth + 70], L"snakeDirection: %d", snakeDirection);
			//wsprintf(&screen[nScreenWidth + 50], L"foodX: %d", foodX);
			//wsprintf(&screen[nScreenWidth + 70], L"foodY: %d", foodY);



			// Draw Snake Body
			for (auto s : snake) {
				screen[s.y * nScreenWidth + s.x] = isDead ? L'+' : L'~';
			}
			// Draw Snake Head
			screen[snake.front().y * nScreenWidth + snake.front().x] = isDead ? L'x' : L'O';

			// Draw Food
			screen[foodY * nScreenWidth + foodX] = L'@';

			// If Snake Died
			if (isDead) {
				wsprintf(&screen[15 * nScreenWidth + 40], L"     PRESS 'SPACE' TO PLAY AGAIN    ");
			}

			// Display Frame
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		}
		// Wait For User to Press Space
		while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0);
	}
	return 0;
}
