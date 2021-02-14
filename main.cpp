// TODO: MAKE SNAKE COME OUT OF OTHER SIDE
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
enum eLayout {
	CLASSIC = 0, MODERN
};
eLayout Layout = MODERN;

const short dx[] = { 0, 1, 0, -1 };
const short dy[] = { -1, 0, 1, 0 };
const short snakeGrows[] = { 5, 10, 5 };
int highScore = 0;
short gameMode = 0;

int rand(int a, int b) {
	return a + rand() % (b - a + 1);
}
bool isKeyPressed(char button) {
	return (0x8000 & GetAsyncKeyState((unsigned char)(button))) != 0;
}


void reactToKeyPresses(bool &keyUpOld, bool &keyDownOld, bool &keyLeftOld, bool &keyRightOld, short &snakeDirection, chrono::milliseconds tHorizontal, chrono::milliseconds tVertical) {
	bool keyUp = false, keyDown = false, keyLeft = false, keyRight = false;

	auto t1 = chrono::system_clock::now();
	// Adjust game delay based on player direction
	while ((chrono::system_clock::now() - t1) < ((snakeDirection % 2) ? tHorizontal : tVertical)) {
		keyRight = isKeyPressed('\x27');
		keyRight |= isKeyPressed('\x44');
		keyLeft = isKeyPressed('\x25');
		keyLeft |= isKeyPressed('\x41');
		keyUp = isKeyPressed('\x26');
		keyUp |= isKeyPressed('\x57');
		keyDown = isKeyPressed('\x28');
		keyDown |= isKeyPressed('\x53');

		if (Layout == MODERN) {
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
		}
		else if (Layout == CLASSIC) {
			if (keyRight && !keyRightOld) {
				snakeDirection++;
				if (snakeDirection == 4) snakeDirection = 0;
			}
			if (keyLeft && !keyLeftOld) {
				snakeDirection--;
				if (snakeDirection == -1) snakeDirection = 3;
			}
		}


		keyRightOld = keyRight;
		keyLeftOld = keyLeft;
		keyUpOld = keyUp;
		keyDownOld = keyDown;
	}
}

int main() {
	srand(time(NULL));
	// Create Screen Buffer. OneLoneCoder.com
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	while (1) {
		list<sSnakeSegment> snake = { {60, 15}, {61, 15}, {62, 15}, {63, 15}, {64, 15}, {65, 15}, {66, 15}, {67, 15}, {68, 15}, {69, 15} };
		int foodX = 50;
		int foodY = 15;
		int score = 0;
		bool isDead = false;
		short snakeDirection = 3;
		bool keyLeftOld = false, keyUpOld = false, keyDownOld = false, keyRightOld = false;

		chrono::milliseconds tHorizontal, tVertical;
		if (gameMode == 0 || gameMode == 2) { // NORMAL
			tHorizontal = 115ms;
			tVertical = 200ms;
		}
		else if (gameMode == 1) { // HARD
			/// TODO: Do something more with this.
			tHorizontal = 55ms;
			tVertical = 110ms;
		}

		while (!isDead) {
			// Timing & Input
			//this::sleep_for(200ms);

			// Get Keyboard Input and change direction accordingly
			reactToKeyPresses(keyUpOld, keyDownOld, keyLeftOld, keyRightOld, snakeDirection, tHorizontal, tVertical);


			// Game Logic
			// Update Snake Position
			snake.push_front({ snake.front().x + dx[snakeDirection], snake.front().y + dy[snakeDirection] });

			// Collision Detection
			// Snake & Food
			if (snake.front().x == foodX && snake.front().y == foodY)
			{
				score++;
				if (gameMode == 2) {
					int randTime = rand(20, 130);
					tHorizontal = (chrono::milliseconds)randTime;
					randTime = rand(15, 180);
					tVertical = (chrono::milliseconds)randTime;
				}
				while (screen[foodY * nScreenWidth + foodX] != L' ')
				{
					// TODO: Change Food Spawning based on difficulty
					foodX = rand() % nScreenWidth;
					foodY = (rand() % (nScreenHeight - 3)) + 3;
				}

				for (int i = 0; i < snakeGrows[gameMode]; i++)
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

			highScore = max(highScore, score);
			wsprintf(&screen[nScreenWidth + 5], L"S N A K E");
			wsprintf(&screen[nScreenWidth + 50], L"SCORE: %d", score);
			/// TODO: Add motivational qoutes if you reach a very high score (that would be pretty fun!)
			wsprintf(&screen[nScreenWidth + 60], L"HI-SCORE: %d", highScore);

			//wsprintf(&screen[nScreenWidth + 50], L"keyUp: %d", keyUp);
			//wsprintf(&screen[nScreenWidth + 70], L"snakeDirection: %d", snakeDirection);
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
				wsprintf(&screen[21 * nScreenWidth + 40], L"     PRESS 'SPACE' TO PLAY    ");
			}

			// Display Frame
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		}
		// Show Gamemode Selection Screen
		if (gameMode == 0) wsprintf(&screen[19 * nScreenWidth + 40], L"     -NORMAL- GAMEMODE SELECTED    ");
		else if (gameMode == 1) wsprintf(&screen[19 * nScreenWidth + 40], L"     --HARD-- GAMEMODE SELECTED    ");
		else if (gameMode == 2) wsprintf(&screen[19 * nScreenWidth + 40], L"     ---???--- GAMEMODE SELECTED    ");
		wsprintf(&screen[20 * nScreenWidth + 40], L"     PRESS 'G' TO CHANGE GAMEMODE    ");
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);

		// Wait for User to press space
		bool isSpacePressed = false;
		while (!isSpacePressed) {
			// Check if User wants to change Gamemode
			bool isGPressed = false;
			auto t1 = chrono::system_clock::now();
			auto eps = 300ms;
			while ((chrono::system_clock::now() - t1) < eps) {
				isGPressed |= isKeyPressed('\x47');
				isSpacePressed |= isKeyPressed('\x20');
			}

			// Change Gamemode and display frame
			if (isGPressed) {
				gameMode++;
				if (gameMode == 3) gameMode = 0;
				if (gameMode == 0) wsprintf(&screen[19 * nScreenWidth + 40], L"     -NORMAL- GAMEMODE SELECTED    ");
				else if (gameMode == 1) wsprintf(&screen[19 * nScreenWidth + 40], L"     --HARD-- GAMEMODE SELECTED    ");
				else if (gameMode == 2) wsprintf(&screen[19 * nScreenWidth + 40], L"     ---???--- GAMEMODE SELECTED    ");
				wsprintf(&screen[20 * nScreenWidth + 40], L"     PRESS 'G' TO CHANGE GAMEMODE    ");
				WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
			}
		}
		// TODO: Add Mayhem mode, where the speed of the snake changes pseudo-randomly with every food eaten.
		// Show 1 Frame of Layout change menu
		/*if (Layout == MODERN)
			wsprintf(&screen[nScreenWidth + 80], L"PRESS 'L' TO CHANGE LAYOUT TO CLASSIC");
		else if (Layout == CLASSIC)
			wsprintf(&screen[nScreenWidth + 80], L"PRESS 'L' TO CHANGE LAYOUT TO MODERN");
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth* nScreenHeight, { 0,0 }, & dwBytesWritten);

		// Wait For User to Press Space
		for (int timePassed = 0; !isKeyPressed('\x20') ; timePassed++) {
			bool keyLayout = (0x8000 & GetAsyncKeyState((unsigned char)('\x4C'))) != 0;
			if (Layout == MODERN)
				wsprintf(&screen[nScreenWidth + 80], L"PRESS 'L' TO CHANGE LAYOUT TO CLASSIC");
			else if (Layout == CLASSIC)
				wsprintf(&screen[nScreenWidth + 80], L"PRESS 'L' TO CHANGE LAYOUT TO MODERN");
			if (keyLayout) {
				switch (Layout) {
				case MODERN:
					Layout = CLASSIC;
					break;
				case CLASSIC:
					Layout = MODERN;
					break;
				}
			}

			// TODO: Work with this..
			if (keyLayout)
				WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth* nScreenHeight, { 0,0 }, & dwBytesWritten);
		}*/
	}
	return 0;
}
