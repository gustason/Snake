#include <iostream>
#include <list>
#include <thread>
using namespace std;

#include <Windows.h>

int nScreenWidth = 120;
int nScreenHeight = 80;

struct sSnakeSegment {
	int x, y;
};

//enum eDirection {NORTH = 0, EAST, SOUTH, WEST};
list<sSnakeSegment> snake = { {60, 15}, {61, 15}, {62, 15}, {63, 15}, {64, 15}, {65, 15}, {66, 15}, {67, 15}, {68, 15}, {69, 15} };
int foodX = 30;
int foodY = 15;
int score = 0;
bool isDead = false;
//eDirection nSnakeDirection = WEST;
int snakeDirection = 3;

const int dx[] = { 0, 1, 0, -1 };
const int dy[] = { -1, 0, 1, 0 };
int main() {
	// Create Screen Buffer. OneLoneCoder.com
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	bool keyRight = false, keyLeft = false, keyRightOld = false, keyLeftOld = false;
	while (1) {
		// Timing & Input
		this_thread::sleep_for(200ms);

		// Get Keyboard Input
        /// TO DO: Add all arrows.
		keyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;
		keyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;

		if (keyRight && !keyRightOld) {
			snakeDirection++;
			if (snakeDirection == 4) snakeDirection = 0;
		}
		if (keyLeft && !keyLeftOld) {
			snakeDirection--;
			if (snakeDirection == -1) snakeDirection = 3;
		}

		keyRightOld = keyRight;
		keyLeftOld = keyLeft;

		// Game Logic
		// Update Snake Position
		snake.push_front({ snake.front().x + dx[snakeDirection], snake.front().y + dy[snakeDirection] });

		// Remove Tail
		snake.pop_back();

		// Display Frame
		for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

		for (int i = 0; i < nScreenWidth; i++) {
			screen[i] = L'=';
			screen[2 * nScreenWidth + i] = L'=';
		}

		wsprintf(&screen[nScreenWidth + 5], L"S N A K E");
		wsprintf(&screen[nScreenWidth + 50], L"SCORE: %d", score);

		// Draw Snake Body
		for (auto s : snake) {
			screen[s.y * nScreenWidth + s.x] = isDead ? L'+' : L'~';
		}
		// Draw Snake Head
		screen[snake.front().y * nScreenWidth + snake.front().x] = isDead ? L'x' : L'O';

		// Draw Food
		screen[foodY * nScreenWidth + foodX] = L'@';
		WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	return 0;
}
